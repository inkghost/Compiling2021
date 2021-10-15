#include "token.h"

using namespace std;

void CompUnit(FILE *, FILE *);
void FuncDef(FILE *, FILE *);
void FuncType(FILE *, FILE *);
void Ident(FILE *, FILE *);
void Block(FILE *, FILE *);
void Stmt(FILE *, FILE *);

void CompUnit(FILE *fp_input, FILE *fp_ir)
{
    fprintf(fp_ir, "define dso_local ");
    FuncDef(fp_input, fp_ir);
    FuncDef(fp_input, fp_ir);
    if (sym.type != 11)
    {
        throw "Error";
    }
    
}

void FuncDef(FILE *fp_input, FILE *fp_ir)
{
    nextsym(fp_input);
    FuncType(fp_input, fp_ir);

    nextsym(fp_input);
    Ident(fp_input, fp_ir);

    nextsym(fp_input);
    if (sym.type == 4)
    {
        fprintf(fp_ir, "(");
    }
    else
    {
        throw "Error";
    }

    nextsym(fp_input);
    if (sym.type == 5)
    {
        fprintf(fp_ir, ")");
    }
    else
    {
        throw "Error";
    }

    nextsym(fp_input);
    Block(fp_input, fp_ir);
}

void FuncType(FILE *fp_input, FILE *fp_ir)
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

void Ident(FILE *fp_input, FILE *fp_ir)
{
    if (sym.type == 2)
    {
        fprintf(fp_ir, "@main");
    }
    else
    {
        throw "Error";
    }
}

void Block(FILE *fp_input, FILE *fp_ir)
{
    if (sym.type == 6)
    {
        fprintf(fp_ir, "{\n    ");
    }
    else
    {
        throw "Error";
    }

    nextsym(fp_input);
    Stmt(fp_input, fp_ir);

    nextsym(fp_input);
    if (sym.type == 7)
    {
        fprintf(fp_ir, "\n}");
    }
    else
    {
        throw "Error";
    }
}

void Stmt(FILE *fp_input, FILE *fp_ir)
{
    if (sym.type == 3)
    {
        fprintf(fp_ir, "ret ");
    }
    else
    {
        throw "Error";
    }

    nextsym(fp_input);
    if (sym.type == 9)
    {
        fprintf(fp_ir, "i32 %d", sym.value);
    }
    else
    {
        throw "Error";
    }

    nextsym(fp_input);
    if (sym.type != 8)
    {
        throw "Error";
    }
}