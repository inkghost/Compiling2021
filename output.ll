declare i32 @getint()
declare i32 @getch()
declare i32 @getarray(i32*)
declare void @putint(i32)
declare void @putch(i32)
declare void @putarray(i32, i32*)
declare void @memset(i32*, i32, i32)

@arr = dso_local global [4 x i32] [i32 1, i32 1, i32 4, i32 5]

define dso_local i32 @main(){
