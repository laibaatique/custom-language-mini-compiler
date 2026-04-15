#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;

const vector<int> acceptanceStates = { 4,5,12,13,14,15,16,17,19,20,22,23,24,25,27,28,29,31,32,33,34,35,36,37,39,41,43,44,45,46 };

struct Token {
	string tokenName;
	string attributeValue;
	string symbol;
	Token(string name = "INVALID", string value = "-1", string symbol = "")
	{
		tokenName = name;
		attributeValue = value;
	}

};


template <typename K, typename V>
struct Node {
	K key;  //for both pair<int,char> and only int
	V nextState;
	Node<K, V>* nextNode;

	Node(const K& key, const V& nextState)
	{
		this->key = key;
		this->nextState = nextState;
		nextNode = nullptr;
	}
	~Node()
	{
		nextNode = nullptr;
	}
};


template <typename T, typename U>
class HashMap {
	vector<Node<T, U>*>transitionTable;
	int numBuckets;
	int numElements;  //number of elements in each bucket

	int hashFunction(int key)
	{
		return (key * 31) % numBuckets;
	}

	int hashFunction(pair<int, char> key)
	{
		return ((hash<int>()(key.first) * 31) + hash<char>()(key.second)) % numBuckets;
	}

public:
	HashMap(int numBuckets, int numElements = 0)
	{
		this->numBuckets = numBuckets;
		this->numElements = numElements;
		transitionTable.resize(numBuckets);
	}

	~HashMap()
	{
		for (int i = 0;i < numBuckets;i++)
		{
			Node<T, U>* temp = transitionTable[i];
			while (temp)
			{
				Node<T, U>* next = temp->nextNode;
				delete temp;
				temp = next;
			}
		}
	}

	void insertNode(T key, U nextState)
	{
		int index = hashFunction(key);
		Node<T, U>* newNode = new Node<T, U>(key, nextState);
		if (!transitionTable[index])
		{
			transitionTable[index] = newNode;
			return;
		}
		else
		{
			Node<T, U>* temp = transitionTable[index];
			Node<T, U>* previous = nullptr;
			while (temp != nullptr)
			{
				if (temp->key == key)
				{
					temp->nextState = newNode->nextState;
					delete newNode;
					return;
				}
				previous = temp;
				temp = temp->nextNode;
			}
			if (previous != nullptr)
			{
				previous->nextNode = newNode;
				numElements++;
				return;
			}
		}
		return;
	}


	int getNextState(int currentState, char character)
	{
		if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'))
		{
			if (!(character == 'E' && (currentState == 11 || currentState == 8)))
			{
				character = '#';
			}
		}
		if ((character >= '0' && character <= '9'))
		{
			character = '@';
		}
		pair<int, char>key = { currentState,character };
		int index = hashFunction(key);
		if (transitionTable[index]->key == key)
		{
			return transitionTable[index]->nextState;
		}
		else
		{
			Node<pair<int, char>, int>* temp = transitionTable[index];
			while (temp)
			{
				if (temp->key == key)
				{
					return temp->nextState;
				}
				else
				{
					temp = temp->nextNode;
				}
			}
		}
		return -1;
	}

	int getNextState(int key)
	{
		int index = hashFunction(key);
		if (transitionTable[index]->key == key)
		{
			return transitionTable[index]->nextState;
		}
		else
		{
			Node<T, U>* temp = transitionTable[index];
			while (temp)
			{
				if (temp->key == key)
				{
					return temp->nextState;
				}
				else
				{
					temp = temp->nextNode;
				}
			}
		}
		return -1;
	}

	Token* getToken(int key)
	{
		int index = hashFunction(key);
		if (transitionTable[index]->key == key)
		{
			return &transitionTable[index]->nextState;
		}
		else
		{
			Node<T, U>* temp = transitionTable[index];
			while (temp)
			{
				if (temp->key == key)
				{
					return &temp->nextState;
				}
				else
				{
					temp = temp->nextNode;
				}
			}
		}
		Token* token = new Token();
		return token;
	}

