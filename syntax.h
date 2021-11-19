#include "token.h"
#include <stack>
#include <map>
#include <list>
#include <vector>

using namespace std;

typedef struct
{
    // 1 操作数，2 运算符，3 i32临时寄存器, 4 正负号运算, 5 布尔值, 6 i32*临时寄存器
    int type;
    // 操作数的值
    // 运算符：18 +,19 -,20 *,21 /,22 %, 23 !, 24 <, 25 >, 26 <=, 27 >=, 28 ==, 29 !=, 30 &&, 31 ||
    // 临时寄存器的次序
    int value;
    // 是否有实值
    bool have_real_value;
    // 实值
    int real_value;
} ExpItem;

typedef struct
{
    // 维度
    vector<int> dimension;
    // 每个维度的步长
    vector<int> step;
    // 数组的值
    vector<int> value;
    // 数组大小
    int size;
} ArrayProper;

typedef struct
{
    // 全局变量
    bool is_global;
    // 是否是常量
    bool is_const;
    // 是否是数组
    bool is_array;
    // 临时寄存器的值
    int register_num;
    // 是否有实值
    bool have_real_value;
    // 实值
    int real_value;
    // 嵌套层数
    int nest_layer;
    // 数组属性
    ArrayProper array_proper;
} VarItem;

typedef struct
{
    // 函数返回值类型 0 void, 1 int
    int type;
    // 函数参数列表 0 void, 1 int, 2 int*
    vector<int> params;
} FuncItem;

typedef struct
{
    VarItem *var_item;
    // 标识符
    char ident[4096];
} LValReturn;

// 全局变量map
map<string, VarItem> global_var_map;
// 变量map
map<string, VarItem> var_map;
// 变量迭代器
map<string, VarItem>::iterator var_it;
// 函数map
map<string, FuncItem> func_map;
// 函数迭代器
map<string, FuncItem>::iterator func_it;

int temp_register = 0;

ExpItem *exp_stack_tmp;

// LVal()获取变量
LValReturn lval_return;
// 表达式栈
stack<ExpItem> exp_stack;
// 变量 map 队列
list<map<string, VarItem>> var_map_list;
// 记录循环语句嵌套栈
stack<int> loop_stack;
// 当前正在声明的数组变量
VarItem *var_in_def;

// 记录运算过程中是否出现变量
bool have_var_in_cal;
// 记录该变量是否是常量
bool this_is_const;
// 记录是否处于 Cond 判断中
bool is_in_cond;
// 记录是否在数组定义过程中
bool is_in_arr_def;
// 数组定义嵌套层数
int arr_def_nest;
// 数组定义偏移量
int arr_def_offset;
// 记录基础块编号
int basic_block;
// 记录是否在全局环境中
bool is_in_global;
// 嵌套层数；
int nest_layer;

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
void LVal();
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
void NotOperation(ExpItem);
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

