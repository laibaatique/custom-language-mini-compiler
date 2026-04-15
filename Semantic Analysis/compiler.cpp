#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

struct Transi {
    int id;
    char charac;
    int next;
    Transi* nxt;

    Transi(int i, char c, int n, bool a) {
        id = i;
        charac = c;
        next = n;
        nxt = NULL;
    }
};

struct state {

    int accept;
    int other;
    string type;

    state(int ac, int ot, string t) {
        accept = ac; other = ot; type = t;
    }
};

struct token {
    string type;
    string value;

    token(string t, string v) {
        type = t; value = v;
    }
};

class HashMap {
private:
    static const int TABLE_SIZE = 50;
    Transi* table[TABLE_SIZE];

    int hashFunction(int state, char input) {
        return (state * 67 + input - 1) % TABLE_SIZE;
    }

public:
    HashMap() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    void insert(int state, char input, int nextState) {
        int index = hashFunction(state, input);
        Transi* newNode = new Transi(state, input, nextState, 0);
        newNode->nxt = table[index];
        table[index] = newNode;
    }

    int get(int state, char input) {
        int index = hashFunction(state, input);
        Transi* current = table[index];

        while (current) {
            if (current->id == state && current->charac == input) {
                return current->next;
            }
            current = current->nxt;
        }

        return -1;
    }

    void loadTransitions() {
        insert(0, '@', 1);
        insert(0, '#', 5);
        insert(0, '_', 4);
        insert(0, '+', 24);
        insert(0, '-', 30);
        insert(0, '<', 15);
        insert(0, '>', 20);
        insert(0, '=', 28);
        insert(0, '%', 41);
        insert(0, '/', 31);
        insert(0, '"', 32);
        insert(0, '!', 35);
        insert(0, '&', 37);
        insert(0, '|', 39);
        insert(0, '(', 48);
        insert(0, ')', 47);
        insert(0, '[', 46);
        insert(0, ']', 45);
        insert(0, '{', 49);
        insert(0, '}', 50);
        insert(0, ':', 42);
        insert(0, '*', 34);
        insert(0, ' ', 52);
        insert(1, '@', 1);
        insert(1, '#', 3);
        insert(1, '_', 4);
        insert(1, '-', 58);
        insert(1, '<', 59);
        insert(3, '@', 3);
        insert(3, '#', 3);
        insert(3, '_', 4);
        insert(4, '@', 4);
        insert(4, '#', 4);
        insert(4, '_', 4);
        insert(5, '#', 5);
        insert(5, '.', 7);
        insert(5, 'E', 9);
        insert(7, '#', 8);
        insert(8, '#', 8);
        insert(8, 'E', 9);
        insert(9, '+', 10);
        insert(9, '-', 10);
        insert(10, '#', 11);
        insert(11, '#', 11);
        insert(11, '.', 12);
        insert(12, '#', 13);
        insert(13, '#', 13);
        insert(15, '<', 18);
        insert(15, '>', 16);
        insert(15, '=', 17);
        insert(20, '=', 21);
        insert(20, '>', 22);
        insert(24, '+', 25);
        insert(24, '=', 26);
        insert(24, '#', 5);
        insert(28, '=', 29);
        insert(30, '#', 5);
        insert(31, '/', 54);
        insert(31, '*', 55);
        insert(32, '@', 32);
        insert(32, '"', 33);
        insert(35, '=', 36);
        insert(37, '&', 38);
        insert(39, '|', 40);
        insert(42, ':', 43);
        insert(42, '=', 44);
        insert(52, ' ', 52);
        insert(54, '@', 54);
        insert(55, '@', 55);
        insert(55, '*', 56);
        insert(56, '/', 57);
        insert(58, '>', 2);
        insert(59, '-', 2);
    }
};

string sym_table[150] = {
        "Wagarna", "Mantiqi","Harf", "Ashriya", "Adadi", "Agar", "Marqazi", "Matn", "while"
};
int sym_index = 9;
const int keyword_count = 9;

