declare i32 @getint()
declare i32 @getch()
declare i32 @getarray(i32*)
declare void @putint(i32)
declare void @putch(i32)
declare void @putarray(i32,i32*)
define dso_local i32 @main(){
    %x1 = alloca i32
    %x2 = call i32 @getint()
    store i32 %x2, i32* %x1
    %x3 = alloca i32
    store i32 0, i32* %x3
    %x4 = alloca i32
    store i32 0, i32* %x4
    br label %basic_block_1
basic_block_1:
    %x5 = load i32, i32* %x3
    %x6 = load i32, i32* %x1
    %x7 = icmp slt i32 %x5, %x6
    %x8 = zext i1 %x7 to i32
    %x9 = icmp ne i32 %x8, 0
    br i1 %x9 ,label %basic_block_2, label %basic_block_3
basic_block_2:
    %x10 = load i32, i32* %x3
    %x11 = srem i32 %x10, 2
    %x12 = icmp eq i32 %x11, 0
    br i1 %x12 ,label %basic_block_4, label %basic_block_5
basic_block_4:
    %x13 = load i32, i32* %x3
    %x14 = add i32 %x13, 1
    store i32 %x14, i32* %x3
    br label %basic_block_1
    br label %basic_block_5
basic_block_5:
    %x15 = load i32, i32* %x3
    %x16 = add i32 %x15, 1
    store i32 %x16, i32* %x3
    %x17 = load i32, i32* %x4
    %x18 = load i32, i32* %x3
    %x19 = add i32 %x17, %x18
    store i32 %x19, i32* %x4
    %x20 = load i32, i32* %x4
    call void @putint(i32 %x20)
    call void @putch(i32 10)
    br label %basic_block_1
basic_block_3:
    ret i32 0
}