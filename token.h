#include <cstdio>
#include <cctype>
#include <cstring>
#include <sstream>
#include <cstdlib>

using namespace std;

bool isNonzeroDigit(int);

bool isOctalDigit(int);

bool isHexadecimalDigit(int);

bool isSymbol(int);

int hex2dec(char *);

int oct2dec(char *);

typedef struct lexical
{
    int type;
    int value;
    char ident[128];
} Lexical;

Lexical sym;

Lexical nextsym(FILE *fp_input)
{
    char first_char;
    char next_char;
    char token[128];

    sym.type = -1;

    while (~(first_char = fgetc(fp_input)))
    {
        // 标识符
        if (isalpha(first_char))
        {
            memset(token, 0, sizeof(token));
            token[0] = first_char;
            while (true)
            {
                next_char = fgetc(fp_input);
                if (isalpha(next_char))
                {
                    token[strlen(token)] = next_char;
                }
                else
                {
                    ungetc(next_char, fp_input);

                    if (strcmp(token, "int") == 0)
                        sym.type = 1;
                    else if (strcmp(token, "main") == 0)
                        sym.type = 2;
                    else if (strcmp(token, "return") == 0)
                        sym.type = 3;
                    else
                    {
                        sym.type = 10;
                        strcpy(sym.ident, token);
                    }

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
                    sym.type = 9;
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
                            sym.type = 9;
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
                        sym.type = 9;
                        sym.value = oct2dec(token);
                        return sym;
                    }
                }
            }
            else
            {
                ungetc(next_char, fp_input);
                sym.type = 9;
                sym.value = oct2dec(token);
                return sym;
            }
        }
        else if (isSymbol(first_char))
        {
            switch (first_char)
            {
            case '(':
                sym.type = 4;
                break;
            case ')':
                sym.type = 5;
                break;
            case '{':
                sym.type = 6;
                break;
            case '}':
                sym.type = 7;
                break;
            case ';':
                sym.type = 8;
                break;
            }
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
            else
            {
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

    sym.type = 11;

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

bool isSymbol(int c)
{
    return c == ';' || c == '(' || c == ')' || c == '{' || c == '}';
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