	void underscoreTransitions()
	{
		for (int i = 0;i < 4;i++)
		{
			insertNode(pair<int, char>(i, '_'), 1);
		}
	}

	void letterTransitions()
	{
		//# is used to represent alphabets
		insertNode(pair<int, char>(0, '#'), 2);
		insertNode(pair<int, char>(1, '#'), 1);
		insertNode(pair<int, char>(2, '#'), 2);
		insertNode(pair<int, char>(3, '#'), 3);
	}

	void digitTransitions()
	{
		//@ is used to represent digits
		insertNode(pair<int, char>(0, '@'), 8);
		insertNode(pair<int, char>(1, '@'), 1);
		insertNode(pair<int, char>(2, '@'), 3);
		insertNode(pair<int, char>(3, '@'), 3);
		insertNode(pair<int, char>(6, '@'), 8);
		insertNode(pair<int, char>(7, '@'), 8);
		insertNode(pair<int, char>(8, '@'), 8);
		insertNode(pair<int, char>(9, '@'), 8);
		insertNode(pair<int, char>(10, '@'), 11);
		insertNode(pair<int, char>(11, '@'), 11);
	}

	void operatorTransitions()
	{
		insertNode(pair<int, char>(0, '+'), 7);
		insertNode(pair<int, char>(7, '+'), 34);
		insertNode(pair<int, char>(9, '+'), 7);
		insertNode(pair<int, char>(0, '-'), 6);
		insertNode(pair<int, char>(9, '-'), 6);
		insertNode(pair<int, char>(0, '='), 30);
		insertNode(pair<int, char>(7, '='), 35);
		insertNode(pair<int, char>(18, '='), 20);
		insertNode(pair<int, char>(21, '='), 24);
		insertNode(pair<int, char>(26, '='), 28);
		insertNode(pair<int, char>(30, '='), 31);
		insertNode(pair<int, char>(42, '='), 43);
		insertNode(pair<int, char>(8, 'E'), 9);
		insertNode(pair<int, char>(11, 'E'), 9);
		insertNode(pair<int, char>(8, '.'), 10);
		insertNode(pair<int, char>(0, '<'), 21);
		insertNode(pair<int, char>(21, '<'), 25);
		insertNode(pair<int, char>(0, '>'), 26);
		insertNode(pair<int, char>(21, '>'), 23);
		insertNode(pair<int, char>(26, '>'), 27);
		insertNode(pair<int, char>(0, '%'), 37);
		insertNode(pair<int, char>(0, '|'), 38);
		insertNode(pair<int, char>(38, '|'), 39);
		insertNode(pair<int, char>(0, '&'), 40);
		insertNode(pair<int, char>(40, '&'), 41);
		insertNode(pair<int, char>(0, '!'), 42);
		insertNode(pair<int, char>(0, '*'), 44);
		insertNode(pair<int, char>(0, '/'), 46);
		insertNode(pair<int, char>(0, '"'), 45);
		insertNode(pair<int, char>(2, '-'), 47);
		insertNode(pair<int, char>(47, '>'), 48);
		insertNode(pair<int, char>(2, '<'), 49);
		insertNode(pair<int, char>(49, '-'), 50);
	}

