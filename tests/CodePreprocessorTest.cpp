#include <gtest/gtest.h>

#include <projectm-evallib/CodePreprocessor.hpp>

static constexpr auto codePreprocessorTestDataPath{ PROJECTM_EVALLIB_TEST_DATA_DIR "/" };

/**
 * Class to make protected function accessible to tests.
 */
class CodePreprocessorMock : public CodePreprocessor
{
public:
    static void StripWhitespace(std::string& code)
    {
        CodePreprocessor::StripWhitespace(code);
    }

    static void ReplaceConstant(std::string& code, const std::string& constant, const std::string& value)
    {
        CodePreprocessor::ReplaceConstant(code, constant, value);
    }

    static void ReplaceHexValues(std::string& code)
    {
        CodePreprocessor::ReplaceHexValues(code);
    }

    static void ReplaceTernaryOperators(std::string& code)
    {
        CodePreprocessor::ReplaceTernaryOperators(code);
    }

    static std::string GetRightParam(std::string& code, size_t startOffset, const std::string& terminatorChars)
    {
        return CodePreprocessor::GetRightParam(code, startOffset, terminatorChars);
    }

    static std::string GetLeftParam(std::string& code, size_t startOffset, const std::string& terminatorChars)
    {
        return CodePreprocessor::GetLeftParam(code, startOffset, terminatorChars);
    }
};
/*
TEST(CodePreprocessor, PreprocessStuff)
{
    FileParser parser;
    parser.Read(std::string(codePreprocessorTestDataPath) + "shifter - robotopia.milk");

    std::cout << parser.GetCode("wave_0_per_point") << std::endl;

    EXPECT_EQ(CodePreprocessor::PreprocessCode(parser.GetCode("wave_0_per_point")), "");
}
*/


TEST(CodePreprocessor, ReplaceConstantSimple)
{
    std::string code{ "$CONST" };

    CodePreprocessorMock::ReplaceConstant(code, "$CONST", "12345");

    ASSERT_EQ(code, "12345");
}

TEST(CodePreprocessor, ReplaceConstantFullEquation)
{
    std::string code{ "x=a+$CONST*sin($PI)" };

    CodePreprocessorMock::ReplaceConstant(code, "$CONST", "12345");

    ASSERT_EQ(code, "x=a+12345*sin($PI)");
}

TEST(CodePreprocessor, ReplaceConstantMultiple)
{
    std::string code{ "x=a+cos($PI*time*0.01)*sin($PI)" };

    CodePreprocessorMock::ReplaceConstant(code, "$PI", "3.141");

    ASSERT_EQ(code, "x=a+cos(3.141*time*0.01)*sin(3.141)");
}

TEST(CodePreprocessor, ReplaceHexValuesSimple)
{
    std::string code{ "$XF" };

    CodePreprocessorMock::ReplaceHexValues(code);

    ASSERT_EQ(code, "15");
}

TEST(CodePreprocessor, ReplaceHexValuesLarge)
{
    // This is not recommended in equations ;)
    std::string code{ "$XFFFFFFFFFFFFFFFF" };

    CodePreprocessorMock::ReplaceHexValues(code);

    ASSERT_EQ(code, "18446744073709551615");
}

TEST(CodePreprocessor, ReplaceHexValuesFullEquation)
{
    std::string code{ "flag|=$XFF&4;" };

    CodePreprocessorMock::ReplaceHexValues(code);

    ASSERT_EQ(code, "flag|=255&4;");
}

TEST(CodePreprocessor, ReplaceHexValuesMultiple)
{
    std::string code{ "val=$XFF+$XA000;" };

    CodePreprocessorMock::ReplaceHexValues(code);

    ASSERT_EQ(code, "val=255+40960;");
}

TEST(CodePreprocessor, ReplaceHexValuesAbused)
{
    // This is valid syntax, but please, preset authors, don't do this!
    std::string code{ "val=$XF$XA000;" };

    CodePreprocessorMock::ReplaceHexValues(code);

    ASSERT_EQ(code, "val=1540960;");
}

TEST(CodePreprocessor, ReplaceTernaryOperatorSimple)
{
    std::string code{ "a?b:c" };

    CodePreprocessorMock::ReplaceTernaryOperators(code);

    ASSERT_EQ(code, "if(a,b,c)");
}

TEST(CodePreprocessor, ReplaceTernaryOperatorFullEquation)
{
    std::string code{ "x=a?b:c;" };

    CodePreprocessorMock::ReplaceTernaryOperators(code);

    ASSERT_EQ(code, "x=if(a,b,c);");
}

TEST(CodePreprocessor, ReplaceTernaryOperatorComplexOperands)
{
    std::string code{ "x=sin(a+0.1)>0.5?pow(b,2):c+floor(d);" };

    CodePreprocessorMock::ReplaceTernaryOperators(code);

    ASSERT_EQ(code, "x=if(sin(a+0.1)>0.5,pow(b,2),c+floor(d));");
}

TEST(CodePreprocessor, ReplaceTernaryOperatorRecursiveLeft)
{
    std::string code{ "x=a?b?c:d:e;" };

    CodePreprocessorMock::ReplaceTernaryOperators(code);

    ASSERT_EQ(code, "x=if(a,if(b,c,d),e);");
}

TEST(CodePreprocessor, ReplaceTernaryOperatorRecursiveRight)
{
    std::string code{ "x=a?b:c?d:e;" };

    CodePreprocessorMock::ReplaceTernaryOperators(code);

    ASSERT_EQ(code, "x=if(a,b,if(c,d,e));");
}

TEST(CodePreprocessor, ReplaceTernaryOperatorRecursiveCheck)
{
    // Need to use parentheses, as in any other language.
    std::string code{ "x=(a?b:c)?d:e;" };

    CodePreprocessorMock::ReplaceTernaryOperators(code);

    ASSERT_EQ(code, "x=if((if(a,b,c)),d,e);");
}

TEST(CodePreprocessor, ReplaceTernaryOperatorRecursiveDeep)
{
    std::string code{ "x=a?b?c?d?e:f:g:h:i;" };

    CodePreprocessorMock::ReplaceTernaryOperators(code);

    ASSERT_EQ(code, "x=if(a,if(b,if(c,if(d,e,f),g),h),i);");
}

TEST(CodePreprocessor, ReplaceTernaryOperatorDefaultFalse)
{
    std::string code{ "x=a?b;" };

    CodePreprocessorMock::ReplaceTernaryOperators(code);

    ASSERT_EQ(code, "x=if(a,b,0);");
}

TEST(CodePreprocessor, ReplaceTernaryOperatorDefaultTrue)
{
    std::string code{ "x=a?:b;" };

    CodePreprocessorMock::ReplaceTernaryOperators(code);

    ASSERT_EQ(code, "x=if(a,0,b);");
}

TEST(CodePreprocessor, ReplaceTernaryOperatorDefaultBoth)
{
    // Makes no sense really, but is supported by Milkdrop.
    std::string code{ "x=a?:;" };

    CodePreprocessorMock::ReplaceTernaryOperators(code);

    ASSERT_EQ(code, "x=if(a,0,0);");
}
