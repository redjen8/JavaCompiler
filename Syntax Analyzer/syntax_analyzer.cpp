#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#define STATE_NUM 86

using namespace std;
vector <string> parseStack;
int parseStackTop;
int currentState;

const string epsilon = "";
const string endSymbol = "$";
const int searchfail = 0x7fffffff;

vector<string> inputStream;
vector <string> originalToken;
int streamIdx = 0;

vector<pair<string, int>> CFGList;

typedef struct State
{
	vector<pair<string, string>> actionTable;
	vector<pair<string, int>> gotoTable;

	void add_action(string terminal, string nextAction)
	{
		actionTable.push_back(make_pair(terminal, nextAction));
	}

	void add_goto(string input, int nextState)
	{
		gotoTable.push_back(make_pair(input, nextState));
	}

	int terminal_getNext(string input)
	{
		vector<pair<string, string>>::iterator iter = actionTable.begin();
		string oper = "";
		for (; iter != actionTable.end(); iter++)
		{
			if ((*iter).first == input)
			{
				oper = (*iter).second;
				break;
			}
		}
		if (iter == actionTable.end())
		{
			return searchfail; //search failure
		}
		else
		{
			if (oper == "acc") //end parse
			{
				return -1;
			}
			if (oper[0] == 's') // shift
			{
				string t = oper.substr(1, oper.length());
				currentState = stoi(t);
				parseStack.push_back(input);
				parseStack.push_back(t);
				parseStackTop += 2;
				streamIdx++;
				return currentState;
			}
			else if (oper[0] == 'r') // reduce
			{
				string t = oper.substr(1, oper.length());
				for (int i = 0; i < 2 * CFGList[stoi(t)].second; i++)
				{
					parseStack.pop_back();
				}
				parseStack.push_back(CFGList[stoi(t)].first);
				parseStackTop -= (2 * CFGList[stoi(t)].second);
				parseStackTop++;
				return stoi(t);
			}
		}
		return searchfail;
	}

	int nonterminal_getNext(string input)
	{
		vector<pair<string, int>>::iterator iter;
		for (iter = gotoTable.begin(); iter != gotoTable.end(); iter++)
		{
			if ((*iter).first == input)
			{
				currentState = (*iter).second;
				parseStack.push_back(to_string(currentState));
				parseStackTop++;
				return currentState;
			}
		}
		return searchfail; //search failure
	}
}State;
State stateArr[STATE_NUM];

void initCFG()
{
	CFGList.push_back(make_pair("S", 1));
	CFGList.push_back(make_pair("CODE", 2));
	CFGList.push_back(make_pair("CODE", 2));
	CFGList.push_back(make_pair("CODE", 2));
	CFGList.push_back(make_pair("CODE", 0));
	CFGList.push_back(make_pair("VDECL", 3));
	CFGList.push_back(make_pair("VDECL", 3));
	CFGList.push_back(make_pair("ASSIGN", 3));
	CFGList.push_back(make_pair("RHS", 1));
	CFGList.push_back(make_pair("RHS", 1));
	CFGList.push_back(make_pair("RHS", 1));
	CFGList.push_back(make_pair("RHS", 1));
	CFGList.push_back(make_pair("EXPR", 3));
	CFGList.push_back(make_pair("EXPR", 1));
	CFGList.push_back(make_pair("EXPRT", 3));
	CFGList.push_back(make_pair("EXPRT", 1));
	CFGList.push_back(make_pair("EXPRF", 3));
	CFGList.push_back(make_pair("EXPRF", 1));
	CFGList.push_back(make_pair("EXPRF", 1));
	CFGList.push_back(make_pair("FDECL", 9));
	CFGList.push_back(make_pair("ARG", 3));
	CFGList.push_back(make_pair("ARG", 0));
	CFGList.push_back(make_pair("MOREARGS", 4));
	CFGList.push_back(make_pair("MOREARGS", 0));
	CFGList.push_back(make_pair("BLOCK", 2));
	CFGList.push_back(make_pair("BLOCK", 0));
	CFGList.push_back(make_pair("STMT", 1));
	CFGList.push_back(make_pair("STMT", 2));
	CFGList.push_back(make_pair("STMT", 8));
	CFGList.push_back(make_pair("STMT", 7));
	CFGList.push_back(make_pair("COND", 3));
	CFGList.push_back(make_pair("COND", 1));
	CFGList.push_back(make_pair("ELSE", 4));
	CFGList.push_back(make_pair("ELSE", 0));
	CFGList.push_back(make_pair("RETURN", 3));
	CFGList.push_back(make_pair("CDECL", 5));
	CFGList.push_back(make_pair("ODECL", 2));
	CFGList.push_back(make_pair("ODECL", 2));
	CFGList.push_back(make_pair("ODECL", 0));
}

