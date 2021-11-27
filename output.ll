declare i32 @getint()
declare i32 @getch()
declare i32 @getarray(i32*)
declare void @putint(i32)
declare void @putch(i32)
declare void @putarray(i32, i32*)
declare void @memset(i32*, i32, i32)
@a = dso_local global [12 x i32] zeroinitializer


define dso_local i32 @main(){
    %x1 = alloca i32
    store i32 0, i32* %x1
    %x2 = alloca i32
    store i32 0, i32* %x2
    br label %basic_block_1
basic_block_1:
    %x3 = load i32, i32* %x1
    %x4 = icmp sle i32 %x3, 5
    %x5 = zext i1 %x4 to i32
    %x6 = icmp ne i32 %x5, 0
    br i1 %x6 ,label %basic_block_2, label %basic_block_3
basic_block_2:
    %x7 = alloca i32
    %x8 = load i32, i32* %x1
    store i32 %x8, i32* %x7
    br label %basic_block_4
basic_block_4:
    %x9 = load i32, i32* %x7
    %x10 = icmp sge i32 %x9, 0
    %x11 = zext i1 %x10 to i32
    %x12 = icmp ne i32 %x11, 0
    br i1 %x12 ,label %basic_block_5, label %basic_block_6
basic_block_5:
    %x13 = load i32, i32* %x7
    %x14 = icmp slt i32 %x13, 4
    %x15 = zext i1 %x14 to i32
    %x16 = icmp ne i32 %x15, 0
    br i1 %x16 ,label %basic_block_9, label %basic_block_8
basic_block_9:
    %x17 = load i32, i32* %x1
    %x18 = load i32, i32* %x7
    %x19 = sub i32 %x17, %x18
    %x20 = icmp slt i32 %x19, 3
    %x21 = zext i1 %x20 to i32
    %x22 = icmp ne i32 %x21, 0
    br i1 %x22 ,label %basic_block_7, label %basic_block_8
basic_block_7:
    %x23 = load i32, i32* %x1
    %x24 = load i32, i32* %x7
    %x25 = sub i32 %x23, %x24
    %x26 = mul i32 %x25, 4
    %x27 = add i32 %x26, 0
    %x28 = load i32, i32* %x7
    %x29 = mul i32 %x28, 1
    %x30 = add i32 %x29, %x27
    %x31 = getelementptr [12 x i32],[12 x i32]* @a, i32 0, i32 %x30
    %x32 = load i32, i32* %x2
    store i32 %x32, i32* %x31
    %x33 = load i32, i32* %x2
    %x34 = add i32 %x33, 1
    store i32 %x34, i32* %x2
    br label %basic_block_8
basic_block_8:
    %x35 = load i32, i32* %x7
    %x36 = sub i32 %x35, 1
    store i32 %x36, i32* %x7
    br label %basic_block_4
basic_block_6:
    %x37 = load i32, i32* %x1
    %x38 = add i32 %x37, 1
    store i32 %x38, i32* %x1
    br label %basic_block_1
basic_block_3:
    store i32 0, i32* %x1
    %x39 = alloca i32
    store i32 0, i32* %x39
    br label %basic_block_10
basic_block_10:
    %x40 = load i32, i32* %x1
    %x41 = icmp slt i32 %x40, 3
    %x42 = zext i1 %x41 to i32
    %x43 = icmp ne i32 %x42, 0
    br i1 %x43 ,label %basic_block_11, label %basic_block_12
basic_block_11:
    store i32 0, i32* %x39
    br label %basic_block_13
basic_block_13:
    %x44 = load i32, i32* %x39
    %x45 = icmp slt i32 %x44, 4
    %x46 = zext i1 %x45 to i32
    %x47 = icmp ne i32 %x46, 0
    br i1 %x47 ,label %basic_block_14, label %basic_block_15
basic_block_14:
    %x48 = load i32, i32* %x1
    %x49 = mul i32 %x48, 4
    %x50 = add i32 %x49, 0
    %x51 = load i32, i32* %x39
    %x52 = mul i32 %x51, 1
    %x53 = add i32 %x52, %x50
    %x54 = getelementptr [12 x i32],[12 x i32]* @a, i32 0, i32 %x53
    %x55 = load i32, i32* %x54
    call void @putint(i32 %x55)
    call void @putch(i32 32)
    %x56 = load i32, i32* %x39
    %x57 = add i32 %x56, 1
    store i32 %x57, i32* %x39
    br label %basic_block_13
basic_block_15:
    call void @putch(i32 10)
    %x58 = load i32, i32* %x1
    %x59 = add i32 %x58, 1
    store i32 %x59, i32* %x1
    br label %basic_block_10
basic_block_12:
    ret i32 0
}