state states[] = {
    state(0, -1,""),    // 0
    state(0, 2,"kw"),    // 1
    state(1, -1,"kw"),  // 2
    state(0, -1,""),    // 3
    state(0, 6,"id"),   // 4
    state(0, 14,"number"),  // 5
    state(1, -1,"id"),  // 6
    state(0, -1,""),     // 7
    state(0, 14,"number"),  // 8
    state(0, -1,""),    // 9
    state(0, -1,""),    // 10
    state(0, 14,"number"),  // 11
    state(0, -1,""),     // 12
    state(0, 14,"number"),  // 13
    state(1, -1,"number"),  // 14
    state(0, 19,"op"),  // 15
    state(1, -1,"op"),  // 16
    state(1, -1,"op"),  // 17
    state(1, -1,"op"),  // 18
    state(1, -1,"op"),  // 19
    state(0, 23,"op"),  // 20
    state(1, -1,"op"),  // 21
    state(1, -1,"op"),  // 22
    state(1, -1,"op"),  // 23
    state(0, 27,"op"),  // 24
    state(1, -1,"op"),  // 25
    state(1, -1,"op"),  // 26
    state(1, -1,"op"),  // 27
    state(0, -1,""),    // 28
    state(1, -1,"op"),  // 29
    state(0, 51,"op"),  // 30
    state(0, 53,"op"),  // 31
    state(0, 33,"lt"),  // 32
    state(1, -1,"lt"),  // 33
    state(1, -1,"op"),  // 34
    state(0, -1,""),    // 35
    state(1, -1,"op"),  // 36
    state(0, -1,""),    // 37
    state(1, -1,"op"),  // 38
    state(0, -1,""),    // 39
    state(1, -1,"op"),  // 40
    state(1, -1,"op"),  // 41
    state(0, -1,""),    // 42
    state(1, -1,"pn"),  // 43
    state(1, -1,"op"),  // 44
    state(1, -1,"pn"),  // 45
    state(1, -1,"pn"),  // 46
    state(1, -1,"pn"),  // 47
    state(1, -1,"pn"),  // 48
    state(1, -1,"pn"),  // 49
    state(1, -1,"pn"),  // 50
    state(1, -1,""),    // 51
    state(0, 0,"sp"),   // 52
    state(1,-1,"op"),    //53
    state(0,0,"scmt"),    //54
    state(0,-1,"mcmt"),     //55
    state(0,55,""),     //56
    state(1,-1,"mcmt"),   //57
    state(0,2,"kw"),      //58
    state(0,2,"kw")       //59
};

int checkSymTable(string lex, int isId) {

    int x = 0;
    int i = 0;

    if (isId) {

        x = sym_index;
        i = keyword_count;
    }
    else {

        x = keyword_count;
        i = 0;
    }

    for (i; i < x; i++) {

        if (lex == sym_table[i]) {

            return i + 1;
        }
    }
    return -1;
}