void initStateAction()
{
	stateArr[0].add_action("vtype", "s5");
	stateArr[0].add_action("class", "s6");
	stateArr[0].add_action(endSymbol, "r4");

	stateArr[1].add_action(endSymbol, "acc");

	stateArr[2].add_action("vtype", "s5");
	stateArr[2].add_action("class", "s6");
	stateArr[2].add_action(endSymbol, "r4");

	stateArr[3].add_action("vtype", "s5");
	stateArr[3].add_action("class", "s6");
	stateArr[3].add_action(endSymbol, "r4");

	stateArr[4].add_action("vtype", "s5");
	stateArr[4].add_action("class", "s6");
	stateArr[4].add_action(endSymbol, "r4");

	stateArr[5].add_action("id", "s10");

	stateArr[6].add_action("id", "s12");

	stateArr[7].add_action(endSymbol, "r1");

	stateArr[8].add_action(endSymbol, "r2");

	stateArr[9].add_action(endSymbol, "r3");

	stateArr[10].add_action("semi", "s13");
	stateArr[10].add_action("assign", "s15");
	stateArr[10].add_action("lparen", "s14");

	stateArr[11].add_action("semi", "s16");

	stateArr[12].add_action("lbrace", "s17");

	stateArr[13].add_action("vtype", "r5");
	stateArr[13].add_action("id", "r5");
	stateArr[13].add_action("rbrace", "r5");
	stateArr[13].add_action("if", "r5");
	stateArr[13].add_action("while", "r5");
	stateArr[13].add_action("return", "r5");
	stateArr[13].add_action("class", "r5");
	stateArr[13].add_action(endSymbol, "r5");

	stateArr[14].add_action("vtype", "s19");
	stateArr[14].add_action("rparen", "r21");

	stateArr[15].add_action("id", "s28");
	stateArr[15].add_action("literal", "s22");
	stateArr[15].add_action("character", "s23");
	stateArr[15].add_action("boolstr", "s24");
	stateArr[15].add_action("lparen", "s27");
	stateArr[15].add_action("num", "s29");

	stateArr[16].add_action("vtype", "r6");
	stateArr[16].add_action("id", "r6");
	stateArr[16].add_action("rbrace", "r6");
	stateArr[16].add_action("if", "r6");
	stateArr[16].add_action("while", "r6");
	stateArr[16].add_action("return", "r6");
	stateArr[16].add_action("class", "r6");
	stateArr[16].add_action(endSymbol, "r6");

	stateArr[17].add_action("vtype", "s5");
	stateArr[17].add_action("rbrace", "r38");

	stateArr[18].add_action("rparen", "s33");

	stateArr[19].add_action("id", "s34");

	stateArr[20].add_action("semi", "r7");

	stateArr[21].add_action("semi", "r8");

	stateArr[22].add_action("semi", "r9");

	stateArr[23].add_action("semi", "r10");

	stateArr[24].add_action("semi", "r11");

	stateArr[25].add_action("semi", "r13");
	stateArr[25].add_action("addsub", "s35");
	stateArr[25].add_action("rparen", "r13");

	stateArr[26].add_action("semi", "r15");
	stateArr[26].add_action("addsub", "r15");
	stateArr[26].add_action("multidiv", "s36");
	stateArr[26].add_action("rparen", "r15");

	stateArr[27].add_action("id", "s28");
	stateArr[27].add_action("lparen", "s27");
	stateArr[27].add_action("num", "s29");

	stateArr[28].add_action("semi", "r17");
	stateArr[28].add_action("addsub", "r17");
	stateArr[28].add_action("multidiv", "r17");
	stateArr[28].add_action("rparen", "r17");

	stateArr[29].add_action("semi", "r18");
	stateArr[29].add_action("addsub", "r18");
	stateArr[29].add_action("multidiv", "r18");
	stateArr[29].add_action("rparen", "r18");

	stateArr[30].add_action("rbrace", "s38");

	stateArr[31].add_action("vtype", "s5");
	stateArr[31].add_action("rbrace", "r38");

	stateArr[32].add_action("vtype", "s5");
	stateArr[32].add_action("rbrace", "r38");

	stateArr[33].add_action("lbrace", "s41");

	stateArr[34].add_action("rparen", "r23");
	stateArr[34].add_action("comma", "s43");

	stateArr[35].add_action("id", "s28");
	stateArr[35].add_action("lparen", "s27");
	stateArr[35].add_action("num", "s29");

	stateArr[36].add_action("id", "s28");
	stateArr[36].add_action("lparen", "s27");
	stateArr[36].add_action("num", "s29");

	stateArr[37].add_action("rparen", "s46");

	stateArr[38].add_action("vtype", "r35");
	stateArr[38].add_action("class", "r35");
	stateArr[38].add_action(endSymbol, "r35");

	stateArr[39].add_action("rbrace", "r36");

	stateArr[40].add_action("rbrace", "r37");

	stateArr[41].add_action("vtype", "s53");
	stateArr[41].add_action("id", "s54");
	stateArr[41].add_action("rbrace", "r25");
	stateArr[41].add_action("if", "s51");
	stateArr[41].add_action("while", "s52");
	stateArr[41].add_action("return", "r25");

	stateArr[42].add_action("rparen", "r20");

	stateArr[43].add_action("vtype", "s55");

	stateArr[44].add_action("semi", "r12");
	stateArr[44].add_action("rparen", "r12");

	stateArr[45].add_action("semi", "r14");
	stateArr[45].add_action("addsub", "r14");
	stateArr[45].add_action("rparen", "r14");

	stateArr[46].add_action("semi", "r16");
	stateArr[46].add_action("addsub", "r16");
	stateArr[46].add_action("multidiv", "r16");
	stateArr[46].add_action("rparen", "r16");

	stateArr[47].add_action("return", "s57");

	stateArr[48].add_action("vtype", "s53");
	stateArr[48].add_action("id", "s54");
	stateArr[48].add_action("rbrace", "r25");
	stateArr[48].add_action("if", "s51");
	stateArr[48].add_action("while", "s52");
	stateArr[48].add_action("return", "r25");

	stateArr[49].add_action("vtype", "r26");
	stateArr[49].add_action("id", "r26");
	stateArr[49].add_action("rbrace", "r26");
	stateArr[49].add_action("if", "r26");
	stateArr[49].add_action("while", "r26");
	stateArr[49].add_action("return", "r26");

	stateArr[50].add_action("semi", "s59");

	stateArr[51].add_action("lparen", "s60");

	stateArr[52].add_action("lparen", "s61");

	stateArr[53].add_action("id", "s62");

	stateArr[54].add_action("assign", "s15");

	stateArr[55].add_action("id", "s63");

	stateArr[56].add_action("rbrace", "s64");

	stateArr[57].add_action("id", "s28");
	stateArr[57].add_action("literal", "s22");
	stateArr[57].add_action("character", "s23");
	stateArr[57].add_action("boolstr", "s24");
	stateArr[57].add_action("lparen", "s27");
	stateArr[57].add_action("num", "s29");

	stateArr[58].add_action("rbrace", "r24");
	stateArr[58].add_action("return", "r24");

	stateArr[59].add_action("vtype", "r27");
	stateArr[59].add_action("id", "r27");
	stateArr[59].add_action("rbrace", "r27");
	stateArr[59].add_action("if", "r27");
	stateArr[59].add_action("while", "r27");
	stateArr[59].add_action("return", "r27");

	stateArr[60].add_action("boolstr", "s67");

	stateArr[61].add_action("boolstr", "s67");

	stateArr[62].add_action("semi", "s13");
	stateArr[62].add_action("assign", "s15");

	stateArr[63].add_action("rparen", "r23");
	stateArr[63].add_action("comma", "s43");

	stateArr[64].add_action("vtype", "r19");
	stateArr[64].add_action("rbrace", "r19");
	stateArr[64].add_action("class", "r19");
	stateArr[64].add_action(endSymbol, "r19");

	stateArr[65].add_action("semi", "s70");

	stateArr[66].add_action("rparen", "s71");

	stateArr[67].add_action("rparen", "r31");
	stateArr[67].add_action("comp", "s72");

	stateArr[68].add_action("rparen", "s73");

	stateArr[69].add_action("rparen", "r22");

	stateArr[70].add_action("rbrace", "r34");

	stateArr[71].add_action("lbrace", "s74");

	stateArr[72].add_action("boolstr", "s67");

	stateArr[73].add_action("lbrace", "s76");

	stateArr[74].add_action("vtype", "s53");
	stateArr[74].add_action("id", "s54");
	stateArr[74].add_action("rbrace", "r25");
	stateArr[74].add_action("if", "s51");
	stateArr[74].add_action("while", "s52");
	stateArr[74].add_action("return", "r25");

	stateArr[75].add_action("rparen", "r30");

	stateArr[76].add_action("vtype", "s53");
	stateArr[76].add_action("id", "s54");
	stateArr[76].add_action("rbrace", "r25");
	stateArr[76].add_action("if", "s51");
	stateArr[76].add_action("while", "s52");
	stateArr[76].add_action("return", "r25");

	stateArr[77].add_action("rbrace", "s79");

	stateArr[78].add_action("rbrace", "s80");

	stateArr[79].add_action("vtype", "r33");
	stateArr[79].add_action("id", "r33");
	stateArr[79].add_action("rbrace", "r33");
	stateArr[79].add_action("if", "r33");
	stateArr[79].add_action("while", "r33");
	stateArr[79].add_action("else", "s82");
	stateArr[79].add_action("return", "r33");

	stateArr[80].add_action("vtype", "r29");
	stateArr[80].add_action("id", "r29");
	stateArr[80].add_action("rbrace", "r29");
	stateArr[80].add_action("if", "r29");
	stateArr[80].add_action("while", "r29");
	stateArr[80].add_action("return", "r29");

	stateArr[81].add_action("vtype", "r28");
	stateArr[81].add_action("id", "r28");
	stateArr[81].add_action("rbrace", "r28");
	stateArr[81].add_action("if", "r28");
	stateArr[81].add_action("while", "r28");
	stateArr[81].add_action("return", "r28");

	stateArr[82].add_action("lbrace", "s83");

	stateArr[83].add_action("vtype", "s53");
	stateArr[83].add_action("id", "s54");
	stateArr[83].add_action("rbrace", "r25");
	stateArr[83].add_action("if", "s51");
	stateArr[83].add_action("while", "s52");
	stateArr[83].add_action("return", "r25");

	stateArr[84].add_action("rbrace", "s85");

	stateArr[85].add_action("vtype", "r32");
	stateArr[85].add_action("id", "r32");
	stateArr[85].add_action("rbrace", "r32");
	stateArr[85].add_action("if", "r32");
	stateArr[85].add_action("while", "r32");
	stateArr[85].add_action("return", "r32");
}

