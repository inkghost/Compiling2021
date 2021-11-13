declare i32 @getint()
declare i32 @getch()
declare i32 @getarray(i32*)
declare void @putint(i32)
declare void @putch(i32)
declare void @putarray(i32,i32*)
define dso_local i32 @main(){
    %x1 = alloca i32
    store i32 1, i32* %x1
