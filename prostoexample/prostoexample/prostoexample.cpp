﻿#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <algorithm>

using namespace std;
enum class Token {
    Const = 0,
    Var,
    Ident,
    Type,
    Assignment,
    Semicolon,
    Relation,
    Numb,
    Minus,
    Mul,
    Div,
    Mod,
    Divine,
    Not,
    LParen,
    RParen,
    Plus,
    Or,
    Comma,
    Noc,
    Rav,
    If,
    Then,
    Else,
    Fi,
    Endm,
    While,
    For,
    Write,
    Read,
    Do,
    Od,
    Rof,
    OpBracket,
    ClBracket,
    OBracket,
    CBracket,
    Quote,
    Daer,
    Tirw,
    Eof,
    String,
    Char,
    Main,
    Default,
    Case,
    Switch,
    Colon,
    Break,
    QOBracket,
    QCBracket
};
map<Token, string> tokenToString = {
    {Token::Const, "Const"},
    {Token::Var, "Var"},
    {Token::Ident, "Ident"},
    {Token::Type, "Type"},
    {Token::Assignment, "Assignment"},
    {Token::Semicolon, "Semicolon"},
    {Token::Relation, "Relation"},
    {Token::Numb, "Numb"},
    {Token::Minus, "Minus"},
    {Token::Mul, "Mul"},
    {Token::Div, "Div"},
    {Token::Mod, "Mod"},
    {Token::Divine, "Divine"},
    {Token::Not, "Not"},
    {Token::LParen, "LParen"},
    {Token::RParen, "RParen"},
    {Token::Plus, "Plus"},
    {Token::Or, "Or"},
    {Token::Comma, "Comma"},
    {Token::Noc, "Noc"},
    {Token::Rav, "Rav"},
    {Token::If, "If"},
    {Token::Then, "Then"},
    {Token::Else, "Else"},
    {Token::Fi, "Fi"},
    {Token::Endm, "Endm"},
    {Token::While, "While"},
    {Token::For, "For"},
    {Token::Write, "Write"},
    {Token::Read, "Read"},
    {Token::Do, "Do"},
    {Token::Od, "Od"},
    {Token::Rof, "Rof"},
    {Token::OpBracket, "OpBracket"},
    {Token::ClBracket, "ClBracket"},
    {Token::OBracket, "OBracket"},
    {Token::CBracket, "CBracket"},
    {Token::QOBracket, "QOBracket"},
    {Token::QCBracket, "QCBracket"},
    {Token::Quote, "Quote"},
    {Token::Daer, "Daer"},
    {Token::Tirw, "Tirw"},
    {Token::String, "String"},
    {Token::Char, "Char"},
    {Token::Noc, "Noc"},
    {Token::Eof, "EOF"},
    {Token::Main, "Main"},
    {Token::Default, "Default"},
    {Token::Colon, "Colon"},
    {Token::Case, "Case"},
    {Token::Switch, "Switch"},
    {Token::Break, "Break"},
};

