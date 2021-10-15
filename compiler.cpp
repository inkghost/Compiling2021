#include <iostream>
#include <cstdio>
#include "syntax.h"

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Wrong number of parameters");
        return -1;
    }

    // 设置输入输出文件
    FILE *fp_input, *fp_ir;
    fp_input = fopen(argv[1], "r");
    fp_ir = fopen(argv[2], "w");

    try
    {
        CompUnit(fp_input, fp_ir);
    }
    catch(const char* error)
    {
        return -1;
    }
    
    fclose(fp_input);
    fclose(fp_ir);

    return 0;
}