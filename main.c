#include <stdio.h>
#include <ctype.h>
#include <string.h>

int issymbol(char);
void printIdent(char *);

int main()
{
    char first_char;
    char next_char;
    char token[128];

    while (~(first_char = getchar()))
    {
        if (isalpha(first_char) || first_char == '_')
        {
            memset(token, 0, sizeof(token));
            token[0] = first_char;
            while (1)
            {
                next_char = getchar();
                if (isalnum(next_char) || next_char == '_')
                {
                    token[strlen(token)] = next_char;
                }
                else
                {
                    printIdent(token);
                    ungetc(next_char, stdin);
                    break;
                }
            }
        }
        else if (isdigit(first_char))
        {
            memset(token, 0, sizeof(token));
            token[0] = first_char;
            while (1)
            {
                next_char = getchar();
                if (isdigit(next_char))
                {
                    token[strlen(token)] = next_char;
                }
                else
                {
                    printf("Number(%s)\n", token);
                    ungetc(next_char, stdin);
                    break;
                }
            }
        }
        else if (issymbol(first_char))
        {
            continue;
        }
        else if (first_char == '=')
        {
            next_char = getchar();
            if (next_char == '=')
            {
                printf("Eq\n");
            }
            else
            {
                printf("Assign\n");
                ungetc(next_char, stdin);
            }
        }
        else if (isspace(first_char))
        {
            continue;
        }
        else
        {
            printf("Err\n");
            return 1;
        }
    }

    return 0;
}

// 判断是否是除 = 外的单分界符
int issymbol(char c)
{
    switch (c)
    {
    case ';':
        printf("Semicolon\n");
        break;
    case '(':
        printf("LPar\n");
        break;
    case ')':
        printf("RPar\n");
        break;
    case '{':
        printf("LBrace\n");
        break;
    case '}':
        printf("RBrace\n");
        break;
    case '+':
        printf("Plus\n");
        break;
    case '*':
        printf("Mult\n");
        break;
    case '/':
        printf("Div\n");
        break;
    case '<':
        printf("Lt\n");
        break;
    case '>':
        printf("Gt\n");
        break;
    default:
        return 0;
    }

    return 1;
}

// 输出标识符
void printIdent(char *token)
{
    if (strcmp("if", token) == 0)
    {
        printf("If\n");
    }
    else if (strcmp("else", token) == 0)
    {
        printf("Else\n");
    }
    else if (strcmp("while", token) == 0)
    {
        printf("While\n");
    }
    else if (strcmp("break", token) == 0)
    {
        printf("Break\n");
    }
    else if (strcmp("continue", token) == 0)
    {
        printf("Continue\n");
    }
    else if (strcmp("return", token) == 0)
    {
        printf("Return\n");
    }
    else
    {
        printf("Ident(%s)\n", token);
    }
}