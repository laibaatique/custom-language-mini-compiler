#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <cctype>
using namespace std;

bool isNum(const std::string& s) {
	if (s.empty()) return false;
	for (char c : s) {
		if (!std::isdigit(c))
			return false;
	}
	return true;
}



int pointer = 0;

struct Token {
	string type;
	string value;
};


vector<Token> readTokens(const string& filename)
{
	vector<Token> tokens;
	ifstream file(filename);
	if (!file) {
		cerr << "Error opening tokens file!" << endl;
		return tokens;
	}
	string line;

	while (getline(file, line))
	{
		size_t startPos = 0;
		while ((startPos = line.find('<')) != string::npos)
		{
			//size_t endPos = line.find('>', startPos);
			size_t endPos = startPos;
			while (true)
			{
				endPos = line.find('>', endPos);
				if (endPos == string::npos) break;

				// Check if the character after '>' is a space or end of line
				if (endPos + 1 == line.size() || isspace(line[endPos + 1]))
				{
					break;
				}
				else
				{
					endPos++;
				}
			}
			string content = line.substr(startPos + 1, endPos - startPos - 1);
			size_t commaPos = content.find(',');
			if (commaPos != string::npos)
			{
				string type = content.substr(0, commaPos - 1);
				if (type != "error")
				{
					string value = content.substr(commaPos + 2);
					tokens.push_back({ type, value });
				}
			}
			line = line.substr(endPos + 1);  //remove the processed token
		}
	}
	file.close();
	return tokens;
}

vector<string>readReserveWords(const string& filename)
{
	vector<string>reserveWords;
	ifstream file_reserve(filename);
	if (!file_reserve) {
		cerr << "Error opening reserve words table file!" << endl;
		return reserveWords;
	}
	string line;
	while (getline(file_reserve, line)) {
		int pos = 0;
		while (line[pos] != '.')
		{
			pos++;
		}
		if (pos != string::npos && pos + 2 < line.length())
		{
			string word = line.substr(pos + 2);
			reserveWords.push_back(word);
		}
	}
	file_reserve.close();
	return reserveWords;
}


void printProduction(const wstring& prod, int level, bool isLast) {
	/*for (int i = 0; i < level - 1; i++) {
		wcout << L"    ";
	}*/

	static vector<bool> levels;
	if (level > levels.size()) {
		levels.push_back(isLast);
	}
	else {
		levels[level - 1] = isLast;
	}

	for (int i = 0; i < level-1 ; i++) {
		if (levels[i])
			wcout << L"    ";
		else
			wcout << L"│   ";
	}

	if (level > 1)
	{
		if (isLast)
		{
			wcout << L"└── ";
		
		}
		else
		{
			wcout << L"├── ";
		}
	}
	if (level == 1)
	{
		wcout << " "<<prod << "\n\n";
		return;
	}
	wcout << prod << "\n\n";
}

wstring stringToWString(const std::string& str) 
{
	return wstring(str.begin(), str.end());
}


class Parser
{
	vector<Token>tokens;
	vector<string> reserveWords;
	int index;
public:
	Parser(const vector<Token>& tokens, const vector<string>& reserveWords)
	{
		this->tokens = tokens;
		this->reserveWords = reserveWords;
		index = 0;
	}

	void Advance()
	{
		index++;
	}

	Token getToken()
	{
		if (index < tokens.size())
		{
			return tokens[index];
		}
		return{ "","" };
	}

	string getType()
	{
		if (index < tokens.size())
		{
			return tokens[index].type;
		}
		return "";
	}

	string getValue()
	{
		if (index < tokens.size())
		{
			return tokens[index].value;
		}
		return "";
	}

	void parser()
	{
		if (!Function(1,true)) {
			wcout << L"Syntax Error\n";
		}
		else {
			wcout << L"Parsing succeeded!\n";
		}
	}

