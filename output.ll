declare i32 @getint()
declare i32 @getch()
declare i32 @getarray(i32*)
declare void @putint(i32)
declare void @putch(i32)
declare void @putarray(i32, i32*)
declare void @memset(i32*, i32, i32)

@a = dso_local global [12 x i32] zeroinitializer

define dso_local i32 @main(){
    %x2 = alloca i32
    store i32 0, i32* %x2
    %x3 = alloca i32
    store i32 0, i32* %x3
    br label %basic_block_1
basic_block_1:
    %x4 = load i32, i32* %x2
    %x5 = icmp sle i32 %x4, 5
    %x6 = zext i1 %x5 to i32
    %x7 = icmp ne i32 %x6, 0
    br i1 %x7 ,label %basic_block_2, label %basic_block_3
basic_block_2:
    %x8 = alloca i32
    %x9 = load i32, i32* %x2
    store i32 %x9, i32* %x8
    br label %basic_block_4
basic_block_4:
    %x10 = load i32, i32* %x8
    %x11 = icmp sge i32 %x10, 0
    %x12 = zext i1 %x11 to i32
    %x13 = icmp ne i32 %x12, 0
    br i1 %x13 ,label %basic_block_5, label %basic_block_6
basic_block_5:
    %x14 = load i32, i32* %x8
    %x15 = icmp slt i32 %x14, 4
    %x16 = zext i1 %x15 to i32
    %x17 = icmp ne i32 %x16, 0
    %x18 = load i32, i32* %x2
    %x19 = load i32, i32* %x8
    %x20 = sub i32 %x18, %x19
    %x21 = icmp slt i32 %x20, 3
    %x22 = zext i1 %x21 to i32
    %x23 = icmp ne i32 %x22, 0
    %x24 = and i1 %x17, %x23
    br i1 %x24 ,label %basic_block_7, label %basic_block_8
basic_block_7:
    %x25 = load i32, i32* %x2
    %x26 = load i32, i32* %x8
    %x27 = sub i32 %x25, %x26
    %x28 = mul i32 %x27, 4
    %x29 = add i32 %x28, 0
    %x30 = load i32, i32* %x8
    %x31 = mul i32 %x30, 1
    %x32 = add i32 %x31, %x29
    %x33 = getelementptr [12 x i32],[12 x i32]* @a, i32 0, i32 %x32
    %x34 = load i32, i32* %x3
    store i32 %x34, i32* %x33
    %x35 = load i32, i32* %x3
    %x36 = add i32 %x35, 1
    store i32 %x36, i32* %x3
    br label %basic_block_8
basic_block_8:
    %x37 = load i32, i32* %x8
    %x38 = sub i32 %x37, 1
    store i32 %x38, i32* %x8
    br label %basic_block_4
basic_block_6:
    %x39 = load i32, i32* %x2
    %x40 = add i32 %x39, 1
    store i32 %x40, i32* %x2
    br label %basic_block_1
basic_block_3:
    store i32 0, i32* %x2
    %x41 = alloca i32
    store i32 0, i32* %x41
    br label %basic_block_9
basic_block_9:
    %x42 = load i32, i32* %x2
    %x43 = icmp slt i32 %x42, 3
    %x44 = zext i1 %x43 to i32
    %x45 = icmp ne i32 %x44, 0
    br i1 %x45 ,label %basic_block_10, label %basic_block_11
basic_block_10:
    store i32 0, i32* %x41
    br label %basic_block_12
basic_block_12:
    %x46 = load i32, i32* %x41
    %x47 = icmp slt i32 %x46, 4
    %x48 = zext i1 %x47 to i32
    %x49 = icmp ne i32 %x48, 0
    br i1 %x49 ,label %basic_block_13, label %basic_block_14
basic_block_13:
    %x50 = load i32, i32* %x2
    %x51 = mul i32 %x50, 4
    %x52 = add i32 %x51, 0
    %x53 = load i32, i32* %x41
    %x54 = mul i32 %x53, 1
    %x55 = add i32 %x54, %x52
    %x56 = getelementptr [12 x i32],[12 x i32]* @a, i32 0, i32 %x55
    %x57 = load i32, i32* %x56
    call void @putint(i32 %x57)
    call void @putch(i32 32)
    %x58 = load i32, i32* %x41
    %x59 = add i32 %x58, 1
    store i32 %x59, i32* %x41
    br label %basic_block_12
basic_block_14:
    call void @putch(i32 10)
    %x60 = load i32, i32* %x2
    %x61 = add i32 %x60, 1
    store i32 %x61, i32* %x2
    br label %basic_block_9
basic_block_11:
    ret i32 0
}
