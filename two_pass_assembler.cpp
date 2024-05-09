#include <bits/stdc++.h>
#include <fstream>
#include <sstream>

using namespace std;

// function to convert decimal to hexadecimal string
string decToHexa(int n, int text_body_length)
{
    // ans string to store hexadecimal number
    string ans = "";

    while (n != 0)
    {
        // remainder variable to store remainder
        int rem = 0;

        // ch variable to store each character
        char ch;
        // storing remainder in rem variable.
        rem = n % 16;

        // check if temp < 10
        if (rem < 10)
        {
            ch = rem + 48;
        }
        else
        {
            ch = rem + ('A' - 10);
        }

        // updating the ans string with the character variable
        ans += ch;
        n = n / 16;
    }

    // reversing the ans string to get the final result
    int i = 0, j = ans.size() - 1;
    while (i <= j)
    {
        swap(ans[i], ans[j]);
        i++;
        j--;
    }
    int len;

    // handling different modes according to the number of bits required in the hex representation
    if (text_body_length == 1)
    {
        len = 2;
    }
    else if (text_body_length == 0)
    {
        len = 4;
    }
    else if (text_body_length == 2)
    {
        len = 6;
    }
    for (int k = ans.size(); k < len; k++)
    {
        ans = "0" + ans;
    }
    return ans;
}

// converts a hexadecimal string to a decimal number
int hex_to_dec(string target)
{
    int ret = 0;
    int i = target.length() - 1;
    int j;

    for (j = 1; i > -1; j *= 16, i--)
    {
        if ((target[i] >= '0') && (target[i] <= '9'))
            ret += ((target[i] - '0') * j);
        else if ((target[i] >= 'A') && (target[i] <= 'F'))
            ret += ((target[i] - 'A' + 10) * j);
    }

    return ret;
}

// Function to parse a line of the input in pass 1
void parse_line1(int &num, string &line, string &mnemonic, string &operand, string &label)
{

    string buffer = line;
    // if read line is a comment or a new line, num, i.e, number of strings=0
    num = 0;

    // skip if line read is a comment or empty line
    if ((buffer[0] != '.') && (buffer[0] != '\0') && (buffer[0] != '\n'))
    {
        stringstream ss(buffer);
        int number_of_strings = 0;
        string word;
        vector<string> instr;

        // get number of strings in the read line
        while (ss >> word)
        {
            number_of_strings++;
            instr.push_back(word);
        }

        num = number_of_strings;
        // cout<<instr[0]<<" "<<instr[1]<<endl;

        // according to the number of strings in line, set the label, mnemonic and operand fields

        // 1 string indicates only 1 mnemonic field
        if (number_of_strings == 1)
        {
            mnemonic = instr[0];
        }
        // 2 strings indicate 1 mnemonic and 1 operand fields
        else if (number_of_strings == 2)
        {
            mnemonic = instr[0];
            operand = instr[1];
        }
        // 3 strings indicate 1 label, 1 mnemonic and 1 operand fields
        else if (number_of_strings == 3)
        {
            label = instr[0];
            mnemonic = instr[1];
            operand = instr[2];
        }
    }
}

// Function to parse lines of intermediate file for pass2
void parse_line2(int &num, string &line, string &label, string &address, string &mnemonic, string &operand)
{
    string buffer = line;
    // if read line is a comment or a new line, num, i.e, number of strings=0
    num = 0;

    // skip if line read is a comment or empty line
    if ((buffer[0] != '.') && (buffer[0] != '\0') && (buffer[0] != '\n'))
    {
        stringstream ss(buffer);
        int number_of_strings = 0;
        string word;
        vector<string> instr;

        // get number of strings in the read line
        while (ss >> word)
        {
            number_of_strings++;
            instr.push_back(word);
        }

        num = number_of_strings;

        // 2 strings indicate address and mnemonic fields
        if (number_of_strings == 2)
        {
            if (instr[0] == "END")
            {
                mnemonic = instr[0];
                operand = instr[1];
            }
            else
            {
                address = instr[0];
                mnemonic = instr[1];
            }
        }
        // 3 strings indicate address, mnemonic and operand fields
        else if (number_of_strings == 3)
        {
            address = instr[0];
            mnemonic = instr[1];
            operand = instr[2];
        }
        // 4 strings indicate address, label, mnemonic and operand fields
        else if (number_of_strings == 4)
        {
            address = instr[0];
            label = instr[1];
            mnemonic = instr[2];
            operand = instr[3];
        }
    }
}

