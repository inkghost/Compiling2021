#include <cstdio>
#include <cctype>
#include <cstring>
#include <sstream>
#include <cstdlib>

using namespace std;

FILE *fp_input, *fp_ir;

bool isNonzeroDigit(int);

bool isOctalDigit(int);

bool isHexadecimalDigit(int);

int isSingleSymbol(int);

int isDoubleSymbol(int);

int hex2dec(char *);

int oct2dec(char *);

void identJudge(char *);

typedef struct lexical
{
    int type;
    int value;
    char ident[128];
} Lexical;

Lexical sym;

Lexical nextsym()
{
    char first_char;
    char next_char;
    char token[128];
    int symbolRet = -1;

    sym.type = -1;

    while (~(first_char = fgetc(fp_input)))
    {
        // 标识符
        if (isalpha(first_char) || first_char == '_')
        {
            memset(token, 0, sizeof(token));
            token[0] = first_char;
            while (true)
            {
                next_char = fgetc(fp_input);
                if (isalpha(next_char) || isdigit(next_char) || next_char == '_')
                {
                    token[strlen(token)] = next_char;
                }
                else
                {
                    ungetc(next_char, fp_input);

                    identJudge(token);

                    return sym;
                }
            }
        }
        // 十进制
        else if (isNonzeroDigit(first_char))
        {
            memset(token, 0, sizeof(token));
            token[0] = first_char;
            while (true)
            {
                next_char = fgetc(fp_input);
                if (isdigit(next_char))
                {
                    token[strlen(token)] = next_char;
                }
                else
                {
                    ungetc(next_char, fp_input);
                    sym.type = 32;
                    sym.value = atoi(token);
                    return sym;
                }
            }
        }
        // 十六进制和八进制
        else if (first_char == '0')
        {
            memset(token, 0, sizeof(token));
            token[0] = first_char;
            next_char = fgetc(fp_input);
            // 十六进制
            if (next_char == 'x' || next_char == 'X')
            {
                token[1] = next_char;
                next_char = fgetc(fp_input);
                if (isHexadecimalDigit(next_char))
                {
                    token[2] = next_char;
                    while (true)
                    {
                        next_char = fgetc(fp_input);
                        if (isHexadecimalDigit(next_char))
                        {
                            token[strlen(token)] = next_char;
                        }
                        else
                        {
                            ungetc(next_char, fp_input);
                            sym.type = 32;
                            sym.value = hex2dec(token);
                            return sym;
                        }
                    }
                }
                else
                {
                    return sym;
                }
            }
            // 八进制
            else if (isOctalDigit(next_char))
            {
                token[1] = next_char;
                while (true)
                {
                    next_char = fgetc(fp_input);
                    if (isOctalDigit(next_char))
                    {
                        token[strlen(token)] = next_char;
                    }
                    else
                    {
                        ungetc(next_char, fp_input);
                        sym.type = 32;
                        sym.value = oct2dec(token);
                        return sym;
                    }
                }
            }
            else
            {
                ungetc(next_char, fp_input);
                sym.type = 32;
                sym.value = oct2dec(token);
                return sym;
            }
        }
        // 单分界符判断
        else if ((symbolRet = isSingleSymbol(first_char)) != 0)
        {
            sym.type = symbolRet;
            return sym;
        }
        // 双分界符判断
        else if ((symbolRet = isDoubleSymbol(first_char)) != 0)
        {
            sym.type = symbolRet;
            return sym;
        }
        // 消除注释
        else if (first_char == '/')
        {
            next_char = fgetc(fp_input);
            if (next_char == '/')
            {
                while (~(next_char = fgetc(fp_input)))
                {
                    if (next_char == '\n')
                        break;
                }
            }
            else if (next_char == '*')
            {
                while (~(next_char = fgetc(fp_input)))
                {
                    if (next_char == '*')
                    {
                        next_char = fgetc(fp_input);
                        if (next_char == '/')
                        {
                            break;
                        }
                    }
                }
            }
            // 除号
            else
            {
                ungetc(next_char, fp_input);
                sym.type = 21;
                return sym;
            }
        }
        else if (isspace(first_char))
        {
            continue;
        }
        else
        {
            return sym;
        }
    }

    sym.type = 34;

    return sym;
}

bool isNonzeroDigit(int c)
{
    return isdigit(c) && c != '0';
}

bool isOctalDigit(int c)
{
    return c >= '0' && c <= '7';
}

bool isHexadecimalDigit(int c)
{
    return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

int isSingleSymbol(int c)
{
    int ret = 0;

    switch (c)
    {
    case '(':
        ret = 9;
        break;
    case ')':
        ret = 10;
        break;
    case '[':
        ret = 11;
        break;
    case ']':
        ret = 12;
        break;
    case '{':
        ret = 13;
        break;
    case '}':
        ret = 14;
        break;
    case ';':
        ret = 15;
        break;
    case ',':
        ret = 16;
        break;
    case '+':
        ret = 18;
        break;
    case '-':
        ret = 19;
        break;
    case '*':
        ret = 20;
        break;
    case '%':
        ret = 22;
        break;
    }

    return ret;
}

int isDoubleSymbol(int c)
{
    char next_char;
    int ret = 0;

    if (c == '=')
    {
        next_char = fgetc(fp_input);
        if (next_char == '=')
        {
            ret = 28;
        }else {
            ungetc(next_char, fp_input);
            ret = 17;
        }
    }
    else if (c == '!')
    {
        next_char = fgetc(fp_input);
        if (next_char == '=')
        {
            ret = 29;
        }else {
            ungetc(next_char, fp_input);
            ret = 23;
        }
    }
    else if (c == '<')
    {
        next_char = fgetc(fp_input);
        if (next_char == '=')
        {
            ret = 26;
        }else {
            ungetc(next_char, fp_input);
            ret = 24;
        }
    }
    else if (c == '>')
    {
        next_char = fgetc(fp_input);
        if (next_char == '=')
        {
            ret = 27;
        }else {
            ungetc(next_char, fp_input);
            ret = 25;
        }
    }
    else if (c == '&')
    {
        next_char = fgetc(fp_input);
        if (next_char == '&')
        {
            ret = 30;
        }else {
            ret = -1;
        }
    }
    else if (c == '|')
    {
        next_char = fgetc(fp_input);
        if (next_char == '|')
        {
            ret = 31;
        }else {
            ret = -1;
        }
    }

    return ret;
}

int hex2dec(char *hexstr)
{
    int decn;
    stringstream ss;
    ss.str(hexstr);
    ss >> hex >> decn;
    return decn;
}

int oct2dec(char *octstr)
{
    int decn;
    stringstream ss;
    ss.str(octstr);
    ss >> oct >> decn;
    return decn;
}

void identJudge(char *token)
{
    if (strcmp(token, "const") == 0)
        sym.type = 0;
    else if (strcmp(token, "int") == 0)
        sym.type = 1;
    else if (strcmp(token, "void") == 0)
        sym.type = 2;
    else if (strcmp(token, "if") == 0)
        sym.type = 3;
    else if (strcmp(token, "else") == 0)
        sym.type = 4;
    else if (strcmp(token, "while") == 0)
        sym.type = 5;
    else if (strcmp(token, "break") == 0)
        sym.type = 6;
    else if (strcmp(token, "continue") == 0)
        sym.type = 7;
    else if (strcmp(token, "return") == 0)
        sym.type = 8;
    else
    {
        sym.type = 33;
        strcpy(sym.ident, token);
    }
}