declare i32 @getint()
declare i32 @getch()
declare i32 @getarray(i32*)
declare void @putint(i32)
declare void @putch(i32)
declare void @putarray(i32, i32*)
declare void @memset(i32*, i32, i32)

define dso_local i32 @foo(i32 %x1, i32 %x2){
    %x3 = alloca i32
    store i32 %x1, i32* %x3
    %x4 = alloca i32
    store i32 %x2, i32* %x4
    %x5 = alloca i32
    %x6 = load i32, i32* %x3
    %x7 = load i32, i32* %x4
    %x8 = add i32 %x6, %x7
    store i32 %x8, i32* %x5
    %x9 = load i32, i32* %x5
    %x10 = load i32, i32* %x3
    %x11 = sub i32 %x9, %x10
    store i32 %x11, i32* %x3
    %x12 = load i32, i32* %x5
    %x13 = load i32, i32* %x4
    %x14 = sub i32 %x12, %x13
    store i32 %x14, i32* %x4
    %x15 = load i32, i32* %x3
    %x16 = load i32, i32* %x4
    %x17 = sub i32 %x15, %x16
    ret i32 %x17
}

define dso_local i32 @main(){
