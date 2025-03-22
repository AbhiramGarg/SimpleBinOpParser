#include <iostream>
#include <sstream>
#include <memory>
#include <map>
#include <vector>

using namespace std;

// Token Types
enum Token {
    tok_eof = -1,
    tok_number = -2,
    tok_identifier = -3
};

// Lexer Globals
static string IdentifierStr;
static double NumVal;
static int CurTok;

static string InputBuffer;
static size_t InputPos = 0;

// Function to get the next character from input buffer
static int getNextChar() {
    if (InputPos >= InputBuffer.size())
        return EOF;
    return InputBuffer[InputPos++];
}

// Tokenizer
static int gettok() {
    static int LastChar = ' ';

    // Skip whitespace
    while (isspace(LastChar))
        LastChar = getNextChar();

    if (isalpha(LastChar)) { // Identifier
        IdentifierStr = LastChar;
        while (isalnum(LastChar = getNextChar()))
            IdentifierStr += LastChar;
        return tok_identifier;
    }

    if (isdigit(LastChar) || LastChar == '.') { // Number
        string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getNextChar();
        } while (isdigit(LastChar) || LastChar == '.');
        NumVal = strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }

    if (LastChar == EOF)
        return tok_eof;

    // Return ASCII value for single-character tokens.
    int ThisChar = LastChar;
    LastChar = getNextChar();
    return ThisChar;
}

static int getNextToken() { return CurTok = gettok(); }

// AST Nodes
class ExprAST {
public:
    virtual ~ExprAST() = default;
    virtual void print(int indent = 0) const = 0;
};

class NumberExprAST : public ExprAST {
    double Val;

public:
    NumberExprAST(double Val) : Val(Val) {}
    void print(int indent = 0) const override {
        cout << string(indent, ' ') << "Number: " << Val << endl;
    }
};

class VariableExprAST : public ExprAST {
    string Name;

public:
    VariableExprAST(const string &Name) : Name(Name) {}
    void print(int indent = 0) const override {
        cout << string(indent, ' ') << "Variable: " << Name << endl;
    }
};

class BinaryExprAST : public ExprAST {
    char Op;
    unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char Op, unique_ptr<ExprAST> LHS, unique_ptr<ExprAST> RHS)
        : Op(Op), LHS(move(LHS)), RHS(move(RHS)) {}

    void print(int indent = 0) const override {
        cout << string(indent, ' ') << "Binary Op: " << Op << endl;
        LHS->print(indent + 2);
        RHS->print(indent + 2);
    }
};

// Parsing Helpers
unique_ptr<ExprAST> LogError(const char *Str) {
    cerr << "Error: " << Str << endl;
    return nullptr;
}

// Operator Precedence
static map<char, int> BinopPrecedence = {
    {'<', 10}, {'+', 20}, {'-', 20}, {'*', 40}, {'/', 50}};

static int getTokPrecedence() {
    if (!isascii(CurTok))
        return -1;

    int TokPrec = BinopPrecedence[CurTok];
    if (TokPrec <= 0)
        return -1;
    return TokPrec;
}

// Parsing Functions
static unique_ptr<ExprAST> ParseNumberExpr() {
    auto Result = make_unique<NumberExprAST>(NumVal);
    getNextToken();
    return move(Result);
}

static unique_ptr<ExprAST> ParseIdentifierExpr() {
    auto Result = make_unique<VariableExprAST>(IdentifierStr);
    getNextToken();
    return move(Result);
}

static unique_ptr<ExprAST> ParseParenExpr();

static unique_ptr<ExprAST> ParsePrimary() {
    switch (CurTok) {
    case tok_identifier:
        return ParseIdentifierExpr();
    case tok_number:
        return ParseNumberExpr();
    case '(':
        return ParseParenExpr();
    default:
        return LogError("Unknown token when expecting an expression");
    }
}

static unique_ptr<ExprAST> parseExpression();

static unique_ptr<ExprAST> ParseParenExpr() {
    getNextToken(); // Eat '('
    auto V = parseExpression();
    if (!V)
        return nullptr;

    if (CurTok != ')')
        return LogError("Expected ')'");
    getNextToken(); // Eat ')'
    return V;
}

static unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, unique_ptr<ExprAST> LHS) {
    while (true) {
        int TokPrec = getTokPrecedence();
        if (TokPrec < ExprPrec)
            return LHS;

        int BinOp = CurTok;
        getNextToken(); // Eat operator

        auto RHS = ParsePrimary();
        if (!RHS)
            return nullptr;

        int NextPrec = getTokPrecedence();
        if (TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec + 1, move(RHS));
            if (!RHS)
                return nullptr;
        }

        LHS = make_unique<BinaryExprAST>(BinOp, move(LHS), move(RHS));
    }
}

static unique_ptr<ExprAST> parseExpression() {
    auto LHS = ParsePrimary();
    if (!LHS)
        return nullptr;
    return ParseBinOpRHS(0, move(LHS));
}

// Main Loop
int main() {
    while (true) {
        cout << "Enter an expression: ";
        if (!getline(cin, InputBuffer) || InputBuffer.empty())
            break;
        InputPos = 0;  // Reset buffer position

        getNextToken();
        auto AST = parseExpression();

        if (AST) {
            cout << "\nParsed AST:\n";
            AST->print();
        } else {
            cout << "Parsing failed." << endl;
        }

        cout << endl;
    }

    return 0;
}