	void punctuationTransitions()
	{
		insertNode(pair<int, char>(0, '['), 12);
		insertNode(pair<int, char>(0, '{'), 13);
		insertNode(pair<int, char>(0, '('), 14);
		insertNode(pair<int, char>(0, ')'), 15);
		insertNode(pair<int, char>(0, '}'), 16);
		insertNode(pair<int, char>(0, ']'), 17);
		insertNode(pair<int, char>(0, ':'), 18);
		insertNode(pair<int, char>(18, ':'), 19);
	}
	void otherTransitions()
	{
		insertNode(1, 4);
		insertNode(2, 5);
		insertNode(8, 32);
		insertNode(11, 32);
		insertNode(21, 22);
		insertNode(26, 29);
		insertNode(7, 33);
		insertNode(6, 36);
		insertNode(48, 5);
		insertNode(50, 5);
		insertNode(47, 5);
		insertNode(49, 5);
	}
	void tokenTransitions()
	{
		insertNode(4, Token("id"));
		insertNode(5, Token("keyword"));
		insertNode(12, Token("punc", "["));
		insertNode(13, Token("punc", "{"));
		insertNode(14, Token("punc", "("));
		insertNode(15, Token("punc", ")"));
		insertNode(16, Token("punc", "}"));
		insertNode(17, Token("punc", "]"));
		insertNode(19, Token("punc", "::"));
		insertNode(20, Token("assign", ":="));
		insertNode(22, Token("relop", "LT", "<"));
		insertNode(23, Token("relop", "NE", "<>"));
		insertNode(24, Token("relop", "LE", "<="));
		insertNode(25, Token("output", "cout", "<<"));
		insertNode(27, Token("input", "cin", ">>"));
		insertNode(28, Token("relop", "GE", ">="));
		insertNode(29, Token("relop", "GT", ">"));
		insertNode(31, Token("relop", "EQ", "=="));
		insertNode(32, Token("num"));
		insertNode(33, Token("op", "ADD", "+"));
		insertNode(34, Token("op", "INC", "++"));
		insertNode(35, Token("op", "ADD_ASSIGN", "+="));
		insertNode(36, Token("op", "SUB", "-"));
		insertNode(37, Token("op", "MOD", "%"));
		insertNode(39, Token("log_op", "OR", "||"));
		insertNode(41, Token("log_op", "AND", "&&"));
		insertNode(43, Token("relop", "NE", "!="));
		insertNode(44, Token("op", "MUL", "*"));
		insertNode(45, Token("punc", "\""));
		insertNode(46, Token("op", "DIV", "/"));
	}
};

bool isStateFinal(int num) {
	for (int i = 0;i < acceptanceStates.size();i++)
	{
		if (acceptanceStates[i] == num)
		{
			return true;
		}
	}
	return false;
}

int getTableEntry(const string& lexeme, fstream& filename)
{
	string line;
	int id;

	while (getline(filename, line))
	{
		size_t pos = line.find(". ");
		if (pos != string::npos) {
			int id = stoi(line.substr(0, pos));
			string existingLexeme = line.substr(pos + 2);

			if (existingLexeme == lexeme) {
				return id;
			}
		}
	}
	return -1;
}

