#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

typedef struct edge {
	char c; // matching Character
	int next; // destination Node
};

typedef struct node {
	vector<edge> edgev; // Vector of Node's Edges
	string token = "";	// if "" then this node is non-terminal, else terminal
} node;

vector<node> nodeList;
vector<string> keywordList = { "if", "else", "while", "class", "return" };
vector<string> variableList = { "int", "char", "boolean", "String" };
vector<string> booleanList = { "true", "false" };

char terminal_type(char c) {
	/*
	*	return l when letter
	*	return 1 when digit except 0
	*	return 0 when digit 0
	*	return \t when white space
	*	return itself when another input
	*/

	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
		return 'l';
	}
	else if (c >= '1' && c <= '9') {
		return '1';
	}
	else if (c == '\t' || c == '\n' || c == ' ') {
		return '\t';
	}
	else {
		return c;
	}
}

void addEdge(int nodeListIndex, char c, int nextIndex) {
	/*
	*					add Edge that,
	*
	*		                       c
	*		nodeListIndex ---------------------> nextIndex
	*
	*
	*
	*/
	edge temp;
	temp.c = c;
	temp.next = nextIndex;
	nodeList[nodeListIndex].edgev.push_back(temp);
}

void setTerminal(int nodeIndex, string token) {
	/*
	*	Set (nodeIndex) th Node's token
	*/
	nodeList[nodeIndex].token = token;
}

bool isThere(vector<string>& objVector, string keyword) {
	/*
	*	Return keyword exist in objVector or not
	*/
	for (int i = 0; i < objVector.size(); i++) {
		if (objVector[i] == keyword) {
			return true;
		}
	}
	return false;
}

void pushToken(vector<string>& list, string token, string detail) {
	/*
	*	Function for when given token, push that to list.
	*/

	string str;

	if (token == "WHITESPACE") {
		return;
	}

	/*
	*	For identifying '-' Symbol
	*/

	else if (token == "MINUS_SIGNED_INTEGER") {
		if (list.size() == 0) {
			str = "SIGNED_INTEGER";
		}
		else {
			if (list[list.size() - 2] == "RPAREN" || list[list.size() - 2] == "SIGNED_INTEGER" || list[list.size() - 2] == "IDENTIFIER") {
				list.push_back("ARITHMETIC_OPERATOR");
				list.push_back("-");
				list.push_back("SIGNED_INTEGER");
				list.push_back(detail.substr(1, detail.size()));
				return;
			}
			else {
				str = "SIGNED_INTEGER";
			}
		}
	}

	/*
	*	if given token is STRING, change it to more detail keyword
	*/

	else if (token == "STRING") {
		if (isThere(keywordList, detail))	str = "KEYWORD";
		else if (isThere(variableList, detail))	str = "VARIABLE_TYPE";
		else if (isThere(booleanList, detail))	str = "BOOLEAN_STRING";
		else str = "IDENTIFIER";
	}
	else if (token == "SINGLE_CHARACTER") {
		str = token;
		detail = detail.substr(1, 1);
	}
	else if (token == "LITERAL_STRING") {
		str = token;
		detail = detail.substr(1, detail.size() - 2);
	}
	else {
		str = token;
	}
	list.push_back(str);
	list.push_back(detail);
}