	bool Function(int level, bool isLast)
	{
		printProduction(L"Function", level, isLast);
		if (!Type(level+1,false))
		{
			wcout << L"\nType not defined in the Function call but found "<<stringToWString(getValue())<<" of type "<<stringToWString(getType()) << endl;
			return false;
		}

		if (getType() != "id")
		{
			wcout << L"\nExpected identifier in the Function call but found '" << stringToWString(getValue()) << "' of type '" << stringToWString(getType()) << "'\n";
			return false;
		}
		printProduction(L"id", level + 1, false);
		Advance();

		if (getType() != "punc" || getValue() != "(")
		{
			wcout << L"\nExpected '(' in the Function call but found '" << stringToWString(getValue()) << " of type " << stringToWString(getType()) << "\n";
			return false;
		}
		printProduction(L"(", level + 1, false);
		Advance();



		if (!ArgList(level+1,false))
		{
			//cout << "Expected Arguments in the Function call but found '" << getValue() << "'\n";
			return false;
		}

		if (getType() != "punc" || getValue() != ")")
		{
			wcout << L"\nExpected ')' in the Function call but found " << stringToWString(getValue()) << " of type " << stringToWString(getType()) << "\n";
			return false;
		}
		printProduction(L")", level + 1, false);
		Advance();

		if (!CompStmt(level+1,true))
		{
			return false;
		}

		return true;
	}

	bool ArgList(int level,bool isLast)
	{
		printProduction(L"ArgList", level, isLast);
		if (!Arg(level + 1, false))
		{
			return false;
		}

		if (!ArgListPrime(level + 1, true))
		{
			return false;
		}

		return true;
	}

	bool ArgListPrime(int level, bool isLast)
	{
		printProduction(L"ArgListPrime", level, isLast);
		if (getType() == "punc" && getValue() == ",")
		{
			printProduction(L"punc: ,", level + 1, false);
			Advance();
			if (!Arg(level+1,false))
			{
				return false;
			}
			if (!ArgListPrime(level+1,true))
			{
				return false;
			}

		}
		else
		{
			printProduction(L"ε", level + 1, true);
		}
		return true;  //for null

	}

	bool Arg(int level, bool isLast)
	{
		printProduction(L"Arg", level, isLast);
		if (!Type(level + 1, false))
		{
			wcout << L"\nType for the argument is not defined. Found value " << stringToWString(getValue()) << " of type " << stringToWString(getType()) << endl;
			return false;
		}

		if (getType() != "id")
		{
			wcout << L"\nExpected identifier in the Argument but found " << stringToWString(getValue()) << " of type " << stringToWString(getType()) << "\n";
			return false;
		}
		printProduction(L"id", level + 1, true);
		Advance();

		return true;
	}

	bool Declaration(int level, bool isLast)
	{
		if (getType() != "keyword" && (getValue() != "Adadi" || getValue() != "Ashriya" || getValue() != "Harf" || getValue() != "Matn" || getValue() != "Mantiqi"))
		{
			return false;
		}
		printProduction(L"Declaration", level, isLast);
		if (!Type(level+1,false))
		{
		//	cout << "Expected Type in the Declaration but found " << getValue() << endl;
			return false;
		}
	
		if (!IdentList(level+1,false))
		{
			//cout << "Error ÍdentList\n";
			return false;
		}

		if (getType() != "punc" || getValue() != "::")
		{
			wcout << L"\nExpected '::' in the declaration but found " << stringToWString(getValue()) << " of type " << stringToWString(getType()) << "\n";
			return false;
		}
		printProduction(L"::", level+1, true);
		Advance();
		return true;
	}

	bool Type(int level, bool isLast)
	{
		printProduction(L"Type", level, isLast);
		if (getType() == "keyword")
		{
			string val = reserveWords[stoi(getValue()) - 1];
			if (val == "Adadi" || val == "Ashriya" || val == "Harf" || val == "Matn" || val == "Mantiqi")
			{
				printProduction(stringToWString(val), level + 1, true);
				Advance();
				return true;
			}
		}
		return false;
	}

	bool IdentList(int level, bool isLast)
	{
		printProduction(L"IdentList", level, isLast);
		if (getType() != "id")
		{
			wcout << L"\nExpected an identifier in the decalaration but found " << stringToWString(getValue()) << " of type " << stringToWString(getType()) << endl;
			return false;
		}
		printProduction(L"id", level + 1, false);
		Advance();
		if (!IdentListPrime(level+1,true))
		{
			return false;
		}
		return true;
	}