void processTokens(HashMap<pair<int, char>, int>* hashMap, HashMap<int, int>* otherMap, HashMap<int, Token>* tokenMap, char* buffer1, char* buffer2, vector<string>& reserveWords, string& pendingLexeme, int& symbol_num, int& literal_num, int& error_num, bool end, int& currentBuffer, int& lexemeBegin, int& forwardPointer, int& temp_state, bool& insideStringLiteral, bool& insideSingleLineComment, bool& insideMultiLineComment)
{
	fstream file_literal("literal_table_22l6681.txt", ios::in | ios::out);
	if (!file_literal) {
		cerr << "Error opening literal table file!" << endl;
		return;
	}
	ofstream file_error("error_22l6681.txt", ios::app);
	if (!file_error) {
		cerr << "Error opening error file!" << endl;
		return;
	}
	fstream file_symbol("symbol_table_22l6681.txt", ios::in | ios::out);
	if (!file_symbol) {
		cerr << "Error opening symbol table file!" << endl;
		return;
	}

	fstream file_token("tokens_22l6681.txt", ios::out | ios::app);
	if (!file_token) {
		cerr << "Error opening token table file!" << endl;
		return;
	}

	int state = 0;
	string lexeme = "";
	string inputBuffer;
	bool lex_completed = false;
	if (currentBuffer == 1)
	{
		inputBuffer = string(buffer1);
	}
	else
	{
		inputBuffer = string(buffer2);
	}

	while (forwardPointer < inputBuffer.size() && inputBuffer[forwardPointer] != '\0')
	{
		bool lex_found = false;  //for final state
		if (lexemeBegin > forwardPointer)
		{
			if (!insideStringLiteral)
				state = temp_state;
		}
		else
		{
			lexemeBegin = forwardPointer;
			state = 0;
		}

		lex_completed = false;  //for buffers
		char ch = inputBuffer[forwardPointer];
		int newState = 0;
		if (insideStringLiteral)
		{
			/*string literal = "";
			if (pendingLexeme == "")
			{
				if (currentBuffer == 1)
				{
					pendingLexeme = string(buffer2).substr(lexemeBegin + 1);
				}
				else
				{
					pendingLexeme = string(buffer1).substr(lexemeBegin + 1);
				}
			}
			else
			{
				literal = pendingLexeme;
			}*/
			while (forwardPointer < inputBuffer.size())
			{
				if (inputBuffer[forwardPointer] == '"')
				{
					insideStringLiteral = false;
					file_literal.clear();
					file_literal.seekg(0, ios::beg);
					//literal = pendingLexeme + literal;
					//pendingLexeme = "";
					int temp = getTableEntry(pendingLexeme, file_literal);
					if (temp == -1) {
						file_literal.clear();
						file_literal.seekp(0, ios::end);
						//file_literal << literal_num << ". " << literal << endl;
						file_literal << literal_num << ". " << pendingLexeme << endl;
						file_literal.flush();
						file_token.clear();
						file_token.seekp(0, ios::end);
						file_token << "  <literal , " << literal_num << ">";
						file_token.flush();
						literal_num++;
					}
					else {
						file_token.clear();
						file_token.seekp(0, ios::end);
						file_token << "  <literal , " << temp << ">";
						file_token.flush();
					}
					forwardPointer++; // Move past closing "
					lexemeBegin = forwardPointer;
					ch = inputBuffer[forwardPointer];
					temp_state = 0;
					pendingLexeme = "";
					break;
				}
				pendingLexeme += inputBuffer[forwardPointer];
				forwardPointer++;
			}
			if (forwardPointer >= inputBuffer.size())
			{
				//pendingLexeme = literal;
				if (end)
				{
					file_error << error_num << ". " << "Unterminated string literal: " << pendingLexeme << endl;
					file_token.clear();
					file_token.seekp(0, ios::end);
					file_token << endl << "<error , " << error_num << ">" << endl;
					file_token.flush();
					error_num++;
					break;
				}
				else
				{
					lexemeBegin = 0;
					forwardPointer = 0;
					return;
				}
			}

		}

		if ((ch == ' ' || ch == '\n') && temp_state == 0)
		{
			while (forwardPointer < inputBuffer.size() && (inputBuffer[forwardPointer] == ' ' || inputBuffer[forwardPointer] == '\n')) {
				forwardPointer++;
			}
			lexemeBegin = forwardPointer;
			ch = inputBuffer[forwardPointer];
			if (forwardPointer >= inputBuffer.size())
			{
				lex_completed = true;
				temp_state = 0;
				break;
			}
		}

		//single line comments
		if ((ch == '/' && forwardPointer + 1 < inputBuffer.size() && inputBuffer[forwardPointer + 1] == '/') || insideSingleLineComment == true)
		{
			if (!insideSingleLineComment) //start of the comment
			{
				insideSingleLineComment = true;
				forwardPointer += 2;
			}
			while (forwardPointer < inputBuffer.size() && inputBuffer[forwardPointer] != '\n')
			{
				forwardPointer++;
			}
			if (forwardPointer < inputBuffer.size() && inputBuffer[forwardPointer] == '\n')
			{
				insideSingleLineComment = false;
				forwardPointer++;
				lexemeBegin = forwardPointer;
			}
			if (forwardPointer >= inputBuffer.size()) {

				lexemeBegin = 0;
				forwardPointer = 0;
				return;
			}
			continue;
		}

		//multiline comments
		if ((ch == '/' && forwardPointer + 1 < inputBuffer.size() && inputBuffer[forwardPointer + 1] == '*') || insideMultiLineComment == true)
		{
			if (!insideMultiLineComment)
			{
				insideMultiLineComment = true;
				forwardPointer += 2;
			}
			while (forwardPointer < inputBuffer.size())
			{
				if (inputBuffer[forwardPointer] == '*' && inputBuffer[forwardPointer + 1] == '/')
				{
					insideMultiLineComment = false;
					forwardPointer += 2;
					lexemeBegin = forwardPointer;
					break;
				}
				forwardPointer++;
			}
			if (forwardPointer >= inputBuffer.size())
			{
				lexemeBegin = 0;
				forwardPointer = 0;
				return;
			}
			continue;
		}

		while (!isStateFinal(state) && forwardPointer < inputBuffer.size())
		{
			newState = hashMap->getNextState(state, ch);
			if (newState == -1)
			{
				lex_completed = true;
				lex_found = true;  //other or error
				newState = otherMap->getNextState(state);
				if (newState == -1)
				{
					string error;
					if (state == 0)
					{
						error = inputBuffer.substr(lexemeBegin, forwardPointer - lexemeBegin + 1);
						forwardPointer++;
					}
					else
					{
						error = inputBuffer.substr(lexemeBegin, forwardPointer - lexemeBegin);
					}
					file_error << error_num << ". Not a part of our language : " << error << endl;
					file_token.clear();
					file_token.seekp(0, ios::end);
					file_token << endl << "<error , " << error_num << ">" << endl;
					file_token.flush();
					error_num++;
					break;
				}
				else
				{
					Token* token = tokenMap->getToken(newState);
					string lexeme = "";
					if (temp_state != 0 && currentBuffer == 1)
					{
						lexeme = string(buffer2).substr(lexemeBegin);
						lexeme += inputBuffer.substr(0, forwardPointer);
						lex_completed = true;
						temp_state = 0;
						lexemeBegin = forwardPointer;
					}
					else if (temp_state != 0 && currentBuffer == 2)
					{
						lexeme = string(buffer1).substr(lexemeBegin);
						lexeme += inputBuffer.substr(0, forwardPointer);
						lex_completed = true;
						temp_state = 0;
						lexemeBegin = forwardPointer;
					}
					else
					{
						lexeme = inputBuffer.substr(lexemeBegin, forwardPointer - lexemeBegin);
					}
					if (token->tokenName == "keyword")
					{
						if (lexeme.length() > 2 && (lexeme.substr(lexeme.length() - 2) == "->" || lexeme.substr(lexeme.length() - 2) == "<-"))
						{
							string temp = lexeme.substr(0, lexeme.length() - 2);
							if (temp != "input" && temp != "output")
							{
								lexeme = lexeme.substr(lexeme.length() - 2);
								if (forwardPointer != 0)
									forwardPointer -= 2;
								lexemeBegin = forwardPointer;
							}
						}
						else if (lexeme.length() > 1 && (lexeme.substr(lexeme.length() - 1) == "-" || lexeme.substr(lexeme.length() - 1) == "<"))
						{
							lexeme = lexeme.substr(0, lexeme.length() - 1);
							if (forwardPointer != 0)
								forwardPointer -= 1;
							lexemeBegin = forwardPointer;
						}
						int count = 0;
						bool found = false;
						for (const string& word : reserveWords)
						{
							count++;
							if (word == lexeme)
							{
								file_token.clear();
								file_token.seekp(0, ios::end);
								file_token << "  <keyword , " << count << ">";
								file_token.flush();
								found = true;
								break;
							}
						}
						if (found == false)
						{
							file_error << error_num << ". " << "Word with wrong format : " << lexeme << endl;
							file_token.clear();
							file_token.seekp(0, ios::end);
							file_token << endl << "<error , " << error_num << ">" << endl;
							file_token.flush();
							error_num++;
						}
					}
					else if (token->tokenName == "id")
					{
						file_symbol.clear();
						file_symbol.seekg(0, ios::beg);
						int temp = getTableEntry(lexeme, file_symbol);
						token->attributeValue = lexeme;
						if (temp == -1)
						{
							file_symbol.clear();
							file_symbol.seekp(0, ios::end);
							file_symbol << symbol_num << ". " << token->attributeValue << endl;
							file_symbol.flush();
							file_token.clear();
							file_token.seekp(0, ios::end);
							file_token << "  <id , " << symbol_num << ">";
							file_token.flush();
							symbol_num++;
						}
						else
						{
							file_token.clear();
							file_token.seekp(0, ios::end);
							file_token << "  <id , " << temp << ">";
							file_token.flush();
						}
					}
					else
					{
						token->attributeValue = lexeme;
						file_token.clear();
						file_token.seekp(0, ios::end);
						file_token << "  <" << token->tokenName << " , " << token->attributeValue << ">";
						file_token.flush();
					}
					break;
				}
			}
			forwardPointer++;
			ch = inputBuffer[forwardPointer];
			state = newState;
			if (isStateFinal(state))
			{
				lex_completed = true;
			}
		}
		if (!lex_found)
		{
			if (forwardPointer >= inputBuffer.size() && !end) {
				temp_state = state;
				break;  // Stop processing and wait for next buffer read
			}
			if (forwardPointer > inputBuffer.size() && end) {
				string error = inputBuffer.substr(lexemeBegin, forwardPointer - lexemeBegin);

				file_error << error_num << ". Not a part of our language : " << error << endl;
				file_token.clear();
				file_token.seekp(0, ios::end);
				file_token << endl << "<error , " << error_num << ">" << endl;
				file_token.flush();
			}
			Token* token = tokenMap->getToken(state);
			string lexeme = inputBuffer.substr(lexemeBegin, forwardPointer - lexemeBegin);
			lex_completed = true;
			if (token->attributeValue == "\"")
			{
				//string literal = "";
				bool foundEnd = false;
				while (forwardPointer < inputBuffer.size() && inputBuffer[forwardPointer] != '"')
				{
					pendingLexeme += inputBuffer[forwardPointer];
					forwardPointer++;
				}
				if (forwardPointer >= inputBuffer.size() && end)
				{

					file_error << error_num << ". " << "Unterminated string literal : " << pendingLexeme << endl;
					file_token.clear();
					file_token.seekp(0, ios::end);
					file_token << endl << "<error , " << error_num << ">" << endl;
					file_token.flush();
					error_num++;
					break;
				}
				else if (forwardPointer >= inputBuffer.size() && !end) {
					lex_completed = false;
					insideStringLiteral = true;
					temp_state = state;
					break;  // Stop processing and wait for next buffer read
				}
				else
				{
					file_literal.clear();
					file_literal.seekg(0, ios::beg);
					int temp = getTableEntry(pendingLexeme, file_literal);
					if (temp == -1)
					{
						file_literal.clear();
						file_literal.seekp(0, ios::end);
						file_literal << literal_num << ". " << pendingLexeme << endl;
						file_literal.flush();
						file_token.clear();
						file_token.seekp(0, ios::end);
						file_token << "  <literal" << " , " << literal_num << ">";
						file_token.flush();
						literal_num++;
					}
					else
					{
						file_token.clear();
						file_token.seekp(0, ios::end);
						file_token << "  <literal" << " , " << temp << ">";
						file_token.flush();
					}

					forwardPointer++;
					if (forwardPointer >= inputBuffer.size())
						return;
				}
				pendingLexeme = "";
			}
			else
			{
				file_token.clear();
				file_token.seekp(0, ios::end);
				file_token << "  <" << token->tokenName << " , " << token->attributeValue << ">";
				file_token.flush();

			}
		}
	}
	forwardPointer = 0;
	if (lex_completed == true)
	{
		lexemeBegin = forwardPointer;
		temp_state = 0;
	}
	file_token.close();
	file_literal.close();
	file_error.close();
	file_symbol.close();
	return;
}