void CompUnit()
{
    // 记录是否经历过循环
    bool have_loop = false;
    FuncMapInit();

    Lexical sym_list[3];

    while (true)
    {
        nextsym();
        sym_list[0] = sym;
        nextsym();
        sym_list[1] = sym;
        nextsym();
        sym_list[2] = sym;

        backsysm(sym_list[2]);
        backsysm(sym_list[1]);
        backsysm(sym_list[0]);

        if (sym.type == 34)
        {
            break;
        }
        else if (sym.type == 9)
        {
            is_in_global = false;
            nextsym();
            fprintf(fp_ir, "define dso_local ");
            FuncDef();
            fprintf(fp_ir, "\n");
        }
        else
        {
            is_in_global = true;
            nextsym();
            Decl();
            fprintf(fp_ir, "\n");
        }

        have_loop = true;
    }

    if (!have_loop)
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
    is_in_arr_def = false;
    if (sym.type != 33)
    {
        throw "Error";
    }
    VarItem *var_item_tmp;
    string ident = sym.ident;

    // 检查变量是否重复声明
    var_it = var_map.find(ident);
    if (var_it != var_map.end())
    {
        throw "Error";
    }

    var_item_tmp = (VarItem *)malloc(sizeof(VarItem));
    var_item_tmp->is_const = true;
    var_item_tmp->register_num = ++temp_register;
    var_item_tmp->have_real_value = true;
    var_item_tmp->is_global = is_in_global;
    var_item_tmp->nest_layer = nest_layer;
    var_item_tmp->is_array = false;

    // 判断是否是数组
    nextsym();
    if (sym.type == 11)
    {
        is_in_arr_def = true;
        var_item_tmp->is_array = true;
        var_item_tmp->array_proper.size = 1;
        have_var_in_cal = false;
        while (sym.type == 11)
        {
            nextsym();
            ConstExp();

            exp_stack_tmp = &exp_stack.top();
            // 数组维度必须为非负常量表达式
            if (exp_stack_tmp->type != 1 || exp_stack_tmp->value <= 0)
            {
                throw "Error";
            }
            // 更新数组信息
            var_item_tmp->array_proper.size *= exp_stack_tmp->value;
            var_item_tmp->array_proper.dimension.push_back(exp_stack_tmp->value);
            for (int i = 0; i < var_item_tmp->array_proper.step.size(); i++)
            {
                var_item_tmp->array_proper.step[i] *= exp_stack_tmp->value;
            }
            var_item_tmp->array_proper.step.push_back(1);

            if (sym.type != 12)
            {
                throw "Error";
            }
            nextsym();
        }
        // 维度计算过程中必须为常量表达式
        if (have_var_in_cal)
        {
            throw "Error";
        }
        // 数组存值初始化
        for (int i = 0; i < var_item_tmp->array_proper.size; i++)
        {
            var_item_tmp->array_proper.value.push_back(0);
        }
    }
    backsysm(sym);

    var_map[sym.ident] = *var_item_tmp;

    // 数组定义处理
    if (is_in_arr_def)
    {
        var_in_def = var_item_tmp;
        arr_def_nest = 0;
        arr_def_offset = 0;

        nextsym();
        if (sym.type != 17)
        {
            throw "Error";
        }

        nextsym();
        have_var_in_cal = false;
        ConstInitVal();
        if (have_var_in_cal)
        {
            throw "Error";
        }

        bool have_nonzero = false;
        for (int i = 0; i < var_in_def->array_proper.size; i++)
        {
            if (var_in_def->array_proper.value[i] != 0)
            {
                have_nonzero = true;
                break;
            }
        }

        // 全局数组
        if (is_in_global)
        {
            fprintf(fp_ir, "@%s = dso_local global [%d x i32] ", ident.c_str(), var_in_def->array_proper.size);
            if (!have_nonzero)
            {
                fprintf(fp_ir, "zeroinitializer\n");
            }
            else
            {
                for (int i = 0; i < var_in_def->array_proper.value.size(); i++)
                {
                    if (i == 0)
                    {
                        fprintf(fp_ir, "[i32 %d", var_in_def->array_proper.value[i]);
                    }
                    else
                    {
                        fprintf(fp_ir, ", i32 %d", var_in_def->array_proper.value[i]);
                    }
                }
                fprintf(fp_ir, "]\n");
            }
        }
        // 局部数组
        else
        {
            PrintSpace();
            fprintf(fp_ir, "%%x%d = alloca [%d x i32]\n", var_in_def->register_num, var_in_def->array_proper.size);
            PrintSpace();
            fprintf(fp_ir, "%%x%d = getelementptr [%d x i32], [%d x i32]* %%x%d, i32 0, i32 0\n",
                    ++temp_register, var_in_def->array_proper.size, var_in_def->array_proper.size, var_in_def->register_num);
            PrintSpace();
            fprintf(fp_ir, "call void @memset(i32* %%x%d,i32 0,i32 %d)\n", temp_register, var_in_def->array_proper.size);
            if (have_nonzero)
            {
                for (int i = 0; i < var_in_def->array_proper.size; i++)
                {
                    if (var_in_def->array_proper.value[i] != 0)
                    {
                        PrintSpace();
                        fprintf(fp_ir, "%%x%d = getelementptr [%d x i32], [%d x i32]* %%x%d, i32 0, i32 %d\n",
                                ++temp_register, var_in_def->array_proper.size, var_in_def->array_proper.size, var_in_def->register_num, i);
                        PrintSpace();
                        fprintf(fp_ir, "store i32 %d, i32* %%x%d\n", var_in_def->array_proper.value[i], temp_register);
                    }
                }
            }
        }

        var_map[ident] = *var_in_def;
    }
    // int 定义处理
    else
    {
        if (!is_in_global)
        {
            PrintSpace();
            fprintf(fp_ir, "%%x%d = alloca i32\n", temp_register);
        }

        nextsym();
        if (sym.type != 17)
        {
            throw "Error";
        }

        nextsym();
        have_var_in_cal = false;
        ConstInitVal();
        if (have_var_in_cal)
        {
            throw "Error";
        }

        exp_stack_tmp = &exp_stack.top();
        if (is_in_global)
        {
            if (exp_stack_tmp->type != 1)
            {
                throw "Error";
            }
            fprintf(fp_ir, "@%s = dso_local global i32 %d\n", ident.c_str(), exp_stack_tmp->value);
            var_item_tmp->real_value = exp_stack_tmp->value;
            var_map[ident] = *var_item_tmp;
        }
        else if (exp_stack_tmp->type == 1)
        {
            PrintSpace();
            fprintf(fp_ir, "store i32 %d, i32* %%x%d\n", exp_stack_tmp->value, var_item_tmp->register_num);
            var_item_tmp->real_value = exp_stack_tmp->value;
            var_map[ident] = *var_item_tmp;
        }
        else if (exp_stack_tmp->type == 3)
        {
            if (exp_stack_tmp->have_real_value)
            {
                PrintSpace();
                fprintf(fp_ir, "store i32 %d, i32* %%x%d\n", exp_stack_tmp->real_value, var_item_tmp->register_num);
                var_item_tmp->real_value = exp_stack_tmp->real_value;
                var_map[ident] = *var_item_tmp;
            }
            else
            {
                throw "Error";
            }
        }
        else
        {
            throw "Error";
        }

        exp_stack.pop();
    }
}

