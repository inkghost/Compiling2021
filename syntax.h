#include "token.h"
#include <stack>
#include <map>
#include <list>
#include <vector>

using namespace std;

void CompUnit();
void Decl();
void ConstDecl();
void BType();
void ConstDef();
void ConstInitVal();
void ConstExp();
void VarDecl();
void VarDef();
void InitVal();
void FuncDef();
void FuncType();
void Block();
void BlockItem();
void Stmt();
void Exp();
void Cond();
int LVal();
void PrimaryExp();
void UnaryExp();
void UnaryOp();
void FuncRParams(int);
void MulExp();
void AddExp();
void RelExp();
void EqExp();
void LAndExp();
void LOrExp();

// 四则运算
void Operation();
// 非运算
void NotOperation(struct ExpItem);
// 比较运算
void CmpOperation();
// 位运算
void BitOperation();
// 打印嵌套空格
void PrintSpace();
// 初始化 FuncMap
void FuncMapInit();
// 函数调用分析
void FuncCall();
// 将栈顶元素转化为 i1
void toBool();
// 将栈顶元素转化为 i32
void toInt();
// 记录运算过程中是否出现变量
bool have_var_in_cal;
// 记录该变量是否是常量
bool this_is_const;
// 记录是否处于 Cond 判断中
bool is_in_cond;
// 记录基础块个数
int basic_block;

struct ExpItem
{
    // 1 操作数，2 运算符，3 临时寄存器, 4 正负号运算, 5 布尔值
    int type;
    // 操作数的值
    // 运算符：18 +,19 -,20 *,21 /,22 %, 23 !, 24 <, 25 >, 26 <=, 27 >=, 28 ==, 29 !=, 30 &&, 31 ||
    // 临时寄存器的次序
    int value;
};

struct VarItem
{
    // 是否是常量
    bool is_const;
    // 临时寄存器的值
    int register_num;
};

struct FuncItem
{
    // 函数返回值类型 0 void, 1 int
    int type;
    // 函数参数列表 0 void, 1 int, 2 int*
    vector<int> params;
};

// 全局变量map
map<string, struct VarItem> global_var_map;
// 变量map
map<string, struct VarItem> var_map;
// 变量迭代器
map<string, struct VarItem>::iterator var_it;
// 函数map
map<string, struct FuncItem> func_map;
// 函数迭代器
map<string, struct FuncItem>::iterator func_it;

int temp_register = 0;

struct ExpItem *exp_stack_tmp;

stack<struct ExpItem> exp_stack;
// 变量 map 队列
list<map<string, struct VarItem>> var_map_list;

void CompUnit()
{
    FuncMapInit();

    fprintf(fp_ir, "define dso_local ");

    nextsym();
    FuncDef();

    nextsym();
    if (sym.type != 34)
    {
        throw "Error";
    }
}

void Decl()
{
    if (sym.type == 0)
    {
        ConstDecl();
    }
    else if (sym.type == 1)
    {
        VarDecl();
    }
    else
    {
        throw "Error";
    }
}

void ConstDecl()
{
    if (sym.type != 0)
    {
        throw "Error";
    }

    nextsym();
    BType();

    do
    {
        nextsym();
        ConstDef();
    } while (sym.type == 16);

    if (sym.type != 15)
    {
        throw "Error";
    }
}

void BType()
{
    if (sym.type != 1)
    {
        throw "Error";
    }
}