vector<token> lexer() {

    HashMap transitionTable;
    transitionTable.loadTransitions();

    int c_state = 0;
    string lexeme = "";
    int index = 1;
    int lit_index = 1;

    vector<token> tokens;
    ofstream errorfile("error.txt", ios::trunc);
    ofstream litfile("literal.txt", ios::trunc);
    ifstream sourcefile("sourcecode.txt");

    if (!sourcefile) {
        cerr << "Error: Unable to open source file." << endl;
        return tokens;
    }

    const int buffer_size = 25;
    char buffer1[buffer_size + 1], buffer2[buffer_size + 1];
    char* forward = buffer1;
    bool useBuffer1 = true;
    int bufferSize = 0;

    sourcefile.read(buffer1, buffer_size);
    bufferSize = sourcefile.gcount();
    buffer1[bufferSize] = '\0';

    if (bufferSize <= 0) return tokens;

    while (true) {
        if (*forward == '\0') {
            if (sourcefile.eof()) break;

            useBuffer1 = !useBuffer1;
            if (useBuffer1) {
                forward = buffer1;
            }
            else {
                forward = buffer2;
            }

            sourcefile.read(forward, buffer_size);
            bufferSize = sourcefile.gcount();
            forward[bufferSize] = '\0';
        }

        char ch = *forward++;
        char mappedChar = ch;

        if (states[c_state].type == "lt" && states[c_state].accept == 0 && ch != '"') {
            mappedChar = '@';
        }
        else if (states[c_state].type == "scmt") {
            if (ch != '\n') {

                mappedChar = '@';
            }
            else {
                mappedChar = ch;
            }
        }
        else if (states[c_state].type == "mcmt" && ch != '*') {

            mappedChar = '@';
        }
        else if (isalpha(ch) && states[c_state].type != "number") {
            mappedChar = '@';
        }
        else if (isdigit(ch)) {
            mappedChar = '#';
        }
        else if (isspace(ch)) {
            mappedChar = ' ';
        }
        else if (ch == '@' || ch == '#') {
            mappedChar = '$';
        }

        int n_state = transitionTable.get(c_state, mappedChar);

        if (n_state == -1) {
            if (states[c_state].other != -1) {
                if (states[c_state].type == "id") {
                    int c_index = checkSymTable(lexeme, 1);
                    if (c_index == -1) {
                        sym_table[sym_index++] = lexeme;
                        tokens.push_back(token("id", to_string(index++)));
                    }
                    else {
                        tokens.push_back(token("id", to_string(c_index-keyword_count)));
                    }
                }
                else if (states[c_state].type == "kw") {

                    if (lexeme[lexeme.length() - 1] == '-' || lexeme[lexeme.length() - 1] == '<') {
                        string keyw = lexeme.substr(0, lexeme.size() - 2);
                        if (keyw != "input" && keyw != "output") {
                            if (lexeme[lexeme.length() - 2] == '-' || lexeme[lexeme.length() - 2] == '<') {

                                lexeme = lexeme.substr(0, lexeme.size() - 2);
                                forward -= 2;
                            }
                            else {

                                lexeme = lexeme.substr(0, lexeme.size() - 1);
                                forward--;
                            }
                        }
                    }
                    if (checkSymTable(lexeme, 0) == -1) {
                        errorfile << "Error invalid word = " << lexeme << endl;
                    }
                    else {
                        tokens.push_back(token("kw", lexeme));
                    }
                }
                else if (states[c_state].type != "sp" && states[c_state].type != "scmt") {
                    tokens.push_back(token(states[c_state].type, lexeme));
                }
            }
            else {
                errorfile << "Error invalid word = " << lexeme << endl;
            }
            if (c_state == 0) {

                forward += 2; 
            }
            lexeme = "";
            forward--;
            c_state = 0;
        }
        else if (states[n_state].accept == 1) {

            if (states[n_state].type != "mcmt") {
                lexeme += ch;
                tokens.push_back(token(states[n_state].type, lexeme));
            }
            if (states[c_state].type == "lt") {

                litfile << lit_index << " : " << lexeme << endl;
            }
            lexeme = "";
            c_state = 0;
        }
        else {
            c_state = n_state;
            lexeme += ch;
        }
    }

    errorfile.close();
    litfile.close();
    sourcefile.close();

    ofstream symFile("symbolTable.txt", ios::trunc);
    for (int i = keyword_count; i < sym_index; i++) {
        symFile << i - 8 << " : " << sym_table[i] << "\n";
    }
    symFile.close();

    return tokens;
}

vector<token> getTokens() {

    ifstream file("token.txt");
    vector<token> tokens;
    string line;

    while (getline(file, line)) {

        if (line.front() != '<' || line.back() != '>') continue;

        line = line.substr(2, line.length() - 4);
        size_t comma = line.find(',');
        if (comma == string::npos) continue;

        string type = line.substr(0, comma);
        string value = line.substr(comma + 1);

        while (!type.empty() && isspace(type.back())) type.pop_back();

        while (!value.empty() && isspace(value.front())) value.erase(value.begin());

        if (value.front() == '"' && value.back() == '"') {

            value = value.substr(1, value.length() - 2);
        }

        tokens.push_back({ type, value });
    }

    file.close();
    return tokens;

}

class parser
{

    vector<token> tokens;
    int curr = 0;
    int count = 1;
    int labelCount = 1;
    string error="";
    string code="";

    token* getToken() {

        return &tokens[curr];
    }

    void advance() {

        curr++;
    }


    bool Stmt() {

        if (getToken()->value == "while") {
            if (!WhileStmt()) { return false; }
        }
        else if (getToken()->value == "Agar") {
            if (!IfStmt()) { return false; }
        }
        else if (getToken()->value == "{") {
            if (!CompStmt()) { return false; }
        }
        else if (getToken()->type == "id") {
            if (!Expr()) { return false; }

            if (getToken()->value == "::") {

                advance();
                return true;
            }
            else {
                error = "Token : < " + getToken()->type + "," + getToken()->value + " > expected : '::'\n";
                return false;
            }
        }
        else {
            error = "Token : < " + getToken()->type + "," + getToken()->value + " > expected : Statment\n";
            return false;
        }
        return true;
    }

    bool Type() {

        string val = getToken()->value;

        if (val == "Adadi" || val == "Ashriya" || val == "Harf" || val == "Matn" || val == "Mantiqi") {
            advance();
            return true;
        }
        else {
            error = "Token : <" + getToken()->type + "," + getToken()->value + "> expected : Type\n";
            return false;
        }
    }