void initStateGoto()
{
	stateArr[0].add_goto("CODE", 1);
	stateArr[0].add_goto("VDECL", 2);
	stateArr[0].add_goto("FDECL", 3);
	stateArr[0].add_goto("CDECL", 4);

	stateArr[2].add_goto("CODE", 7);	
	stateArr[2].add_goto("VDECL", 2);
	stateArr[2].add_goto("FDECL", 3);
	stateArr[2].add_goto("CDECL", 4);

	stateArr[3].add_goto("CODE", 8);
	stateArr[3].add_goto("VDECL", 2);
	stateArr[3].add_goto("FDECL", 3);
	stateArr[3].add_goto("CDECL", 4);

	stateArr[4].add_goto("CODE", 9);
	stateArr[4].add_goto("VDECL", 2);
	stateArr[4].add_goto("FDECL", 3);
	stateArr[4].add_goto("CDECL", 4);

	stateArr[5].add_goto("ASSIGN", 11);

	stateArr[14].add_goto("ARG", 18);

	stateArr[15].add_goto("RHS", 20);
	stateArr[15].add_goto("EXPR", 21);
	stateArr[15].add_goto("EXPRT", 25);
	stateArr[15].add_goto("EXPRF", 26);

	stateArr[17].add_goto("VDECL", 31);
	stateArr[17].add_goto("FDECL", 32);
	stateArr[17].add_goto("ODECL", 30);

	stateArr[27].add_goto("EXPR", 37);
	stateArr[27].add_goto("EXPRT", 25);
	stateArr[27].add_goto("EXPRF", 26);

	stateArr[31].add_goto("VDECL", 31);
	stateArr[31].add_goto("FDECL", 32);
	stateArr[31].add_goto("ODECL", 39);

	stateArr[32].add_goto("VDECL", 31);
	stateArr[32].add_goto("FDECL", 32);
	stateArr[32].add_goto("ODECL", 40);

	stateArr[34].add_goto("MOREARGS", 42);
	
	stateArr[35].add_goto("EXPR", 44);
	stateArr[35].add_goto("EXPRT", 25);
	stateArr[35].add_goto("EXPRF", 26);

	stateArr[36].add_goto("EXPRT", 45);
	stateArr[36].add_goto("EXPRF", 26);

	stateArr[41].add_goto("VDECL", 49);
	stateArr[41].add_goto("ASSIGN", 50);
	stateArr[41].add_goto("BLOCK", 47);
	stateArr[41].add_goto("STMT", 48);

	stateArr[47].add_goto("RETURN", 56);

	stateArr[48].add_goto("VDECL", 49);
	stateArr[48].add_goto("ASSIGN", 50);
	stateArr[48].add_goto("BLOCK", 58);
	stateArr[48].add_goto("STMT", 48);

	stateArr[53].add_goto("ASSIGN", 11);

	stateArr[57].add_goto("RHS", 65);
	stateArr[57].add_goto("EXPR", 21);
	stateArr[57].add_goto("EXPRT", 25);
	stateArr[57].add_goto("EXPRF", 26);

	stateArr[60].add_goto("COND", 66);

	stateArr[61].add_goto("COND", 68);

	stateArr[63].add_goto("MOREARGS", 69);

	stateArr[72].add_goto("COND", 75);

	stateArr[74].add_goto("VDECL", 49);
	stateArr[74].add_goto("ASSIGN", 50);
	stateArr[74].add_goto("BLOCK", 77);
	stateArr[74].add_goto("STMT", 48);

	stateArr[76].add_goto("VDECL", 49);
	stateArr[76].add_goto("ASSIGN", 50);
	stateArr[76].add_goto("BLOCK", 78);
	stateArr[76].add_goto("STMT", 48);

	stateArr[79].add_goto("ELSE", 81);

	stateArr[83].add_goto("VDECL", 49);
	stateArr[83].add_goto("ASSIGN", 50);
	stateArr[83].add_goto("BLOCK", 84);
	stateArr[83].add_goto("STMT", 48);
}