	bool IdentListPrime(int level, bool isLast)
	{
		printProduction(L"IdentListPrime", level, isLast);

		if (getType() == "punc" && getValue() == ",")
		{
			Advance();
			printProduction(L",", level + 1, false);
			if (!IdentList(level + 1, true))
				return false;
		}
		else
		{
			printProduction(L"ε", level + 1, true);
		}
		return true;  //for null
	}

	bool Stmt(int level, bool isLast)
	{
		printProduction(L"Stmt", level, isLast);
		if (isNum(getValue()) && stoi(getValue()) >= 0 && reserveWords[stoi(getValue()) - 1] == "for")
		{
			if (!ForStmt(level + 1, true)) return false;

		}
		else if (isNum(getValue()) && stoi(getValue()) >= 0 && reserveWords[stoi(getValue()) - 1] == "while") {
			if (!WhileStmt(level + 1, true)) return false;
		}
		else if (isNum(getValue()) && stoi(getValue()) >= 0 && reserveWords[stoi(getValue()) - 1] == "Agar") {
			if (!IfStmt(level + 1, true)) return false;
		}
		else if (getValue() == "{") {
			if (!CompStmt(level + 1, true)) return false;
		}
		else if (isNum(getValue()) && stoi(getValue())>=0&& (reserveWords[stoi(getValue()) - 1] == "Adadi" || reserveWords[stoi(getValue()) - 1] == "Ashriya" || reserveWords[stoi(getValue()) - 1] == "Harf" || reserveWords[stoi(getValue()) - 1] == "Matn" || reserveWords[stoi(getValue()) - 1] == "Mantiqi")) {
			if (!Declaration(level + 1, true)) return false;
		}
		else if (getType() == "id"||getValue()=="("||getType()=="num") {

			if (!Expr(level + 1, false)) return false;
			if (getType() != "punc" || getValue() != "::")
			{
				wcout << L"\nExpected '::' after Expr but found " << stringToWString(getValue()) << " of type " << stringToWString(getType()) << endl;
				return false;
			}
			printProduction(L":: ", level + 1, true);
			Advance();
			return true;

		}
		else if (getType() == "punc" && getValue() == "::")
		{
			printProduction(L"::", level + 1, true);
			Advance();
			return true;
		}
		else
		{

			return false;
		}
		//if (ForStmt(level+1,true) || WhileStmt(level + 1, true) || IfStmt(level + 1, true) || CompStmt(level + 1, true) || Declaration(level + 1, true)) return true;

		/*else if (Expr(level+1,false))
		{
			if (getType() != "punc" || getValue() != "::")
			{
				wcout << L"\nExpected '::' after Expr but found " << stringToWString(getValue()) << " of type " << stringToWString(getType()) << endl;
				return false;
			}
			printProduction(L":: ", level + 1, true);
			Advance();
			return true;
		}
		else if (getType() == "punc" && getValue() == "::")
		{
			printProduction(L"::", level + 1, true);
			Advance();
			return true;
		}
		else
		{

			return false;
			}*/

	}

	bool ForStmt(int level,bool isLast)
	{

		if (getType() != "keyword" || reserveWords[stoi(getValue()) - 1] != "for")
		{
			return false;
		}
		printProduction(L"ForStmt", level, isLast);
		printProduction(L"for", level + 1, false);
		Advance();

		if (getType() != "punc" || getValue() != "(")
		{
			wcout << L"Expected '(' after for but found" << stringToWString(getValue()) << " of type " << stringToWString(getType()) << endl;
			return false;
		}
		printProduction(L"(", level + 1, false);
		Advance();
		if (!Expr(level+1,false))
		{
			wcout << L"Error in For expresssion" << endl;
			return false;
		}
		if (getType() != "punc" || getValue() != "::")
		{
			wcout << L"Expected '::' in condition of the for loop but found " << stringToWString(getValue()) << " of type " << stringToWString(getType()) << endl;
			return false;
		}
		printProduction(L"::", level + 1, false);
		Advance();
		if (!OptExpr(level+1,false))
		{
			return false;
		}
		if (getType() != "punc" || getValue() != "::")
		{
			wcout << L"Expected '::' in condition of the for loop but found " << stringToWString(getValue()) << " of type " << stringToWString(getType()) << endl;
			return false;
		}
		printProduction(L"::", level + 1, false);
		Advance();
		if (!OptExpr(level+1,false))
		{
			return false;
		}
		if (getType() != "punc" || getValue() != ")")
		{
			wcout << L"Expected ')' in condition of the for loop but found " << stringToWString(getValue()) << " of type " << stringToWString(getType()) << endl;
			return false;
		}
		printProduction(L")", level + 1, false);
		Advance();
		if (!Stmt(level+1,true))
		{
			wcout << L"Error in For Statement " << endl;
			return false;
		}

		return true;

	}

