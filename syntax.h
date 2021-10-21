#include "token.h"

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
        fprintf(fp_ir, "{\n    ");
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
    if (sym.type == 8)
    {
        fprintf(fp_ir, "ret ");
    }
    else
    {
        throw "Error";
    }

    nextsym();
    if (sym.type == 32)
    {
        fprintf(fp_ir, "i32 %d", sym.value);
    }
    else
    {
        throw "Error";
    }

    nextsym();
    if (sym.type != 15)
    {
        throw "Error";
    }
}