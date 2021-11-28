declare i32 @getint()
declare i32 @getch()
declare i32 @getarray(i32*)
declare void @putint(i32)
declare void @putch(i32)
declare void @putarray(i32, i32*)
declare void @memset(i32*, i32, i32)

define dso_local i32 @f(){
    call void @putint(i32 0)
    ret i32 0
}

define dso_local i32 @g(){
    call void @putint(i32 1)
    ret i32 1
}

define dso_local i32 @main(){
    %x1 = call i32 @f()
    %x2 = icmp ne i32 %x1, 0
    br i1 %x2 ,label %basic_block_3, label %basic_block_2
basic_block_3:
    %x3 = call i32 @g()
    %x4 = icmp ne i32 %x3, 0
    br i1 %x4 ,label %basic_block_1, label %basic_block_4
basic_block_4:
    %x5 = call i32 @f()
    %x6 = icmp eq i32 %x5, 0
    %x7 = zext i1 %x6 to i32    %x8 = icmp ne i32 %x7, 0
    br i1 %x8 ,label %basic_block_5, label %basic_block_2
basic_block_5:
    %x9 = call i32 @g()
    %x10 = icmp ne i32 %x9, 0
    br i1 %x10 ,label %basic_block_1, label %basic_block_2
basic_block_1:
    ret i32 0
    br label %basic_block_6
basic_block_2:
    ret i32 1
    br label %basic_block_6
basic_block_6:
    ret i32 0
}