void init() {
	/*
	*	Add Node and Edge
	*/

	node temp_node;
	temp_node.token = "";
	int start_terminal_length1 = 25;
	int count = 0;
	int i;
	for (i = 0; i < 45; i++) {
		nodeList.push_back(temp_node);
	}
	// >=, >
	addEdge(0, '>', 2);
	setTerminal(2, "COMPARISON_OPERATOR");
	addEdge(2, '=', 9);
	setTerminal(9, "COMPARISON_OPERATOR");

	// <, <=
	addEdge(0, '<', 3);
	setTerminal(3, "COMPARISON_OPERATOR");
	addEdge(3, '=', 10);
	setTerminal(10, "COMPARISON_OPERATOR");

	//!=
	addEdge(0, '!', 4);
	addEdge(4, '=', 11);
	setTerminal(11, "COMPARISON_OPERATOR");

	// =, ==
	addEdge(0, '=', 5);
	setTerminal(5, "ASSIGNMENT_OPERATOR");
	addEdge(5, '=', 12);
	setTerminal(12, "COMPARISON_OPERATOR");

	// single char
	addEdge(0, '\'', 6);
	addEdge(6, 'l', 13);
	addEdge(6, '\t', 14);
	addEdge(6, '0', 15);
	addEdge(6, '1', 16);
	addEdge(13, '\'', 17);
	addEdge(14, '\'', 17);
	addEdge(15, '\'', 17);
	addEdge(16, '\'', 17);
	setTerminal(17, "SINGLE_CHARACTER");

	// literal string
	addEdge(0, '\"', 7);
	addEdge(7, 'l', 18);
	addEdge(7, '0', 19);
	addEdge(7, '1', 20);
	addEdge(7, '\t', 21);
	addEdge(7, '\"', 22);
	for (i = 18; i <= 21; i++) {
		addEdge(i, 'l', 18);
		addEdge(i, '0', 19);
		addEdge(i, '1', 20);
		addEdge(i, '\t', 21);
		addEdge(i, '\"', 22);
	}
	setTerminal(22, "LITERAL_STRING");

	//positive digit
	addEdge(0, '1', 8);
	setTerminal(8, "SIGNED_INTEGER");
	addEdge(8, '1', 8);
	addEdge(8, '0', 8);

	//negative digit or minus + positive digit
	addEdge(0, '-', 23);
	setTerminal(23, "ARITHMETIC_OPERATOR");
	addEdge(23, '1', 24);
	setTerminal(24, "MINUS_SIGNED_INTEGER");
	addEdge(24, '0', 24);
	addEdge(24, '1', 24);

	setTerminal(8, "SIGNED_INTEGER");

	//letter & identifier
	addEdge(0, 'l', 1);
	addEdge(0, '_', 1);
	addEdge(1, 'l', 1);
	addEdge(1, '_', 1);
	addEdge(1, '1', 1);
	addEdge(1, '0', 1);
	setTerminal(1, "STRING");

	// terminals with depth 1
	setTerminal(start_terminal_length1 + count, "SIGNED_INTEGER");
	addEdge(0, '0', start_terminal_length1 + count++);

	setTerminal(start_terminal_length1 + count, "LBRACE");
	addEdge(0, '{', start_terminal_length1 + count++);

	setTerminal(start_terminal_length1 + count, "RBRACE");
	addEdge(0, '}', start_terminal_length1 + count++);

	setTerminal(start_terminal_length1 + count, "LBRACKET");
	addEdge(0, '[', start_terminal_length1 + count++);

	setTerminal(start_terminal_length1 + count, "RBRACKET");
	addEdge(0, ']', start_terminal_length1 + count++);

	setTerminal(start_terminal_length1 + count, "LPAREN");
	addEdge(0, '(', start_terminal_length1 + count++);

	setTerminal(start_terminal_length1 + count, "RPAREN");
	addEdge(0, ')', start_terminal_length1 + count++);

	setTerminal(start_terminal_length1 + count, "WHITESPACE");
	addEdge(0, '\t', start_terminal_length1 + count++);

	for (i = 0; i < 3; i++) {
		setTerminal(start_terminal_length1 + count + i, "ARITHMETIC_OPERATOR");
	}
	addEdge(0, '+', start_terminal_length1 + count++);
	addEdge(0, '*', start_terminal_length1 + count++);
	addEdge(0, '/', start_terminal_length1 + count++);

	setTerminal(start_terminal_length1 + count, "TERMINATING_SYMBOL");
	addEdge(0, ';', start_terminal_length1 + count++);

	setTerminal(start_terminal_length1 + count, "SEPERATING_SYMBOL");
	addEdge(0, ',', start_terminal_length1 + count++);
}

int main(int argc, char* argv[]) {
	string str;
	string temp;
	vector<string> result_token;
	fstream fs;

	int idx;
	int startIdx;
	int nowNode;
	int i;
	int prevTokenNum;
	int line;
	bool flag;
	bool flag_error = false;

	if (argc == 1) {
		cout << "Please input file name." << endl << "(ex) lexical_analyzer.exe test.java";
		return 1;
	}

	fs.open(argv[1]);
	if (!fs) {
		cout << "Can't find file " << argv[1];
		return 1;
	}


	init();

	line = 0;
	while (!fs.eof()) {
		getline(fs, str);
		line++;
		idx = 0;
		startIdx = 0;
		nowNode = 0;
		prevTokenNum = result_token.size();

		while (idx <= str.length()) {
			flag = false;
			if (idx < str.length()) {
				for (i = 0; i < nodeList[nowNode].edgev.size(); i++) {
					if (terminal_type(str[idx]) == nodeList[nowNode].edgev[i].c) {
						flag = true;
						nowNode = nodeList[nowNode].edgev[i].next;
						break;
					}
				}
			}
			if (!flag) {
				if (nowNode == 0 || nodeList[nowNode].token == "") {
					if (idx == str.length()) {
						break;
					}
					//Exception
					cout << "Error occurs with symbol [ " << str[idx] << " ] ";
					if (result_token.size() - prevTokenNum > 0) {
						cout << " following [ " << result_token.back() << " ]" << endl;
					}
					else {
						cout << " at first of line : " << endl;
					}
					cout << "at line " << line << " : " << str << endl << endl << endl;
					flag_error = true;
					return 1;
					//break;
				}
				temp = str.substr(startIdx, idx - startIdx);
				pushToken(result_token, nodeList[nowNode].token, temp);
				nowNode = 0;
				startIdx = idx;
				continue;
			}

			idx++;
		}
		if (flag_error)
			break;
	}

	// output
	for (i = 1; i < result_token.size(); i += 2) {
		cout << result_token[i - 1] << " : ";

		if (result_token[i] == "\n") {
			cout << "NEW_LINE" << endl;
		}
		else if (result_token[i] == "\t") {
			cout << "TAB" << endl;
		}
		else if (result_token[i] == " ") {
			cout << "BLANK" << endl;
		}
		else {
			cout << result_token[i] << endl;
		}
	}

	fs.close();

	string output_name = argv[1];
	int point_index = 0;
	for (i = output_name.length(); i > 0; i--) {
		if (output_name[i] == '.') {
			point_index = i;
			break;
		}
	}

	output_name = output_name.substr(0, i) + "_out.txt";

	if (!flag_error) {
		fs.open(output_name, fstream::out);
		if (!fs) {
			cout << "Can't make file " << "output_name";
			return 1;
		}
		for (i = 0; i < result_token.size(); i += 2) {
			fs << result_token[i] << "\t" << result_token[i + 1] << endl;
		}
		fs.close();
	}

	cout << endl << output_name << " created." << endl;
}