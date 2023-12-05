# PROJECT DESCRIPTION

The project should be divided into 4 parts:

# Configuration

In this part, the project turns a .set archive into a hashtable that has all the information for the assembler to run itself properly

# Lexic Analysis and Slicing the lines

In this part, the project runs a basic Analysis to check for Lexic Errors, and then it should also slice the lines with the given tokens below:

1. Instruction
2. Registers
3. Immediate
4. Labels and variables
5. Text and Data Sections

# Assembling

In this part, the project assembles the .asm file into a logisim memory formatted image using the configuration from the .set file

# Reports

This part of the program shall provide a set of functions related to all the other parts above, those functions should be used to generate reports, like:

1. Reports on configuration file errors
2. Reports on lexic errors within the .asm file
3. Reports on statistics (needs to be determined)
4. Dynamic Reports (needs to be determined)

