#pragma once

#include <string>

/**
 * @brief Equation parser for Milkdrop math expressions.
 *
 * Takes a block of code and parses it into an equation tree that can be executed on demand.
 *
 * DELETEME DELETEME
 * All equation statements are assignments in the form of "val = expr".
 * Parsing steps:
 * 1. Outer loop extracts an assignment, terminated by either semicolon or EOF
 * 2.
 *
 */
class CodePreprocessor
{
public:
    /**
     * @brief Preprocesses the equation code and turns it into more machine-readable form.
     * @param code The code to preprocess
     * @return The preprocessed code, ready for generating an expression tree from it.
     */
    [[nodiscard]] static std::string PreprocessCode(const std::string& code);

protected:

    /**
     * @brief Strips all whitespace from the equation.
     * @param code The code to strip.
     */
    static void StripWhitespace(std::string& code);

    /**
     * @brief Searches for all occurrences of the given constant and replaces it with the numerical value.
     * @param code The code to process.
     * @param constant The constant to look for, e.g. "$PI".
     * @param value The numerical value to replace each constant with, e.g. "3.141..."
     */
    static void ReplaceConstant(std::string& code, const std::string& constant, const std::string& value);

    /**
     * @brief Searches for all occurrences of $X and replaces it with a decimal value of the hex value following.
     * @param code The code to process.
     */
    static void ReplaceHexValues(std::string& code);

    /**
     * @brief Searches for all occurrences of ternary operators and replaces them with if().
     * @param code The code to process.
     */
    static void ReplaceTernaryOperators(std::string& code);

    /**
     * @brief Finds the right side expression of an operand.
     *
     * Takes care of opening/closing parentheses.
     *
     * @param code The code to search.
     * @param startOffset The start offset to begin searching.
     * @param terminatorChars The chars to count as terminators.
     * @return The expression between the start offset and a terminator char. Can be empty.
     */
    static std::string GetRightParam(std::string& code, size_t startOffset, const std::string& terminatorChars);

    /**
     * @brief Finds the left side expression of an operand.
     *
     * Takes care of opening/closing parentheses. Always terminates at the beginning of the code.
     *
     * @param code The code to search.
     * @param startOffset The start offset to begin searching.
     * @param terminatorChars The chars to count as terminators.
     * @return The expression before the start offset. Can be empty.
     */
    static std::string GetLeftParam(std::string& code, size_t startOffset, const std::string& terminatorChars);

};