int main()
{
	HashMap<pair<int, char>, int>* hashMap = new HashMap<pair<int, char>, int>(17);
	HashMap<int, int>* otherMap = new HashMap<int, int>(8);
	HashMap<int, Token>* tokenMap = new HashMap<int, Token>(8);
	hashMap->underscoreTransitions();
	hashMap->letterTransitions();
	hashMap->digitTransitions();
	hashMap->operatorTransitions();
	hashMap->punctuationTransitions();
	otherMap->otherTransitions();
	tokenMap->tokenTransitions();
	char buffer1[26];
	char buffer2[26];
	string pendingLexeme = "";
	int symbol_num = 1;
	int literal_num = 1;
	int error_num = 1;
	int currentBuffer = 1;
	int bufferBegin = 0;
	int forwardPointer = 0;
	int temp_state = 0;
	bool insideStringLiteral = false;
	bool insideSingleLineComment = false;
	bool insideMultiLineComment = false;
	ifstream file_code("test_code_22l6681.txt");
	if (!file_code) {
		cerr << "Error opening test code file!" << endl;
		return -1;
	}

	ifstream file_reserve("reserve_words_22l6681.txt");
	if (!file_reserve) {
		cerr << "Error opening reserve words table file!" << endl;
		return -1;
	}
	vector<string>reserveWords;
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

	while (true)
	{
		if (currentBuffer == 1)
		{
			file_code.read(buffer1, 25);
			streamsize bytesRead = file_code.gcount();
			buffer1[bytesRead] = '\0';
			if (bytesRead == 0)
			{
				break;
			}
			else if (bytesRead < 25)
			{
				processTokens(hashMap, otherMap, tokenMap, buffer1, buffer2, reserveWords, pendingLexeme, symbol_num, literal_num, error_num, true, currentBuffer, bufferBegin, forwardPointer, temp_state, insideStringLiteral, insideSingleLineComment, insideMultiLineComment);
				break;
			}
			else
			{
				processTokens(hashMap, otherMap, tokenMap, buffer1, buffer2, reserveWords, pendingLexeme, symbol_num, literal_num, error_num, false, currentBuffer, bufferBegin, forwardPointer, temp_state, insideStringLiteral, insideSingleLineComment, insideMultiLineComment);
			}
			currentBuffer = 2;
		}
		else
		{
			file_code.read(buffer2, 25);
			streamsize bytesRead = file_code.gcount();
			buffer2[bytesRead] = '\0';

			if (bytesRead == 0)
			{
				break;
			}
			else if (bytesRead < 25)
			{
				processTokens(hashMap, otherMap, tokenMap, buffer1, buffer2, reserveWords, pendingLexeme, symbol_num, literal_num, error_num, true, currentBuffer, bufferBegin, forwardPointer, temp_state, insideStringLiteral, insideSingleLineComment, insideMultiLineComment);
				break;
			}
			else
			{
				processTokens(hashMap, otherMap, tokenMap, buffer1, buffer2, reserveWords, pendingLexeme, symbol_num, literal_num, error_num, false, currentBuffer, bufferBegin, forwardPointer, temp_state, insideStringLiteral, insideSingleLineComment, insideMultiLineComment);
			}
			currentBuffer = 1;
		}
	}
	file_code.close();
	file_reserve.close();
	return 0;
}