    bool WhileStmt() {

        if (getToken()->value == "while") {
            advance();

            string begin = "L" + to_string(labelCount++);
            string next = "L" + to_string(labelCount++);
            cout << begin << " : ";
            code += begin + " : ";

            if (getToken()->value == "(") {
                advance();

                string rvFlase = next;

                if (!Rvalue(rvFlase)) { return false; }

                if (getToken()->value == ")") {
                    advance();

                    if (!Stmt()) { return false; }

                    cout << "  goto " << begin<<endl<<endl;
                    cout << next << " : ";
                    code += "  goto " + begin + "\n\n" + next + " : ";
                }
                else {
                    error = "Token : <" + getToken()->type + "," + getToken()->value + "> expected : ')'\n";
                    return false;
                }
            }
            else {
                error = "Token : <" + getToken()->type + "," + getToken()->value + "> expected : '('\n";
                return false;
            }
        }
        else {
            error = "Token : <" + getToken()->type + "," + getToken()->value + "> expected : 'While'\n";
            return false;
        }
        return true;
    }

    bool IfStmt() {

        if (getToken()->value == "Agar") {
            advance();

            string falseLabel = "L" + to_string(labelCount++);
            string next = "L" + to_string(labelCount++);

            if (getToken()->value == "(") {
                advance();

                if (!Rvalue(falseLabel)) { return false; }

                if (getToken()->value == ")") {
                    advance();

                    if (!Stmt()) { return false; }

                    cout << "  goto " << next<<endl<<endl;
                    code += "  goto " + next+"\n\n";

                    if (!ElsePart(falseLabel)) { return false; }

                    cout << endl<<next + " : ";
                    code += "\n" + next + " : ";
                }
                else {
                    error = "Token : < " + getToken()->type + "," + getToken()->value + " > expected : ')'\n";
                    return false;
                }
            }
            else {
                error = "Token : < " + getToken()->type + "," + getToken()->value + " > expected : '('\n";
                return false;
            }
        }
        else {
            error = "Token : < " + getToken()->type + "," + getToken()->value + " > expected : 'If'\n";
            return false;
        }
        return true;
    }

    bool ElsePart(string falseLabel) {

        if (getToken()->value == "Wagarna") {
            advance();

            cout << falseLabel << " : ";
            code += falseLabel + " : ";

            if (!Stmt()) { return false; }
        }
        return true;
    }

    bool CompStmt() {

        if (getToken()->value == "{") {
            advance();

            if (!StmtList()) { return false; }

            if (getToken()->value == "}") advance();
            else {
                error = "Token : < " + getToken()->type + "," + getToken()->value + " > expected : '}'\n";
                return false;
            }
        }
        else {
            error = "Token : < " + getToken()->type + "," + getToken()->value + " > expected : '{'\n";
            return false;
        }
        return true;
    }

    bool StmtList() {

        if (!S1()) { return false; }
        return true;
    }

    bool S1() {

        if (getToken()->value != "}") {

            if (!Stmt()) { return false; }
            if (!S1()) { return false; }
        }
        return true;
    }

    bool Expr() {

        if (getToken()->type == "id") {
            int i = stoi(getToken()->value)+keyword_count -1;
            advance();

            if (getToken()->value == ":=") { 
                advance(); 
            }
            else {

                error = "Token : < " + getToken()->type + "," + getToken()->value + " > expected : 'Expression'\n";
                return false;
            }
            string mAddr = "";
            if (!Mag(mAddr)) { return false; }
            
            cout << sym_table[i] << " = " << mAddr<<endl;
            code += sym_table[i] + " = " + mAddr + "\n";
        }
        return true;
    }

    bool Rvalue(string falseLabel) {
        
        string mAddr = "";
        if(!Mag(mAddr)) { return false; }
        if(!R1(mAddr,falseLabel)) { return false; }
        
        return true;
    }

    bool R1(string inherited, string falseLabel) {

        if (getToken()->value == "==" || getToken()->value == "<" || getToken()->value == ">" ||
            getToken()->value == "<=" || getToken()->value == ">=" || getToken()->value == "<>") {

            string relopVal = "";
            if (!RelOp(relopVal)) { return false; }
            string mAddr = "";
            if (!Mag(mAddr)) { return false; }

            string trueLabel = "L" + to_string(labelCount++);
            cout << "if " + inherited +' ' + relopVal +' ' + mAddr + "  goto " + trueLabel << endl;
            cout << "  goto " + falseLabel << endl<<endl;
            cout << trueLabel + " : ";
            code += "if " + inherited + ' ' + relopVal + ' ' + mAddr + "  goto " + trueLabel + "\n" + "  goto " + falseLabel + "\n\n" + trueLabel + " : ";

            if (!R1(mAddr, falseLabel)) { return false; }
        }

        return true;
    }

