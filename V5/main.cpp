/*-----------------------------------------
CS323 Final Project
Group 2

Abeer Salam
Eric Rosenzweig
Karthnik Dundigalla
Dong Ho
-----------------------------------------*/

#include <algorithm>  
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <queue>

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
	int input = 0;
	int filestat = 1;		//set to 0 or 1, 1 if compiling issues are present
							//initially set to 1 to force a once run-through
	

	cout << endl;
	cout << "Group 2 Final Project" << endl;
	cout << "Name of File: final.txt" << endl;

	while (input != 4)			//User Menu
	{
		cout << endl << "Select an option:" << endl;
		cout << "1. Clean file" << endl;
		cout << "2. Build file" << endl;
		cout << "3. Convert file to C++" << endl;
		cout << "4. Quit" << endl;
		cout << "Selection: ";
		cin >> input;
		cout << endl << endl;

		switch (input)
		{
		case 1:
			cout << "Cleaning file..." << endl << endl;
			trimm();											//Removes all comments and extra spacing 
			cout << "Comments and excessive spacing removed." << endl;
			break;
		case 2:
			cout << "Building file..." << endl;
			filestat = build(variables);						//Build the file
			cout << "Build complete" << endl;
			break;
		case 3:
				cout << "Converting to C++..." << endl;
				convert();										//Convert the file to c++
				cout << "Conversion complete" << endl;
				break;
		case 4:
			break;

		default:
			cout << "Invalid selection, please select an appropriate option (1 - 4) from the menu" << endl;
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


	if (infile.fail())											//File open error check
	{
		cout << "Error: Could not open input file\n";
		exit(-1);
	}
	if (outfile.fail())
	{
		cout << "Error: Could not open output file\n";
		exit(-1);
	}

	if (infile.is_open())
	{
		//Skip until *) is found
		while (getline(infile, skip))
		{
			if (skip.find("*)") != std::string::npos)
			{
				break;
			}

		}

		int length = 0;
		while (getline(infile, line))
		{

			cout << line << endl;

			if(!line.empty())
			{
				do
				{
					//Find start of comment
					temp = line;
					length = line.length();

					//Search for (*, if found, delete the rest of the line
					size_t loc = temp.find("(*");
					if (loc < 100)
						temp.replace(loc, temp.length() - loc, "");

					bool push = false;

					temp.erase(0, temp.find_first_not_of(" \t"));

					//search temp as a character array, if alphanumeric then line is non-blank, it can be pushed
					for (int i = 0; i < temp.length(); i++)
					{
						if (isalnum(temp[i]))
						{
							push = true;
							break;
						}
						else
							push = false;
					}

					temp.append("\n");
					if (push)Q.push(temp);

					cout << "Temp is " << temp << endl;
				}while(getline(infile, line));
			}

			else
			{
				continue;
			}

			//Tokenize each line

			size_t x = Q.size();

			for (int i = 0; i < x; i++)
			{
				string s = Q.front();

				//Check if current string s is a blank line
				if (s.find_first_not_of(" ") != std::string::npos)
				{
					//Do nothing, continue through
				}
				else
				{
					//S must be blank, skip and continue to next iteration of loop
					Q.pop();
					continue;
				}

				char* p;

				char* str = new char[s.size() + 1];
				str[s.size()] = 0;
				memcpy(str, s.c_str(), s.size());	//Memcpy copies the string into a character array
				char* tok;
				tok = strtok_s(str, " ", &p);		//Strtok tokenizes each line by spaces

				while (tok != NULL)
				{
					cout << "tok is: " << tok << endl;
					ss << tok << " ";
					tok = strtok_s(NULL, " ", &p);
				}
				Q.pop();
			}
			infile.close();


			// Open output file and write to file.
			outfile << ss.str();
			outfile.close();
		}
	}
}


int build(string variables[])
{
	int location = 0;						//store the location of the keywords (if they are in the string)

	int fail = 0;							//Fail counter, > 0 indicates an error
	string line;
	stringstream pline;

	ifstream infile("corrected.txt");					//Open the trimmed file

	if (infile.fail())
	{
		perror("There was an error opening the file: corrected.txt");		//Error opening the file
		exit(-1);
	}

	if (infile.is_open())
	{
		fail = find("PROGRAM", variables, fail);	//Find PROGRAM, increment fail if not found
		fail = find("VAR", variables, fail);		//" VAR, " " " " "
		fail = find("BEGIN", variables, fail);		//" BEGIN, " " " " "
		fail = find("END.", variables,fail);		//" END, " " " " "
		fail = find("INTEGER", variables, fail);	//" INTEGER, " " " " "

		if (fail == 0)
		{
			while (getline(infile, line))
			{
				//Remove all spacing
				line.erase(remove_if(line.begin(), line.end(), isspace), line.end());

				//Condense all terms to 1 character, to place on stack
				if (line.find("PROGRAM") != string::npos)
				{
					location = line.find("PROGRAM");
					line.replace(location, 7, "&");
				}
				if (line.find("VAR") != string::npos)
				{
					location = line.find("VAR");
					line.replace(location, 3, "^");
				}
				if (line.find("BEGIN") != string::npos)
				{
					location = line.find("BEGIN");
					line.replace(location, 5, "@");
				}
				if (line.find("END.") != string::npos)
				{
					location = line.find("END.");
					line.replace(location, 5, "#");
				}
				if (line.find("PRINT") != string::npos)
				{
					location = line.find("PRINT");
					line.replace(location, 5, "!");

					if(size_t found = line.find("\"") != string::npos)
					{
						location = found;
						found = line.find("\"", found + 1);
						found = found - location;
						line.erase(location + 1, found-1);
					}
					else if(size_t found = line.find("\'") != string::npos)
					{						
						location = found;
						found = line.find("\'", found + 1);
						found = found - location;
						line.erase(location + 1, found-1);

						while(line.find("\'") != string::npos)
						{
							line.replace(line.find("\'"), 1, "\"");
						}
					}
				}
				if (line.find("INTEGER") != string::npos)
				{
					location = line.find("INTEGER");
					line.replace(location, 7, "%");
				}

				pline << line;
			}
		}
		line = pline.str();
		fail = parser(line, fail);
	}

	if (fail != 0)
	{
		cout << endl;
		cout << "There we issues during the building process, please check and correct the problems and try again!" << endl;
		return 1;
	}
	else
	{
		cout << "There were no problems compiling this file." << endl;
		return 0;
	}
}

int parser(string line, int fail)
{

	int x = 0;				//X coordinate
	int y = 0;				//Y coordinate
	int wordcount = 0;		//Word counter
	int temp = 0;
	char flag = '$';
	int error = 0;			//Error counter

	bool match = true;

	char top;				//Temp pointer for the top of the stack

	std::stack<char> itemstack;  //Stack for the variables

	ofstream outfile;
	outfile.open("build.txt");

	if (outfile.fail())
	{
		perror("There was an error opening the file: build.txt");		//ERROR opening the file
		exit(-1);
	}

	//Table holds the grammar table values, in our case the 0's are empty, and 88 is our lambda/terminal
	int table[24][34] = 
	{
		99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 16, 99, 99, 99,
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
		99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 88, 88, 99, 99, 99, 99, 30, 99, 99, 99, 99, 99, 99, 99,
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
	/* Below are the values associated with each value in the above table.
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

	outfile << "Preparing for line: " << line << endl;

	itemstack.push(flag);		//Add the end flag to the stack
	line = line + flag;			//Add the end flag to the end of the input

	itemstack.push('S');		//Push the first stage, E into the stack

	cout << "PUSHED: " << itemstack.top() << endl;
	outfile << "PUSHED: " << itemstack.top() << endl;

	while (!itemstack.empty())		//While the stack is not empty
	{
		if (match == true)
		{
			switch (line[wordcount])		//Find the column value
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
			case '!':			//build() uses ! for PRINT
				y = 28;
				break;
			case '%':			//build() uses % for INTEGER
				y = 29;
				break;
			case '&':			//build() uses & for PROGRAM
				y = 30;
				break;
			case '^':			//build() uses ^ for VAR
				y = 31;
				break;
			case '@':			//build() uses @ for BEGIN
				y = 32;
				break;
			case '#':			//build() uses # for END
				y = 33;
				break;
			}

			cout << "Y Value is now: " << y << endl;
			outfile << "Y Value is now: " << y << endl;
			match = false;
		}
		else
		{
			top = itemstack.top();
			if (top == line[wordcount])
			{
				match = true;
				wordcount++;

				if (top == '&')
				{
					cout << "MATCH FOUND: PROGRAM" << endl;
					outfile << "MATCH FOUND: PROGRAM" << endl;
				}
				else if (top == '^')
				{
					cout << "MATCH FOUND: VAR" << endl;
					outfile << "MATCH FOUND: VAR" << endl;
				}
				else if(top == '@')
				{
					cout << "MATCH FOUND: BEGIN" << endl;
					outfile << "MATCH FOUND: BEGIN" << endl;
				}
				else if (top == '#')
				{
					cout << "MATCH FOUND: END." << endl;
					outfile << "MATCH FOUND: END." << endl;
				}
				else if (top == '!')
				{
					cout << "MATCH FOUND: PRINT" << endl;
					outfile << "MATCH FOUND: PRINT" << endl;
				}
				else if (top == '%')
				{
					cout << "MATCH FOUND: INTEGER" << endl;
					outfile << "MATCH FOUND: INTEGER" << endl;
				}
				else
				{
					cout << "MATCH FOUND: " << top << endl;
					outfile << "MATCH FOUND: " << top << endl;
				}

				cout << "POPPING " << itemstack.top() << endl;
				outfile << "POPPING " << itemstack.top() << endl;

				itemstack.pop();
				cout << endl;
				outfile << endl;
			}

			if (!match)
			{

				switch (itemstack.top())			//Find what the row value is
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

				cout << "X value is now: " << x << endl;
				outfile << "X value is now: " << x << endl;
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

				//Used this as a pseudo - log file
				cout << "POPPING " << itemstack.top() << endl;
				outfile << "POPPING " << itemstack.top() << endl;
				itemstack.pop();
				cout << "The table value is " << table[x][y] << endl;
				outfile << "The table value is " << table[x][y] << endl;
				switch (table[x][y])
				{
				case 99:
					cout << "REJECTED: String is not acceptable by this grammar" << endl;
					fail++;
					return fail;	//return to main, failure
					break;
				case 88:
					break;
				case 0:
					itemstack.push('0');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 1:
					itemstack.push('1');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 2:
					itemstack.push('2');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 3:
					itemstack.push('3');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 4:
					itemstack.push('4');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 5:
					itemstack.push('5');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 6:
					itemstack.push('6');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 7:
					itemstack.push('7');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 8:
					itemstack.push('8');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 9:
					itemstack.push('9');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 10:
					itemstack.push('a');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 11:
					itemstack.push('b');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 12:
					itemstack.push('c');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 13:
					itemstack.push('d');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 14:
					itemstack.push('e');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 15:
					itemstack.push('f');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 16:		//&<pname>; ^<dec-list>; @<stat-list>#
					itemstack.push('#');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('O');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('@');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push(';');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('E');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('^');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push(';');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('N');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('&');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 17:		//^<dec-list>; @<stat-list>#
					itemstack.push('#');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('O');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('@');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push(';');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('E');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('^');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 18:		//@<stat-list>#
					itemstack.push('#');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('O');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('@');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 19:		//<id>
					itemstack.push('I');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 20:		//<letter><id - ext>
					itemstack.push('J');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('L');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 21:		//<digit><id - ext>
					itemstack.push('J');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('D');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 22:		//<dec>:<type>
					itemstack.push('T');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push(':');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('F');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 23:		//<id><dec - ext>
					itemstack.push('G');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('I');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 24:		//,<dec>
					itemstack.push('F');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push(',');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 25:		//<stat - list - ext><stat>
					itemstack.push('K');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('Q');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 26:		//<stat>;
					itemstack.push(';');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('K');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 27:		//<assign>
					itemstack.push('A');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 28:		//<print>
					itemstack.push('P');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 29:		//PRINT( <output> );
					itemstack.push(';');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push(')');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('U');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('(');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('!');										//! bound to print
					cout << "PUSHED PRINT" << endl;
					break;
				case 30:		//<output - ext><id>
					itemstack.push('I');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('V');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 31:		//'string',
					itemstack.push(',');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('\"');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('\"');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 32:		// <id> = <expr>;
					itemstack.push(';');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('X');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('=');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('I');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 33:		//<term><expr - ext>
					itemstack.push('Y');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('H');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 34:		//+<term><expr - ext>
					itemstack.push('Y');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('H');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('+');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 35:		//-<term><expr - ext>
					itemstack.push('Y');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('H');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('-');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 36:		//<factor><term - ext>
					itemstack.push('M');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('C');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 37:		//*<factor><term - ext>
					itemstack.push('M');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('C');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('*');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 38:		// /<factor><term - ext>
					itemstack.push('M');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('C');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('/');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 39:		//<number>
					itemstack.push('W');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 40:		//( <expr> )
					itemstack.push(')');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('X');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('(');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 41:		//<digit><number - ext>
					itemstack.push('Z');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					itemstack.push('D');
					cout << "PUSHED " << itemstack.top() << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
					break;
				case 42:		//INTEGER
					itemstack.push('%');
					cout << "PUSHED INTEGER" << endl;
					outfile << "PUSHED " << itemstack.top() << endl;
				}
			}

		}
	}

	while (!itemstack.empty())
		itemstack.pop();

	cout << "ACCEPTED: Entered word is valid" << endl;
	outfile << "ACCEPTED: Entered word is valid" << endl;

	return fail;
}

int find(string need, string variables[], int fail)
{
	ifstream infile("corrected.txt");
	bool match = false;
	string line;
	

	while (getline(infile, line))
	{

		if (line.find(need) != std::string::npos)
			match = true;
	}

	if (match == false)
	{
		cout << need << " is expected" << endl;
		fail++;
	}
	else
		match = false;

	infile.close();
	return fail;
}

int findvar(string variables[], int fail)
{
	ifstream infile;
	string varline;
	string varline2;
	infile.open("corrected.txt");
	queue<string> vars;

	if (infile.is_open())
	{
		while (getline(infile, varline))
		{
			if (varline.find("VAR") != std::string::npos)
			{
				cout << "Found Var\n";
				getline(infile, varline2);

				char* myString = new char[varline2.size() + 1];

				myString[varline2.size()] = 0;
				memcpy(myString, varline2.c_str(), varline2.size());

				char* tok;
				char* p;

				tok = strtok_s(myString, " ,", &p);

				while (tok) {

					//: marks the end of the variables
					if (!strcmp(tok, ":"))
						break;

					//push variable token to queue
					vars.push(tok);

					tok = strtok_s(NULL, " ,", &p);
				}
			}
		}
	}

	int i = 0;
	while (!vars.empty())
	{
		// output the variables
		cout << "vars.front is:" << vars.front() << endl;
		variables[i] = vars.front();
		i++;

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
			break;
	}

	while (getline(infile, line))
	{
		if (line.find("PRINT(") != string::npos)
		{
			if (line.find(",") != string::npos)
				line.erase(0, 8);
		}
		size_t p = line.find_first_not_of(" \t");	//Remove white space from line
		line.erase(0, p);
		p = line.find_last_not_of(" \t");
		
		if (string::npos != p)
			line.erase(p + 1);

		for (int x = 0; x < line.length(); x++)
		{
			if (line[x] != '=' || line[x] != ' ')
				temp = temp + line[x];

			if (line[x] == '=' || line[x] == ' ')
				term = true;

			if (term == true)
			{
				size_t p = temp.find_first_not_of(" \t");	//Remove white space from temp
				temp.erase(0, p);
				p = temp.find_last_not_of(" \t");
				if (string::npos != p)
					temp.erase(p + 1);

				if (temp.back() == '=' || temp.back() == ')')
					temp.pop_back();

				cout << temp << endl;


				if (!isdigit(temp[0]))		//check if first slot is a digit
				{
					if (isalpha(temp[0]))	//check if first slot is a letter
					{
						for (int f = 0; f < 20; f++)
						{
							if (variables[f] == temp || temp == "eb")
								exists++;
						}
					}
				}

				if (exists == 0)
				{
					cout << "UNKNOWN IDENTIFIER FOUND, identifier is not declared" << endl;
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
	if (infile.fail())									//Error check
	{
		cout << "Input file could not be opened.\n";
		exit(-1);
	}
	
	while (getline(infile, line))
	{
		size_t p = line.find_first_not_of(" \t");	//Remove white space
		line.erase(0, p);
		p = line.find_last_not_of(" \t");
		if (string::npos != p)
			line.erase(p + 1);

		length = line.length();
		last = line[length];
		if (line.find("VAR") != string::npos || line.find("BEGIN") != string::npos || line.find("END.") != string::npos)
		{
			//DO NOTHING
		}
		else if (last == ';')
		{
			cout << "ERROR: ; is Missing" << endl;
		}
	}
	infile.close();
	return fail;

}

void convert()
{
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

	if (infile.fail())	 //Error check
	{
		cout << "Corrected Input file could not be opened.\n";
		exit(-1);
	}

	int k = 0;
	while (getline(infile, line)){
		cout << "k:" << k << "\n";

		if (!line.find("PROGRAM")){
			cout << "Found PROGRAM\n";

			temp = line;

			//delete "PROGRAM "
			size_t found = line.find_first_of(" ");
			temp = line.substr(found, 99);

			//erase ";"
			size_t locSemiCol = temp.find(';');
			temp.erase(locSemiCol, 99);

			//remove leading white space
			temp.erase(0, temp.find_first_not_of(" \t"));

			//save program name
			programName = temp;
		}


		if (line.find("VAR") != std::string::npos)
		{  
			cout << "Found Var\n";

			getline(infile, line2);     // get one more line, this has the variables

			cout<<"line2 is"<<line2<<endl;


			char* myString = new char[line2.size() + 1];

			myString[line2.size()] = 0;

			memcpy(myString, line2.c_str(), line2.size());

			char* tok;
			char*p;
			string temp;

			tok = strtok_s(myString, " ,:\t", &p);

			while (tok) 
			{
				if (strcmp(tok, "INTEGER;") == 0)
					break;

				//push variable token onto queue
				vars.push(tok);

				tok = strtok_s(NULL, " ,:\t", &p);



			}

		}
		else
		{
			cout << "Var not found\n";
		}
		if (line.find("BEGIN") != std::string::npos)		//Enter BEGIN section
		{
			cout << "Found Begin\n";
			while (getline(infile, lineStmt))
			{
				if (lineStmt.find("END.") != std::string::npos)
					cout << "End found";
				else
				{
					while(lineStmt.find("\'") != string::npos)
						{
							lineStmt.replace(lineStmt.find("\'"), 1, "\"");
						}
					//Grab the statement
					statements.push(lineStmt);
				}
			}
		}
		else
		{
			cout << "BEGIN not found\n";
		}

		k++;
	}

	ostringstream outSS;

	outSS << programName << ".cpp";

	ofstream outfile(outSS.str());

	outfile << "#include <iostream>" << endl;

	outfile << "using namespace std;" << endl;

	outfile << "int main()" << endl;

	outfile << "{" << endl;

	outfile << "int ";

	while (!vars.empty())
	{
		// output the variables
		outfile << vars.front();

		if (vars.size() != 1)
			outfile << ",";

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


	while (!statements.empty())
	{
		tempStr = statements.front();
		if (tempStr.find("PRINT") != string::npos)
		{
			//Obtain contents insid the parentheses
			location1 = tempStr.find("(");

			location2 = tempStr.find(")");

			length = location2 - location1;

			tempStr2 = tempStr.substr(location1 + 1, length - 1);

			//Replace "," with "<<"
			if(tempStr2.find(",") != string::npos)
			{

				location3 = tempStr2.find(",");
				tempStr2.replace(location3, string(",").length(), "<<");
			}

			//test, get rid of leading white space if there is any
			tempStr2.erase(0, tempStr2.find_first_not_of(" \t\r\n"));

			outfile << "cout<<" << tempStr2 << ";" << endl;
		}

		else
		{
			tempStr3 = statements.front();

			//delete any leading white space if found
			tempStr3.erase(0, tempStr3.find_first_not_of(" \t\r\n"));

			outfile << tempStr3 << endl;
			cout << "TempStr3 is:" << tempStr3 << endl;
		}

		statements.pop();
	}

	outfile << "return 0;" << endl << "}";
	outfile.close();
}
