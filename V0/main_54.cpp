#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <stack>
#include <queue>
#include <algorithm>  

using namespace std;

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

void trimm();
int build(string variables[]);
int parser(string line, int fail);
int find(string need, string variables[], int fail);
int findvar(string variables[], int fail);
int	unknownid(string variables[], int fail);
int semicolon(int fail);
void convert();

int main()
{
	string variables[20];
	int choice = 0;
	int filestat = 1;		//this holds 0 or 1, depending on if the file had any issues while compiling. If issues exist
							//this will be 1, it is set to 1 initially so that the user MUST build the file once successfully to
							//be able to convert to C++;
	

	std::cout << endl;
	std::cout << "Welcome to Group 2 Final Project" << endl;
	std::cout << "Name of File Used: final.txt" << endl;

	while (choice != 4)			//Create a menu for the user
	{
		std::cout << endl;
		std::cout << "Please Choose from the following options:" << endl;
		std::cout << "1. Trimm File" << endl;
		std::cout << "2. Build File" << endl;
		std::cout << "3. Convert to C++" << endl;
		std::cout << "4. Quit!" << endl;
		std::cout << "Execute Order: ";
		cin >> choice;
		std::cout << endl;
		std::cout << endl;

		switch (choice)
		{
		case 1:
			std::cout << "Preparing to trimm file..." << endl;
			std::cout << endl;
			trimm();			//Trimm File of all comments and extra spaces. 
			std::cout << "The File has been trimmed to the correct format!" << endl;
			break;
		case 2:
			std::cout << "Building the file now...." << endl;
			filestat = build(variables);								//build the file
			std::cout << "Build successfully completed" << endl;
			break;
		case 3:
				std::cout << "Converting to C++ Now...." << endl;
				convert();										//convert the file to c++
				break;
		case 4:
			break;
		default:
			std::cout << "That was not a proper response, please select an option from the menu!" << endl;
		}
	}

	system("PAUSE");
	
	return 0;
}

void trimm()
{
	ifstream infile;
	infile.open("final.txt");
	ofstream outfile("corrected.txt");

	string skip;
	string line;
	string line2;
	string temp;
	stringstream ss;

	queue<string> Q;


	if (infile.fail())									//Fail/Error Checking for file opening Fstream.
	{
		cout << "Input file could not be opened.\n";
		exit(-1);
	}
	if (outfile.fail())
	{
		cout << "Output file could not be opened.\n";
		exit(-1);
	}

	if (infile.is_open())
	{

		//Skip first few lines until *) is found (signifying end of first few lines)
		while (getline(infile, skip))
		{
			if (skip.find("*)") != std::string::npos)
			{
				break;
			}

		}


		while (getline(infile, line))
		{

			cout << line << endl;
			//char* p;	//Looking at http://stackoverflow.com/questions/20210468/strtok-s-was-not-declared-in-this-scope
			//for information, strtok_s needed to be used, and requires this pointer. Unsure as to function.

			int length = 0;



			while (getline(infile, line))
			{
				//Find beginning of comment
				temp = line;
				length = line.length();

				//delete comments by looking for (*, and deleting rest of line
				size_t loc = temp.find("(*");
				if (loc < 100)
				{
					//kill rest of comment line
					temp.replace(loc, temp.length() - loc, "");
					//cout << "Found";
					//cout << temp << endl;
				}

				//line is non blank line
				// not sure if this works, so a redundant step follows

				bool push = false;


				temp.erase(0, temp.find_first_not_of(" \t"));

				//go through each character of temp string, if a alphanumeric is found, we know line is not blank,
				//and we can push it
				for (int i = 0; i<temp.length(); i++){
					if (isalnum(temp[i])){
						push = true;
						break;
					}
					else{
						push = false;
					}
				}

				temp.append("\n");
				if (push)Q.push(temp);

				cout << "Hello temp is" << temp << endl;

				//Place temp in a queue for further parsing

			}

			// Tokenize each line 1 at a time

			size_t x = Q.size();

			for (int i = 0; i < x; i++){
				//cout << Q.front();

				string s = Q.front();


				//http://www.cplusplus.com/reference/string/string/find_first_not_of/
				//Check if current string s, is a blank line

				if (s.find_first_not_of(" ") != std::string::npos){
					// S is non blank line, continue normally
					//cout << "s is:" << s<< endl;
				}
				else{
					// S is blank line, skip this and continue to next iteration of loop
					// We do this because we want to ignore blank lines
					//cout << "s is blank" << endl;
					Q.pop();
					continue;
				}

				char* p;

				//http://www.cplusplus.com/reference/cstring/strtok/
				//Strtok used to tokenize each line by spaces

				//http://www.cplusplus.com/reference/cstring/memcpy/
				//Memcpy used to copy string into an array of characters

				char* str = new char[s.size() + 1];
				str[s.size()] = 0;
				memcpy(str, s.c_str(), s.size());
				char* tok;
				tok = strtok_s(str, " ", &p);

				//if (addSpaceThisLine)ss << "    ";

				while (tok != NULL)
				{
					cout << "tok is:" << tok << endl;
					ss << tok << " ";

					//ss << tok;
					tok = strtok_s(NULL, " ", &p);
				}
				Q.pop();

				//ss<<endl<<endl;
				//ss << endl;

			}
			//cout << "The following will be written to output.txt:\n" << endl;

			//cout << ss.str();
			infile.close();


			// Open output file and use string stream to write to file.
			outfile << ss.str();
			outfile.close();
		}
	}
}