void ConstInitVal()
{
    if (is_in_arr_def)
    {
        if (sym.type == 13)
        {
            nextsym();
            if (sym.type == 14)
            {
                nextsym();
                return;
            }

            backsysm(sym);
            int i = 0;
            do
            {
                if (i >= var_in_def->array_proper.dimension[arr_def_nest])
                {
                    throw "Error";
                }

                int offset = var_in_def->array_proper.step[arr_def_nest] * i;
                arr_def_offset += offset;
                arr_def_nest++;

                nextsym();
                ConstInitVal();

                arr_def_nest--;
                arr_def_offset -= offset;
                i++;
            } while (sym.type == 16);

            if (sym.type != 14)
            {
                throw "Error";
            }
            nextsym();
        }
        else if (arr_def_nest == var_in_def->array_proper.dimension.size())
        {
            ConstExp();
            exp_stack_tmp = &exp_stack.top();
            if (exp_stack_tmp->type != 1)
            {
                throw "Error";
            }
            var_in_def->array_proper.value[arr_def_offset] = exp_stack_tmp->value;
            exp_stack.pop();
        }
        else
        {
            throw "Error";
        }
    }
    else
    {
        ConstExp();
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
    is_in_arr_def = false;
    if (sym.type != 33)
    {
        throw "Error";
    }
    VarItem *var_item_tmp;
    string ident = sym.ident;

    // 检查变量是否重复声明
    var_it = var_map.find(ident);
    if (var_it != var_map.end())
    {
        throw "Error";
    }

    var_item_tmp = (VarItem *)malloc(sizeof(VarItem));
    var_item_tmp->is_const = false;
    var_item_tmp->register_num = ++temp_register;
    var_item_tmp->have_real_value = false;
    var_item_tmp->is_global = is_in_global;
    var_item_tmp->nest_layer = nest_layer;
    var_item_tmp->is_array = false;

    // 判断是否是数组
    nextsym();
    if (sym.type == 11)
    {
        is_in_arr_def = true;
        var_item_tmp->is_array = true;
        var_item_tmp->array_proper.size = 1;
        have_var_in_cal = false;
        while (sym.type == 11)
        {
            nextsym();
            ConstExp();

            exp_stack_tmp = &exp_stack.top();
            // 数组维度必须为非负常量表达式
            if (exp_stack_tmp->type != 1 || exp_stack_tmp->value <= 0)
            {
                throw "Error";
            }
            // 更新数组信息
            var_item_tmp->array_proper.size *= exp_stack_tmp->value;
            var_item_tmp->array_proper.dimension.push_back(exp_stack_tmp->value);
            for (int i = 0; i < var_item_tmp->array_proper.step.size(); i++)
            {
                var_item_tmp->array_proper.step[i] *= exp_stack_tmp->value;
            }
            var_item_tmp->array_proper.step.push_back(1);

            if (sym.type != 12)
            {
                throw "Error";
            }
            nextsym();
        }
        // 维度计算过程中必须为常量表达式
        if (have_var_in_cal)
        {
            throw "Error";
        }
        // 数组存值初始化
        for (int i = 0; i < var_item_tmp->array_proper.size; i++)
        {
            var_item_tmp->array_proper.value.push_back(0);
        }
    }
    backsysm(sym);

    var_map[sym.ident] = *var_item_tmp;

    // 数组定义处理
    if (is_in_arr_def)
    {
        var_in_def = var_item_tmp;
        arr_def_nest = 0;
        arr_def_offset = 0;

        if (!is_in_global)
        {
            PrintSpace();
            fprintf(fp_ir, "%%x%d = alloca [%d x i32]\n", var_in_def->register_num, var_in_def->array_proper.size);
        }

        nextsym();
        if (sym.type != 17)
        {
            if (is_in_global)
            {
                fprintf(fp_ir, "@%s = dso_local global [%d x i32] zeroinitializer\n", ident.c_str(), var_in_def->array_proper.size);
                var_map[ident] = *var_item_tmp;
            }
            return;
        }

        nextsym();
        have_var_in_cal = false;
        InitVal();
        if (is_in_global && have_var_in_cal)
        {
            throw "Error";
        }

        // 全局数组
        if (is_in_global)
        {
            bool have_nonzero = false;
            for (int i = 0; i < var_in_def->array_proper.size; i++)
            {
                if (var_in_def->array_proper.value[i] != 0)
                {
                    have_nonzero = true;
                    break;
                }
            }
            fprintf(fp_ir, "@%s = dso_local global [%d x i32] ", ident.c_str(), var_in_def->array_proper.size);
            if (!have_nonzero)
            {
                fprintf(fp_ir, "zeroinitializer\n");
            }
            else
            {
                for (int i = 0; i < var_in_def->array_proper.value.size(); i++)
                {
                    if (i == 0)
                    {
                        fprintf(fp_ir, "[i32 %d", var_in_def->array_proper.value[i]);
                    }
                    else
                    {
                        fprintf(fp_ir, ", i32 %d", var_in_def->array_proper.value[i]);
                    }
                }
                fprintf(fp_ir, "]\n");
            }
        }

        var_map[ident] = *var_in_def;
    }
    // int 定义处理
    else
    {
        if (!is_in_global)
        {
            PrintSpace();
            fprintf(fp_ir, "%%x%d = alloca i32\n", temp_register);
        }

        nextsym();
        if (sym.type != 17)
        {
            if (is_in_global)
            {
                fprintf(fp_ir, "@%s = dso_local global i32 0\n", ident.c_str());
                var_item_tmp->have_real_value = true;
                var_item_tmp->real_value = 0;
                var_map[ident] = *var_item_tmp;
            }
            return;
        }

        nextsym();
        have_var_in_cal = false;
        InitVal();
        if (is_in_global && have_var_in_cal)
        {
            throw "Error";
        }

        exp_stack_tmp = &exp_stack.top();
        if (is_in_global)
        {
            if (exp_stack_tmp->type != 1)
            {
                throw "Error";
            }
            fprintf(fp_ir, "@%s = dso_local global i32 %d\n", ident.c_str(), exp_stack_tmp->value);
            var_item_tmp->have_real_value = true;
            var_item_tmp->real_value = exp_stack_tmp->value;
            var_map[ident] = *var_item_tmp;
        }
        else if (exp_stack_tmp->type == 1)
        {
            PrintSpace();
            fprintf(fp_ir, "store i32 %d, i32* %%x%d\n", exp_stack_tmp->value, var_item_tmp->register_num);
            var_item_tmp->have_real_value = true;
            var_item_tmp->real_value = exp_stack_tmp->value;
            var_map[ident] = *var_item_tmp;
        }
        else if (exp_stack_tmp->type == 3)
        {
            if (exp_stack_tmp->have_real_value)
            {
                PrintSpace();
                fprintf(fp_ir, "store i32 %d, i32* %%x%d\n", exp_stack_tmp->real_value, var_item_tmp->register_num);
                var_item_tmp->have_real_value = true;
                var_item_tmp->real_value = exp_stack_tmp->real_value;
                var_map[ident] = *var_item_tmp;
            }
            else
            {
                PrintSpace();
                fprintf(fp_ir, "store i32 %%x%d, i32* %%x%d\n", exp_stack_tmp->value, var_item_tmp->register_num);
            }
        }
        else
        {
            throw "Error";
        }

        exp_stack.pop();
    }
}

void InitVal()
{
    if (is_in_arr_def)
    {
        if (sym.type == 13)
        {
            nextsym();
            if (sym.type == 14)
            {
                nextsym();
                return;
            }

            backsysm(sym);
            int i = 0;
            do
            {
                if (i >= var_in_def->array_proper.dimension[arr_def_nest])
                {
                    throw "Error";
                }

                int offset = var_in_def->array_proper.step[arr_def_nest] * i;
                arr_def_offset += offset;
                arr_def_nest++;

                nextsym();
                InitVal();

                arr_def_nest--;
                arr_def_offset -= offset;
                i++;
            } while (sym.type == 16);

            if (sym.type != 14)
            {
                throw "Error";
            }
            nextsym();
        }
        else if (arr_def_nest == var_in_def->array_proper.dimension.size())
        {
            Exp();
            exp_stack_tmp = &exp_stack.top();

            if (is_in_global)
            {
                if (exp_stack_tmp->type != 1)
                {
                    throw "Error";
                }
                var_in_def->array_proper.value[arr_def_offset] = exp_stack_tmp->value;
            }
            else
            {
                if (exp_stack_tmp->type == 1)
                {
                    PrintSpace();
                    fprintf(fp_ir, "%%x%d = getelementptr [%d x i32],[%d x i32]* %%x%d, i32 0, i32 %d\n",
                            ++temp_register, var_in_def->array_proper.size, var_in_def->array_proper.size, var_in_def->register_num, arr_def_offset);
                    PrintSpace();
                    fprintf(fp_ir, "store i32 %d, i32* %%x%d\n", exp_stack_tmp->value, temp_register);
                }
                else if (exp_stack_tmp->type == 3)
                {
                    PrintSpace();
                    fprintf(fp_ir, "%%x%d = getelementptr [%d x i32],[%d x i32]* %%x%d, i32 0, i32 %d\n",
                            ++temp_register, var_in_def->array_proper.size, var_in_def->array_proper.size, var_in_def->register_num, arr_def_offset);
                    PrintSpace();
                    fprintf(fp_ir, "store i32 %%x%d, i32* %%x%d\n", exp_stack_tmp->value, temp_register);
                }
                else
                {
                    throw "Error";
                }
            }

            exp_stack.pop();
        }
        else
        {
            throw "Error";
        }
    }
    else
    {
        Exp();
    }
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

    fprintf(fp_ir, "}");
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
    nest_layer++;
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
    nest_layer--;
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
    // return 语句
    if (sym.type == 8)
    {
        nextsym();
        Exp();

        exp_stack_tmp = &exp_stack.top();
        PrintSpace();
        if (exp_stack_tmp->type == 1)
        {
            fprintf(fp_ir, "ret i32 %d\n", exp_stack_tmp->value);
        }
        else if (exp_stack_tmp->type == 3)
        {
            fprintf(fp_ir, "ret i32 %%x%d\n", exp_stack_tmp->value);
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

        nest_layer++;
        nextsym();
        Cond();
        nest_layer--;

        if (sym.type != 10)
        {
            throw "Error";
        }

        ExpItem cond = exp_stack.top();
        if (cond.type != 5)
        {
            throw "Error";
        }
        exp_stack.pop();

        int if_block = ++basic_block;
        int out_block = ++basic_block;

        PrintSpace();
        fprintf(fp_ir, "br i1 %%x%d ,label %%basic_block_%d, label %%basic_block_%d\n", cond.value, if_block, out_block);

        // if bolck
        fprintf(fp_ir, "basic_block_%d:\n", if_block);

        nest_layer++;
        nextsym();
        Stmt();
        nest_layer--;

        Lexical tmp_sym = sym;
        nextsym();

        if (sym.type == 4)
        {
            // else block
            int else_block = out_block;
            out_block = ++basic_block;

            PrintSpace();
            fprintf(fp_ir, "br label %%basic_block_%d\n", out_block);
            fprintf(fp_ir, "basic_block_%d:\n", else_block);

            nest_layer++;
            nextsym();
            Stmt();
            nest_layer--;

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

        // out block
        fprintf(fp_ir, "basic_block_%d:\n", out_block);
    }
    // 循环语句
    else if (sym.type == 5)
    {
        nextsym();
        if (sym.type != 9)
        {
            throw "Error";
        }

        int cond_block = ++basic_block;
        PrintSpace();

        fprintf(fp_ir, "br label %%basic_block_%d\n", cond_block);
        fprintf(fp_ir, "basic_block_%d:\n", cond_block);
        nest_layer++;
        nextsym();
        Cond();
        nest_layer--;

        if (sym.type != 10)
        {
            throw "Error";
        }

        ExpItem cond = exp_stack.top();
        if (cond.type != 5)
        {
            throw "Error";
        }
        exp_stack.pop();

        int while_block = ++basic_block;
        int out_block = ++basic_block;
        loop_stack.push(out_block);
        loop_stack.push(cond_block);

        PrintSpace();
        fprintf(fp_ir, "br i1 %%x%d ,label %%basic_block_%d, label %%basic_block_%d\n", cond.value, while_block, out_block);

        // while bolck
        fprintf(fp_ir, "basic_block_%d:\n", while_block);

        nest_layer++;
        nextsym();
        Stmt();
        nest_layer--;

        loop_stack.pop();
        loop_stack.pop();

        PrintSpace();
        fprintf(fp_ir, "br label %%basic_block_%d\n", cond_block);
        // out block
        fprintf(fp_ir, "basic_block_%d:\n", out_block);
    }
    // break 语句
    else if (sym.type == 6)
    {
        if (loop_stack.size() < 2)
        {
            throw "Error";
        }

        int cond_block = loop_stack.top();
        loop_stack.pop();
        PrintSpace();
        fprintf(fp_ir, "br label %%basic_block_%d\n", loop_stack.top());
        loop_stack.push(cond_block);

        nextsym();
        if (sym.type != 15)
        {
            throw "Error";
        }
    }
    // continue 语句
    else if (sym.type == 7)
    {
        if (loop_stack.size() < 2)
        {
            throw "Error";
        }

        PrintSpace();
        fprintf(fp_ir, "br label %%basic_block_%d\n", loop_stack.top());

        nextsym();
        if (sym.type != 15)
        {
            throw "Error";
        }
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
        // 变量赋值语句
        else if (sym.type == 17)
        {
            sym = tmp_sym;

            this_is_const = false;
            LVal();

            LValReturn tmp_lval_return = lval_return;

            if (this_is_const || lval_return.var_item->is_array)
            {
                throw "Error";
            }

            nextsym();
            Exp();

            exp_stack_tmp = &exp_stack.top();
            PrintSpace();
            if (exp_stack_tmp->type == 1)
            {
                if (tmp_lval_return.var_item->is_global)
                {
                    fprintf(fp_ir, "store i32 %d, i32* @%s\n", exp_stack_tmp->value, tmp_lval_return.ident);
                }
                else
                {
                    fprintf(fp_ir, "store i32 %d, i32* %%x%d\n", exp_stack_tmp->value, tmp_lval_return.var_item->register_num);
                }
                tmp_lval_return.var_item->have_real_value = tmp_lval_return.var_item->nest_layer == nest_layer;
                tmp_lval_return.var_item->real_value = exp_stack_tmp->value;
            }
            else if (exp_stack_tmp->type == 3)
            {
                if (tmp_lval_return.var_item->is_global)
                {
                    fprintf(fp_ir, "store i32 %%x%d, i32* @%s\n", exp_stack_tmp->value, tmp_lval_return.ident);
                }
                else
                {
                    fprintf(fp_ir, "store i32 %%x%d, i32* %%x%d\n", exp_stack_tmp->value, tmp_lval_return.var_item->register_num);
                }
                tmp_lval_return.var_item->have_real_value = exp_stack_tmp->have_real_value && tmp_lval_return.var_item->nest_layer == nest_layer;
                tmp_lval_return.var_item->real_value = exp_stack_tmp->real_value;
            }
            else
            {
                throw "Error";
            }

            exp_stack.pop();
        }

        if (sym.type != 15)
        {
            Exp();
            exp_stack.pop();

            if (sym.type != 15)
            {
                throw "Error";
            }
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

void LVal()
{
    strcpy(lval_return.ident, sym.ident);

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
        for (list<map<string, VarItem>>::reverse_iterator it = var_map_list.rbegin(); it != var_map_list.rend(); ++it)
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

    lval_return.var_item = &(*var_it).second;

    // 数组变量特殊处理
    if (lval_return.var_item->is_array)
    {
        nextsym();
        // 变量使用的数组维度
        int dimension_cnt = 0;
        // 乘积寄存器
        int mul_register;
        // 加和寄存器
        int add_register;
        while (sym.type == 11)
        {
            nextsym();
            Exp();

            exp_stack_tmp = &exp_stack.top();
            PrintSpace();
            if (exp_stack_tmp->type == 1)
            {
                fprintf(fp_ir, "%%x%d = mul i32 %d, %d\n",
                        ++temp_register, exp_stack_tmp->value, lval_return.var_item->array_proper.step[dimension_cnt]);
            }
            else
            {
                fprintf(fp_ir, "%%x%d = mul i32 %%x%d, %d\n",
                        ++temp_register, exp_stack_tmp->value, lval_return.var_item->array_proper.step[dimension_cnt]);
            }
            mul_register = temp_register;

            PrintSpace();
            if (dimension_cnt == 0)
            {
                fprintf(fp_ir, "%%x%d = add i32 %%x%d, 0\n", ++temp_register, mul_register);
            }
            else
            {
                fprintf(fp_ir, "%%x%d = add i32 %%x%d, %%x%d\n", ++temp_register, mul_register, add_register);
            }
            add_register = temp_register;

            if (sym.type != 12)
            {
                throw "Error";
            }
            nextsym();

            dimension_cnt++;
            exp_stack.pop();
        }

        PrintSpace();
        // 全局变量
        if (lval_return.var_item->is_global)
        {
            fprintf(fp_ir, "%%x%d = getelementptr [%d x i32],[%d x i32]* @%s, i32 0, i32 %%x%d\n",
                    ++temp_register, lval_return.var_item->array_proper.size, lval_return.var_item->array_proper.size, lval_return.ident, add_register);
        }
        // 局部变量
        else
        {
            fprintf(fp_ir, "%%x%d = getelementptr [%d x i32],[%d x i32]* %%x%d, i32 0, i32 %%x%d\n",
                    ++temp_register, lval_return.var_item->array_proper.size, lval_return.var_item->array_proper.size, lval_return.var_item->register_num, add_register);
        }

        VarItem *var_item_tmp;
        var_item_tmp = (VarItem *)malloc(sizeof(VarItem));
        var_item_tmp->is_const = lval_return.var_item->is_const;
        var_item_tmp->register_num = temp_register;
        var_item_tmp->have_real_value = false;
        // 该标志用于 load 操作的 @/% 输出判断
        var_item_tmp->is_global = false;
        var_item_tmp->nest_layer = lval_return.var_item->nest_layer;
        var_item_tmp->is_array = dimension_cnt != lval_return.var_item->array_proper.dimension.size();

        lval_return.var_item = var_item_tmp;
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
        exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
        exp_stack_tmp->type = 1;
        exp_stack_tmp->value = sym.value;
        exp_stack_tmp->have_real_value = true;
        exp_stack_tmp->real_value = sym.value;
        exp_stack.push(*exp_stack_tmp);
        nextsym();
    }
    else if (sym.type == 33)
    {
        LVal();

        exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
        if (lval_return.var_item->have_real_value && lval_return.var_item->nest_layer == nest_layer)
        {
            exp_stack_tmp->type = 1;
            exp_stack_tmp->value = lval_return.var_item->real_value;
            exp_stack_tmp->have_real_value = true;
            exp_stack_tmp->real_value = lval_return.var_item->real_value;
            exp_stack.push(*exp_stack_tmp);
        }
        // 数组地址
        else if (lval_return.var_item->is_array)
        {
            exp_stack_tmp->type = 6;
            exp_stack_tmp->value = lval_return.var_item->register_num;
            exp_stack_tmp->have_real_value = false;
            exp_stack.push(*exp_stack_tmp);
        }
        else
        {
            exp_stack_tmp->type = 3;
            exp_stack_tmp->value = ++temp_register;
            exp_stack_tmp->have_real_value = false;
            exp_stack_tmp->real_value = lval_return.var_item->real_value;
            exp_stack.push(*exp_stack_tmp);

            PrintSpace();
            if (lval_return.var_item->is_global)
            {
                fprintf(fp_ir, "%%x%d = load i32, i32* @%s\n", exp_stack_tmp->value, lval_return.ident);
            }
            else
            {
                fprintf(fp_ir, "%%x%d = load i32, i32* %%x%d\n", exp_stack_tmp->value, lval_return.var_item->register_num);
            }
        }

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

        ExpItem num = exp_stack.top();
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

        ExpItem op = exp_stack.top();
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
            if (num.type == 1)
            {
                num.value = num.real_value = -num.value;
            }
            else
            {
                PrintSpace();
                fprintf(fp_ir, "%%x%d = sub i32 0, %%x%d\n", ++temp_register, num.value);
                num.type = 3;
                num.value = temp_register;
            }
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
                if (num.have_real_value)
                {
                    num.value == 0 ? num.value = num.real_value = 1 : num.value = num.real_value = 0;
                }
                else
                {
                    NotOperation(num);
                    return;
                }
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
            backsysm(tmp_sym);
            LVal();

            exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
            if (lval_return.var_item->have_real_value && lval_return.var_item->nest_layer == nest_layer)
            {
                exp_stack_tmp->type = 1;
                exp_stack_tmp->value = lval_return.var_item->real_value;
                exp_stack_tmp->have_real_value = true;
                exp_stack_tmp->real_value = lval_return.var_item->real_value;
                exp_stack.push(*exp_stack_tmp);
            }
            else
            {
                exp_stack_tmp->type = 3;
                exp_stack_tmp->value = ++temp_register;
                exp_stack_tmp->have_real_value = false;
                exp_stack_tmp->real_value = lval_return.var_item->real_value;
                exp_stack.push(*exp_stack_tmp);

                PrintSpace();
                if (lval_return.var_item->is_global)
                {
                    fprintf(fp_ir, "%%x%d = load i32, i32* @%s\n", exp_stack_tmp->value, lval_return.ident);
                }
                else
                {
                    fprintf(fp_ir, "%%x%d = load i32, i32* %%x%d\n", exp_stack_tmp->value, lval_return.var_item->register_num);
                }
            }
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
        exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
        exp_stack_tmp->type = 4;
        exp_stack_tmp->value = sym.type;
        exp_stack_tmp->have_real_value = false;
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
        exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack_tmp->have_real_value = false;
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
        exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack_tmp->have_real_value = false;
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
        exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack_tmp->have_real_value = false;
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
        exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack_tmp->have_real_value = false;
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
        exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack_tmp->have_real_value = false;
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
        exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack_tmp->have_real_value = false;
        exp_stack.push(*exp_stack_tmp);
        nextsym();
        LAndExp();

        BitOperation();
    }
}

// 处理四则运算
void Operation()
{
    ExpItem num1, num2, op;

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

    exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));

    if (num1.have_real_value && num2.have_real_value)
    {
        exp_stack_tmp->type = 1;
        exp_stack_tmp->have_real_value = true;

        switch (op.value)
        {
        case 18:
            exp_stack_tmp->value = exp_stack_tmp->real_value = num1.real_value + num2.real_value;
            break;
        case 19:
            exp_stack_tmp->value = exp_stack_tmp->real_value = num1.real_value - num2.real_value;
            break;
        case 20:
            exp_stack_tmp->value = exp_stack_tmp->real_value = num1.real_value * num2.real_value;
            break;
        case 21:
            exp_stack_tmp->value = exp_stack_tmp->real_value = num1.real_value / num2.real_value;
            break;
        case 22:
            exp_stack_tmp->value = exp_stack_tmp->real_value = num1.real_value % num2.real_value;
            break;
        default:
            throw "Error";
            break;
        }

        exp_stack.push(*exp_stack_tmp);
    }
    else
    {
        exp_stack_tmp->type = 3;
        exp_stack_tmp->value = ++temp_register;
        exp_stack_tmp->have_real_value = false;
        exp_stack.push(*exp_stack_tmp);

        PrintSpace();
        fprintf(fp_ir, "%%x%d = ", exp_stack_tmp->value);
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
            fprintf(fp_ir, "%%x%d, ", num1.value);
        }
        if (num2.type == 1)
        {
            fprintf(fp_ir, "%d\n", num2.value);
        }
        else
        {
            fprintf(fp_ir, "%%x%d\n", num2.value);
        }
    }
}

void NotOperation(ExpItem num)
{
    // 进行非运算
    PrintSpace();
    fprintf(fp_ir, "%%x%d = icmp eq i32 %%x%d, 0\n", ++temp_register, num.value);

    exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
    exp_stack_tmp->type = 3;
    exp_stack_tmp->value = ++temp_register;
    exp_stack_tmp->have_real_value = false;
    exp_stack.push(*exp_stack_tmp);

    // 将非运算的结果转化为 i32
    PrintSpace();
    fprintf(fp_ir, "%%x%d = zext i1 %%x%d to i32", temp_register, temp_register - 1);
}

// 处理比较运算
void CmpOperation()
{
    ExpItem num1, num2, op;

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

    exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
    exp_stack_tmp->type = 5;
    exp_stack_tmp->value = ++temp_register;
    exp_stack_tmp->have_real_value = false;
    exp_stack.push(*exp_stack_tmp);

    PrintSpace();
    fprintf(fp_ir, "%%x%d = icmp ", exp_stack_tmp->value);
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
        fprintf(fp_ir, "%%x%d, ", num1.value);
    }
    if (num2.type == 1)
    {
        fprintf(fp_ir, "%d\n", num2.value);
    }
    else
    {
        fprintf(fp_ir, "%%x%d\n", num2.value);
    }
}

// 处理与或运算
void BitOperation()
{
    ExpItem num1, num2, op;

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

    exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
    exp_stack_tmp->type = 5;
    exp_stack_tmp->value = ++temp_register;
    exp_stack_tmp->have_real_value = false;
    exp_stack.push(*exp_stack_tmp);

    PrintSpace();
    if (op.value == 30)
    {
        fprintf(fp_ir, "%%x%d = and i1 %%x%d, %%x%d\n", temp_register, num1.value, num2.value);
    }
    else if (op.value == 31)
    {
        fprintf(fp_ir, "%%x%d = or i1 %%x%d, %%x%d\n", temp_register, num1.value, num2.value);
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
    FuncItem func_getint;
    vector<int> getint;
    func_getint.type = 1;
    func_getint.params = getint;
    func_map["getint"] = func_getint;

    fprintf(fp_ir, "declare i32 @getch()\n");
    FuncItem func_getch;
    vector<int> getch;
    func_getch.type = 1;
    func_getch.params = getch;
    func_map["getch"] = func_getch;

    fprintf(fp_ir, "declare i32 @getarray(i32*)\n");
    FuncItem func_getarray;
    vector<int> getarray;
    getarray.push_back(2);
    func_getarray.type = 1;
    func_getarray.params = getarray;
    func_map["getarray"] = func_getarray;

    fprintf(fp_ir, "declare void @putint(i32)\n");
    FuncItem func_putint;
    vector<int> putint;
    putint.push_back(1);
    func_putint.type = 0;
    func_putint.params = putint;
    func_map["putint"] = func_putint;

    fprintf(fp_ir, "declare void @putch(i32)\n");
    FuncItem func_putch;
    vector<int> putch;
    putch.push_back(1);
    func_putch.type = 0;
    func_putch.params = putch;
    func_map["putch"] = func_putch;

    fprintf(fp_ir, "declare void @putarray(i32, i32*)\n");
    FuncItem func_putarray;
    vector<int> putarray;
    putarray.push_back(1);
    putarray.push_back(2);
    func_putarray.type = 0;
    func_putarray.params = putarray;
    func_map["putarray"] = func_putarray;

    fprintf(fp_ir, "declare void @memset(i32*, i32, i32)\n\n");
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
        exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
        exp_stack_tmp->type = 3;
        exp_stack_tmp->value = ++temp_register;
        exp_stack_tmp->have_real_value = false;
        PrintSpace();
        fprintf(fp_ir, "%%x%d = call i32 @%s", exp_stack_tmp->value, (*func_it).first.c_str());
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
            if (exp_stack.top().type == 1)
            {
                fprintf(fp_ir, "i32 %d", exp_stack.top().value);
            }
            else if (exp_stack.top().type == 3)
            {
                fprintf(fp_ir, "i32 %%x%d", exp_stack.top().value);
            }
            else
            {
                throw "Error";
            }
        }
        else if ((*func_it).second.params[i] == 2 && exp_stack.top().type == 6)
        {
            fprintf(fp_ir, "i32* %%x%d", exp_stack.top().value);
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
    ExpItem num;

    num = exp_stack.top();
    if (num.type != 1 && num.type != 3)
    {
        return;
    }
    exp_stack.pop();

    exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
    exp_stack_tmp->type = 5;
    exp_stack_tmp->value = ++temp_register;
    exp_stack_tmp->have_real_value = false;
    exp_stack.push(*exp_stack_tmp);

    PrintSpace();
    if (num.type == 1)
    {
        fprintf(fp_ir, "%%x%d = icmp ne i32 %d, 0\n", temp_register, num.value);
    }
    else if (num.type == 3)
    {
        fprintf(fp_ir, "%%x%d = icmp ne i32 %%x%d, 0\n", temp_register, num.value);
    }
    else
    {
        throw "Error";
    }
}

void toInt()
{
    ExpItem num;

    num = exp_stack.top();
    if (num.type != 5)
    {
        return;
    }
    exp_stack.pop();

    exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
    exp_stack_tmp->type = 3;
    exp_stack_tmp->value = ++temp_register;
    exp_stack_tmp->have_real_value = false;
    exp_stack.push(*exp_stack_tmp);

    PrintSpace();
    fprintf(fp_ir, "%%x%d = zext i1 %%x%d to i32\n", temp_register, num.value);
}