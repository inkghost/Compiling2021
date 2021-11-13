declare i32 @getint()
declare i32 @getch()
declare i32 @getarray(i32*)
declare void @putint(i32)
declare void @putch(i32)
declare void @putarray(i32,i32*)
@a = dso_local global i32 7
%x1 = load i32, i32* @a
%x3 = add i32 %x1, i32 %x1
@b = dso_local global i32 %3
define dso_local i32 @main(){
    %x2 = alloca i32
    store i32 1, i32* %x2
    ret i32 0
}