int build(string variables[])
{
	int location = 0;						//Before calling the parser we will store the location of the keywords (if they are in the string), and thereby
											//replace them with a character.

	int fail = 0;							//Fail counter, if this is more than 0 there was a problem, print such to the user.
	string line;
	stringstream parseline;

	ifstream infile("corrected.txt");					//Open the trimmed file

	if (infile.fail())
	{
		perror("There was an error opening the file: corrected.txt");		//ERROR opening the file
		exit(-1);
	}

	if (infile.is_open())
	{
		fail = find("PROGRAM", variables, fail);	//See if we can find PROGRAM, if not we increment fail
		fail = find("VAR", variables, fail);		//See if we can find VAR, if not we increment fail
		fail = find("BEGIN", variables, fail);		//See if we can find BEGIN, if not we increment fail
		fail = find("END.", variables,fail);		//See if we can find END, if not we increment fail
		fail = find("INTEGER", variables, fail);	//See if we can find INTEGER, if not we increment fail

		//fail = findvar(variables, fail);			//Get variables that are being initialized.

		//cout << "Variables located, attempting to discover non-initialized variables" << endl;
		//cout << endl;
		
		//fail = unknownid(variables, fail);	//Check for unknown identifiers
		//fail = semicolon(fail);		//Check for semicolon
		if (fail == 0)
		{
			while (getline(infile, line))
			{
				//cout << "Line to Parse: " << line << endl;
				//Remove all spaces from the line, for the parser
				line.erase(remove_if(line.begin(), line.end(), isspace), line.end());

				//ANYTHING going into the stack of the parser needs to be 1 character maximum, as such we
				//need to condense all the terms down into 1 single character this is done below.
				if (line.find("PROGRAM") != string::npos)
				{
					location = line.find("PROGRAM");
					line.replace(location, 7, "&");
					//cout << line << endl;
				}
				if (line.find("VAR") != string::npos)
				{
					location = line.find("VAR");
					line.replace(location, 3, "^");
					//cout << line << endl;
				}
				if (line.find("BEGIN") != string::npos)
				{
					location = line.find("BEGIN");
					line.replace(location, 5, "@");
					//cout << line << endl;
				}
				if (line.find("END.") != string::npos)
				{
					location = line.find("END.");
					line.replace(location, 5, "#");
					//cout << line << endl;
				}
				if (line.find("PRINT") != string::npos)
				{
					location = line.find("PRINT");
					line.replace(location, 5, "!");

					size_t found = line.find("\"");
					location = found;
					//cout << location << endl;
					found = line.find("\"", found + 1);
					//cout << found << endl;
					found = found - location;
					line.erase(location + 1, found-1);
					//cout << line << endl;
				}
				if (line.find("INTEGER") != string::npos)
				{
					location = line.find("INTEGER");
					line.replace(location, 7, "%");
					//cout << line << endl;
				}

				//cout << "LOOKING AT LINE: " << line << endl;
				//parser(line);		//Call the LL Parser
				//cout << endl;

				parseline << line;
				
			}
		}
		line = parseline.str();
		//std::cout << line << endl;
		fail = parser(line, fail);
	}

	if (fail != 0)
	{
		std::cout << endl;
		std::cout << "There we issues during the building process, please check and correct the problems and try again!" << std::endl;
		return 1;
	}
	else
	{
		std::cout << "There were no problems compiling this file." << endl;
		return 0;
	}
}