bool isNumber(const string& str)
{
	for (char const& c : str) {
		if (std::isdigit(c) == 0) return false;
	}
	return true;
}

void printParseStack()
{
	for (vector<string>::iterator iter = parseStack.begin(); iter != parseStack.end(); iter++)
	{
		cout << *iter << ' ';
	}
	cout << endl;
}

void insertInputStream(string a, string b)
{
	if (a == "VARIABLE_TYPE")
	{
		inputStream.push_back("vtype");
		originalToken.push_back(b);
	}
	else if (a == "SIGNED_INTEGER")
	{
		inputStream.push_back("num");
		originalToken.push_back(b);
	}
	else if (a == "SINGLE_CHARCTER")
	{
		inputStream.push_back("character");
		originalToken.push_back(b);
	}
	else if (a == "BOOLEAN_STRING")
	{
		inputStream.push_back("boolstr");
		originalToken.push_back(b);
	}
	else if (a == "LITERAL_STRING")
	{
		inputStream.push_back("literal");
		originalToken.push_back(b);
	}
	else if (a == "IDENTIFIER")
	{
		inputStream.push_back("id");
		originalToken.push_back(b);
	}
	else if (a == "KEYWORD")
	{
		if (b == "if") inputStream.push_back("if");
		else if (b == "else") inputStream.push_back("else");
		else if (b == "while") inputStream.push_back("while");
		else if (b == "return") inputStream.push_back("return");
		else if (b == "class") inputStream.push_back("class");
		originalToken.push_back(b);
	}
	else if (a == "ARITHMETIC_OPERATOR")
	{
		if (b == "+" || b == "-") inputStream.push_back("addsub");
		else if (b == "*" || b == "/") inputStream.push_back("multidiv");
		originalToken.push_back(b);
	}
	else if (a == "ASSIGNMENT_OPERATOR")
	{
		inputStream.push_back("assign");
		originalToken.push_back(b);
	}
	else if (a == "COMPARISON_OPERATOR")
	{
		inputStream.push_back("comp");
		originalToken.push_back(b);
	}
	else if (a == "TERMINATING_SYMBOL")
	{
		inputStream.push_back("semi");
		originalToken.push_back(b);
	}
	else if (a == "SEPERATING_SYMBOL")
	{
		inputStream.push_back("comma");
		originalToken.push_back(b);
	}
	else if (a == "LPAREN")
	{
		inputStream.push_back("lparen");
		originalToken.push_back(b);
	}
	else if (a == "RPAREN")
	{
		inputStream.push_back("rparen");
		originalToken.push_back(b);
	}
	else if (a == "LBRACE")
	{
		inputStream.push_back("lbrace");
		originalToken.push_back(b);
	}
	else if (a == "RBRACE")
	{
		inputStream.push_back("rbrace");
		originalToken.push_back(b);
	}
}

