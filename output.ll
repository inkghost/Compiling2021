declare i32 @getint()
declare i32 @getch()
declare i32 @getarray(i32*)
declare void @putint(i32)
declare void @putch(i32)
declare void @putarray(i32, i32*)
declare void @memset(i32*, i32, i32)
@n = dso_local global i32 0


define dso_local i32 @swap(i32* %x1, i32 %x2, i32 %x3){
    %x4 = alloca i32
    store i32 %x2, i32* %x4
    %x5 = alloca i32
    store i32 %x3, i32* %x5
    %x6 = alloca i32
    %x7 = load i32, i32* %x4
    %x8 = mul i32 %x7, 1
    %x9 = add i32 %x8, 0
    %x10 = getelementptr i32, i32* %x1, i32 %x9
    %x11 = load i32, i32* %x10
    store i32 %x11, i32* %x6
    %x12 = load i32, i32* %x4
    %x13 = mul i32 %x12, 1
    %x14 = add i32 %x13, 0
    %x15 = getelementptr i32, i32* %x1, i32 %x14
    %x16 = load i32, i32* %x5
    %x17 = mul i32 %x16, 1
    %x18 = add i32 %x17, 0
    %x19 = getelementptr i32, i32* %x1, i32 %x18
    %x20 = load i32, i32* %x19
    store i32 %x20, i32* %x15
    %x21 = load i32, i32* %x5
    %x22 = mul i32 %x21, 1
    %x23 = add i32 %x22, 0
    %x24 = getelementptr i32, i32* %x1, i32 %x23
    %x25 = load i32, i32* %x6
    store i32 %x25, i32* %x24
    ret i32 0
}

define dso_local i32 @heap_ajust(i32* %x1, i32 %x2, i32 %x3){
    %x4 = alloca i32
    store i32 %x3, i32* %x4
    %x5 = alloca i32
    store i32 %x2, i32* %x5
    %x6 = alloca i32
    %x7 = load i32, i32* %x5
    store i32 %x7, i32* %x6
    %x8 = alloca i32
    %x9 = load i32, i32* %x6
    %x10 = mul i32 %x9, 2
    %x11 = add i32 %x10, 1
    store i32 %x11, i32* %x8
    br label %basic_block_1
basic_block_1:
    %x12 = load i32, i32* %x8
    %x13 = load i32, i32* %x4
    %x14 = add i32 %x13, 1
    %x15 = icmp slt i32 %x12, %x14
    %x16 = zext i1 %x15 to i32
    %x17 = icmp ne i32 %x16, 0
    br i1 %x17 ,label %basic_block_2, label %basic_block_3
basic_block_2:
    %x18 = load i32, i32* %x8
    %x19 = load i32, i32* %x4
    %x20 = icmp slt i32 %x18, %x19
    %x21 = zext i1 %x20 to i32
    %x22 = icmp ne i32 %x21, 0
    %x23 = load i32, i32* %x8
    %x24 = mul i32 %x23, 1
    %x25 = add i32 %x24, 0
    %x26 = getelementptr i32, i32* %x1, i32 %x25
    %x27 = load i32, i32* %x26
    %x28 = load i32, i32* %x8
    %x29 = add i32 %x28, 1
    %x30 = mul i32 %x29, 1
    %x31 = add i32 %x30, 0
    %x32 = getelementptr i32, i32* %x1, i32 %x31
    %x33 = load i32, i32* %x32
    %x34 = icmp slt i32 %x27, %x33
    %x35 = zext i1 %x34 to i32
    %x36 = icmp ne i32 %x35, 0
    %x37 = and i1 %x22, %x36
    br i1 %x37 ,label %basic_block_4, label %basic_block_5
basic_block_4:
    %x38 = load i32, i32* %x8
    %x39 = add i32 %x38, 1
    store i32 %x39, i32* %x8
    br label %basic_block_5
basic_block_5:
    %x40 = load i32, i32* %x6
    %x41 = mul i32 %x40, 1
    %x42 = add i32 %x41, 0
    %x43 = getelementptr i32, i32* %x1, i32 %x42
    %x44 = load i32, i32* %x43
    %x45 = load i32, i32* %x8
    %x46 = mul i32 %x45, 1
    %x47 = add i32 %x46, 0
    %x48 = getelementptr i32, i32* %x1, i32 %x47
    %x49 = load i32, i32* %x48
    %x50 = icmp sgt i32 %x44, %x49
    %x51 = zext i1 %x50 to i32
    %x52 = icmp ne i32 %x51, 0
    br i1 %x52 ,label %basic_block_6, label %basic_block_7
basic_block_6:
    ret i32 0
    br label %basic_block_8
basic_block_7:
    %x53 = getelementptr i32, i32* %x1, i32 0
    %x54 = load i32, i32* %x6
    %x55 = load i32, i32* %x8
    %x56 = call i32 @swap(i32* %x53, i32 %x54, i32 %x55)
    store i32 %x56, i32* %x6
    %x57 = load i32, i32* %x8
    store i32 %x57, i32* %x6
    %x58 = load i32, i32* %x6
    %x59 = mul i32 %x58, 2
    %x60 = add i32 %x59, 1
    store i32 %x60, i32* %x8
    br label %basic_block_8
basic_block_8:
    br label %basic_block_1
basic_block_3:
    ret i32 0
}