int parser(string line, int fail)
{

	//std::string word;		//will hold the string we want to verify
	int x = 0;				//x-counter
	int y = 0;				//y-counter
	int wc = 0;				//word counter
	int temp = 0;
	char flag = '$';
	int error = 0;			//Holds number of errors

	bool match = true;

	char top;				//temporary pointer for the top of the stack

	std::stack<char> items;  //stack for the variables

	ofstream outfile;
	outfile.open("build.txt");

	if (outfile.fail())
	{
		perror("There was an error opening the file: build.txt");		//ERROR opening the file
		exit(-1);
	}

	//Table holds the grammar table values, in our case the 0's are empty, and 4 is our lambda
	int table[24][34] = {
		99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 16, /*17*/99, /*18*/99, 99,
		19, 19, 19, 19, 19, 19, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		20, 20, 20, 20, 20, 20, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		20, 20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 88, 88, 88, 88, 99, 88, 88, 88, 88, 88, 99, 99, 99, 99, 99, 99, 99, 99,
		22, 22, 22, 22, 22, 22, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		23, 23, 23, 23, 23, 23, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 88, 99, 24, 99, 99, 99, 99, 99, 99, 99, 99,
		25, 25, 25, 25, 25, 25, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 25, 99, 99, 99, 99, 99,
		26, 26, 26, 26, 26, 26, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 26, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		27, 27, 27, 27, 27, 27, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 28, 99, 99, 99, 99, 99,
		99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 29, 99, 99, 99, 99, 99,
		99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 88, 99, 99, 99, 99, 30, 99, 99, 99, 99, 99, 99, 99,
		88, 88, 88, 88, 88, 88, 88, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 31, 99, 99, 99, 99, 99, 99, 99,
		32, 32, 32, 32, 32, 32, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 99, 99, 99, 99, 33, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		32, 32, 32, 32, 32, 32, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 34, 35, 99, 99, 99, 88, 99, 99, 88, 99, 99, 99, 88, 99, 99, 99, 99, 99,
		36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 99, 99, 99, 99, 36, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 37, 38, 99, 99, 99, 99, 88, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		19, 19, 19, 19, 19, 19, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 99, 99, 99, 99, 40, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		99, 99, 99, 99, 99, 99, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		99, 99, 99, 99, 99, 99, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 88, 88, 88, 88, 99, 99, 99, 99, 88, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 42, 99, 99, 99, 99,
		99, 99, 99, 99, 99, 99, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		10, 11, 12, 13, 14, 15, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99
	};

	/*Please Note below are the values and there meanings for the above table
	the value on LHS is the value in table, the RHS is its actual value.
	99 - empty
	88 - lambda
	0 - 0
	1 - 1
	2 - 2
	3 - 3
	4 - 4
	5 - 5
	6 - 6
	7 - 7
	8 - 8
	9 - 9
	a - 10
	b - 11
	c - 12
	d - 13
	e - 14
	f - 15
	16 - &<pname>; ^<dec-list>; @<stat-list>#
	17 - ^<dec-list>;@<stat-list>#
	18 - @<stat-list>#
	19 - <id>
	20 - <letter><id-ext>
	21 - <digit><id-ext>
	22 - <dec>:<type>
	23 - <id><dec-ext>
	24 - ,<dec>
	25 - <stat-list-ext><stat>
	26 - <stat>;
	27 - <assign>
	28 - <print>
	29 - PRINT( <output> );
	30 - <output-ext><id>
	31 - 'string',
	32 - <id>=<expr>;
	33 - <term><expr-ext>
	34 - +<term><expr-ext>
	35 - -<term><expr-ext>
	36 - <factor><term-ext>
	37 - *<factor><term-ext>
	38 - /<factor><term-ext>
	39 - <number>
	40 - ( <expr> )
	41 - <digit><number-ext>
	42 - INTEGER
	*/

	/*for (int x = 0; x < 5; x++)
	{
	for (int y = 0; y < 8; y++)
	{
	cout << " " << table[x][y] << " ";
	}
	cout << endl;
	}*/

	//std::cout << "Please Enter a word to check acceptance in the language:";
	//std::cin >> word;		//Get User input


	outfile << "Preparing for line: " << line << endl;

	items.push(flag);		//Add the end flag to the stack
	line = line + flag;		//Add the end flag to the end of the input

	items.push('S');		//push the first stage, E into the stack.

	std::cout << "PUSHED: " << items.top() << std::endl;
	outfile << "PUSHED: " << items.top() << std::endl;

	while (!items.empty())		//While the stack is not empty
	{
		if (match == true)
		{
			switch (line[wc])		//FIND WHAT THE COLUMN VALUE IS
			{
			case 'a':
				y = 0;
				break;
			case 'b':
				y = 1;
				break;
			case 'c':
				y = 2;
				break;
			case 'd':
				y = 3;
				break;
			case 'e':
				y = 4;
				break;
			case 'f':
				y = 5;
				break;
			case '0':
				y = 6;
				break;
			case '1':
				y = 7;
				break;
			case '2':
				y = 8;
				break;
			case '3':
				y = 9;
				break;
			case '4':
				y = 10;
				break;
			case '5':
				y = 11;
				break;
			case '6':
				y = 12;
				break;
			case '7':
				y = 13;
				break;
			case '8':
				y = 14;
				break;
			case '9':
				y = 15;
				break;
			case '+':
				y = 16;
				break;
			case '-':
				y = 17;
				break;
			case '*':
				y = 18;
				break;
			case '/':
				y = 19;
				break;
			case '(':
				y = 20;
				break;
			case ')':
				y = 21;
				break;
			case '=':
				y = 22;
				break;
			case ':':
				y = 23;
				break;
			case ';':
				y = 24;
				break;
			case ',':
				y = 25;
				break;
			case '\"':
				y = 26;
				break;
			case '$':
				y = 27;
				break;
			case '!':			//IN THE BUILD FUNCTION: PRINT gets replaced with this
				y = 28;
				break;
			case '%':			//IN THE BUILD FUNCTION: INTEGER gets replaced with this
				y = 29;
				break;
			case '&':			//IN THE BUILD FUNCTION: PROGRAM gets replaced with this
				y = 30;
				break;
			case '^':			//IN THE BUILD FUNCTION: VAR gets replaced with this
				y = 31;
				break;
			case '@':			//IN THE BUILD FUNCTION: BEGIN gets replaced with this
				y = 32;
				break;
			case '#':			//IN THE BUILD FUNCTION: END. gets replaced with this
				y = 33;
				break;
			}


			std::cout << "Y Value is now: " << y << std::endl;
			outfile << "Y Value is now: " << y << std::endl;
			match = false;
		}
		else
		{
			top = items.top();
			if (top == line[wc])
			{
				match = true;
				wc++;

				if (top == '&')
				{
					std::cout << "MATCH FOUND: PROGRAM" << std::endl;
					outfile << "MATCH FOUND: PROGRAM" << std::endl;
				}
				else if (top == '^')
				{
					std::cout << "MATCH FOUND: VAR" << std::endl;
					outfile << "MATCH FOUND: VAR" << std::endl;
				}
				else if(top == '@')
				{
					std::cout << "MATCH FOUND: BEGIN" << std::endl;
					outfile << "MATCH FOUND: BEGIN" << std::endl;
				}
				else if (top == '#')
				{
					std::cout << "MATCH FOUND: END." << std::endl;
					outfile << "MATCH FOUND: END." << std::endl;
				}
				else if (top == '!')
				{
					std::cout << "MATCH FOUND: PRINT" << std::endl;
					outfile << "MATCH FOUND: PRINT" << std::endl;
				}
				else if (top == '%')
				{
					std::cout << "MATCH FOUND: INTEGER" << std::endl;
					outfile << "MATCH FOUND: INTEGER" << std::endl;
				}
				else
				{
					std::cout << "MATCH FOUND: " << top << std::endl;
					outfile << "MATCH FOUND: " << top << std::endl;
				}

				std::cout << "POPPING " << items.top() << std::endl;
				outfile << "POPPING " << items.top() << std::endl;

				items.pop();
				std::cout << std::endl;
				outfile << std::endl;
			}

			if (match == false)
			{

				switch (items.top())			//Find what the row value is
				{
				case 'S':
					x = 0;
					break;
				case 'N':
					x = 1;
					break;
				case 'I':
					x = 2;
					break;
				case 'J':
					x = 3;
					break;
				case 'E':
					x = 4;
					break;
				case 'F':
					x = 5;
					break;
				case 'G':
					x = 6;
					break;
				case 'O':
					x = 7;
					break;
				case 'Q':
					x = 8;
					break;
				case 'K':
					x = 9;
					break;
				case 'P':
					x = 10;
					break;
				case 'U':
					x = 11;
					break;
				case 'V':
					x = 12;
					break;
				case 'A':
					x = 13;
					break;
				case 'X':
					x = 14;
					break;
				case 'Y':
					x = 15;
					break;
				case 'H':
					x = 16;
					break;
				case 'M':
					x = 17;
					break;
				case 'C':
					x = 18;
					break;
				case 'W':
					x = 19;
					break;
				case 'Z':
					x = 20;
					break;
				case 'T':
					x = 21;
					break;
				case 'D':
					x = 22;
					break;
				case 'L':
					x = 23;
					break;
				}

				std::cout << "X value is now: " << x << std::endl;
				outfile << "X value is now: " << x << std::endl;
				/*The values of the above switch statements are:
				LHS is the One letter identifier as shown above, the RHS is the actual meaning
				S - <S>
				N - <pname>
				I - <id>
				J - <id-ext>
				E - <dec-list>
				F - <dec>
				G - <dec-ext>
				O - <stat-list>
				Q - <stat-list-ext>
				K - <stat>
				P - <print>
				U - <output>
				V - <output-ext>
				A - <assign>
				X - <expr>
				Y - <expr-ext>
				H - <term>
				M - <term-ext>
				C - <factor>
				W - <number>
				Z - <number-ext>
				T - <type>
				D - <digit>
				L - <letter>*/


				std::cout << "POPPING " << items.top() << std::endl;
				outfile << "POPPING " << items.top() << std::endl;
				items.pop();
				std::cout << "The table value is " << table[x][y] << std::endl;
				outfile << "The table value is " << table[x][y] << std::endl;
				switch (table[x][y])
				{
				case 99:
					std::cout << "REJECTED: The String you entered is not accepted by this grammar!" << std::endl;
					fail++;
					return fail;	//return to main, failure.
					break;
				case 88:
					break;
				case 0:
					items.push('0');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 1:
					items.push('1');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 2:
					items.push('2');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 3:
					items.push('3');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 4:
					items.push('4');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 5:
					items.push('5');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 6:
					items.push('6');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 7:
					items.push('7');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 8:
					items.push('8');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 9:
					items.push('9');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 10:
					items.push('a');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 11:
					items.push('b');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 12:
					items.push('c');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 13:
					items.push('d');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 14:
					items.push('e');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 15:
					items.push('f');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 16:		//&<pname>; ^<dec-list>; @<stat-list>#
					items.push('#');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('O');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('@');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push(';');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('E');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('^');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push(';');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('N');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('&');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 17:		//^<dec-list>; @<stat-list>#
					items.push('#');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('O');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('@');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push(';');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('E');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('^');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 18:		//@<stat-list>#
					items.push('#');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('O');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('@');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 19:		//<id>
					items.push('I');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 20:		//<letter><id - ext>
					items.push('J');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('L');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 21:		//<digit><id-ext>
					items.push('J');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('D');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 22:		//<dec>:<type>
					items.push('T');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push(':');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('F');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 23:		//<id><dec-ext>
					items.push('G');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('I');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 24:		//,<dec>
					items.push('F');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push(',');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 25:		//<stat - list - ext><stat>
					items.push('K');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('Q');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 26:		//<stat>;
					items.push(';');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('K');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 27:		//<assign>
					items.push('A');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 28:		//<print>
					items.push('P');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 29:		//PRINT( <output> );
					items.push(';');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push(')');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('U');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('(');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('!');										//Since we are limited to one character p will define print
					std::cout << "PUSHED PRINT" << std::endl;
					break;
				case 30:		//<output-ext><id>
					items.push('I');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('V');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 31:		//'string',
					items.push(',');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('\"');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('\"');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 32:		// <id>=<expr>;
					items.push(';');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('X');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('=');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('I');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 33:		//<term><expr-ext>
					items.push('Y');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('H');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 34:		//+<term><expr-ext>
					items.push('Y');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('H');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('+');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 35:		//-<term><expr-ext>
					items.push('Y');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('H');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('-');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 36:		//<factor><term-ext>
					items.push('M');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('C');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 37:		//*<factor><term-ext>
					items.push('M');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('C');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('*');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 38:		// /<factor><term-ext>
					items.push('M');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('C');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('/');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 39:		//<number>
					items.push('W');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 40:		//( <expr> )
					items.push(')');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('X');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('(');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 41:		//<digit><number-ext>
					items.push('Z');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					items.push('D');
					std::cout << "PUSHED " << items.top() << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
					break;
				case 42:		//INTEGER
					items.push('%');
					std::cout << "PUSHED INTEGER" << std::endl;
					outfile << "PUSHED " << items.top() << std::endl;
				}
			}

		}
	}

	//SKIP:

	while (!items.empty())
	{
		items.pop();
	}

	std::cout << "ACCEPTED: The word you entered is accepted by this grammar!" << std::endl;
	outfile << "ACCEPTED: The word you entered is accepted by this grammar!" << std::endl;

	return fail;
}