// parse operand to check if addressing mode is direct or indexed
bool determine_indexing(string &operand)
{
    int len = operand.length();

    bool indexed = 0;

    if ((operand[len - 2] == ',') && (operand[len - 1] == 'X'))
    {
        indexed = 1;
        operand.pop_back();
        operand.pop_back();
    }

    return indexed;
}

// Function to format constant for pass 2
string format_operand_constant(string operand)
{
    int val = 0;
    string temp;
    int len = operand.length() - 1;
    int i;

    temp = operand;
    string final_constant;

    if (temp[0] == 'C')
    {
        for (i = 2; i < len; i++)
            val = (val * 256) + (int)temp[i];

        final_constant = decToHexa(val, 2);
    }
    else if (temp[0] == 'X')
    {

        for (i = 2; i < len; i++)
            final_constant.push_back(temp[i]);
    }

    return final_constant;
}

int main()
{
    // Input and output file streams
    ifstream inputFile("input.txt");
    ofstream intermediateFile("intermediate.txt");
    ofstream outputFile("output.txt");

    // Opcode table with mnemonic and corresponding machine code
    unordered_map<string, string> opcodes = {
    {"LDA", "00"}, {"LDX", "04"}, {"LDL", "08"}, {"STA", "0C"}, {"STX", "10"}, {"STL", "14"}, {"LDCH", "50"}, 
    {"STCH", "54"}, {"ADD", "18"}, {"SUB", "1C"}, {"MUL", "20"}, {"DIV", "24"}, {"COMP", "28"}, {"J", "3C"}, 
    {"JLT", "38"}, {"JEQ", "30"}, {"JGT", "34"}, {"JSUB", "48"}, {"RSUB", "4C"}, {"TIX", "2C"}, {"TD", "E0"}, 
    {"RD", "D8"}, {"WD", "DC"}};

    // shows errors in file handling
    if (inputFile.fail())
    {
        cout << "Error: cannot open input file, try again" << endl;
        return 0;
    }

    if (intermediateFile.fail())
    {
        cout << "Error: cannot open intermediate file, try again" << endl;
        return 0;
    }

    if (outputFile.fail())
    {
        cout << "Error: cannot open output file, try again" << endl;
        return 0;
    }

    // Symbol table to store labels and their addresses
    unordered_map<string, int> symbolTable;

    // Program's location counter
    int locationCounter = 0;

    // Starting address of the program
    int starting_address = 0;

    // Program name
    string program_name;

    // temporary line to store line read from input file
    string line;

    // Line number in the input file
    int num_lines = 0;

    // Pass1
    // Processing input file to generate intermediate file and symbol table
    while (getline(inputFile, line))
    {
        num_lines++;

        // definining strings to hold field values
        string mnemonic;
        string operand;
        string label;
        int number_of_strings = 0;

        // Parse the line to get mnemonic, operand, and label
        parse_line1(number_of_strings, line, mnemonic, operand, label);

        // Update symbol table for labels
        if (number_of_strings == 3)
        {
            // check if read label is duplicate and show error
            if (symbolTable.find(label) != symbolTable.end())
            {
                cout << "Error: Duplicate symbol exists in symbol table" << endl;
                return 0;
            }
            symbolTable[label] = locationCounter;
        }

        // Process the first line to get starting address and program name
        if (num_lines == 1 && mnemonic == "START")
        {
            // set starting address to operand, locationCounter to starting address
            locationCounter = starting_address = hex_to_dec(operand);
            // label of line with mnemonic = START is assumed to be program name
            program_name = label;
            // write line to intermediate file
            intermediateFile << decToHexa(locationCounter, 0);
            intermediateFile << " " << label << " " << mnemonic << " " << operand << endl;
        }
        else if (num_lines == 1 && mnemonic != "START")
        {
            // if mnemonic is not START, setting locationCounter to 0 and starting address of program is also 0
            locationCounter = starting_address = 0;
            // program named untitled
            program_name = "untitled";
        }
        // while mnemonic not equal to END continue readung from input file
        else if (num_lines > 1 && mnemonic != "END")
        {
            // if read line not a comment than process line
            if (number_of_strings != 0)
            {
                // write the locationCounter of current line to intermediate file
                intermediateFile << decToHexa(locationCounter, 0);
                intermediateFile << " ";

                // search for mnemonic in opcodes
                if (opcodes.find(mnemonic) != opcodes.end())
                {
                    // if mnemonic found in opcodes add 3 {instruction length} to locationCounter
                    locationCounter += 3;
                }
                // check if mnemonic is WORD
                else if (mnemonic == "WORD")
                {
                    // if mnemonic = WORD add 3 to locationCounter
                    locationCounter += 3;
                }
                // check if mnemonic is RESW
                else if (mnemonic == "RESW")
                {
                    // add 3 * (operand's value) to locationCounter
                    locationCounter += (3 * stoi(operand));
                }
                // check if mnemonic is RESB
                else if (mnemonic == "RESB")
                {
                    // add operand's value to locationCounter
                    locationCounter += stoi(operand);
                }
                // check if mnemonic is BYTE
                else if (mnemonic == "BYTE")
                {
                    // finding length of constant in bytes and adding it to the locationCounter
                    int ret = operand.length() - 3;

                    if (operand[0] == 'X')
                        ret /= 2;
                    locationCounter += ret;
                }
                // display error if mnemonic with invalid opcode is found, i.e, it is not present in optable
                else
                {
                    cout << "Error: mnemonic with invalid opcode found in pass1" << endl;
                    return 0;
                }
                if (number_of_strings == 3) // write the read line to intermediate file, line has label mnemonic and operand
                    intermediateFile << label << " " << mnemonic << " " << operand << endl;
                else if (number_of_strings == 2) // write the read line to intermediate file, line has mnemonic and operand
                    intermediateFile << mnemonic << " " << operand << endl;
                else if (number_of_strings == 1) // // write the read line to intermediate file, line has mnemonic
                    intermediateFile << mnemonic << endl;
            }
            else
            {
                intermediateFile << line << endl;
            }
        }
        // writing last line of input file to output file
        else if (num_lines > 1 && mnemonic == "END")
        {
            intermediateFile  << line << endl;
        }
    }

    // pass2
    // Generate output file with machine code
    num_lines = 0;

    // closing files
    inputFile.close();
    intermediateFile.close();

    // shows errors in file handling
    ifstream inter("intermediate.txt");
    ofstream flist("assembly_listing.txt");

    if (inter.fail())
    {
        cout << "Error: cannot open intermediate file, try again" << endl;
        return 0;
    }

    if (flist.fail())
    {
        cout << "Error: cannot open assembly listing file, try again" << endl;
        return 0;
    }

    string mnemonic;
    string address;
    string operand;
    string label;

    // read line from intermediate file
    getline(inter, line);
    int num_strings = 0;

    // parse line for address, label, mnemonic and operand
    parse_line2(num_strings, line, label, address, mnemonic, operand);

    num_lines++;

    if (num_lines == 1 && mnemonic == "START")
    {
        // write listing line
        flist << line << endl;
        // read line from intermediate file
        getline(inter, line);
        // parse line for address, label, mnemonic and operand
        parse_line2(num_strings, line, label, address, mnemonic, operand);
    }

    // print header record to output file
    int program_length = locationCounter - starting_address;
    int l = 6 - program_name.length();
    int len;
    if (l <= 0)
    {
        len = 0;
    }
    else
    {
        len = l;
    }
    outputFile << "H^" << program_name;
    for (int i = 0; i < len; i++)
    {
        outputFile << " ";
    }

    outputFile << "^" << decToHexa(starting_address, 2);
    outputFile << "^" << decToHexa(program_length, 2) << endl;

    // Initialize variables for text record
    int text_record_starting_address = 0;
    int text_record_length = 0;
    string text_record_body;

    string assembled_opcode;
    int increment = 0;

    // Loop through intermediate file to generate machine code
    while (mnemonic != "END")
    {
        num_lines++;

        int operand_address = 0;
        // cout << label << " " << mnemonic << " " << operand << " \n";
        // checking that the read line is not a comment or an empty line
        if (num_strings != 0)
        {
            // searching for the opcode in map
            if (opcodes.find(mnemonic) != opcodes.end())
            {
                // checking if there exists a symbol in operand field
                if (num_strings == 3 || num_strings == 4)
                {
                    // checking if the operand is indexed or not
                    bool indexed = determine_indexing(operand);

                    // checking if the operand is in symbol table
                    if (symbolTable.find(operand) != symbolTable.end())
                    {   
                        operand_address = symbolTable[operand];
                        if (indexed)
                            // as "x" bit in instruction has to be made 1, 32768 = 8000 (hex) makes 15th bit 1
                            operand_address += 32768;
                    }
                    else
                    {
                        cout << "Error: Undefined symbol found in pass2" << endl;
                        return 0;
                    }
                }
                else
                {
                    // no operand field, hence opernad address set to 0
                    operand_address = 0;
                }
                assembled_opcode = opcodes[mnemonic] + decToHexa(operand_address, 0);
                // as an opcode is 3 bytes
                increment = 3;
            }
            else if (mnemonic == "BYTE" || mnemonic == "WORD")
            {
                if (operand[0] == 'C' || operand[0] == 'X')
                {
                    int ret = operand.length() - 3;

                    if (operand[0] == 'X')
                        ret /= 2;

                    increment = ret;
                    assembled_opcode = format_operand_constant(operand);
                }
                else
                {
                    assembled_opcode = decToHexa(stoi(operand), 2);
                    increment = 3;
                }
            }
            // display error if mnemonic with invalid opcode is found, i.e, it is not present in optable
            else
            {
                if (mnemonic != "RESB" && mnemonic != "RESW")
                {
                    cout << "Error: Unspecified symbol not in OPTABLE!";
                    cout << "Kindly modify input" << endl;
                    return 0;
                }
            }

            // Update text record information
            text_record_length += increment;

            //If the program doesn't start from the first memory location and has a text record starting address of 0, 
            //it indicates that after a RESB or RESW directive, the subsequent line might be a comment, causing the address 
            //field to default to 0 in the text record.
            if ((starting_address != 0) && (text_record_starting_address == 0))
                text_record_starting_address = hex_to_dec(address);

            // Check if text record length exceeds limit
            if (text_record_length > 30)
            {
                outputFile << "T^" << decToHexa(text_record_starting_address, 2);
                outputFile << "^" << decToHexa(text_record_length - increment, 1);
                outputFile << text_record_body << endl;
                // Initialize next text record
                text_record_starting_address = hex_to_dec(address); // convert the address read from intermediate file to hex
                text_record_length = increment;                     // new text record length
                text_record_body.clear();
            }

            // add assembled opcode to text record body
            text_record_body += "^";
            text_record_body += assembled_opcode;

            // add assembled opcode at end of line read from intermediate file
            line += " ";
            line += assembled_opcode;
        }

        // Write line to assembly listing file
        flist << line << endl;
        getline(inter, line);
        parse_line2(num_strings, line, label, address, mnemonic, operand);

        // Check if mnemonic is RESB or RESW
        if (mnemonic == "RESB" || mnemonic == "RESW")
        {
            outputFile << "T^" << decToHexa(text_record_starting_address, 2);
            outputFile << "^" << decToHexa(text_record_length, 1);
            outputFile << text_record_body << endl;
            // Initialize next text record
            text_record_length = 0; // new text record length
            text_record_body.clear();
        }

        // Skip lines with RESB or RESW
        while (mnemonic == "RESB" || mnemonic == "RESW")
        {
            flist << line << endl;
            getline(inter, line);
            parse_line2(num_strings, line, label, address, mnemonic, operand);
            if (mnemonic != "END")
                text_record_starting_address = hex_to_dec(address);
        }
    }

    if (text_record_body.length())
    {
        outputFile << "T^" << decToHexa(text_record_starting_address, 2);
        outputFile << "^" << decToHexa(text_record_length, 1);
        outputFile << text_record_body << endl;
        text_record_length = 0;
    }

    // Write END record to output file
    outputFile << "E^" << decToHexa(starting_address, 2);
    // write last assembly listing line
    flist << line << endl;
    text_record_body.clear();

    cout<<"The input assembly code has been successfully converted to machine code given in output.txt file"<<endl;

    return 0;
}