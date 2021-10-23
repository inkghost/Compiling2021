#include "token.h"
#include <stack>

using namespace std;

void CompUnit();
void FuncDef();
void FuncType();
void Ident();
void Block();
void Stmt();
void Exp();
void AddExp();
void MulExp();
void UnaryExp();
void PrimaryExp();
void UnaryOp();

// 四则运算
void Operation();

struct ExpItem
{
    // 1 操作数，2 运算符，3 临时寄存器, 4 正负号运算
    int type;
    // 操作数的值
    // 运算符：18 +,19 -,20 *,21 /,22 %
    // 临时寄存器的次序
    int value;
};

int temp_register = 0;

struct ExpItem *exp_stack_tmp;

stack<struct ExpItem> exp_stack;

void CompUnit()
{
    fprintf(fp_ir, "define dso_local ");

    nextsym();
    FuncDef();

    nextsym();
    if (sym.type != 34)
    {
        throw "Error";
    }
}

void FuncDef()
{
    FuncType();

    nextsym();
    Ident();

    nextsym();
    if (sym.type == 9)
    {
        fprintf(fp_ir, "(");
    }
    else
    {
        throw "Error";
    }

    nextsym();
    if (sym.type == 10)
    {
        fprintf(fp_ir, ")");
    }
    else
    {
        throw "Error";
    }

    nextsym();
    Block();
}

void FuncType()
{
    if (sym.type == 1)
    {
        fprintf(fp_ir, "i32 ");
    }
    else
    {
        throw "Error";
    }
}

void Ident()
{
    if (sym.type == 33)
    {
        fprintf(fp_ir, "@%s", sym.ident);
    }
    else
    {
        throw "Error";
    }
}

void Block()
{
    if (sym.type == 13)
    {
        fprintf(fp_ir, "{\n");
    }
    else
    {
        throw "Error";
    }

    nextsym();
    Stmt();

    nextsym();
    if (sym.type == 14)
    {
        fprintf(fp_ir, "\n}");
    }
    else
    {
        throw "Error";
    }
}

void Stmt()
{
    if (sym.type != 8)
    {
        throw "Error";
    }

    nextsym();
    Exp();

    if (sym.type == 15)
    {
        exp_stack_tmp = &exp_stack.top();
        if (exp_stack_tmp->type == 1)
        {
            fprintf(fp_ir, "    ret i32 %d", exp_stack_tmp->value);
        }
        else if (exp_stack_tmp->type == 3)
        {
            fprintf(fp_ir, "    ret i32 %%%d", exp_stack_tmp->value);
        }
        else
        {
            throw "Error";
        }

        exp_stack.pop();
    }
    else
    {
        throw "Error";
    }
}

void Exp()
{
    AddExp();
}

void AddExp()
{
    MulExp();
    while (true)
    {
        exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
        // 使用 MulExp() 中最后读入的 sym 即可
        if (sym.type != 18 && sym.type != 19)
        {
            break;
        }
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack.push(*exp_stack_tmp);
        nextsym();
        MulExp();

        Operation();
    }
}

void MulExp()
{
    UnaryExp();
    while (true)
    {
        exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
        if (sym.type != 20 && sym.type != 21 && sym.type != 22)
        {
            break;
        }
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack.push(*exp_stack_tmp);
        nextsym();
        UnaryExp();

        Operation();
    }
}

void UnaryExp()
{
    if (sym.type == 9 || sym.type == 32)
    {
        PrimaryExp();
    }
    else if (sym.type == 18 || sym.type == 19)
    {
        UnaryOp();
        nextsym();
        UnaryExp();

        struct ExpItem num = exp_stack.top();
        exp_stack.pop();
        if (num.type != 1 && num.type != 3)
        {
            throw "Error";
        }

        if (exp_stack.empty())
        {
            exp_stack.push(num);
            return;
        }

        struct ExpItem op = exp_stack.top();
        exp_stack.pop();
        if (op.type != 4)
        {
            exp_stack.push(op);
            exp_stack.push(num);
            return;
        }
        else if (op.value == 19)
        {
            if (num.type == 1)
            {
                fprintf(fp_ir, "    %%%d = sub i32 0, %d\n", ++temp_register, num.value);
            }
            else
            {
                fprintf(fp_ir, "    %%%d = sub i32 0, %%%d\n", ++temp_register, num.value);
            }
            num.type = 3;
            num.value = temp_register;
        }

        exp_stack.push(num);
    }
    else
    {
        throw "Error";
    }
}

void PrimaryExp()
{
    if (sym.type == 9)
    {
        nextsym();
        Exp();
        if (sym.type == 10)
        {
            nextsym();
        }
        else
        {
            throw "Error";
        }
    }
    else if (sym.type == 32)
    {
        exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
        exp_stack_tmp->type = 1;
        exp_stack_tmp->value = sym.value;
        exp_stack.push(*exp_stack_tmp);
        nextsym();
    }
    else
    {
        throw "Error";
    }
}

void UnaryOp()
{
    if (sym.type == 18 || sym.type == 19)
    {
        exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
        exp_stack_tmp->type = 4;
        exp_stack_tmp->value = sym.type;
        exp_stack.push(*exp_stack_tmp);
    }
    else
    {
        throw "Error";
    }
}

// 处理四则运算
void Operation()
{
    struct ExpItem num1, num2, op;

    num2 = exp_stack.top();
    if (num2.type != 1 && num2.type != 3)
    {
        throw "Error";
    }
    exp_stack.pop();

    op = exp_stack.top();
    if (op.type != 2)
    {
        throw "Error";
    }
    exp_stack.pop();

    num1 = exp_stack.top();
    if (num1.type != 1 && num1.type != 3)
    {
        throw "Error";
    }
    exp_stack.pop();

    exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
    exp_stack_tmp->type = 3;
    exp_stack_tmp->value = ++temp_register;
    exp_stack.push(*exp_stack_tmp);

    fprintf(fp_ir, "    %%%d = ", exp_stack_tmp->value);
    switch (op.value)
    {
    case 18:
        fprintf(fp_ir, "add i32 ");
        break;
    case 19:
        fprintf(fp_ir, "sub i32 ");
        break;
    case 20:
        fprintf(fp_ir, "mul i32 ");
        break;
    case 21:
        fprintf(fp_ir, "sdiv i32 ");
        break;
    case 22:
        fprintf(fp_ir, "srem i32 ");
        break;
    default:
        throw "Error";
        break;
    }
    if (num1.type == 1)
    {
        fprintf(fp_ir, "%d, ", num1.value);
    }
    else
    {
        fprintf(fp_ir, "%%%d, ", num1.value);
    }
    if (num2.type == 1)
    {
        fprintf(fp_ir, "%d\n", num2.value);
    }
    else
    {
        fprintf(fp_ir, "%%%d\n", num2.value);
    }
}