int find(string need, string variables[], int fail)
{
	ifstream infile("corrected.txt");
	bool match = false;
	string line;
	

	while (getline(infile, line))
	{

		if (line.find(need) != std::string::npos) {
			match = true;
		}

	}

	if (match == false)
	{
		std::cout << need << " is expected, please check that it is not missing or mispelled!" << std::endl;
		fail++;
	}
	else
	{
		match = false;
	}

	infile.close();
	return fail;
}


int findvar(string variables[], int fail)
{

	ifstream infile;
	string varline2;
	infile.open("corrected.txt");
	queue<string> vars;


	string varline;


	if (infile.is_open())
	{
		while (getline(infile, varline))
		{
			if (varline.find("VAR") != std::string::npos){  // IF var is found...
				cout << "Found Var\n";

				getline(infile, varline2);     // get one more line, this has the variables


				char* myString = new char[varline2.size() + 1];

				myString[varline2.size()] = 0;

				memcpy(myString, varline2.c_str(), varline2.size());

				//cout << "mystring is:"<<myString<<endl;
				//char* p;	//Looking at http://stackoverflow.com/questions/20210468/strtok-s-was-not-declared-in-this-scope
				//for information, strtok_s needed to be used, and requires this pointer. Unsure as to function.

				char* tok;
				char* p;

				tok = strtok_s(myString, " ,", &p);

				//vars.push(tok);



				while (tok) {

					//cout << "tok" << endl;

					//printf ("Token: %s\n", tok);

					//if we reach ":", we have gotten all the variables
					if (!strcmp(tok, ":")){

						break;

					}

					//push token, which is a variable, onto variable queue
					vars.push(tok);

					tok = strtok_s(NULL, " ,", &p);



				}


			}

		}
	}

	int j = 0;
	while (!vars.empty()){
		// output the variables
		cout << "vars.front is:" << vars.front() << endl;
		variables[j] = vars.front();
		j++;

		vars.pop();
	}
	infile.close();
	return fail;

}