	bool OptExpr(int level,bool isLast)
	{
		if (getType() == "id" || getValue() == "(" || getType() == "num")
		{
			if (!Expr(level + 1, true))
			{
				return false;
			}
		}
		else
		{
			printProduction(L"ε", level + 1, true);
		}
		return true;//for null
	}

	bool WhileStmt(int level, bool isLast)
	{
		if (getType() != "keyword" || reserveWords[stoi(getValue()) - 1] != "while")
		{
			return false;
		}
		printProduction(L"WhileStmt", level, isLast);
		printProduction(L"while", level + 1, false);
		Advance();
		if (getType() != "punc" || getValue() != "(")
		{
			wcout << L"Expected '(' after while but found " << stringToWString(getValue()) << " of type " << stringToWString(getType()) << endl;
			return false;
		}
		printProduction(L"(", level + 1, false);
		Advance();
		if (!Expr(level+1,false))
		{
			wcout << L"Error in while expression" << endl;
			return false;
		}
		if (getType() != "punc" || getValue() != ")")
		{
			wcout << L"Expected ')' at the end of while loop condition but found " << stringToWString(getValue()) << " of type " << stringToWString(getType()) << endl;
			return false;
		}
		printProduction(L")", level + 1, false);
		Advance();
		if (!Stmt(level+1,true))
		{
			wcout << L"Error in While Stmt " << endl;
			return false;
		}
		return true;
	}

	bool IfStmt(int level,bool isLast)
	{
		if (getType() != "keyword" || reserveWords[stoi(getValue()) - 1] != "Agar")
		{
			return false;
		}
		printProduction(L"IfStmt", level, isLast);
		printProduction(L"Agar", level + 1, false);
		Advance();

		if (getType() != "punc" || getValue() != "(")
		{
			wcout << L"Expected '(' in condition of if but found " << stringToWString(getValue()) << " of type " << stringToWString(getType()) << endl;
			return false;
		}
		printProduction(L"(", level + 1, false);
		Advance();

		if (!Expr(level+1,false))
		{
			wcout << L"Error in expression for if statement" << endl;
			return false;
		}

		if (getType() != "punc" || getValue() != ")")
		{
			wcout << L"Expected ')' in condition of if but found " << stringToWString(getValue()) << " of type " << stringToWString(getType()) << endl;
			return false;
		}
		printProduction(L")", level + 1, false);
		Advance();

		if (!Stmt(level+1,false))
		{
			wcout << L"Error in If statement" << endl;
			return false;
		}

		if (!ElsePart(level+1,true))
		{
			return false;
		}

		return true;
	}

	bool ElsePart(int level,bool isLast)
	{
		printProduction(L"ElsePart", level, isLast);
		if (getType() == "keyword" && reserveWords[stoi(getValue()) - 1] == "Wagarna")
		{
			printProduction(L"Wagarna", level+1, false);
			Advance();
			if (!Stmt(level+1,true))
			{
				return false;
			}
			return true;
		}
		else
		{
			printProduction(L"ε", level + 1, true);
		}
		return true; // for null
	}

	bool CompStmt(int level, bool isLast)
	{
		if (getType() != "punc" || getValue() != "{")
		{
			return false;
		}
		printProduction(L"CompStmt", level, isLast);
		printProduction(L"{", level+1, false);
		Advance();

		if (!StmtList(level+1,false))
		{

			return false;
		}

		if (getType() != "punc" || getValue() != "}")
		{
			wcout << L"Expected '}' in compStmt " << stringToWString(getValue()) << " of type " << stringToWString(getType()) << endl;
			return false;
		}
		printProduction(L"}", level + 1, true);
		Advance();
		return true;
	}