    bool RelOp(string& value) {

        if (getToken()->value == "==" || getToken()->value == "<" || getToken()->value == ">" ||
            getToken()->value == "<=" || getToken()->value == ">=" || getToken()->value == "<>") {

            value = getToken()->value;

            advance();
        }
        else {
            error = "Token : < " + getToken()->type + "," + getToken()->value + " > expected : 'RelOp'\n";
            return false;
        }
        return true;
    }

    bool Mag(string& magAddr) {

        string tAddr = "";
        if (!Term(tAddr)) { return false; }
        if (!M1(magAddr,tAddr)) { return false; }
        return true;
    }

    bool M1(string& m1Addr, string inherited) {

        if (getToken()->value == "+") {
            advance();

            string tAddr = "";
            if (!Term(tAddr)) { return false; }
            m1Addr = "t" + to_string(count++);
            cout << m1Addr << " = " << inherited << " + " << tAddr<<endl;
            code += m1Addr + " = " + inherited + " + " + tAddr + "\n";

            if (!M1(m1Addr,m1Addr)) { return false; }
        }
        else if (getToken()->value == "-") {
            advance();

            string tAddr = "";
            if (!Term(tAddr)) { return false; }
            m1Addr = "t" + to_string(count++);
            cout << m1Addr << " = " << inherited << " - " << tAddr<<endl;
            code += m1Addr + " = " + inherited + " - " + tAddr + "\n";

            if (!M1(m1Addr, tAddr)) { return false; }
        }
        else {
            
            m1Addr = inherited;
        }
        return true;
    }

    bool Term(string& termAddr) {

        string fAddr = "";
        if (!Factor(fAddr)) { return false; }
        if (!T1(termAddr,fAddr)) { return false; }
        return true;
    }

    bool T1(string& t1Addr, string inherited) {

        if (getToken()->value == "*") {
            advance();

            string fAddr = "";
            if (!Factor(fAddr)) { return false; }
            t1Addr = "t" + to_string(count++);
            cout << t1Addr << " = " << inherited << " * " << fAddr << endl;
            code += t1Addr + " = " + inherited + " * " + fAddr + "\n";

            if (!T1(t1Addr,t1Addr)) { return false; }
        }
        else if (getToken()->value == "/") {
            advance();

            string fAddr = "";
            if (!Factor(fAddr)) { return false; }
            t1Addr = "t" + to_string(count++);
            cout << t1Addr + " = " + inherited + " / " + fAddr << endl;
            code += t1Addr + " = " + inherited + " / " + fAddr + "\n";

            if (!T1(t1Addr, t1Addr)) { return false; }
        }
        else {
            
            t1Addr = inherited;
        }

        return true;
    }

    bool Factor(string& factorAddr) {

        if (getToken()->value == "(") {
            advance();

            if (!Mag(factorAddr)) { return false; }

            if (getToken()->value == ")") { advance(); }

            else {
                error = "Token : <" + getToken()->type + "," + getToken()->value + "> expected : ')'\n";
                return false;
            }
        }
        else if (getToken()->type == "number") {

            factorAddr = getToken()->value;
            advance();
        }
        else if (getToken()->type == "id") {

            int i = stoi(getToken()->value)+keyword_count-1;
            factorAddr = sym_table[i];
            advance();
        }
        else {
            error = "Token : <" + getToken()->type + "," + getToken()->value + "> expected : 'Identifier or number'\n";
            return false;
        }
        return true;
    }

public:

    parser(vector<token> t) : tokens(t), curr(0) {}

    void parse() {
        if (Stmt()) {
            cout << " End\n\n";
            cout << "Parsing done\n";
            
            ofstream tacFile("code.tac", ios::trunc);
            tacFile << code;
        }
        else
        {
            cout <<endl << endl<<error;
        }
    }
};

int main() {
    vector<token> tokenList = lexer();
    ofstream tokenFile("token.txt", ios::trunc);

    for (const auto& tok : tokenList) {

        tokenFile << "< " << tok.type << " , " << tok.value << " >" << endl;
    }

    vector<token> tokens = getTokens();
    parser p(tokens);
    p.parse();

    return 0;
}