int unknownid(string variables[], int fail)
{
	ifstream infile;
	infile.open("corrected.txt");

	string line;
	string temp;
	bool term = false;
	int exists = 0;
	int locat = 0;

	while (getline(infile, line))
	{
		if (line.find("BEGIN") != string::npos)
		{
			break;
		}
	}

	while (getline(infile, line))
	{
		if (line.find("PRINT(") != string::npos)
		{
			if (line.find(",") != string::npos)
			{
				line.erase(0, 8);
			}
			else
			{

			}
		}
		size_t p = line.find_first_not_of(" \t");	//Remove Whitespace from line
		line.erase(0, p);
		p = line.find_last_not_of(" \t");
		if (string::npos != p)
		{
			line.erase(p + 1);
		}

		//cout << line << endl;

		for (int x = 0; x < line.length(); x++)
		{
			if (line[x] != '=' || line[x] != ' ')
			{
				temp = temp + line[x];
			}
			if (line[x] == '=' || line[x] == ' ')
			{
				term = true;
			}

			if (term == true)
			{
				size_t p = temp.find_first_not_of(" \t");	//Remove Whitespace from temp
				temp.erase(0, p);
				p = temp.find_last_not_of(" \t");
				if (string::npos != p)
				{
					temp.erase(p + 1);
				}

				if (temp.back() == '=' || temp.back() == ')')
				{
					temp.pop_back();
				}

				std::cout << temp << std::endl;

				

				if (!isdigit(temp[0]))		//check is first place of the temp string is a number
				{
					if (isalpha(temp[0]))	//check is first place of the temp string is a alphabetical character
					{
						for (int f = 0; f < 20; f++)
						{
							if (variables[f] == temp || temp == "eb")
							{
								exists++;
								//cout << exists << endl;
							}
						}
					}
				}

				if (exists == 0)
				{
					std::cout << "UNKNOWN IDENTIFIER FOUND, identifier is not declared" << std::endl;
					fail++;
					exists = 0;
				}

			
				temp.clear();
				line.clear();
				term = false;
				exists = 0;
				locat = 0;
			}
		}
	}
	infile.close();
	return fail;
}