	bool StmtList(int level, bool isLast)
	{
		printProduction(L"StmtList", level, isLast);
		if (!StmtListPrime(level+1,true))
		{
			return false;
		}

		return true;
	}


	bool StmtListPrime(int level, bool isLast)
	{
		printProduction(L"StmtListPrime", level, isLast);
		//if (Stmt(level+1,false))
		//{
		//	
		//	if (!StmtListPrime(level+1,true))
		//	{
		//		return false;
		//	}

		//}
		//else
		//{
		//	printProduction(L"ε", level + 1, true);
		//}
		//return true; // for null

		if (getValue() != "}")
		{
			if (!Stmt(level + 1, false))
				return false;
			if (!StmtListPrime(level + 1, true))
				return false;
		}
		else
		{
			printProduction(L"ε", level + 1, true);
		}
		return true;
	}

	bool Rvalue(int level,bool isLast)
	{
		printProduction(L"Rvalue", level, isLast);
		if (!Mag(level+1,false))
		{

			return false;
		}
		if (!RvaluePrime(level+1,true))
		{

			return false;
		}
	}


	bool RvaluePrime(int level,bool isLast)
	{
		printProduction(L"RvaluePrime", level, isLast);
		/*if (Compare(level+1,false))
		{
			if (!Mag(level+1,false))
			{
				return false;
			}
			if (!RvaluePrime(level+1,true))
			{
				return false;
			}
		}
		printProduction(L"ε", level + 1, true);*/
		if (getType() == "relop" && (getValue() == "==" || getValue() == "<" || getValue() == "<>" ||
			getValue() == "<=" || getValue() == ">=" || getValue() == ">" || getValue() == "!="))
		{
			if (!Compare(level + 1, false)) return false;
			if (!Mag(level + 1, false))
			{
				return false;
			}
			if (!RvaluePrime(level + 1, true))
			{
				return false;
			}

		}
		else
		{
			printProduction(L"ε", level + 1, true);
		}
		return true;
	}

	bool Mag(int level, bool isLast)
	{
		printProduction(L"Mag", level, isLast);
		if (!Term(level+1,false))
		{
			return false;
		}
		if (!MagPrime(level+1,true))
		{
			return false;
		}
	}

	bool MagPrime(int level, bool isLast)
	{
		printProduction(L"MagPrime", level, isLast);
		if (getType() == "op" && getValue() == "+")
		{
			printProduction(L"op: "+stringToWString(getValue()), level+1, false);
			Advance();
			if (!Term(level+1,false))
			{
				return false;
			}
			if (!MagPrime(level+1,true))
			{
				return false;
			}
			return true;
		}
		else if (getType() == "op" && getValue() == "-")
		{
			printProduction(L"op: " + stringToWString(getValue()), level + 1, false);
			Advance();
			if (!Term(level + 1, false))
			{
				return false;
			}
			if (!MagPrime(level + 1, true))
			{
				return false;
			}
			return true;
		}
		else
		{
			printProduction(L"ε", level + 1, true);
		}
		return true;
	}

	bool Term(int level,bool isLast)
	{
		printProduction(L"Term", level, isLast);
		if (!Factor(level+1,false))
		{
			wcout << L"Error in Factor in Term call" << endl;
			return false;
		}
		if (!TermPrime(level+1,true))
		{
			return false;
		}
		return true;
	}

	bool TermPrime(int level,bool isLast)
	{
		printProduction(L"TermPrime", level, isLast);
		if (getType() == "op" && getValue() == "*")
		{
			printProduction(L"op: " + stringToWString(getValue()), level + 1, false);
			Advance();
			if (!Factor(level+1,false))
			{
				wcout << L"Error Factor in TermPrime call " << endl;
				return false;
			}
			if (!TermPrime(level+1,true))
			{
				return false;
			}
			return true;
		}
		else if (getType() == "op" && getValue() == "/")
		{
			printProduction(L"op: " + stringToWString(getValue()), level + 1, false);
			Advance();
			if (!Factor(level+1,false))
			{
				wcout << L"Error Factor in TermPrime call " << endl;
				return false;
			}
			if (!TermPrime(level+1,true))
			{
				return false;
			}
			return true;
		}
		else
		{
			printProduction(L"ε", level + 1, true);
		}
		return true;
	}

