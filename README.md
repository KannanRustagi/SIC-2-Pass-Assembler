# SIC-2-Pass-Assembler
Name- Kannan Rustagi
Roll no.-210101054

## INSTRUCTIONS FOR RUNNING THE CODE
1. The environment required for running the code is Linux system with g++ compiler installed.

2. Open the terminal and navigate to the directory where the contents of this zip file have been extracted.

3. The input assembly code should be put in the input.txt file.

4. Each instruction should be on separate lines.
   In case, addressing mode is indexed, eg- instruction of the form- "LDCH RECORD,X", here there should be no space between the ',' and 'X' characters.

5. As sample the code on page 49 of TEXT BOOK has been included in the input.txt file.

6. For compilation, type
				g++ two_pass_assembler.cpp

7. To run the file, type
				./a.out

========================================================================================================================

## CODE EXPLANATION
1. The assembler uses File I/O Management through C++. If any error in opening or creating files
   is encountered, the assembler displays an error message on the terminal and then halts execution.

2. The name of intermediate file is set to "intermediate.txt" and that of output file is set to
   "output.txt".

3. If there is any error in the input file (duplicate symbol, undefined symbol, invalid operation code),
   the assembler displays an error message on the terminal and then halts execution.

4. The assembler uses stringstream for handling strings in input.

The code is divided into two main parts: Pass 1 and Pass 2. Here's a brief overview of the main components:

## Pass 1:
- Symbol Table: Uses an unordered map (symbolTable) to store labels and their corresponding addresses which are later referred in pass 2 to generate machine code.

- Intermediate File Generation: Parses the input source program line by line and generates an intermediate file (intermediate.txt) with the location counter, label, mnemonic, and operand fields. 

- Handling Directives: Update the location counter at each line based on the mnemonics present in the optable map and others such as RESW, RESB, WORD, and BYTE.

- Error Checking: Checks for errors in the input file, such as duplicate symbols, invalid opcodes


## Pass 2:
- Output File Generation: Generates the final output file (output.txt) containing the machine code instructions.

- Basically, we get the corresponding opcode of the mnemonic present in the instruction from the opcode map and for instructions with operands, the code checks if the operand is present in the symbol table. If yes, it retrieves the operand's address. If the operand is indexed, it also modifies the operand's address to set the indexing bit.

- Assembly Listing File: Creates an assembly listing file (assembly_listing.txt) with the processed source program, including addresses, mnemonics, operands, and machine code.

- Text Record Generation: Breaks the machine code instructions into text records based on the length constraint and writes them to the output file.