void ConstDef()
{
    if (sym.type != 33)
    {
        throw "Error";
    }
    struct VarItem *var_item_tmp;

    // 检查变量是否重复声明
    var_it = var_map.find((string)sym.ident);
    if (var_it != var_map.end())
    {
        throw "Error";
    }

    var_item_tmp = (struct VarItem *)malloc(sizeof(struct VarItem));
    var_item_tmp->is_const = true;
    var_item_tmp->register_num = ++temp_register;

    var_map[sym.ident] = *var_item_tmp;

    PrintSpace();
    fprintf(fp_ir, "%%%d = alloca i32\n", temp_register);

    nextsym();
    if (sym.type != 17)
    {
        throw "Error";
    }

    nextsym();
    ConstInitVal();

    exp_stack_tmp = &exp_stack.top();
    PrintSpace();
    if (exp_stack_tmp->type == 1)
    {
        fprintf(fp_ir, "store i32 %d, i32* %%%d\n", exp_stack_tmp->value, var_item_tmp->register_num);
    }
    else if (exp_stack_tmp->type == 3)
    {
        fprintf(fp_ir, "store i32 %%%d, i32* %%%d\n", exp_stack_tmp->value, var_item_tmp->register_num);
    }
    else
    {
        throw "Error";
    }

    exp_stack.pop();
}

void ConstInitVal()
{
    have_var_in_cal = false;
    ConstExp();
    if (have_var_in_cal)
    {
        throw "Error";
    }
}

void ConstExp()
{
    AddExp();
}

void VarDecl()
{
    BType();

    do
    {
        nextsym();
        VarDef();
    } while (sym.type == 16);

    if (sym.type != 15)
    {
        throw "Error";
    }
}

void VarDef()
{
    if (sym.type != 33)
    {
        throw "Error";
    }
    struct VarItem *var_item_tmp;

    // 检查变量是否重复声明
    var_it = var_map.find((string)sym.ident);
    if (var_it != var_map.end())
    {
        throw "Error";
    }

    var_item_tmp = (struct VarItem *)malloc(sizeof(struct VarItem));
    var_item_tmp->is_const = false;
    var_item_tmp->register_num = ++temp_register;

    var_map[sym.ident] = *var_item_tmp;

    PrintSpace();
    fprintf(fp_ir, "%%%d = alloca i32\n", temp_register);

    nextsym();
    if (sym.type != 17)
    {
        return;
    }

    nextsym();
    InitVal();

    exp_stack_tmp = &exp_stack.top();
    PrintSpace();
    if (exp_stack_tmp->type == 1)
    {
        fprintf(fp_ir, "store i32 %d, i32* %%%d\n", exp_stack_tmp->value, var_item_tmp->register_num);
    }
    else if (exp_stack_tmp->type == 3)
    {
        fprintf(fp_ir, "store i32 %%%d, i32* %%%d\n", exp_stack_tmp->value, var_item_tmp->register_num);
    }
    else
    {
        throw "Error";
    }

    exp_stack.pop();
}

void InitVal()
{
    Exp();
}