int main(int argc, char* argv[])
{
	ifstream inf;
	string inputBuf;
	if (argc == 1)
	{
		cout << "please input file name. " << endl << "ex) syntax_anaylzer test_out.txt";
		return 1;
	}
	inf.open(argv[1]);
	if (!inf)
	{
		cout << "Cannot find file " << argv[1] << endl;
		return 1;
	}

	initCFG();
	initStateAction();
	initStateGoto();
	parseStack.push_back("0");
	parseStackTop = 0;
	currentState = 0;
	
	while (!inf.eof())
	{
		getline(inf, inputBuf);
		for (int i = 0; i < (int)inputBuf.length(); i++)
		{
			if (inputBuf[i] == '\t')
			{
				string tokentype = inputBuf.substr(0, i);
				string tokenname = inputBuf.substr(i+1, inputBuf.length());
				insertInputStream(tokentype, tokenname);
			}
		}
	}
	inputStream.push_back(endSymbol);
	originalToken.push_back(endSymbol);
	for (int i = 0; i < (int)inputStream.size(); i++)
	{
		//cout << inputStream[i] << ' ' << originalToken[i] << endl;
	}
	cout << endl;
	while (true)
	{
		string nextInputSymbol = inputStream[streamIdx];
		if (isNumber(parseStack[parseStackTop])) // 스택의 최상단 원소가 상태인 경우
		{
			//상태의 action 취해준다
			int r = stateArr[stoi(parseStack[parseStackTop])].terminal_getNext(nextInputSymbol);
			if (r == searchfail) // 실패한 경우
			{
				cout << endl << "REJECT!" << endl << endl;
				cout << "An error has been occurred during terminal transition. Terminating Parse..." << endl;
				cout << "Next input symbol was line " << streamIdx << ": \'" << originalToken[streamIdx] << "\', recognized as \'" << nextInputSymbol << "\'" << endl;
				break;
				//error
			}
			if (r == -1)
			{
				cout << endl << "ACCEPT! Parse End.." << endl;
				break;
			}
			//printParseStack();
		}
		else //스택의 최상단 원소가 nonTerminal인 경우
		{
			string rs = parseStack[parseStackTop - 1];
			int r = stateArr[stoi(rs)].nonterminal_getNext(parseStack[parseStackTop]);
			if (r == searchfail) // 실패한 경우
			{
				cout << endl << "REJECT!" << endl << endl;
				cout << "An error has been occurred during nonterminal transition. Terminating Parse..." << endl;
				cout << "Next input symbol was line " << streamIdx << ": \'" << originalToken[streamIdx] << "\', recognized as \'" << nextInputSymbol << "\'" << endl;
				break;
				//error
			}
			//printParseStack();
		}
	}
	return 0;
}