define dso_local i32 @heap_sort(i32* %x1, i32 %x2){
    %x3 = alloca i32
    store i32 %x2, i32* %x3
    %x4 = alloca i32
    %x5 = alloca i32
    %x6 = load i32, i32* %x3
    %x7 = sdiv i32 %x6, 2
    %x8 = sub i32 %x7, 1
    store i32 %x8, i32* %x4
    br label %basic_block_9
basic_block_9:
    %x9 = load i32, i32* %x4
    %x10 = icmp sgt i32 %x9, -1
    %x11 = zext i1 %x10 to i32
    %x12 = icmp ne i32 %x11, 0
    br i1 %x12 ,label %basic_block_10, label %basic_block_11
basic_block_10:
    %x13 = load i32, i32* %x3
    %x14 = sub i32 %x13, 1
    store i32 %x14, i32* %x5
    %x15 = getelementptr i32, i32* %x1, i32 0
    %x16 = load i32, i32* %x4
    %x17 = load i32, i32* %x5
    %x18 = call i32 @heap_ajust(i32* %x15, i32 %x16, i32 %x17)
    store i32 %x18, i32* %x5
    %x19 = load i32, i32* %x4
    %x20 = sub i32 %x19, 1
    store i32 %x20, i32* %x4
    br label %basic_block_9
basic_block_11:
    %x21 = load i32, i32* %x3
    %x22 = sub i32 %x21, 1
    store i32 %x22, i32* %x4
    br label %basic_block_12
basic_block_12:
    %x23 = load i32, i32* %x4
    %x24 = icmp sgt i32 %x23, 0
    %x25 = zext i1 %x24 to i32
    %x26 = icmp ne i32 %x25, 0
    br i1 %x26 ,label %basic_block_13, label %basic_block_14
basic_block_13:
    %x27 = alloca i32
    store i32 0, i32* %x27
    %x28 = getelementptr i32, i32* %x1, i32 0
    %x29 = load i32, i32* %x4
    %x30 = call i32 @swap(i32* %x28, i32 0, i32 %x29)
    store i32 %x30, i32* %x5
    %x31 = load i32, i32* %x4
    %x32 = sub i32 %x31, 1
    store i32 %x32, i32* %x5
    %x33 = getelementptr i32, i32* %x1, i32 0
    %x34 = load i32, i32* %x5
    %x35 = call i32 @heap_ajust(i32* %x33, i32 0, i32 %x34)
    store i32 %x35, i32* %x5
    %x36 = load i32, i32* %x4
    %x37 = sub i32 %x36, 1
    store i32 %x37, i32* %x4
    br label %basic_block_12
basic_block_14:
    ret i32 0
}

define dso_local i32 @main(){
    %x1 = alloca [10 x i32]
    %x2 = getelementptr [10 x i32],[10 x i32]* %x1, i32 0, i32 0
    %x3 = call i32 @getarray(i32* %x2)
    store i32 %x3, i32* @n
    %x4 = alloca i32
    store i32 0, i32* %x4
    %x5 = getelementptr [10 x i32],[10 x i32]* %x1, i32 0, i32 0
    %x6 = load i32, i32* @n
    %x7 = call i32 @heap_sort(i32* %x5, i32 %x6)
    store i32 %x7, i32* %x4
    %x8 = load i32, i32* @n
    %x9 = getelementptr [10 x i32],[10 x i32]* %x1, i32 0, i32 0
    call void @putarray(i32 %x8, i32* %x9)
    ret i32 0
}