void FuncDef()
{
    FuncType();

    nextsym();
    if (sym.type == 33)
    {
        fprintf(fp_ir, "@%s", sym.ident);
    }
    else
    {
        throw "Error";
    }

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

    fprintf(fp_ir, "{\n");

    nextsym();
    Block();

    fprintf(fp_ir, "\n}");
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

void Block()
{
    var_map_list.push_back(var_map);
    var_map.clear();

    if (sym.type != 13)
    {
        throw "Error";
    }

    nextsym();
    while (sym.type != 14)
    {
        BlockItem();
        nextsym();
    }

    var_map = var_map_list.back();
    var_map_list.pop_back();
}

void BlockItem()
{
    if (sym.type == 0 || sym.type == 1)
    {
        Decl();
    }
    else
    {
        Stmt();
    }
}

void Stmt()
{
    if (sym.type == 8)
    {
        nextsym();
        Exp();

        exp_stack_tmp = &exp_stack.top();
        PrintSpace();
        if (exp_stack_tmp->type == 1)
        {
            fprintf(fp_ir, "ret i32 %d", exp_stack_tmp->value);
        }
        else if (exp_stack_tmp->type == 3)
        {
            fprintf(fp_ir, "ret i32 %%%d", exp_stack_tmp->value);
        }
        else
        {
            throw "Error";
        }

        exp_stack.pop();

        if (sym.type != 15)
        {
            throw "Error";
        }
    }
    else if (sym.type == 13)
    {
        Block();
    }
    // 条件语句
    else if (sym.type == 3)
    {
        nextsym();
        if (sym.type != 9)
        {
            throw "Error";
        }

        nextsym();
        Cond();

        if (sym.type != 10)
        {
            throw "Error";
        }

        struct ExpItem cond = exp_stack.top();
        if (cond.type != 5)
        {
            throw "Error";
        }
        exp_stack.pop();

        int if_block = ++basic_block;
        int out_block = ++basic_block;

        PrintSpace();
        fprintf(fp_ir, "br i1 %%%d ,label %%basic_block_%d, label %%basic_block_%d\n", cond.value, if_block, out_block);

        fprintf(fp_ir, "basic_block_%d:\n", if_block);

        nextsym();
        Stmt();

        Lexical tmp_sym = sym;
        nextsym();

        if (sym.type == 4)
        {
            int else_block = out_block;
            out_block = ++basic_block;

            PrintSpace();
            fprintf(fp_ir, "br label %%basic_block_%d\n", out_block);
            fprintf(fp_ir, "basic_block_%d:\n", else_block);

            nextsym();
            Stmt();

            PrintSpace();
            fprintf(fp_ir, "br label %%basic_block_%d\n", out_block);
        }
        else
        {
            swap(tmp_sym, sym);
            backsysm(tmp_sym);
            PrintSpace();
            fprintf(fp_ir, "br label %%basic_block_%d\n", out_block);
        }

        fprintf(fp_ir, "basic_block_%d:\n", out_block);
    }
    else if (sym.type == 33)
    {
        Lexical tmp_sym = sym;
        nextsym();
        if (sym.type == 9)
        {
            ungetc('(', fp_input);
            sym = tmp_sym;
            Exp();
        }
        else if (sym.type == 17)
        {
            sym = tmp_sym;

            this_is_const = false;
            int lval_register = LVal();

            if (this_is_const)
            {
                throw "Error";
            }

            nextsym();
            Exp();

            exp_stack_tmp = &exp_stack.top();
            PrintSpace();
            if (exp_stack_tmp->type == 1)
            {
                fprintf(fp_ir, "store i32 %d, i32* %%%d\n", exp_stack_tmp->value, lval_register);
            }
            else if (exp_stack_tmp->type == 3)
            {
                fprintf(fp_ir, "store i32 %%%d, i32* %%%d\n", exp_stack_tmp->value, lval_register);
            }
            else
            {
                throw "Error";
            }

            exp_stack.pop();
        }

        if (sym.type != 15)
        {
            throw "Error";
        }
    }
    else if (sym.type != 15)
    {
        Exp();
        exp_stack.pop();

        if (sym.type != 15)
        {
            throw "Error";
        }
    }
}

void Exp()
{
    AddExp();
}

void Cond()
{
    is_in_cond = true;
    LOrExp();
    is_in_cond = false;
}

int LVal()
{
    if (sym.type != 33)
    {
        throw "Error";
    }

    bool is_declared = false;

    // 检查变量是否被声明
    var_it = var_map.find((string)sym.ident);
    if (var_it != var_map.end())
    {
        is_declared = true;
    }

    if (!is_declared)
    {
        for (list<map<string, struct VarItem>>::reverse_iterator it = var_map_list.rbegin(); it != var_map_list.rend(); ++it)
        {
            var_it = (*it).find((string)sym.ident);
            if (var_it != (*it).end())
            {
                is_declared = true;
                break;
            }
        }
    }

    if (!is_declared)
    {
        var_it = global_var_map.find((string)sym.ident);
        if (var_it == global_var_map.end())
        {
            throw "Error";
        }
    }

    if ((*var_it).second.is_const)
    {
        this_is_const = true;
    }
    else
    {
        have_var_in_cal = true;
    }

    return (*var_it).second.register_num;
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
    else if (sym.type == 33)
    {
        int lval_register = LVal();

        exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
        exp_stack_tmp->type = 3;
        exp_stack_tmp->value = ++temp_register;
        exp_stack.push(*exp_stack_tmp);

        PrintSpace();
        fprintf(fp_ir, "%%%d = load i32, i32* %%%d", exp_stack_tmp->value, lval_register);

        nextsym();
    }
    else
    {
        throw "Error";
    }
}

void UnaryExp()
{
    if (sym.type == 9 || sym.type == 32)
    {
        PrimaryExp();
    }
    else if (sym.type == 18 || sym.type == 19 || (is_in_cond && sym.type == 23))
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
        // 进行单目前缀运算
        if (op.type != 4)
        {
            exp_stack.push(op);
            exp_stack.push(num);
            return;
        }
        else if (op.value == 19)
        {
            PrintSpace();
            if (num.type == 1)
            {
                fprintf(fp_ir, "%%%d = sub i32 0, %d\n", ++temp_register, num.value);
            }
            else
            {
                fprintf(fp_ir, "%%%d = sub i32 0, %%%d\n", ++temp_register, num.value);
            }
            num.type = 3;
            num.value = temp_register;
        }
        else if (is_in_cond && op.value == 23)
        {
            bool doNotOperation = true;
            op = exp_stack.top();
            while (op.value == 23)
            {
                doNotOperation = !doNotOperation;
                exp_stack.pop();
                op = exp_stack.top();
            }
            if (doNotOperation)
            {
                NotOperation(num);
                return;
            }
        }

        exp_stack.push(num);
    }
    else if (sym.type == 33)
    {
        Lexical tmp_sym = sym;
        nextsym();
        // 函数调用处理
        if (sym.type == 9)
        {
            swap(tmp_sym, sym);

            FuncCall();

            if (sym.type != 10)
            {
                throw "Error";
            }

            nextsym();
        }
        else
        {
            swap(tmp_sym, sym);
            int lval_register = LVal();

            exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
            exp_stack_tmp->type = 3;
            exp_stack_tmp->value = ++temp_register;
            exp_stack.push(*exp_stack_tmp);

            PrintSpace();
            fprintf(fp_ir, "%%%d = load i32, i32* %%%d\n", exp_stack_tmp->value, lval_register);

            swap(tmp_sym, sym);
        }
    }
    else
    {
        throw "Error";
    }
}