	bool Factor(int level,bool isLast)
	{
		printProduction(L"Factor", level, isLast);
		if (getType() == "punc" && getValue() == "(")
		{
			printProduction(L"(", level + 1, false);
			Advance();
			if (!Expr(level+1,false))
			{
				return false;
			}

			if (getType() == "punc" && getValue() == ")")
			{
				printProduction(L")", level + 1, true);
				Advance();
				return true;
			}

			wcout << L"Expected ')' in Factor but found " << stringToWString(getValue()) << " of type " << stringToWString(getType()) << endl;
			return false;
		}
		else if (getType() == "id")
		{
			printProduction(L"id", level + 1, true);
			Advance();
			return true;
		}
		else if (getType() == "num")
		{
			printProduction(L"num: " + stringToWString(getValue()), level + 1, true);
			Advance();
			return true;
		}
		return false;
	}


	bool Expr(int level,bool isLast)
	{
		
		if (getType() == "id")
		{
			printProduction(L"Expr", level, isLast);
			printProduction(L"id", level + 1, false);
			Advance();
			if (!ExprPrime(level+1,true))
			{
				return false;
			}
			return true;
		}
		else if (getType() == "punc" && getValue() == "(")
		{
			printProduction(L"Expr", level, isLast);
			printProduction(L"(" , level + 1, false);
			Advance();
			if (!Expr(level+1,false))
			{
				return false;
			}
			if (getType() != "punc" || getValue() != ")")
			{
				wcout << L"Expected ')' in (Expr) but found " << stringToWString(getValue()) << " of type " << stringToWString(getType()) << endl;
				return false;
			}
			printProduction(L")", level + 1, false);
			Advance();
			if (!APrime(level+1,true))
			{
				return false;
			}
			return true;
		}
		else if (getType() == "num")
		{
			printProduction(L"Expr", level, isLast);
			printProduction(L"num: " + stringToWString(getValue()), level + 1, false);
			Advance();
			if (!APrime(level + 1, true))
			{
				return false;
			}
			return true;
		}


		return false;
	}

	bool ExprPrime(int level,bool isLast)
	{
		printProduction(L"ExprPrime", level, isLast);
		if (getType() == "assign" && getValue() == ":=")
		{
			printProduction(L":=", level + 1, false);
			Advance();
			if (!Expr(level+1,true))
			{
				return false;
			}
			return true;
		}
		else if (APrime(level+1,true))
		{
			return true;
		}
		return false;
	}

	bool APrime(int level,bool isLast)
	{
		printProduction(L"APrime", level, isLast);
		if (!TermPrime(level+1,false))
		{
			return false;
		}
		if (!MagPrime(level+1,false))
		{
			return false;
		}
		if (!RvaluePrime(level+1,true))
		{
			return false;
		}
		return true;
	}

	bool Compare(int level,bool isLast)
	{
		if (getType() == "relop" && (getValue() == "==" || getValue() == "<" || getValue() == "<>" ||
			getValue() == "<=" || getValue() == ">=" || getValue() == ">" || getValue() == "!="))
		{
			printProduction(L"Compare", level, false);
			printProduction(stringToWString(getValue()), level+1, true);
			Advance();
			return true;
		}
		//cout << "Expected a relational operator but found " << getValue() << endl;
		return false;
	}

};

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	_setmode(_fileno(stdout), _O_U16TEXT);


	string filename = "tokens_22l6681.txt";
	vector<Token>tokens = readTokens(filename);
	string filename2 = "reserveWords_22l6681.txt";
	vector<string> reserveWords = readReserveWords(filename2);
	if (tokens.size() == 0)
	{
		wcout << L"\nNo tokens";
		return -1;
	}
	if (reserveWords.size() == 0)
	{
		wcout << L"\nNo reserve words";
		return -1;
	}
	Parser p1(tokens, reserveWords);
	p1.parser();

	return 0;
}