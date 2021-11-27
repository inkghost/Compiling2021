#include "token.h"
#include <stack>
#include <map>
#include <list>
#include <vector>

using namespace std;

typedef struct
{
    // 维度
    vector<int> dimension;
    // 每个维度的步长
    vector<int> step;
    // 数组的值
    map<int, int> value;
    // 数组大小
    int size;
} ArrayProper;

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
    // 数组属性
    ArrayProper array_proper;
} ExpItem;

typedef struct
{
    // 全局变量
    bool is_global;
    // 是否是常量
    bool is_const;
    // 是否是数组
    bool is_array;
    // 是否以 i32* 的类型表示
    bool is_i32_ptr;
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
    // 函数参数类型 1 i32, 2 i32*
    int type;
    ArrayProper array_proper;
} ParamItem;

typedef struct
{
    // 函数返回值类型 0 void, 1 int
    int type;
    // 函数参数列表
    vector<ParamItem> params;
} FuncItem;

typedef struct
{
    VarItem *var_item;
    // 标识符
    char ident[4096];
} LValReturn;

// 变量map
map<string, VarItem> var_map;
// 变量迭代器
map<string, VarItem>::iterator var_it;
// 函数map
map<string, FuncItem> func_map;
// 函数迭代器
map<string, FuncItem>::iterator func_it;
// 数组值迭代器
map<int, int>::iterator arr_value_it;

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
// 选择语句块嵌套栈
stack<int> cond_block_stack;
// 执行语句块嵌套栈
stack<int> exec_block_stack;
// 退出语句块嵌套栈
stack<int> out_block_stack;
// 当前正在声明的数组变量
VarItem *var_in_def;
// 当前正在声明的函数变量
FuncItem *func_in_def;

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
// 记录当前函数是否已经返回
bool have_return;
// 记录基础块编号
int basic_block;
// 记录是否在全局环境中
bool is_in_global;
// 嵌套层数；
int nest_layer;
// 记录是否是符合要求的 main 函数
bool have_main_func;

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
void FuncFParams();
void FuncFParam();
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
// 短路求值 type 0 OR , 1 AND
void shortCircuit(int);

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
            nextsym();
            FuncDef();
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

    // 检查是否和函数定义重名
    if (is_in_global)
    {
        func_it = func_map.find(ident);
        if (func_it != func_map.end())
        {
            throw "Error";
        }
    }

    // 检查变量是否重复声明
    var_it = var_map.find(ident);
    if (var_it != var_map.end())
    {
        throw "Error";
    }

    var_item_tmp = (VarItem *)malloc(sizeof(VarItem));
    memset(var_item_tmp, 0, sizeof(VarItem));
    var_item_tmp->is_const = true;
    var_item_tmp->register_num = ++temp_register;
    var_item_tmp->have_real_value = true;
    var_item_tmp->is_global = is_in_global;
    var_item_tmp->nest_layer = nest_layer;
    var_item_tmp->is_array = false;
    var_item_tmp->is_i32_ptr = false;
    var_item_tmp->array_proper.dimension.clear();
    var_item_tmp->array_proper.step.clear();

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
            exp_stack.pop();
            nextsym();
        }
        // 维度计算过程中必须为常量表达式
        if (have_var_in_cal)
        {
            throw "Error";
        }
        // 数组存值初始化
        var_item_tmp->array_proper.value.clear();
    }
    backsysm(sym);

    var_map[ident] = *var_item_tmp;

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

        bool have_nonzero = !(var_in_def->array_proper.value.empty());

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
                for (int i = 0; i < var_in_def->array_proper.size; i++)
                {
                    arr_value_it = var_in_def->array_proper.value.find(i);

                    int value_in_pos = 0;
                    if (arr_value_it != var_in_def->array_proper.value.end())
                    {
                        value_in_pos = arr_value_it->second;
                    }

                    if (i == 0)
                    {
                        fprintf(fp_ir, "[i32 %d", value_in_pos);
                    }
                    else
                    {
                        fprintf(fp_ir, ", i32 %d", value_in_pos);
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
                    arr_value_it = var_in_def->array_proper.value.find(i);

                    if (arr_value_it != var_in_def->array_proper.value.end())
                    {
                        PrintSpace();
                        fprintf(fp_ir, "%%x%d = getelementptr [%d x i32], [%d x i32]* %%x%d, i32 0, i32 %d\n",
                                ++temp_register, var_in_def->array_proper.size, var_in_def->array_proper.size, var_in_def->register_num, i);
                        PrintSpace();
                        fprintf(fp_ir, "store i32 %d, i32* %%x%d\n", arr_value_it->second, temp_register);
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
    free(var_item_tmp);
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

    // 检查是否和函数定义重名
    if (is_in_global)
    {
        func_it = func_map.find(ident);
        if (func_it != func_map.end())
        {
            throw "Error";
        }
    }

    // 检查变量是否重复声明
    var_it = var_map.find(ident);
    if (var_it != var_map.end())
    {
        throw "Error";
    }

    var_item_tmp = (VarItem *)malloc(sizeof(VarItem));
    memset(var_item_tmp, 0, sizeof(VarItem));
    var_item_tmp->is_const = false;
    var_item_tmp->register_num = ++temp_register;
    var_item_tmp->have_real_value = false;
    var_item_tmp->is_global = is_in_global;
    var_item_tmp->nest_layer = nest_layer;
    var_item_tmp->is_array = false;
    var_item_tmp->is_i32_ptr = false;
    var_item_tmp->array_proper.dimension.clear();
    var_item_tmp->array_proper.step.clear();

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
            exp_stack.pop();
            nextsym();
        }
        // 维度计算过程中必须为常量表达式
        if (have_var_in_cal)
        {
            throw "Error";
        }
        // 数组存值初始化
        var_item_tmp->array_proper.value.clear();
    }
    backsysm(sym);

    var_map[ident] = *var_item_tmp;

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
            bool have_nonzero = !(var_in_def->array_proper.value.empty());
            fprintf(fp_ir, "@%s = dso_local global [%d x i32] ", ident.c_str(), var_in_def->array_proper.size);
            if (!have_nonzero)
            {
                fprintf(fp_ir, "zeroinitializer\n");
            }
            else
            {
                for (int i = 0; i < var_in_def->array_proper.size; i++)
                {
                    arr_value_it = var_in_def->array_proper.value.find(i);

                    int value_in_pos = 0;
                    if (arr_value_it != var_in_def->array_proper.value.end())
                    {
                        value_in_pos = arr_value_it->second;
                    }

                    if (i == 0)
                    {
                        fprintf(fp_ir, "[i32 %d", value_in_pos);
                    }
                    else
                    {
                        fprintf(fp_ir, ", i32 %d", value_in_pos);
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

    free(var_item_tmp);
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
    temp_register = 0;
    var_map_list.push_back(var_map);
    var_map.clear();

    FuncItem *func_item_tmp;
    func_item_tmp = (FuncItem *)malloc(sizeof(FuncItem));
    memset(func_item_tmp, 0, sizeof(FuncItem));
    func_in_def = func_item_tmp;

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

    string ident = sym.ident;
    // 检查函数命名是否冲突
    var_it = var_map_list.front().find(ident);
    if (var_it != var_map_list.front().end())
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
    if (sym.type != 10)
    {
        FuncFParams();
    }

    if (sym.type == 10)
    {
        fprintf(fp_ir, ")");
    }
    else
    {
        throw "Error";
    }

    if (strcmp("main", ident.c_str()) == 0)
    {
        if (func_in_def->type != 1 || func_in_def->params.size() != 0)
        {
            throw "Error";
        }
        have_main_func = true;
    }

    fprintf(fp_ir, "{\n");

    // 迭代器遍历当前 var_map （即函数参量）
    for (var_it = var_map.begin(); var_it != var_map.end(); var_it++)
    {
        if (!var_it->second.is_i32_ptr)
        {
            PrintSpace();
            fprintf(fp_ir, "%%x%d = alloca i32\n", ++temp_register);
            PrintSpace();
            fprintf(fp_ir, "store i32 %%x%d, i32* %%x%d\n", var_it->second.register_num, temp_register);
            var_it->second.register_num = temp_register;
        }
    }

    func_map[ident] = *func_in_def;

    nextsym();
    is_in_global = false;
    have_return = false;
    Block();
    is_in_global = true;

    if (!have_return)
    {
        if (func_in_def->type == 0)
        {
            PrintSpace();
            fprintf(fp_ir, "ret void\n");
        }
        else
        {
            PrintSpace();
            fprintf(fp_ir, "ret i32 0\n");
        }
    }

    fprintf(fp_ir, "}\n");

    var_map = var_map_list.back();
    var_map_list.pop_back();
}

void FuncType()
{
    if (sym.type == 1)
    {
        fprintf(fp_ir, "\ndefine dso_local i32 ");
        func_in_def->type = 1;
    }
    else if (sym.type == 2)
    {
        fprintf(fp_ir, "\ndefine dso_local void ");
        func_in_def->type = 0;
    }
    else
    {
        throw "Error";
    }
}

void FuncFParams()
{
    do
    {
        FuncFParam();
    } while (sym.type == 16);
}

void FuncFParam()
{
    if (func_in_def->params.size() != 0)
    {
        nextsym();
        fprintf(fp_ir, ", ");
    }

    ParamItem param_item_tmp;
    memset(&param_item_tmp, 0, sizeof(ParamItem));

    BType();

    nextsym();
    if (sym.type != 33)
    {
        throw "Error";
    }

    VarItem *var_item_tmp;
    string ident = sym.ident;

    var_item_tmp = (VarItem *)malloc(sizeof(VarItem));
    memset(var_item_tmp, 0, sizeof(VarItem));
    var_item_tmp->is_const = false;
    var_item_tmp->register_num = ++temp_register;
    var_item_tmp->have_real_value = false;
    var_item_tmp->is_global = false;
    var_item_tmp->nest_layer = nest_layer;
    var_item_tmp->is_array = false;
    var_item_tmp->is_i32_ptr = false;
    var_item_tmp->array_proper.dimension.clear();
    var_item_tmp->array_proper.step.clear();

    nextsym();
    if (sym.type == 11)
    {
        param_item_tmp.type = 2;
        param_item_tmp.array_proper.dimension.push_back(1);
        param_item_tmp.array_proper.step.push_back(1);
        var_item_tmp->is_array = true;
        var_item_tmp->is_i32_ptr = true;

        nextsym();
        if (sym.type != 12)
        {
            throw "Error";
        }
        nextsym();

        if (sym.type == 11)
        {
            while (sym.type == 11)
            {
                have_var_in_cal = false;
                nextsym();
                Exp();
                exp_stack_tmp = &exp_stack.top();
                if (have_var_in_cal || exp_stack_tmp->type != 1 || exp_stack_tmp->value <= 0)
                {
                    throw "Error";
                }
                param_item_tmp.array_proper.dimension.push_back(exp_stack_tmp->value);
                for (int i = 0; i < param_item_tmp.array_proper.step.size(); i++)
                {
                    param_item_tmp.array_proper.step[i] *= exp_stack_tmp->value;
                }
                param_item_tmp.array_proper.step.push_back(1);
                exp_stack.pop();
            }

            if (sym.type != 12)
            {
                throw "Error";
            }
            nextsym();
        }

        var_item_tmp->array_proper = param_item_tmp.array_proper;
        fprintf(fp_ir, "i32* %%x%d", var_item_tmp->register_num);
    }
    else
    {
        param_item_tmp.type = 1;
        fprintf(fp_ir, "i32 %%x%d", var_item_tmp->register_num);
    }

    func_in_def->params.push_back(param_item_tmp);
    var_map[ident] = *var_item_tmp;
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
        have_return = true;
        nextsym();

        // void 类型
        if (sym.type == 15)
        {
            PrintSpace();
            fprintf(fp_ir, "ret void\n");
            return;
        }

        if (func_in_def->type == 0)
        {
            throw "Error";
        }

        // int 类型
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
        int if_block = ++basic_block;
        int out_block = ++basic_block;

        nextsym();
        if (sym.type != 9)
        {
            throw "Error";
        }

        exec_block_stack.push(if_block);
        out_block_stack.push(out_block);

        nest_layer++;
        nextsym();
        Cond();
        nest_layer--;

        exec_block_stack.pop();
        out_block_stack.pop();

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
        have_return = false;
    }
    // 循环语句
    else if (sym.type == 5)
    {
        int cond_block = ++basic_block;
        int while_block = ++basic_block;
        int out_block = ++basic_block;

        nextsym();
        if (sym.type != 9)
        {
            throw "Error";
        }

        PrintSpace();

        fprintf(fp_ir, "br label %%basic_block_%d\n", cond_block);
        fprintf(fp_ir, "basic_block_%d:\n", cond_block);

        exec_block_stack.push(while_block);
        out_block_stack.push(out_block);

        nest_layer++;
        nextsym();
        Cond();
        nest_layer--;

        exec_block_stack.pop();
        out_block_stack.pop();

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
        have_return = false;
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
        vector<Lexical> tmp_syms;
        tmp_syms.push_back(sym);
        // 是否是赋值语句
        bool is_assign = false;
        while (sym.type != 15)
        {
            nextsym();
            tmp_syms.push_back(sym);
            if (sym.type == 17)
            {
                is_assign = true;
                break;
            }

            if (sym.type == 34)
            {
                throw "Error";
            }
        }

        for (int i = tmp_syms.size() - 1; i >= 0; i--)
        {
            backsysm(tmp_syms[i]);
        }
        nextsym();

        if (!is_assign)
        {
            int older_exp_stack_size = exp_stack.size();
            Exp();
            if (exp_stack.size() != older_exp_stack_size)
            {
                exp_stack.pop();
            }

            if (sym.type != 15)
            {
                throw "Error";
            }
            return;
        }

        this_is_const = false;
        LVal();

        LValReturn tmp_lval_return = lval_return;

        if (this_is_const || lval_return.var_item->is_array)
        {
            throw "Error";
        }

        nextsym();
        if (sym.type != 17)
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

    if ((*var_it).second.is_const)
    {
        this_is_const = true;
    }
    else
    {
        have_var_in_cal = true;
    }

    lval_return.var_item = &((*var_it).second);
    LValReturn tmp_lval_return = lval_return;

    // 数组变量特殊处理
    if (tmp_lval_return.var_item->is_array)
    {
        nextsym();
        // 变量使用的数组维度
        int dimension_cnt = 0;
        // 乘积寄存器
        int mul_register = 0;
        // 加和寄存器
        int add_register = 0;
        while (sym.type == 11)
        {
            nextsym();
            Exp();

            exp_stack_tmp = &exp_stack.top();
            PrintSpace();
            if (exp_stack_tmp->type == 1)
            {
                fprintf(fp_ir, "%%x%d = mul i32 %d, %d\n",
                        ++temp_register, exp_stack_tmp->value, tmp_lval_return.var_item->array_proper.step[dimension_cnt]);
            }
            else
            {
                fprintf(fp_ir, "%%x%d = mul i32 %%x%d, %d\n",
                        ++temp_register, exp_stack_tmp->value, tmp_lval_return.var_item->array_proper.step[dimension_cnt]);
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
        if (tmp_lval_return.var_item->is_global)
        {
            if (add_register == 0)
            {
                fprintf(fp_ir, "%%x%d = getelementptr [%d x i32],[%d x i32]* @%s, i32 0, i32 0\n",
                        ++temp_register, tmp_lval_return.var_item->array_proper.size, tmp_lval_return.var_item->array_proper.size, tmp_lval_return.ident);
            }
            else
            {
                fprintf(fp_ir, "%%x%d = getelementptr [%d x i32],[%d x i32]* @%s, i32 0, i32 %%x%d\n",
                        ++temp_register, tmp_lval_return.var_item->array_proper.size, tmp_lval_return.var_item->array_proper.size, tmp_lval_return.ident, add_register);
            }
        }
        // 局部变量
        else
        {
            // 以 i32* 表示
            if (tmp_lval_return.var_item->is_i32_ptr)
            {
                if (add_register == 0)
                {
                    fprintf(fp_ir, "%%x%d = getelementptr i32, i32* %%x%d, i32 0\n",
                            ++temp_register, tmp_lval_return.var_item->register_num);
                }
                else
                {
                    fprintf(fp_ir, "%%x%d = getelementptr i32, i32* %%x%d, i32 %%x%d\n",
                            ++temp_register, tmp_lval_return.var_item->register_num, add_register);
                }
            }
            // 以 [n x i32]* 表示
            else
            {
                if (add_register == 0)
                {
                    fprintf(fp_ir, "%%x%d = getelementptr [%d x i32],[%d x i32]* %%x%d, i32 0, i32 0\n",
                            ++temp_register, tmp_lval_return.var_item->array_proper.size, tmp_lval_return.var_item->array_proper.size, tmp_lval_return.var_item->register_num);
                }
                else
                {
                    fprintf(fp_ir, "%%x%d = getelementptr [%d x i32],[%d x i32]* %%x%d, i32 0, i32 %%x%d\n",
                            ++temp_register, tmp_lval_return.var_item->array_proper.size, tmp_lval_return.var_item->array_proper.size, tmp_lval_return.var_item->register_num, add_register);
                }
            }
        }

        VarItem *var_item_tmp;
        var_item_tmp = (VarItem *)malloc(sizeof(VarItem));
        memset(var_item_tmp, 0, sizeof(VarItem));
        var_item_tmp->is_const = tmp_lval_return.var_item->is_const;
        var_item_tmp->register_num = temp_register;
        var_item_tmp->have_real_value = false;
        // 该标志用于 load 操作的 @/% 输出判断
        var_item_tmp->is_global = false;
        var_item_tmp->nest_layer = tmp_lval_return.var_item->nest_layer;
        var_item_tmp->is_array = false;
        var_item_tmp->is_i32_ptr = false;
        if (dimension_cnt != tmp_lval_return.var_item->array_proper.dimension.size())
        {
            var_item_tmp->is_array = true;
            var_item_tmp->is_i32_ptr = true;
            for (int i = dimension_cnt; i < tmp_lval_return.var_item->array_proper.dimension.size(); i++)
            {
                var_item_tmp->array_proper.dimension.push_back(tmp_lval_return.var_item->array_proper.dimension[i]);
                var_item_tmp->array_proper.step.push_back(tmp_lval_return.var_item->array_proper.step[i]);
            }
            var_item_tmp->array_proper.size = var_item_tmp->array_proper.dimension[0] * var_item_tmp->array_proper.step[0];
        }

        tmp_lval_return.var_item = var_item_tmp;
        lval_return = tmp_lval_return;
        backsysm(sym);
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
        memset(exp_stack_tmp, 0, sizeof(ExpItem));
        exp_stack_tmp->type = 1;
        exp_stack_tmp->value = sym.value;
        exp_stack_tmp->have_real_value = true;
        exp_stack_tmp->real_value = sym.value;
        exp_stack.push(*exp_stack_tmp);
        free(exp_stack_tmp);
        nextsym();
    }
    else if (sym.type == 33)
    {
        LVal();

        exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
        memset(exp_stack_tmp, 0, sizeof(ExpItem));
        if (lval_return.var_item->have_real_value && lval_return.var_item->nest_layer == nest_layer)
        {
            exp_stack_tmp->type = 1;
            exp_stack_tmp->value = lval_return.var_item->real_value;
            exp_stack_tmp->have_real_value = true;
            exp_stack_tmp->real_value = lval_return.var_item->real_value;
            exp_stack.push(*exp_stack_tmp);
            free(exp_stack_tmp);
        }
        // 数组地址
        else if (lval_return.var_item->is_array)
        {
            exp_stack_tmp->type = 6;
            exp_stack_tmp->value = lval_return.var_item->register_num;
            exp_stack_tmp->have_real_value = false;
            exp_stack_tmp->array_proper = lval_return.var_item->array_proper;
            exp_stack.push(*exp_stack_tmp);
            free(exp_stack_tmp);
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
            free(exp_stack_tmp);
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
            if (!exp_stack.empty())
            {
                op = exp_stack.top();
                while (op.value == 23)
                {
                    doNotOperation = !doNotOperation;
                    exp_stack.pop();
                    op = exp_stack.top();
                }
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
        // PrimaryExp
        else
        {
            backsysm(sym);
            swap(tmp_sym, sym);
            PrimaryExp();
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
        memset(exp_stack_tmp, 0, sizeof(ExpItem));
        exp_stack_tmp->type = 4;
        exp_stack_tmp->value = sym.type;
        exp_stack_tmp->have_real_value = false;
        exp_stack.push(*exp_stack_tmp);
        free(exp_stack_tmp);
    }
    else
    {
        throw "Error";
    }
}

void FuncRParams(int params_count)
{
    stack<ExpItem> tmp_exp_stack;
    do
    {
        nextsym();
        Exp();
        tmp_exp_stack.push(exp_stack.top());
        exp_stack.pop();
        params_count--;
    } while (sym.type == 16);

    // 倒序入栈
    while (!tmp_exp_stack.empty())
    {
        exp_stack.push(tmp_exp_stack.top());
        tmp_exp_stack.pop();
    }

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
        memset(exp_stack_tmp, 0, sizeof(ExpItem));
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack_tmp->have_real_value = false;
        exp_stack.push(*exp_stack_tmp);
        free(exp_stack_tmp);
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
        memset(exp_stack_tmp, 0, sizeof(ExpItem));
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack_tmp->have_real_value = false;
        exp_stack.push(*exp_stack_tmp);
        free(exp_stack_tmp);
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
        memset(exp_stack_tmp, 0, sizeof(ExpItem));
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack_tmp->have_real_value = false;
        exp_stack.push(*exp_stack_tmp);
        free(exp_stack_tmp);
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
        memset(exp_stack_tmp, 0, sizeof(ExpItem));
        exp_stack_tmp->type = 2;
        exp_stack_tmp->value = sym.type;
        exp_stack_tmp->have_real_value = false;
        exp_stack.push(*exp_stack_tmp);
        free(exp_stack_tmp);
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
        // 与运算短路求值
        shortCircuit(1);
        nextsym();
        EqExp();
        toBool();
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
        // 或运算短路求值
        shortCircuit(0);
        nextsym();
        LAndExp();
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
    memset(exp_stack_tmp, 0, sizeof(ExpItem));

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
    free(exp_stack_tmp);
}

void NotOperation(ExpItem num)
{
    // 进行非运算
    PrintSpace();
    fprintf(fp_ir, "%%x%d = icmp eq i32 %%x%d, 0\n", ++temp_register, num.value);

    exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
    memset(exp_stack_tmp, 0, sizeof(ExpItem));
    exp_stack_tmp->type = 3;
    exp_stack_tmp->value = ++temp_register;
    exp_stack_tmp->have_real_value = false;
    exp_stack.push(*exp_stack_tmp);
    free(exp_stack_tmp);

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
    memset(exp_stack_tmp, 0, sizeof(ExpItem));
    exp_stack_tmp->type = 5;
    exp_stack_tmp->value = ++temp_register;
    exp_stack_tmp->have_real_value = false;
    exp_stack.push(*exp_stack_tmp);

    PrintSpace();
    fprintf(fp_ir, "%%x%d = icmp ", exp_stack_tmp->value);
    free(exp_stack_tmp);
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

void PrintSpace()
{
    fprintf(fp_ir, "    ");
}

void FuncMapInit()
{
    ParamItem param_item_tmp;

    fprintf(fp_ir, "declare i32 @getint()\n");
    FuncItem func_getint;
    func_getint.type = 1;
    func_map["getint"] = func_getint;

    fprintf(fp_ir, "declare i32 @getch()\n");
    FuncItem func_getch;
    func_getch.type = 1;
    func_map["getch"] = func_getch;

    fprintf(fp_ir, "declare i32 @getarray(i32*)\n");
    FuncItem func_getarray;
    func_getarray.type = 1;
    memset(&param_item_tmp, 0, sizeof(ParamItem));
    param_item_tmp.type = 2;
    param_item_tmp.array_proper.step.push_back(1);
    func_getarray.params.push_back(param_item_tmp);
    func_map["getarray"] = func_getarray;

    fprintf(fp_ir, "declare void @putint(i32)\n");
    FuncItem func_putint;
    func_putint.type = 0;
    memset(&param_item_tmp, 0, sizeof(ParamItem));
    param_item_tmp.type = 1;
    func_putint.params.push_back(param_item_tmp);
    func_map["putint"] = func_putint;

    fprintf(fp_ir, "declare void @putch(i32)\n");
    FuncItem func_putch;
    func_putch.type = 0;
    memset(&param_item_tmp, 0, sizeof(ParamItem));
    param_item_tmp.type = 1;
    func_putch.params.push_back(param_item_tmp);
    func_map["putch"] = func_putch;

    fprintf(fp_ir, "declare void @putarray(i32, i32*)\n");
    FuncItem func_putarray;
    func_putarray.type = 0;
    memset(&param_item_tmp, 0, sizeof(ParamItem));
    param_item_tmp.type = 1;
    func_putarray.params.push_back(param_item_tmp);
    memset(&param_item_tmp, 0, sizeof(ParamItem));
    param_item_tmp.type = 2;
    param_item_tmp.array_proper.step.push_back(1);
    func_putarray.params.push_back(param_item_tmp);
    func_map["putarray"] = func_putarray;

    fprintf(fp_ir, "declare void @memset(i32*, i32, i32)\n");
}

void FuncCall()
{
    // 检查函数是否被声明
    map<string, FuncItem>::iterator func_it_tmp = func_map.find((string)sym.ident);
    if (func_it_tmp == func_map.end())
    {
        throw "Error";
    }

    int params_count = (*func_it_tmp).second.params.size();
    nextsym();
    if (params_count > 0)
    {
        backsysm(sym);
        FuncRParams(params_count);
    }

    if ((*func_it_tmp).second.type == 0)
    {
        PrintSpace();
        fprintf(fp_ir, "call void @%s", (*func_it_tmp).first.c_str());
    }
    else
    {
        exp_stack_tmp = (ExpItem *)malloc(sizeof(ExpItem));
        memset(exp_stack_tmp, 0, sizeof(ExpItem));
        exp_stack_tmp->type = 3;
        exp_stack_tmp->value = ++temp_register;
        exp_stack_tmp->have_real_value = false;
        PrintSpace();
        fprintf(fp_ir, "%%x%d = call i32 @%s", exp_stack_tmp->value, (*func_it_tmp).first.c_str());
    }

    fprintf(fp_ir, "(");
    // 打印函数参数
    for (int i = 0; i < params_count; i++)
    {
        if (i != 0)
        {
            fprintf(fp_ir, ", ");
        }

        // i32 参数
        if ((*func_it_tmp).second.params[i].type == 1)
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
        // i32* 参数
        else if (exp_stack.top().type == 6)
        {
            if ((*func_it_tmp).second.params[i].array_proper.step.size() != exp_stack.top().array_proper.step.size())
            {
                throw "Error";
            }

            for (int j = 0; j < exp_stack.top().array_proper.step.size(); j++)
            {
                // 检查各维度步长是否相同
                if ((*func_it_tmp).second.params[i].array_proper.step[j] != exp_stack.top().array_proper.step[j])
                {
                    throw "Error";
                }
            }

            fprintf(fp_ir, "i32* %%x%d", exp_stack.top().value);
        }
        else
        {
            throw "Error";
        }

        exp_stack.pop();
    }

    if ((*func_it_tmp).second.type == 1)
    {
        exp_stack.push(*exp_stack_tmp);
        free(exp_stack_tmp);
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
    memset(exp_stack_tmp, 0, sizeof(ExpItem));
    exp_stack_tmp->type = 5;
    exp_stack_tmp->value = ++temp_register;
    exp_stack_tmp->have_real_value = false;
    exp_stack.push(*exp_stack_tmp);
    free(exp_stack_tmp);

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
    memset(exp_stack_tmp, 0, sizeof(ExpItem));
    exp_stack_tmp->type = 3;
    exp_stack_tmp->value = ++temp_register;
    exp_stack_tmp->have_real_value = false;
    exp_stack.push(*exp_stack_tmp);
    free(exp_stack_tmp);

    PrintSpace();
    fprintf(fp_ir, "%%x%d = zext i1 %%x%d to i32\n", temp_register, num.value);
}

// 短路求值 type 0 OR , 1 AND
void shortCircuit(int type)
{
    ExpItem num = exp_stack.top();

    if (num.type != 5)
    {
        throw "Error";
    }
    exp_stack.pop();

    if (type == 0)
    {
        PrintSpace();
        fprintf(fp_ir, "br i1 %%x%d ,label %%basic_block_%d, label %%basic_block_%d\n", num.value, exec_block_stack.top(), cond_block_stack.top());
    }
    else
    {
        PrintSpace();
        fprintf(fp_ir, "br i1 %%x%d ,label %%basic_block_%d, label %%basic_block_%d\n", num.value, cond_block_stack.top(), out_block_stack.top());
    }
}