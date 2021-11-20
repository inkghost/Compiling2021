declare i32 @getint()
declare i32 @getch()
declare i32 @getarray(i32*)
declare void @putint(i32)
declare void @putch(i32)
declare void @putarray(i32, i32*)
declare void @memset(i32*, i32, i32)

define dso_local i32 @exgcd(i32 %x1, i32 %x2, i32* %x3, i32* %x4){
    %x5 = alloca i32
    store i32 %x1, i32* %x5
    %x6 = alloca i32
    store i32 %x2, i32* %x6
    %x7 = load i32, i32* %x6
    %x8 = icmp eq i32 %x7, 0
    br i1 %x8 ,label %basic_block_1, label %basic_block_2
basic_block_1:
    %x9 = mul i32 0, 1
    %x10 = add i32 %x9, 0
    %x11 = getelementptr i32, i32* %x3, i32 %x10
    store i32 1, i32* %x11
    %x12 = mul i32 0, 1
    %x13 = add i32 %x12, 0
    %x14 = getelementptr i32, i32* %x4, i32 %x13
    store i32 0, i32* %x14
    %x15 = load i32, i32* %x5
    ret i32 %x15
    br label %basic_block_3
basic_block_2:
    %x16 = alloca i32
    %x17 = load i32, i32* %x6
    %x18 = load i32, i32* %x5
    %x19 = load i32, i32* %x6
    %x20 = srem i32 %x18, %x19
    %x21 = getelementptr i32, i32* %x3, i32 0
    %x22 = getelementptr i32, i32* %x4, i32 0
    %x23 = call i32 @exgcd(i32 %x17, i32 %x20, i32* %x21, i32* %x22)
    store i32 %x23, i32* %x16
    %x24 = alloca i32
    %x25 = mul i32 0, 1
    %x26 = add i32 %x25, 0
    %x27 = getelementptr i32, i32* %x3, i32 %x26
    %x28 = load i32, i32* %x27
    store i32 %x28, i32* %x24
    %x29 = mul i32 0, 1
    %x30 = add i32 %x29, 0
    %x31 = getelementptr i32, i32* %x3, i32 %x30
    %x32 = mul i32 0, 1
    %x33 = add i32 %x32, 0
    %x34 = getelementptr i32, i32* %x4, i32 %x33
    %x35 = load i32, i32* %x34
    store i32 %x35, i32* %x31
    %x36 = mul i32 0, 1
    %x37 = add i32 %x36, 0
    %x38 = getelementptr i32, i32* %x4, i32 %x37
    %x39 = load i32, i32* %x24
    %x40 = load i32, i32* %x5
    %x41 = load i32, i32* %x6
    %x42 = sdiv i32 %x40, %x41
    %x43 = mul i32 0, 1
    %x44 = add i32 %x43, 0
    %x45 = getelementptr i32, i32* %x4, i32 %x44
    %x46 = load i32, i32* %x45
    %x47 = mul i32 %x42, %x46
    %x48 = sub i32 %x39, %x47
    store i32 %x48, i32* %x38
    %x49 = load i32, i32* %x16
    ret i32 %x49
    br label %basic_block_3
basic_block_3:
    ret i32 0
}

define dso_local i32 @main(){
    %x1 = alloca i32
    store i32 7, i32* %x1
    %x2 = alloca i32
    store i32 15, i32* %x2
    %x3 = alloca [1 x i32]
    %x4 = getelementptr [1 x i32],[1 x i32]* %x3, i32 0, i32 0
    store i32 1, i32* %x4
    %x5 = alloca [1 x i32]
    %x6 = getelementptr [1 x i32],[1 x i32]* %x5, i32 0, i32 0
    store i32 1, i32* %x6
    %x7 = getelementptr [1 x i32],[1 x i32]* %x3, i32 0, i32 0
    %x8 = getelementptr [1 x i32],[1 x i32]* %x5, i32 0, i32 0
    %x9 = call i32 @exgcd(i32 7, i32 15, i32* %x7, i32* %x8)
    %x10 = mul i32 0, 1
    %x11 = add i32 %x10, 0
    %x12 = getelementptr [1 x i32],[1 x i32]* %x3, i32 0, i32 %x11
    %x13 = mul i32 0, 1
    %x14 = add i32 %x13, 0
    %x15 = getelementptr [1 x i32],[1 x i32]* %x3, i32 0, i32 %x14
    %x16 = load i32, i32* %x15
    %x17 = srem i32 %x16, 15
    %x18 = add i32 %x17, 15
    %x19 = srem i32 %x18, 15
    store i32 %x19, i32* %x12
    %x20 = mul i32 0, 1
    %x21 = add i32 %x20, 0
    %x22 = getelementptr [1 x i32],[1 x i32]* %x3, i32 0, i32 %x21
    %x23 = load i32, i32* %x22
    call void @putint(i32 %x23)
    ret i32 0
}
