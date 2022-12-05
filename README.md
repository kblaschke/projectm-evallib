# projectM Expression Evaluation Library

This library, short EvalLib, is projectM's pendant to Milkdrop's "ns-eel2" library. Its purpose is to parse, analyze,
compile and finally run the expression code contained in Milkdrop preset files.

It has a relatively small C++ interface to process code, create execution contexts and run the code in the prepared
environment. It also keeps the global memory buffers which can be associated with execution contexts. This allows
multiple independent instances of projectM or other applications using their own global memory buffer within the same
process.

For build instructions, please refer to the main projectM repository for now.

## Features

The main purpose of this library is to be as compatible to Milkdrop's implementation as possible, while being more
portable. The following list are features this library will implement once finished:

- **Preprocessing of code:**  
  Replacing constants and other special constructs like `$PI` or `megabuf[index]` into a more generic format.
- **Parsing and compiling:**  
  Check the expression's syntax and compile it into an executable data structure. This library uses GNU Bison for this
  purpose, same as the original ns-eel2 library.
- **Allow creating execution contexts:**  
  A context is a set of variables each code block runs on. The context is a list of variables which can be set to
  certain initial values and may be changed by the expression code.
- **Full Milkdrop expression language support:**  
  Support all features of the latest Milkdrop release to ensure maximum compatibility. This includes returning pointers
  as function results instead of actual float values to enable code like `_set(condition ? a : b, 5)`
  which would set either `a` or `b` to 5 depending on the condition.
- **Performant execution:**
  While this library will never achieve the speed of Milkdrop's assembly code, it strives to be as fast as possible.
  This is achieved by reducing as much overhead as possible from the implementaion and also optionally supporting JIT
  compilers, starting with LLVM.

This library is not responsible for parsing Milkdrop presets. It only consumes code blocks from these files and executes
them in each specific context.