int semicolon(int fail)
{
	ifstream infile;
	infile.open("corrected.txt");
	string line;
	char last = NULL;
	int length = 0;
	if (infile.fail())									//Fail/Error Checking for file opening Fstream.
	{
		std::cout << "Input file could not be opened.\n";
		exit(-1);
	}
	
	while (getline(infile, line))
	{
		size_t p = line.find_first_not_of(" \t");	//Remove Whitespace from variables
		line.erase(0, p);
		p = line.find_last_not_of(" \t");
		if (string::npos != p)
		{
			line.erase(p + 1);
		}
		length = line.length();
		last = line[length];
		if (line.find("VAR") != string::npos || line.find("BEGIN") != string::npos || line.find("END.") != string::npos)
		{
			//DO NOTHING
		}
		else if (last == ';')
		{
			std::cout << "ERROR: ; is Missing" << std::endl;
		}

		
	}
	infile.close();
	return fail;

}

void convert()
{
	//cout << "Ok trying to convert";

	ifstream infile;
	string line;
	string line2;
	string lineStmt;
	string temp;
	string temp2;
	string tempForPop;
	string programName;
	string tempCString;
	infile.open("corrected.txt");
	queue<string> vars;
	queue<string> statements;

	if (infile.fail())	 //Fail/Error Checking for file opening Fstream.

	{

		cout << "Corrected Input file could not be opened.\n";

		exit(-1);

	}


	int k = 0;
	while (getline(infile, line)){
		cout << "k:" << k << "\n";
		//cout << line;

		if (!line.find("PROGRAM")){
			cout << "Found PROGRAM\n";
			//cout << line;

			temp = line;


			//delete "PROGRAM "
			size_t found = line.find_first_of(" ");
			temp = line.substr(found, 99);

			//erase ";" from end
			size_t locSemiCol = temp.find(';');
			temp.erase(locSemiCol, 99);

			//delete any leading white space
			temp.erase(0, temp.find_first_not_of(" \t"));

			//get the program name and save it
			programName = temp;

		}




		if (line.find("VAR") != std::string::npos){  // IF var is found...
			cout << "Found Var\n";
			/* //Removing this which kinda works temporarily

			getline(infile, line2);

			while (line2 == ""){        // getlines until lines arent blank

			getline(infile, line2);

			}
			*/
			getline(infile, line2);     // get one more line, this has the variables

			cout<<"line2 is"<<line2<<endl;


			char* myString = new char[line2.size() + 1];

			myString[line2.size()] = 0;

			memcpy(myString, line2.c_str(), line2.size());

			//cout << "mystring is:"<<myString<<endl;
			//char* p;	//Looking at http://stackoverflow.com/questions/20210468/strtok-s-was-not-declared-in-this-scope
			//for information, strtok_s needed to be used, and requires this pointer. Unsure as to function.

			char* tok;
			char*p;
			string temp;

			tok = strtok_s(myString, " ,:\t", &p);

			//vars.push(tok);
			while (tok) {

				//temp = tok;
				
				//if(
				//cout << "tok" << endl;

				//printf ("Token: %s\n", tok);

				//if we reach ":", we have gotten all the variables
		
				if (strcmp(tok, "INTEGER;") == 0){

					break;

				}

				
				

				//push token, which is a variable, onto variable queue
				vars.push(tok);

				tok = strtok_s(NULL, " ,:\t", &p);



			}
			/*

			while(!vars.empty()){

			cout << vars.front() << endl;

			vars.pop();

			}

			*/

		}
		else{
			cout << "Var not found\n";
		}
		if (line.find("BEGIN") != std::string::npos){ //Entered BEGIN section


			cout << "Found Begin\n";
			while (getline(infile, lineStmt)){

				if (lineStmt.find("END.") != std::string::npos){
					cout << "End found";
					//If we reach the end, stop otherwise,

					//statements.push(lineStmt);

					//cout << lineStmt;

				}
				else{
					//Grab the statement
					statements.push(lineStmt);

				}

			}



		}
		else{
			cout << "BEGIN not found\n";
		}

		k++;
	}



	//int a,b,d;



	ostringstream outSS;

	outSS << programName << ".cpp";
	//outSS << programName << ".txt";


	ofstream outfile(outSS.str());

	outfile << "#include <iostream>" << endl;

	outfile << "using namespace std;" << endl;

	outfile << "int main()" << endl;

	outfile << "{" << endl;

	outfile << "int ";

	while (!vars.empty()){
		// output the variables
		outfile << vars.front();

		if (vars.size() != 1){

			outfile << ",";

		}

		vars.pop();

	}
	//output ";" at the end of the variables
	outfile << ";" << endl;



	string tempStr;
	string tempStr2;
	string tempStr3;
	string tempStr4;
	size_t location1;
	size_t location2;
	size_t location3;
	size_t length;



	queue<string> printVars;


	while (!statements.empty()){

		tempStr = statements.front();
		if (tempStr.find("PRINT") != string::npos){

			//cout << "PRINT FOUND"<<endl;
			// 	the lines we are getting are in the form:
			//  print("eb=" , eb)   ;
			// so we find location of first and second ( )
			// and grab the inside contents
			location1 = tempStr.find("(");

			location2 = tempStr.find(")");

			length = location2 - location1;

			tempStr2 = tempStr.substr(location1 + 1, length - 1);

			//cout << "HEY TEMPSTR2 IS:"<<tempStr2<<endl;

			// We need to replace the "," with a "<<" for the cout statement to process correctly
			// "eb=" , eb

			if(tempStr2.find(",") != string::npos){

				location3 = tempStr2.find(",");
				tempStr2.replace(location3, string(",").length(), "<<");
			}


			//test, get rid of leading whitespace if there is any
			tempStr2.erase(0, tempStr2.find_first_not_of(" \t\r\n"));

			outfile << "cout<<" << tempStr2 << ";" << endl;

		}

		else{

			tempStr3 = statements.front();

			//delete any leading whitespace if found
			tempStr3.erase(0, tempStr3.find_first_not_of(" \t\r\n"));

			outfile << tempStr3 << endl;
			cout << "TempStr3 is:" << tempStr3 << endl;

		}
		statements.pop();
	}

	outfile << "return 0;" << endl << "}";
	outfile.close();

}
