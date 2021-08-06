# Simpilfied Java Language Compiler 
A Lexical and Syntax Analyzer for Simplified Java Language.

## How to
First, compile "lexical_analyzer.cpp" in linux environment. You can simply type following command:
```g++ -o lexical_analyzer lexical_analyzer.cpp```

Next, run on created executed binary file, or you can just run 'lexical_analyzer' binary file.
```./lexical_analyzer <input_file_name>```
Result file would be created as "input_file_name_output.txt" unless there are lexical error on input file.

Output text files will be used as input file of syntax analyzer.
Run syntax_analyzer binary file with the new input file.
```./syntax_analyzer <input_file_name>```

Result message will be printed as 'ACCEPT' or 'REJECT'.
In case of REJECT, syntax analyzer will print the line which makes syntax error.
