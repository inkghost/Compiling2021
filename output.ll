declare i32 @getint()
declare i32 @getch()
declare i32 @getarray(i32*)
declare void @putint(i32)
declare void @putch(i32)
declare void @putarray(i32,i32*)
define dso_local i32 @main(){
    %x1 = alloca i32
    store i32 3, i32* %x1
    %x2 = alloca i32
    %x3 = call i32 @getint()
    store i32 %x3, i32* %x2
    %x4 = load i32, i32* %x2
    %x5 = add i32 3, %x4
    call void @putint(i32 %x5)
    ret i32 0
}