string toLower(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

std::map<std::string, Token> keywords = {
    std::make_pair(toLower("main"), Token::Main),
    std::make_pair(toLower("int"), Token::Type),
    std::make_pair(toLower("char"), Token::Type),
    std::make_pair(toLower("float"), Token::Type),
    std::make_pair(toLower("string"), Token::Type),
    std::make_pair(toLower("for"), Token::For),
    std::make_pair(toLower("rof"), Token::Rof),
    std::make_pair(toLower("while"), Token::While),
    std::make_pair(toLower("if"), Token::If),
    std::make_pair(toLower("fi"), Token::Fi),
    std::make_pair(toLower("else"), Token::Else),
    std::make_pair(toLower("then"), Token::Then),
    std::make_pair(toLower("const"), Token::Const),
    std::make_pair(toLower("noc"), Token::Noc),
    std::make_pair(toLower("read"), Token::Read),
    std::make_pair(toLower("dear"), Token::Daer),
    std::make_pair(toLower("endm"), Token::Endm),
    std::make_pair(toLower("write"), Token::Write),
    std::make_pair(toLower("tirw"), Token::Tirw),
    std::make_pair(toLower("do"), Token::Do),
    std::make_pair(toLower("od"), Token::Od),
    std::make_pair(toLower("div"), Token::Div),
    std::make_pair(toLower("mod"), Token::Mod),
    std::make_pair(toLower("var"), Token::Var),
    std::make_pair(toLower("rav"), Token::Rav),
    std::make_pair(toLower("/"), Token::Divine),
    std::make_pair(toLower("break"), Token::Break),
    std::make_pair(toLower("default"), Token::Default),
    std::make_pair(toLower("case"), Token::Case),
    std::make_pair(toLower("switch"), Token::Switch)
};

std::map<std::string, Token> separators = {
    std::make_pair(toLower("("), Token::OpBracket),
    std::make_pair(toLower(")"), Token::ClBracket),
    std::make_pair(toLower("["), Token::QOBracket),
    std::make_pair(toLower("]"), Token::QCBracket),
    std::make_pair(toLower("{"), Token::OBracket),
    std::make_pair(toLower("}"), Token::CBracket),
    std::make_pair(toLower(";"), Token::Semicolon),
    std::make_pair(toLower(","), Token::Comma),
    std::make_pair(toLower("/"), Token::Divine),
    std::make_pair(toLower("*"), Token::Mul),
    std::make_pair(toLower("+"), Token::Plus),
    std::make_pair(toLower("-"), Token::Minus),
    std::make_pair(toLower("or"), Token::Or),
    std::make_pair(toLower(":="), Token::Assignment),
    std::make_pair(toLower(":>"), Token::Colon)
};
std::map<std::string, Token> Relation = {
    std::make_pair("==", Token::Relation),
    std::make_pair("<=", Token::Relation),
    std::make_pair(">=", Token::Relation),
    std::make_pair("=<", Token::Relation),
    std::make_pair("<", Token::Relation),
    std::make_pair(">", Token::Relation)
};

class Lexer {
private:
    ifstream inputFile;
    ofstream outputFile;
    deque<string> stringAndChar;
    int lineNumber;
    string currentLine;
    size_t currentPosition;
    vector<Token> tokenList;
    Token token;
    int count = 0;
    Token getNextToken() {
        token = tokenList[count++];
        return token;
    }
public:
    Lexer(const string& filename) : lineNumber(0), currentPosition(0) {
        inputFile.open(filename);
        outputFile.open("output.txt");
    }

    ~Lexer() {
        inputFile.close();
        outputFile.close();
    }
   
    Token GetToken() {
        return getNextToken();
    }
    void parseConsts() {
        if (GetToken() != Token::Main) {
            throw exception("Expected main at the beginning of the program");
        }
        if (GetToken() != Token::Ident) {
            throw exception("Expected identifier at the beginning of the program");
        }
        Consts();
    }

    void EmptySt() {}
    //declaration

    void SectionVar()
    {
        if (GetToken() == Token::Type && GetToken() == Token::Ident)
        {
            token = GetToken();
            if (token == Token::Assignment)
            {
                Expr();
            }
            else if (token == Token::Comma)
            {
                ListId();
            }
            else
                throw exception("section var error");
        }
        else
            throw exception("section var error");
    }

    void ListSectionVars()
    {
        SectionVar();
        if (token == Token::Comma)
            ListSectionVars();
    }

    void Variables()
    {
        ListSectionVars();
        if (token != Token::Rav)
        {
            throw exception("expected 'Rav'");
        }
    }

    void SectionConst()
    {
        if (
            token == Token::Type
            && GetToken() == Token::Ident
            && GetToken() == Token::Assignment
            )
            Expr();
        else
            throw exception("section const error");
    }

    void ListSectionsConst()
    {
        token = GetToken();
        if (token == Token::Noc)
            return;
        else if (token == Token::Type)
        {
            SectionConst();
          //  token = GetToken();
            if (token == Token::Noc)
                return;
            else if (token == Token::Semicolon)
                ListSectionsConst();
            else
                throw exception("section const error1");

        }
    }

    void Consts()
    {
        if (GetToken() != Token::Const)
        {
            throw exception("expected 'Const'");

        }
        ListSectionsConst();
        if (token != Token::Noc)
        {
            throw exception("expected 'Noc'");
        }
    }
    //expression
    bool CheckMul()
    {
        return token == Token::Mul || token == Token::Divine
            || token == Token::Div || token == Token::Mod;
    }

    void TermPrime()
    {
     //   token = GetToken();
        if (CheckMul())
        {
            F();
            TermPrime();
        }
       /* else if (token != Token::Separator)
        {
            throw exception("excepted separator");
        }*/
    }

    void Term()
    {
        F();
        TermPrime();
    }

    bool CheckPlus()
    {
        return token == Token::Plus
            || token == Token::Or;
    }

    void SimpleExprPrime()
    {
        if (CheckPlus())
        {
            Term();
            SimpleExprPrime();
        }
    }

    void SimpleExpr()
    {
        Term();
        SimpleExprPrime();
    }

    bool CheckRelation()
    {
        return token == Token::Relation;
    }

    void ExprPrime()
    {
     //   token = GetToken();
        if (CheckRelation())
        {
            SimpleExpr();
            ExprPrime();
        }
    }

    void Expr()
    {
        SimpleExpr();
        ExprPrime();
    }

    void F()
    {
        token = GetToken();
        if (
            token == Token::Minus
            || token == Token::Not
            )
        {
            F();
        }
        else if (token == Token::OpBracket)
        {
            SimpleExpr();
            if (token == Token::ClBracket) {
                return;
            }
            else
            {
                throw exception("f error");
            }
        }
        else if (
            token == Token::Numb
            || token == Token::Ident || token == Token ::String || token == Token::Char
            )
        {
            token = GetToken();
            return;
        }
        else
            throw exception("f error");
    }
    //if
    void Assign()
    {
        token = GetToken();

        if (token != Token::Assignment)
        {
            throw exception("excepted ':='");
            return;
        }

        Expr();

        //token = GetToken();

        if (token != Token::Semicolon)
        {
            throw exception("excepted ';'");
        }
    }

    void A()
    {
        token = GetToken();

        if (token == Token::Semicolon)
        {
            EmptySt();
        }
        else if (token == Token::Else)
        {
            St();
        }
        else throw exception("Che napisat'-to?");
    }

    void If()
    {
        Expr();

        //token = GetToken();

        if (token != Token::Then)
        {
            throw exception("excepted 'then'");
            return;
        }

        St();
        A();

        token = GetToken();

        if (token != Token::Fi)
        {
            throw exception("excepted 'fi'");
        }
    }

    void St()
    {
        token = GetToken();

        switch (token)
        {
        case Token::Semicolon:
            EmptySt();
            break;
        case Token::Ident:
            Assign();
            break;
        case Token::If:
            If();
            break;
        case Token::While:
            While();
            break;
        case Token::For:
            For();
            break;
        case Token::Write:
            Write();
            break;
        case Token::Read:
            Read();
            break;
        case Token::Switch:
            SwitchCaseBlock();
            break;
        case Token::CBracket:
            break;
        case Token::Endm:
            throw exception("expected }");
            break;
        default:
            throw exception("St error");
            break;
        }
    }
    void ListStms()
    {

        St();

        //token = GetToken();

        if (token != Token::CBracket)
        {
            if (count == tokenList.size())
            {
                throw exception("expected }");
            }
            ListStms();
        }
    }
    //readwrite
    void ListId()
    {
        if (GetToken() != Token::Ident)
            throw exception("expected 'Ident'");
        if (GetToken() == Token::Comma) {
            ListId();
        }
        else if ( token == Token::Ident) {
            throw exception("expected comma");//обработка если нет запятой и за запятой любая белеберда!!!!!
        }
    }

    void Read()
    {
        if (GetToken() != Token::OpBracket) {
            throw exception("expected '('");
        }
        ListId();

        if (GetToken() != Token::ClBracket) {
            throw exception("expected ')'");
        }
        if (GetToken() != Token::Daer) {
            throw exception("expected 'Daer'");
        }
    }
    //Говно переделывай добавить обработку запятой могут быть несколько идентификаторов или строк некоторые gettoken пропускают
    void ElemWrite()
    {
        token = GetToken();
        if (token == Token::Ident || token == Token:: String || token ==Token:: CBracket)
        {
            token = GetToken();
        }
        else if (token != Token::String )//char пропускаем если не идентификатор теряем токен
        {
            throw exception("expected 'String'");
        }
        else if (token != Token::Char)
        {
            throw exception("expected 'Char'");
        }
        else {
            token = GetToken();
        }
    }

    void ListWrite()
    {
        ElemWrite();
        if (/*GetToken()*/token == Token::Comma) {
            ListWrite();
        }
    }

    void Write()
    {
        if (GetToken() != Token::OpBracket) {
            throw exception("expected '('");
        }
        ListWrite();

        if (token != Token::ClBracket) {
            throw exception("expected ')'");
        }
        if (GetToken() != Token::Tirw) {
            throw exception("expected 'Tirw'");
        }
    }

    //while
    void While() {
      /*  if (GetToken() != Token::While)
        {
            throw exception("Expected 'while'");
        }*/
        Expr();
        if (token != Token::Do)
        {
            throw exception("Expected 'do'");
        }
        ListStms();
        if (token == Token::Od)//НЕ ВОЗВРАЩАЕТСЯ В OD
        {
            throw exception("Expected 'od'");
        }
    }
    //for
    void For()
    {
        if (GetToken() != Token::OpBracket)
        {
            throw exception("Expected 'OpBracket'");
        }
        if (GetToken() != Token::Type)
        {
            throw exception("Expected type");
        }
        if (GetToken() != Token::Ident)
        {
            throw exception("Expected identi");
        }
        if (GetToken() != Token::Assignment)
        {
            throw exception("Expected assignment");
        }
        Expr();
        if (token != Token::Semicolon)
        {
            throw exception("Expected semicolon");
        }
        Expr();
        if (token != Token::Semicolon)
        {
            throw exception("Expected comma");
        }
        Expr();
        ListStms();
        if (GetToken() != Token::Rof)
        {
            throw exception("Expected 'rof'");
        }
    }


    void parseVariables() {
        if (GetToken() != Token::Var) {
            throw exception("Expected var");
        }
        while (token != Token::Rav) {
            SectionVar();
        }
    }

    void parseSt() {
        if (GetToken() != Token::OBracket) {
            throw exception("expected {");
        }

        ListStms();// Анализ основных инструкций программы

        if (GetToken() != Token::Endm) {
            throw exception("expected endm");
        }
    }

    void parse() {
            lineNumber++;
            currentPosition = 0;
            Process(inputFile, outputFile, tokenList);
            parseConsts();
            parseVariables();
            parseSt();
            printTokens(tokenList);

    }
    string tokenToStringFunc(Token token) {
        if (tokenToString.find(token) != tokenToString.end()) {
            return tokenToString[token];
        }
        else {
            return "Unknown Token";
        }
    }
    bool MakeToken(string& word, int indexStr, size_t position, deque<string>& vectorOfStringAndChar, ofstream& outputFile, vector<Token>& tokenList)
    {
        if (word == "")
        {
            return false;
        }

        for (auto& item : keywords)
        {
            size_t index = word.find(item.first);
            if (index != string::npos)
            {
                word.erase(index, item.first.size());
                string secondStr = word.substr(index, word.size() - index);
                word = word.substr(0, index);
                MakeToken(word, indexStr, position, vectorOfStringAndChar, outputFile, tokenList);
                tokenList.push_back(item.second);
                MakeToken(secondStr, indexStr, position + index + word.size(), vectorOfStringAndChar, outputFile, tokenList);
                return true;
            }
        }

        for (auto& item : separators)
        {
            size_t index = word.find(item.first);
            if (index != string::npos)
            {
                word.erase(index, item.first.size());
                string secondStr = word.substr(index, word.size() - index);
                word = word.substr(0, index);
                MakeToken(word, indexStr, position, vectorOfStringAndChar, outputFile, tokenList);
                tokenList.push_back(item.second);
                MakeToken(secondStr, indexStr, position + index + word.size(), vectorOfStringAndChar, outputFile, tokenList);
                return true;
            }
        }

        for (auto& item : Relation)
        {
            size_t index = word.find(item.first);
            if (index != string::npos)
            {
                word.erase(index, item.first.size());
                string secondStr = word.substr(index, word.size() - index);
                word = word.substr(0, index);
                MakeToken(word, indexStr, position, vectorOfStringAndChar, outputFile, tokenList);
                tokenList.push_back(item.second);
                MakeToken(secondStr, indexStr, position + index + word.size(), vectorOfStringAndChar, outputFile, tokenList);
                return true;
            }
        }

        if (word.front() == '\"' && word.back() == '\"')
        {
            tokenList.push_back(Token::String);
            return true;
        }
        else if (word.front() == '\'' && word.back() == '\'')
        {
            tokenList.push_back(Token::Char);
            return true;
        }


        //numbers
        if (word.find_first_not_of("0123456789.") == string::npos)
        {
            tokenList.push_back(Token::Numb);
            return true;
        }

        Token output;
        if (keywords.find(word) != keywords.end())
        {
            output = keywords[word];
            tokenList.push_back(output);
            return true;
        }

        if (separators.find(word) != separators.end())
        {
            output = separators[word];
            tokenList.push_back(output);
            return true;
        }

        if (word == "\"" && !vectorOfStringAndChar.empty())
        {
            word = vectorOfStringAndChar[0];
            vectorOfStringAndChar.pop_front();
            size_t i = word.find("\"") + 1;
            if (i < word.size() && word.find("\"", i) != string::npos)
            {
                tokenList.push_back(Token::Type);
            }
            else
            {
                outputFile << word << indexStr << " --- position " << position << "\n";
            }
            return true;
        }

        if (word == "\'" && !vectorOfStringAndChar.empty())
        {
            word = vectorOfStringAndChar[0];
            vectorOfStringAndChar.pop_front();
            size_t i = word.find("\'") + 1;
            if (i < word.size() && word.find("\'", i) != string::npos && word.size() == 3)
            {
                tokenList.push_back(Token::Type);
            }
            else
            {
                outputFile << word << indexStr << " --- position " << position << "\n";
            }
            return true;
        }

        if (isalpha(word[0]) || word[0] == '_') {
            for (auto i = 1; i < word.size(); i++) {
                if (!isalnum(word[i]) && word[i] != '_') {
                    outputFile << word << indexStr << " --- position " << position << "\n";
                    return true;
                }
            }

            tokenList.push_back(Token::Ident);
            return true;
        }

        outputFile << word << indexStr << " --- position " << position << "\n";
    }

    void processStringAndChar(deque<string>& vectorOfStringAndChar, string& inputStr, size_t indexStart)
    {
        if (inputStr.find("\"", indexStart) != string::npos)
        {
            string str;
            size_t start = inputStr.find("\"", indexStart);
            size_t end = inputStr.size() - 1;
            if (start + 1 < inputStr.size() && inputStr.find("\"", start + 1) != string::npos)
            {
                end = inputStr.find("\"", start + 1);
            }
            str = inputStr.substr(start, end - start + 1);
            vectorOfStringAndChar.push_back(str);
            inputStr.erase(start + 1, end - start);

            processStringAndChar(vectorOfStringAndChar, inputStr, start + 1);
        }
        if (inputStr.find("\'", indexStart) != string::npos)
        {
            string str;
            size_t start = inputStr.find("\'", indexStart);
            size_t end = inputStr.size() - 1;
            if (start + 1 < inputStr.size() and inputStr.find("\'", start + 1) != string::npos)
            {
                end = inputStr.find("\'", start + 1);
            }
            str = inputStr.substr(start, end - start + 1);
            vectorOfStringAndChar.push_back(str);
            inputStr.erase(start + 1, end - start);

            processStringAndChar(vectorOfStringAndChar, inputStr, start + 1);
        }
    }

    void ProcessStr(ifstream& inputFile, string& inputStr, int& indexStr, ofstream& outputFile, vector<Token>& tokenList)
    {
        deque<string> vectorOfStringAndChar;
        processStringAndChar(vectorOfStringAndChar, inputStr, 0);

        size_t position = 1;
        if (!inputStr.empty())
        {
            vector<string> words;
            string item;
            istringstream strStream(inputStr);
            while (getline(strStream, item, ' '))
            {
                MakeToken(item, indexStr, position, vectorOfStringAndChar, outputFile, tokenList);
                position += item.size() + 1;
            }
        }
    }
    void printTokens(const vector<Token>& tokenList) {
        for (Token token : tokenList) {
            cout << "Token: " << tokenToStringFunc(token) << endl;
        }
    }
   
    void Process(ifstream& input, ofstream& output, vector<Token>& tokenList) {
        string inputStr, inputString;
        int indexStr = 1;

        while (getline(input, inputStr)) {
            if (inputStr.empty()) {
                continue;
            }
            inputString = toLower(inputStr);
            ProcessStr(input, inputString, indexStr, output, tokenList);
            indexStr++;
        }

        tokenList.push_back(Token::Eof);
        output << "EOF --- End of File\n";
    }

    void CaseBody()
    {
        if (GetToken() == Token::Colon)
        {
            St();
            if (GetToken() == Token::Semicolon &&
                GetToken() == Token::Break &&
                GetToken() == Token::Semicolon)
            {
                return;
            }
            else
            {
                throw exception("Expected sequence '; break ;' after statement");
            }
        }
        else
        {
            throw exception("Expected sequence 'ident :' inside switch case");
        }
    }

    void SwitchCaseBlock()
    {
        SwitchCase();
        //GetToken();
    }

    void SwitchCase()
    {
        if (token == Token::Switch && GetToken() == Token::Ident && GetToken() == Token::QOBracket)
        {
            const unsigned CaseCountLimit = 1000;
            for (unsigned caseNumber = 0; caseNumber <= CaseCountLimit; caseNumber++)
            {
                token = GetToken();
                if (token == Token::QCBracket)
                {
                    if (caseNumber == 0)
                    {
                        throw exception("Expected min 1 case or default case inside switch case body!'");
                    }
                    return;
                } 
                else if (caseNumber == CaseCountLimit)
                {
                    string msg = "Error: Out of switch case range! Max available cases count ";
                    stringstream ss;
                    ss << msg << CaseCountLimit;
                    throw exception(ss.str().c_str());
                }
                else if (token == Token::Default)
                {
                    CaseBody();
                    if (GetToken() == Token::QCBracket)
                    {
                        return;
                    } 
                    else
                    {
                        throw exception("Expected ']' after default case !'");
                    }
                }
                else if (token == Token::Case)
                {
                    if (GetToken() == Token::Ident)
                    {
                        CaseBody();
                    }
                    else
                    {
                        throw exception("Expected ident after 'case'");
                    }
                }
                else
                {
                    throw exception("Expected 'case' or 'default' inside switch case");
                }
            }
        }
        else
        {
            throw exception("Expected sequence 'switch ident ['");
        }
    }
};
int main()
{
    Lexer lexer("input.txt");
    vector<Token> tokenList;

    try {
        // Начать с анализа объявлений констант и переменных
        lexer.parse();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    lexer.printTokens(tokenList);
    return 0;
}