void UnaryOp()
{
    if (sym.type == 18 || sym.type == 19 || (is_in_cond && sym.type == 23))
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

void FuncRParams(int params_count)
{
    do
    {
        Exp();
        params_count--;
    } while (sym.type == 16);
    if (params_count != 0)
    {
        throw "Error";
    }
}

void MulExp()
{
    UnaryExp();
    while (true)
    {
        if (sym.type != 20 && sym.type != 21 && sym.type != 22)
        {
            break;
        }
        exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack.push(*exp_stack_tmp);
        nextsym();
        UnaryExp();

        Operation();
    }
}

void AddExp()
{
    MulExp();
    while (true)
    {
        // 使用 MulExp() 中最后读入的 sym 即可
        if (sym.type != 18 && sym.type != 19 && !(is_in_cond && sym.type == 23))
        {
            break;
        }
        exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack.push(*exp_stack_tmp);
        nextsym();
        MulExp();

        Operation();
    }
}

void RelExp()
{
    AddExp();
    while (true)
    {
        if (sym.type < 24 || sym.type > 27)
        {
            break;
        }
        exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack.push(*exp_stack_tmp);
        nextsym();
        AddExp();

        CmpOperation();
    }
}

void EqExp()
{
    RelExp();
    toInt();
    while (true)
    {
        if (sym.type != 28 && sym.type != 29)
        {
            break;
        }
        exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack.push(*exp_stack_tmp);
        nextsym();
        RelExp();
        toInt();

        CmpOperation();
    }
}

void LAndExp()
{
    EqExp();
    toBool();
    while (true)
    {
        if (sym.type != 30)
        {
            break;
        }
        exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack.push(*exp_stack_tmp);
        nextsym();
        EqExp();
        toBool();

        BitOperation();
    }
}

void LOrExp()
{
    LAndExp();
    while (true)
    {
        if (sym.type != 31)
        {
            break;
        }
        exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack.push(*exp_stack_tmp);
        nextsym();
        LAndExp();

        BitOperation();
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

    PrintSpace();
    fprintf(fp_ir, "%%%d = ", exp_stack_tmp->value);
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

void NotOperation(struct ExpItem num)
{
    // 进行非运算
    PrintSpace();
    if (num.type == 1)
    {
        fprintf(fp_ir, "%%%d = icmp eq i32 %d, 0\n", ++temp_register, num.value);
    }
    else
    {
        fprintf(fp_ir, "%%%d = icmp eq i32 %%%d, 0\n", ++temp_register, num.value);
    }

    exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
    exp_stack_tmp->type = 3;
    exp_stack_tmp->value = ++temp_register;
    exp_stack.push(*exp_stack_tmp);

    // 将非运算的结果转化为 i32
    PrintSpace();
    fprintf(fp_ir, "%%%d = zext i1 %%%d to i32", temp_register, temp_register - 1);
}

// 处理比较运算
void CmpOperation()
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
    exp_stack_tmp->type = 5;
    exp_stack_tmp->value = ++temp_register;
    exp_stack.push(*exp_stack_tmp);

    PrintSpace();
    fprintf(fp_ir, "%%%d = icmp ", exp_stack_tmp->value);
    switch (op.value)
    {
    case 24:
        fprintf(fp_ir, "slt i32 ");
        break;
    case 25:
        fprintf(fp_ir, "sgt i32 ");
        break;
    case 26:
        fprintf(fp_ir, "sle i32 ");
        break;
    case 27:
        fprintf(fp_ir, "sge i32 ");
        break;
    case 28:
        fprintf(fp_ir, "eq i32 ");
        break;
    case 29:
        fprintf(fp_ir, "ne i32 ");
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

// 处理与或运算
void BitOperation()
{
    struct ExpItem num1, num2, op;

    num2 = exp_stack.top();
    if (num2.type != 5)
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
    if (num1.type != 5)
    {
        throw "Error";
    }
    exp_stack.pop();

    exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
    exp_stack_tmp->type = 5;
    exp_stack_tmp->value = ++temp_register;
    exp_stack.push(*exp_stack_tmp);

    PrintSpace();
    if (op.value == 30)
    {
        fprintf(fp_ir, "%%%d = and i1 %%%d, %%%d\n", temp_register, num1.value, num2.value);
    }
    else if (op.value == 31)
    {
        fprintf(fp_ir, "%%%d = or i1 %%%d, %%%d\n", temp_register, num1.value, num2.value);
    }
    else
    {
        throw "Error";
    }
}

void PrintSpace()
{
    fprintf(fp_ir, "    ");
}

void FuncMapInit()
{
    fprintf(fp_ir, "declare i32 @getint()\n");
    struct FuncItem func_getint;
    vector<int> getint;
    func_getint.type = 1;
    func_getint.params = getint;
    func_map["getint"] = func_getint;

    fprintf(fp_ir, "declare i32 @getch()\n");
    struct FuncItem func_getch;
    vector<int> getch;
    func_getch.type = 1;
    func_getch.params = getch;
    func_map["getch"] = func_getch;

    fprintf(fp_ir, "declare i32 @getarray(i32*)\n");
    struct FuncItem func_getarray;
    vector<int> getarray;
    getarray.push_back(2);
    func_getarray.type = 1;
    func_getarray.params = getarray;
    func_map["getarray"] = func_getarray;

    fprintf(fp_ir, "declare void @putint(i32)\n");
    struct FuncItem func_putint;
    vector<int> putint;
    putint.push_back(1);
    func_putint.type = 0;
    func_putint.params = putint;
    func_map["putint"] = func_putint;

    fprintf(fp_ir, "declare void @putch(i32)\n");
    struct FuncItem func_putch;
    vector<int> putch;
    putch.push_back(1);
    func_putch.type = 0;
    func_putch.params = putch;
    func_map["putch"] = func_putch;

    fprintf(fp_ir, "declare void @putarray(i32,i32*)\n");
    struct FuncItem func_putarray;
    vector<int> putarray;
    putarray.push_back(1);
    putarray.push_back(2);
    func_putarray.type = 0;
    func_putarray.params = putarray;
    func_map["putarray"] = func_putarray;
}

void FuncCall()
{
    // 检查函数是否被声明
    func_it = func_map.find((string)sym.ident);
    if (func_it == func_map.end())
    {
        throw "Error";
    }

    int params_count = (*func_it).second.params.size();
    nextsym();
    if (params_count > 0)
    {
        FuncRParams(params_count);
    }

    if ((*func_it).second.type == 0)
    {
        PrintSpace();
        fprintf(fp_ir, "call void @%s", (*func_it).first.c_str());
    }
    else
    {
        exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
        exp_stack_tmp->type = 3;
        exp_stack_tmp->value = ++temp_register;
        PrintSpace();
        fprintf(fp_ir, "%%%d = call i32 @%s", exp_stack_tmp->value, (*func_it).first.c_str());
    }

    fprintf(fp_ir, "(");
    // 打印函数参数
    for (int i = 0; i < params_count; i++)
    {
        if (i != 0)
        {
            fprintf(fp_ir, ", ");
        }

        if ((*func_it).second.params[i] == 1)
        {
            fprintf(fp_ir, "i32 ");
        }
        else if ((*func_it).second.params[i] == 2)
        {
            fprintf(fp_ir, "i32* ");
        }
        else
        {
            throw "Error";
        }

        if (exp_stack.top().type == 1)
        {
            fprintf(fp_ir, "%d", exp_stack.top().value);
        }
        else if (exp_stack.top().type == 3)
        {
            fprintf(fp_ir, "%%%d", exp_stack.top().value);
        }
        else
        {
            throw "Error";
        }

        exp_stack.pop();
    }

    if ((*func_it).second.type == 1)
    {
        exp_stack.push(*exp_stack_tmp);
    }

    fprintf(fp_ir, ")\n");
}

void toBool()
{
    struct ExpItem num;

    num = exp_stack.top();
    if (num.type != 1 && num.type != 3)
    {
        return;
    }
    exp_stack.pop();

    exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
    exp_stack_tmp->type = 5;
    exp_stack_tmp->value = ++temp_register;
    exp_stack.push(*exp_stack_tmp);

    PrintSpace();
    if (num.type == 1)
    {
        fprintf(fp_ir, "%%%d = icmp ne i32 %d, 0\n", temp_register, num.value);
    }
    else
    {
        fprintf(fp_ir, "%%%d = icmp ne i32 %%%d, 0\n", temp_register, num.value);
    }
}

void toInt()
{
    struct ExpItem num;

    num = exp_stack.top();
    if (num.type != 5)
    {
        return;
    }
    exp_stack.pop();

    exp_stack_tmp = (struct ExpItem *)malloc(sizeof(struct ExpItem));
    exp_stack_tmp->type = 3;
    exp_stack_tmp->value = ++temp_register;
    exp_stack.push(*exp_stack_tmp);

    PrintSpace();
    fprintf(fp_ir, "%%%d = zext i1 %%%d to i32\n", temp_register, num.value);
}