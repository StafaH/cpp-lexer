# cpp-lexer
This project is a simple lexer with some parser elements.

## Overview
The goal of this project was to begin investigating the concepts of lexing and parsing. The idea was to build a type of command line inside a running application that can be used to manipulate variables, execute certain functions, and so on. At this point, most of the application is fletched out, and the entire flow from input, lexing to tokens, and parsing tokens to perform actions has been completed. 

### Warning
Much of this code would not be considered "safe practice" for C++. This was a learning project, going down to the very low level of programming, the idea was to write everything I needed from scratch and not rely on modern C++ features and libraries, and using only the stdio.h library to get our input from the console. 

### Design Choice
One of the most important design choices that could have gone either way was using pointers into the original string for token information, or copying the strings into each token, and getting rid of the original string.

## Building
Build using build.bat in misc if you have Visual Studio 2017 installed. Otherwise use your own compiler.

## Advice
Feel free to use this code if you come by it. Experiment and adapt the code, and try to do new things.
