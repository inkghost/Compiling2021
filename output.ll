declare i32 @getint()
declare i32 @getch()
declare i32 @getarray(i32*)
declare void @putint(i32)
declare void @putch(i32)
declare void @putarray(i32,i32*)
define dso_local i32 @main(){
    %x1 = alloca i32
    store i32 5, i32* %x1
    %x2 = alloca i32
    store i32 10, i32* %x2
    %x3 = load i32, i32* %x1
    %x4 = icmp eq i32 %x3, 6
    %x5 = load i32, i32* %x2
    %x6 = icmp eq i32 %x5, 11
    %x7 = or i1 %x4, %x6
    br i1 %x7 ,label %basic_block_1, label %basic_block_2
basic_block_1:
    %x8 = load i32, i32* %x1
    ret i32 %x8
    br label %basic_block_3
basic_block_2:
    %x9 = load i32, i32* %x2
    %x10 = icmp eq i32 %x9, 10
    %x11 = load i32, i32* %x1
    %x12 = icmp eq i32 %x11, 1
    %x13 = and i1 %x10, %x12
    br i1 %x13 ,label %basic_block_4, label %basic_block_5
basic_block_4:
    store i32 25, i32* %x1
    br label %basic_block_6
basic_block_5:
    %x14 = load i32, i32* %x2
    %x15 = icmp eq i32 %x14, 10
    %x16 = load i32, i32* %x1
    %x17 = sub i32 0, 5
    %x18 = icmp eq i32 %x16, %x17
    %x19 = and i1 %x15, %x18
    br i1 %x19 ,label %basic_block_7, label %basic_block_8
basic_block_7:
    %x20 = load i32, i32* %x1
    %x21 = add i32 %x20, 15
    store i32 %x21, i32* %x1
    br label %basic_block_9
basic_block_8:
    %x22 = load i32, i32* %x1
    %x23 = sub i32 0, %x22
    store i32 %x23, i32* %x1
    br label %basic_block_9
basic_block_9:
    br label %basic_block_6
basic_block_6:
    br label %basic_block_3
basic_block_3:
    %x24 = load i32, i32* %x1
    call void @putint(i32 %x24)
    ret i32 0
}