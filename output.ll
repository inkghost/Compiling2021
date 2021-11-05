declare i32 @getint()
declare i32 @getch()
declare i32 @getarray(i32*)
declare void @putint(i32)
declare void @putch(i32)
declare void @putarray(i32,i32*)
define dso_local i32 @main(){
    %1 = alloca i32
    store i32 10, i32* %1
    %2 = load i32, i32* %1
    %3 = icmp eq i32 %2, 0
    %4 = zext i1 %3 to i32    %5 = sub i32 0, %4
    %6 = icmp ne i32 %5, 0
    br i1 %6 ,label %basic_block_1, label %basic_block_2
basic_block_1:
    %7 = sub i32 0, 1
    %8 = sub i32 0, %7
    %9 = sub i32 0, %8
    store i32 %9, i32* %1
    br label %basic_block_3
basic_block_2:
    store i32 0, i32* %1
    br label %basic_block_3
basic_block_3:
    %10 = load i32, i32* %1
    call void @putint(i32 %10)
    ret i32 0
}