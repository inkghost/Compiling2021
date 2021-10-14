#include <cstdio>
#include <cctype>
#include <cstring>

int nextsym(FILE *fp_input)
{
    char first_char;
    char next_char;
    char token[128];

    while (!(first_char = fgetc(fp_input)))
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
                    printf("Ident: %s\n", token);
                    ungetc(next_char, fp_input);
                    break;
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
                    printf("decimal-const: %s\n", token);
                    ungetc(next_char, fp_input);
                    break;
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
                            printf("hexadecimal-const: %s\n", token);
                            ungetc(next_char, fp_input);
                            break;
                        }
                    }
                }
                else
                {
                    printf("Err\n");
                    ungetc(next_char, fp_input);
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
                        printf("octal-const: %s\n", token);
                        ungetc(next_char, fp_input);
                        break;
                    }
                }
            }
            else
            {
                printf("octal-const: %s\n", token);
                ungetc(next_char, fp_input);
            }
        }
        else if (isSymbol(first_char))
        {
            printf("symbol: %c\n", first_char);
        }
        else if (isspace(first_char))
        {
            continue;
        }
        else
        {
            printf("Err\n");
            return 0;
        }
    }

    return 0;
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