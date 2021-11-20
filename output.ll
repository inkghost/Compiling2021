declare i32 @getint()
declare i32 @getch()
declare i32 @getarray(i32*)
declare void @putint(i32)
declare void @putch(i32)
declare void @putarray(i32, i32*)
declare void @memset(i32*, i32, i32)
@ints = dso_local global [10000 x i32] zeroinitializer

@intt = dso_local global i32 0

@chas = dso_local global [10000 x i32] zeroinitializer

@chat = dso_local global i32 0

@i = dso_local global i32 0
@ii = dso_local global i32 1

@c = dso_local global i32 0

@get = dso_local global [10000 x i32] zeroinitializer

@get2 = dso_local global [10000 x i32] zeroinitializer


define dso_local i32 @isdigit(i32 %x1){
    %x2 = alloca i32
    store i32 %x1, i32* %x2
    %x3 = load i32, i32* %x2
    %x4 = icmp sge i32 %x3, 48
    %x5 = zext i1 %x4 to i32
    %x6 = icmp ne i32 %x5, 0
    %x7 = load i32, i32* %x2
    %x8 = icmp sle i32 %x7, 57
    %x9 = zext i1 %x8 to i32
    %x10 = icmp ne i32 %x9, 0
    %x11 = and i1 %x6, %x10
    br i1 %x11 ,label %basic_block_1, label %basic_block_2
basic_block_1:
    ret i32 1
    br label %basic_block_2
basic_block_2:
    ret i32 0
}

define dso_local i32 @power(i32 %x1, i32 %x2){
    %x3 = alloca i32
    store i32 %x2, i32* %x3
    %x4 = alloca i32
    store i32 %x1, i32* %x4
    %x5 = alloca i32
    store i32 1, i32* %x5
    br label %basic_block_3
basic_block_3:
    %x6 = load i32, i32* %x3
    %x7 = icmp ne i32 %x6, 0
    br i1 %x7 ,label %basic_block_4, label %basic_block_5
basic_block_4:
    %x8 = load i32, i32* %x5
    %x9 = load i32, i32* %x4
    %x10 = mul i32 %x8, %x9
    store i32 %x10, i32* %x5
    %x11 = load i32, i32* %x3
    %x12 = sub i32 %x11, 1
    store i32 %x12, i32* %x3
    br label %basic_block_3
basic_block_5:
    %x13 = load i32, i32* %x5
    ret i32 %x13
}

define dso_local i32 @getstr(i32* %x1){
    %x2 = alloca i32
    %x3 = call i32 @getch()
    store i32 %x3, i32* %x2
    %x4 = alloca i32
    store i32 0, i32* %x4
    br label %basic_block_6
basic_block_6:
    %x5 = load i32, i32* %x2
    %x6 = icmp ne i32 %x5, 13
    %x7 = load i32, i32* %x2
    %x8 = icmp ne i32 %x7, 10
    %x9 = and i1 %x6, %x8
    br i1 %x9 ,label %basic_block_7, label %basic_block_8
basic_block_7:
    %x10 = load i32, i32* %x4
    %x11 = mul i32 %x10, 1
    %x12 = add i32 %x11, 0
    %x13 = getelementptr i32, i32* %x1, i32 %x12
    %x14 = load i32, i32* %x2
    store i32 %x14, i32* %x13
    %x15 = load i32, i32* %x4
    %x16 = add i32 %x15, 1
    store i32 %x16, i32* %x4
    %x17 = call i32 @getch()
    store i32 %x17, i32* %x2
    br label %basic_block_6
basic_block_8:
    %x18 = load i32, i32* %x4
    ret i32 %x18
}

define dso_local void @intpush(i32 %x1){
    %x2 = alloca i32
    store i32 %x1, i32* %x2
    %x3 = load i32, i32* @intt
    %x4 = add i32 %x3, 1
    store i32 %x4, i32* @intt
    %x5 = load i32, i32* @intt
    %x6 = mul i32 %x5, 1
    %x7 = add i32 %x6, 0
    %x8 = getelementptr [10000 x i32],[10000 x i32]* @ints, i32 0, i32 %x7
    %x9 = load i32, i32* %x2
    store i32 %x9, i32* %x8
    ret void
}

define dso_local void @chapush(i32 %x1){
    %x2 = alloca i32
    store i32 %x1, i32* %x2
    %x3 = load i32, i32* @chat
    %x4 = add i32 %x3, 1
    store i32 %x4, i32* @chat
    %x5 = load i32, i32* @chat
    %x6 = mul i32 %x5, 1
    %x7 = add i32 %x6, 0
    %x8 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x7
    %x9 = load i32, i32* %x2
    store i32 %x9, i32* %x8
    ret void
}

define dso_local i32 @intpop(){
    %x1 = load i32, i32* @intt
    %x2 = sub i32 %x1, 1
    store i32 %x2, i32* @intt
    %x3 = load i32, i32* @intt
    %x4 = add i32 %x3, 1
    %x5 = mul i32 %x4, 1
    %x6 = add i32 %x5, 0
    %x7 = getelementptr [10000 x i32],[10000 x i32]* @ints, i32 0, i32 %x6
    %x8 = load i32, i32* %x7
    ret i32 %x8
}

define dso_local i32 @chapop(){
    %x1 = load i32, i32* @chat
    %x2 = sub i32 %x1, 1
    store i32 %x2, i32* @chat
    %x3 = load i32, i32* @chat
    %x4 = add i32 %x3, 1
    %x5 = mul i32 %x4, 1
    %x6 = add i32 %x5, 0
    %x7 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x6
    %x8 = load i32, i32* %x7
    ret i32 %x8
}

define dso_local void @intadd(i32 %x1){
    %x2 = alloca i32
    store i32 %x1, i32* %x2
    %x3 = load i32, i32* @intt
    %x4 = mul i32 %x3, 1
    %x5 = add i32 %x4, 0
    %x6 = getelementptr [10000 x i32],[10000 x i32]* @ints, i32 0, i32 %x5
    %x7 = load i32, i32* @intt
    %x8 = mul i32 %x7, 1
    %x9 = add i32 %x8, 0
    %x10 = getelementptr [10000 x i32],[10000 x i32]* @ints, i32 0, i32 %x9
    %x11 = load i32, i32* %x10
    %x12 = mul i32 %x11, 10
    store i32 %x12, i32* %x6
    %x13 = load i32, i32* @intt
    %x14 = mul i32 %x13, 1
    %x15 = add i32 %x14, 0
    %x16 = getelementptr [10000 x i32],[10000 x i32]* @ints, i32 0, i32 %x15
    %x17 = load i32, i32* @intt
    %x18 = mul i32 %x17, 1
    %x19 = add i32 %x18, 0
    %x20 = getelementptr [10000 x i32],[10000 x i32]* @ints, i32 0, i32 %x19
    %x21 = load i32, i32* %x20
    %x22 = load i32, i32* %x2
    %x23 = add i32 %x21, %x22
    store i32 %x23, i32* %x16
    ret void
}

define dso_local i32 @find(){
    %x1 = call i32 @chapop()
    store i32 %x1, i32* @c
    %x2 = load i32, i32* @ii
    %x3 = mul i32 %x2, 1
    %x4 = add i32 %x3, 0
    %x5 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x4
    store i32 32, i32* %x5
    %x6 = load i32, i32* @ii
    %x7 = add i32 %x6, 1
    %x8 = mul i32 %x7, 1
    %x9 = add i32 %x8, 0
    %x10 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x9
    %x11 = load i32, i32* @c
    store i32 %x11, i32* %x10
    %x12 = load i32, i32* @ii
    %x13 = add i32 %x12, 2
    store i32 %x13, i32* @ii
    %x14 = load i32, i32* @chat
    %x15 = icmp eq i32 %x14, 0
    br i1 %x15 ,label %basic_block_9, label %basic_block_10
basic_block_9:
    ret i32 0
    br label %basic_block_10
basic_block_10:
    ret i32 1
}

define dso_local i32 @main(){
    store i32 0, i32* @intt
    store i32 0, i32* @chat
    %x1 = alloca i32
    %x2 = getelementptr [10000 x i32],[10000 x i32]* @get, i32 0, i32 0
    %x3 = call i32 @getstr(i32* %x2)
    store i32 %x3, i32* %x1
    br label %basic_block_11
basic_block_11:
    %x4 = load i32, i32* @i
    %x5 = load i32, i32* %x1
    %x6 = icmp slt i32 %x4, %x5
    %x7 = zext i1 %x6 to i32
    %x8 = icmp ne i32 %x7, 0
    br i1 %x8 ,label %basic_block_12, label %basic_block_13
basic_block_12:
    %x9 = load i32, i32* @i
    %x10 = mul i32 %x9, 1
    %x11 = add i32 %x10, 0
    %x12 = getelementptr [10000 x i32],[10000 x i32]* @get, i32 0, i32 %x11
    %x13 = load i32, i32* %x12
    %x14 = call i32 @isdigit(i32 %x13)
    %x15 = icmp eq i32 %x14, 1
    br i1 %x15 ,label %basic_block_14, label %basic_block_15
basic_block_14:
    %x16 = load i32, i32* @ii
    %x17 = mul i32 %x16, 1
    %x18 = add i32 %x17, 0
    %x19 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x18
    %x20 = load i32, i32* @i
    %x21 = mul i32 %x20, 1
    %x22 = add i32 %x21, 0
    %x23 = getelementptr [10000 x i32],[10000 x i32]* @get, i32 0, i32 %x22
    %x24 = load i32, i32* %x23
    store i32 %x24, i32* %x19
    %x25 = load i32, i32* @ii
    %x26 = add i32 %x25, 1
    store i32 %x26, i32* @ii
    br label %basic_block_16
basic_block_15:
    %x27 = load i32, i32* @i
    %x28 = mul i32 %x27, 1
    %x29 = add i32 %x28, 0
    %x30 = getelementptr [10000 x i32],[10000 x i32]* @get, i32 0, i32 %x29
    %x31 = load i32, i32* %x30
    %x32 = icmp eq i32 %x31, 40
    br i1 %x32 ,label %basic_block_17, label %basic_block_18
basic_block_17:
    call void @chapush(i32 40)
    br label %basic_block_18
basic_block_18:
    %x33 = load i32, i32* @i
    %x34 = mul i32 %x33, 1
    %x35 = add i32 %x34, 0
    %x36 = getelementptr [10000 x i32],[10000 x i32]* @get, i32 0, i32 %x35
    %x37 = load i32, i32* %x36
    %x38 = icmp eq i32 %x37, 94
    br i1 %x38 ,label %basic_block_19, label %basic_block_20
basic_block_19:
    call void @chapush(i32 94)
    br label %basic_block_20
basic_block_20:
    %x39 = load i32, i32* @i
    %x40 = mul i32 %x39, 1
    %x41 = add i32 %x40, 0
    %x42 = getelementptr [10000 x i32],[10000 x i32]* @get, i32 0, i32 %x41
    %x43 = load i32, i32* %x42
    %x44 = icmp eq i32 %x43, 41
    br i1 %x44 ,label %basic_block_21, label %basic_block_22
basic_block_21:
    %x45 = call i32 @chapop()
    store i32 %x45, i32* @c
    br label %basic_block_23
basic_block_23:
    %x46 = load i32, i32* @c
    %x47 = icmp ne i32 %x46, 40
    br i1 %x47 ,label %basic_block_24, label %basic_block_25
basic_block_24:
    %x48 = load i32, i32* @ii
    %x49 = mul i32 %x48, 1
    %x50 = add i32 %x49, 0
    %x51 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x50
    store i32 32, i32* %x51
    %x52 = load i32, i32* @ii
    %x53 = add i32 %x52, 1
    %x54 = mul i32 %x53, 1
    %x55 = add i32 %x54, 0
    %x56 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x55
    %x57 = load i32, i32* @c
    store i32 %x57, i32* %x56
    %x58 = load i32, i32* @ii
    %x59 = add i32 %x58, 2
    store i32 %x59, i32* @ii
    %x60 = call i32 @chapop()
    store i32 %x60, i32* @c
    br label %basic_block_23
basic_block_25:
    br label %basic_block_22
basic_block_22:
    %x61 = load i32, i32* @i
    %x62 = mul i32 %x61, 1
    %x63 = add i32 %x62, 0
    %x64 = getelementptr [10000 x i32],[10000 x i32]* @get, i32 0, i32 %x63
    %x65 = load i32, i32* %x64
    %x66 = icmp eq i32 %x65, 43
    br i1 %x66 ,label %basic_block_26, label %basic_block_27
basic_block_26:
    br label %basic_block_28
basic_block_28:
    %x67 = load i32, i32* @chat
    %x68 = mul i32 %x67, 1
    %x69 = add i32 %x68, 0
    %x70 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x69
    %x71 = load i32, i32* %x70
    %x72 = icmp eq i32 %x71, 43
    %x73 = load i32, i32* @chat
    %x74 = mul i32 %x73, 1
    %x75 = add i32 %x74, 0
    %x76 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x75
    %x77 = load i32, i32* %x76
    %x78 = icmp eq i32 %x77, 45
    %x79 = or i1 %x72, %x78
    %x80 = load i32, i32* @chat
    %x81 = mul i32 %x80, 1
    %x82 = add i32 %x81, 0
    %x83 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x82
    %x84 = load i32, i32* %x83
    %x85 = icmp eq i32 %x84, 42
    %x86 = or i1 %x79, %x85
    %x87 = load i32, i32* @chat
    %x88 = mul i32 %x87, 1
    %x89 = add i32 %x88, 0
    %x90 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x89
    %x91 = load i32, i32* %x90
    %x92 = icmp eq i32 %x91, 47
    %x93 = or i1 %x86, %x92
    %x94 = load i32, i32* @chat
    %x95 = mul i32 %x94, 1
    %x96 = add i32 %x95, 0
    %x97 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x96
    %x98 = load i32, i32* %x97
    %x99 = icmp eq i32 %x98, 37
    %x100 = or i1 %x93, %x99
    %x101 = load i32, i32* @chat
    %x102 = mul i32 %x101, 1
    %x103 = add i32 %x102, 0
    %x104 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x103
    %x105 = load i32, i32* %x104
    %x106 = icmp eq i32 %x105, 94
    %x107 = or i1 %x100, %x106
    br i1 %x107 ,label %basic_block_29, label %basic_block_30
basic_block_29:
    %x108 = call i32 @find()
    %x109 = icmp eq i32 %x108, 0
    br i1 %x109 ,label %basic_block_31, label %basic_block_32
basic_block_31:
    br label %basic_block_30
    br label %basic_block_32
basic_block_32:
    br label %basic_block_28
basic_block_30:
    call void @chapush(i32 43)
    br label %basic_block_27
basic_block_27:
    %x110 = load i32, i32* @i
    %x111 = mul i32 %x110, 1
    %x112 = add i32 %x111, 0
    %x113 = getelementptr [10000 x i32],[10000 x i32]* @get, i32 0, i32 %x112
    %x114 = load i32, i32* %x113
    %x115 = icmp eq i32 %x114, 45
    br i1 %x115 ,label %basic_block_33, label %basic_block_34
basic_block_33:
    br label %basic_block_35
basic_block_35:
    %x116 = load i32, i32* @chat
    %x117 = mul i32 %x116, 1
    %x118 = add i32 %x117, 0
    %x119 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x118
    %x120 = load i32, i32* %x119
    %x121 = icmp eq i32 %x120, 43
    %x122 = load i32, i32* @chat
    %x123 = mul i32 %x122, 1
    %x124 = add i32 %x123, 0
    %x125 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x124
    %x126 = load i32, i32* %x125
    %x127 = icmp eq i32 %x126, 45
    %x128 = or i1 %x121, %x127
    %x129 = load i32, i32* @chat
    %x130 = mul i32 %x129, 1
    %x131 = add i32 %x130, 0
    %x132 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x131
    %x133 = load i32, i32* %x132
    %x134 = icmp eq i32 %x133, 42
    %x135 = or i1 %x128, %x134
    %x136 = load i32, i32* @chat
    %x137 = mul i32 %x136, 1
    %x138 = add i32 %x137, 0
    %x139 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x138
    %x140 = load i32, i32* %x139
    %x141 = icmp eq i32 %x140, 47
    %x142 = or i1 %x135, %x141
    %x143 = load i32, i32* @chat
    %x144 = mul i32 %x143, 1
    %x145 = add i32 %x144, 0
    %x146 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x145
    %x147 = load i32, i32* %x146
    %x148 = icmp eq i32 %x147, 37
    %x149 = or i1 %x142, %x148
    %x150 = load i32, i32* @chat
    %x151 = mul i32 %x150, 1
    %x152 = add i32 %x151, 0
    %x153 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x152
    %x154 = load i32, i32* %x153
    %x155 = icmp eq i32 %x154, 94
    %x156 = or i1 %x149, %x155
    br i1 %x156 ,label %basic_block_36, label %basic_block_37
basic_block_36:
    %x157 = call i32 @find()
    %x158 = icmp eq i32 %x157, 0
    br i1 %x158 ,label %basic_block_38, label %basic_block_39
basic_block_38:
    br label %basic_block_37
    br label %basic_block_39
basic_block_39:
    br label %basic_block_35
basic_block_37:
    call void @chapush(i32 45)
    br label %basic_block_34
basic_block_34:
    %x159 = load i32, i32* @i
    %x160 = mul i32 %x159, 1
    %x161 = add i32 %x160, 0
    %x162 = getelementptr [10000 x i32],[10000 x i32]* @get, i32 0, i32 %x161
    %x163 = load i32, i32* %x162
    %x164 = icmp eq i32 %x163, 42
    br i1 %x164 ,label %basic_block_40, label %basic_block_41
basic_block_40:
    br label %basic_block_42
basic_block_42:
    %x165 = load i32, i32* @chat
    %x166 = mul i32 %x165, 1
    %x167 = add i32 %x166, 0
    %x168 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x167
    %x169 = load i32, i32* %x168
    %x170 = icmp eq i32 %x169, 42
    %x171 = load i32, i32* @chat
    %x172 = mul i32 %x171, 1
    %x173 = add i32 %x172, 0
    %x174 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x173
    %x175 = load i32, i32* %x174
    %x176 = icmp eq i32 %x175, 47
    %x177 = or i1 %x170, %x176
    %x178 = load i32, i32* @chat
    %x179 = mul i32 %x178, 1
    %x180 = add i32 %x179, 0
    %x181 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x180
    %x182 = load i32, i32* %x181
    %x183 = icmp eq i32 %x182, 37
    %x184 = or i1 %x177, %x183
    %x185 = load i32, i32* @chat
    %x186 = mul i32 %x185, 1
    %x187 = add i32 %x186, 0
    %x188 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x187
    %x189 = load i32, i32* %x188
    %x190 = icmp eq i32 %x189, 94
    %x191 = or i1 %x184, %x190
    br i1 %x191 ,label %basic_block_43, label %basic_block_44
basic_block_43:
    %x192 = call i32 @find()
    %x193 = icmp eq i32 %x192, 0
    br i1 %x193 ,label %basic_block_45, label %basic_block_46
basic_block_45:
    br label %basic_block_44
    br label %basic_block_46
basic_block_46:
    br label %basic_block_42
basic_block_44:
    call void @chapush(i32 42)
    br label %basic_block_41
basic_block_41:
    %x194 = load i32, i32* @i
    %x195 = mul i32 %x194, 1
    %x196 = add i32 %x195, 0
    %x197 = getelementptr [10000 x i32],[10000 x i32]* @get, i32 0, i32 %x196
    %x198 = load i32, i32* %x197
    %x199 = icmp eq i32 %x198, 47
    br i1 %x199 ,label %basic_block_47, label %basic_block_48
basic_block_47:
    br label %basic_block_49
basic_block_49:
    %x200 = load i32, i32* @chat
    %x201 = mul i32 %x200, 1
    %x202 = add i32 %x201, 0
    %x203 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x202
    %x204 = load i32, i32* %x203
    %x205 = icmp eq i32 %x204, 42
    %x206 = load i32, i32* @chat
    %x207 = mul i32 %x206, 1
    %x208 = add i32 %x207, 0
    %x209 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x208
    %x210 = load i32, i32* %x209
    %x211 = icmp eq i32 %x210, 47
    %x212 = or i1 %x205, %x211
    %x213 = load i32, i32* @chat
    %x214 = mul i32 %x213, 1
    %x215 = add i32 %x214, 0
    %x216 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x215
    %x217 = load i32, i32* %x216
    %x218 = icmp eq i32 %x217, 37
    %x219 = or i1 %x212, %x218
    %x220 = load i32, i32* @chat
    %x221 = mul i32 %x220, 1
    %x222 = add i32 %x221, 0
    %x223 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x222
    %x224 = load i32, i32* %x223
    %x225 = icmp eq i32 %x224, 94
    %x226 = or i1 %x219, %x225
    br i1 %x226 ,label %basic_block_50, label %basic_block_51
basic_block_50:
    %x227 = call i32 @find()
    %x228 = icmp eq i32 %x227, 0
    br i1 %x228 ,label %basic_block_52, label %basic_block_53
basic_block_52:
    br label %basic_block_51
    br label %basic_block_53
basic_block_53:
    br label %basic_block_49
basic_block_51:
    call void @chapush(i32 47)
    br label %basic_block_48
basic_block_48:
    %x229 = load i32, i32* @i
    %x230 = mul i32 %x229, 1
    %x231 = add i32 %x230, 0
    %x232 = getelementptr [10000 x i32],[10000 x i32]* @get, i32 0, i32 %x231
    %x233 = load i32, i32* %x232
    %x234 = icmp eq i32 %x233, 37
    br i1 %x234 ,label %basic_block_54, label %basic_block_55
basic_block_54:
    br label %basic_block_56
basic_block_56:
    %x235 = load i32, i32* @chat
    %x236 = mul i32 %x235, 1
    %x237 = add i32 %x236, 0
    %x238 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x237
    %x239 = load i32, i32* %x238
    %x240 = icmp eq i32 %x239, 42
    %x241 = load i32, i32* @chat
    %x242 = mul i32 %x241, 1
    %x243 = add i32 %x242, 0
    %x244 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x243
    %x245 = load i32, i32* %x244
    %x246 = icmp eq i32 %x245, 47
    %x247 = or i1 %x240, %x246
    %x248 = load i32, i32* @chat
    %x249 = mul i32 %x248, 1
    %x250 = add i32 %x249, 0
    %x251 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x250
    %x252 = load i32, i32* %x251
    %x253 = icmp eq i32 %x252, 37
    %x254 = or i1 %x247, %x253
    %x255 = load i32, i32* @chat
    %x256 = mul i32 %x255, 1
    %x257 = add i32 %x256, 0
    %x258 = getelementptr [10000 x i32],[10000 x i32]* @chas, i32 0, i32 %x257
    %x259 = load i32, i32* %x258
    %x260 = icmp eq i32 %x259, 94
    %x261 = or i1 %x254, %x260
    br i1 %x261 ,label %basic_block_57, label %basic_block_58
basic_block_57:
    %x262 = call i32 @find()
    %x263 = icmp eq i32 %x262, 0
    br i1 %x263 ,label %basic_block_59, label %basic_block_60
basic_block_59:
    br label %basic_block_58
    br label %basic_block_60
basic_block_60:
    br label %basic_block_56
basic_block_58:
    call void @chapush(i32 37)
    br label %basic_block_55
basic_block_55:
    %x264 = load i32, i32* @ii
    %x265 = mul i32 %x264, 1
    %x266 = add i32 %x265, 0
    %x267 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x266
    store i32 32, i32* %x267
    %x268 = load i32, i32* @ii
    %x269 = add i32 %x268, 1
    store i32 %x269, i32* @ii
    br label %basic_block_16
basic_block_16:
    %x270 = load i32, i32* @i
    %x271 = add i32 %x270, 1
    store i32 %x271, i32* @i
    br label %basic_block_11
basic_block_13:
    br label %basic_block_61
basic_block_61:
    %x272 = load i32, i32* @chat
    %x273 = icmp sgt i32 %x272, 0
    %x274 = zext i1 %x273 to i32
    %x275 = icmp ne i32 %x274, 0
    br i1 %x275 ,label %basic_block_62, label %basic_block_63
basic_block_62:
    %x276 = alloca i32
    %x277 = call i32 @chapop()
    store i32 %x277, i32* %x276
    %x278 = load i32, i32* @ii
    %x279 = mul i32 %x278, 1
    %x280 = add i32 %x279, 0
    %x281 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x280
    store i32 32, i32* %x281
    %x282 = load i32, i32* @ii
    %x283 = add i32 %x282, 1
    %x284 = mul i32 %x283, 1
    %x285 = add i32 %x284, 0
    %x286 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x285
    %x287 = load i32, i32* %x276
    store i32 %x287, i32* %x286
    %x288 = load i32, i32* @ii
    %x289 = add i32 %x288, 2
    store i32 %x289, i32* @ii
    br label %basic_block_61
basic_block_63:
    %x290 = load i32, i32* @ii
    %x291 = mul i32 %x290, 1
    %x292 = add i32 %x291, 0
    %x293 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x292
    store i32 64, i32* %x293
    store i32 1, i32* @i
    br label %basic_block_64
basic_block_64:
    %x294 = load i32, i32* @i
    %x295 = mul i32 %x294, 1
    %x296 = add i32 %x295, 0
    %x297 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x296
    %x298 = load i32, i32* %x297
    %x299 = icmp ne i32 %x298, 64
    br i1 %x299 ,label %basic_block_65, label %basic_block_66
basic_block_65:
    %x300 = load i32, i32* @i
    %x301 = mul i32 %x300, 1
    %x302 = add i32 %x301, 0
    %x303 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x302
    %x304 = load i32, i32* %x303
    %x305 = icmp eq i32 %x304, 43
    %x306 = load i32, i32* @i
    %x307 = mul i32 %x306, 1
    %x308 = add i32 %x307, 0
    %x309 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x308
    %x310 = load i32, i32* %x309
    %x311 = icmp eq i32 %x310, 45
    %x312 = or i1 %x305, %x311
    %x313 = load i32, i32* @i
    %x314 = mul i32 %x313, 1
    %x315 = add i32 %x314, 0
    %x316 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x315
    %x317 = load i32, i32* %x316
    %x318 = icmp eq i32 %x317, 42
    %x319 = or i1 %x312, %x318
    %x320 = load i32, i32* @i
    %x321 = mul i32 %x320, 1
    %x322 = add i32 %x321, 0
    %x323 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x322
    %x324 = load i32, i32* %x323
    %x325 = icmp eq i32 %x324, 47
    %x326 = or i1 %x319, %x325
    %x327 = load i32, i32* @i
    %x328 = mul i32 %x327, 1
    %x329 = add i32 %x328, 0
    %x330 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x329
    %x331 = load i32, i32* %x330
    %x332 = icmp eq i32 %x331, 37
    %x333 = or i1 %x326, %x332
    %x334 = load i32, i32* @i
    %x335 = mul i32 %x334, 1
    %x336 = add i32 %x335, 0
    %x337 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x336
    %x338 = load i32, i32* %x337
    %x339 = icmp eq i32 %x338, 94
    %x340 = or i1 %x333, %x339
    br i1 %x340 ,label %basic_block_67, label %basic_block_68
basic_block_67:
    %x341 = alloca i32
    %x342 = call i32 @intpop()
    store i32 %x342, i32* %x341
    %x343 = alloca i32
    %x344 = call i32 @intpop()
    store i32 %x344, i32* %x343
    %x345 = alloca i32
    %x346 = load i32, i32* @i
    %x347 = mul i32 %x346, 1
    %x348 = add i32 %x347, 0
    %x349 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x348
    %x350 = load i32, i32* %x349
    %x351 = icmp eq i32 %x350, 43
    br i1 %x351 ,label %basic_block_69, label %basic_block_70
basic_block_69:
    %x352 = load i32, i32* %x341
    %x353 = load i32, i32* %x343
    %x354 = add i32 %x352, %x353
    store i32 %x354, i32* %x345
    br label %basic_block_70
basic_block_70:
    %x355 = load i32, i32* @i
    %x356 = mul i32 %x355, 1
    %x357 = add i32 %x356, 0
    %x358 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x357
    %x359 = load i32, i32* %x358
    %x360 = icmp eq i32 %x359, 45
    br i1 %x360 ,label %basic_block_71, label %basic_block_72
basic_block_71:
    %x361 = load i32, i32* %x343
    %x362 = load i32, i32* %x341
    %x363 = sub i32 %x361, %x362
    store i32 %x363, i32* %x345
    br label %basic_block_72
basic_block_72:
    %x364 = load i32, i32* @i
    %x365 = mul i32 %x364, 1
    %x366 = add i32 %x365, 0
    %x367 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x366
    %x368 = load i32, i32* %x367
    %x369 = icmp eq i32 %x368, 42
    br i1 %x369 ,label %basic_block_73, label %basic_block_74
basic_block_73:
    %x370 = load i32, i32* %x341
    %x371 = load i32, i32* %x343
    %x372 = mul i32 %x370, %x371
    store i32 %x372, i32* %x345
    br label %basic_block_74
basic_block_74:
    %x373 = load i32, i32* @i
    %x374 = mul i32 %x373, 1
    %x375 = add i32 %x374, 0
    %x376 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x375
    %x377 = load i32, i32* %x376
    %x378 = icmp eq i32 %x377, 47
    br i1 %x378 ,label %basic_block_75, label %basic_block_76
basic_block_75:
    %x379 = load i32, i32* %x343
    %x380 = load i32, i32* %x341
    %x381 = sdiv i32 %x379, %x380
    store i32 %x381, i32* %x345
    br label %basic_block_76
basic_block_76:
    %x382 = load i32, i32* @i
    %x383 = mul i32 %x382, 1
    %x384 = add i32 %x383, 0
    %x385 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x384
    %x386 = load i32, i32* %x385
    %x387 = icmp eq i32 %x386, 37
    br i1 %x387 ,label %basic_block_77, label %basic_block_78
basic_block_77:
    %x388 = load i32, i32* %x343
    %x389 = load i32, i32* %x341
    %x390 = srem i32 %x388, %x389
    store i32 %x390, i32* %x345
    br label %basic_block_78
basic_block_78:
    %x391 = load i32, i32* @i
    %x392 = mul i32 %x391, 1
    %x393 = add i32 %x392, 0
    %x394 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x393
    %x395 = load i32, i32* %x394
    %x396 = icmp eq i32 %x395, 94
    br i1 %x396 ,label %basic_block_79, label %basic_block_80
basic_block_79:
    %x397 = load i32, i32* %x343
    %x398 = load i32, i32* %x341
    %x399 = call i32 @power(i32 %x397, i32 %x398)
    store i32 %x399, i32* %x345
    br label %basic_block_80
basic_block_80:
    %x400 = load i32, i32* %x345
    call void @intpush(i32 %x400)
    br label %basic_block_81
basic_block_68:
    %x401 = load i32, i32* @i
    %x402 = mul i32 %x401, 1
    %x403 = add i32 %x402, 0
    %x404 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x403
    %x405 = load i32, i32* %x404
    %x406 = icmp ne i32 %x405, 32
    br i1 %x406 ,label %basic_block_82, label %basic_block_83
basic_block_82:
    %x407 = load i32, i32* @i
    %x408 = mul i32 %x407, 1
    %x409 = add i32 %x408, 0
    %x410 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x409
    %x411 = load i32, i32* %x410
    %x412 = sub i32 %x411, 48
    call void @intpush(i32 %x412)
    store i32 1, i32* @ii
    br label %basic_block_84
basic_block_84:
    %x413 = load i32, i32* @i
    %x414 = load i32, i32* @ii
    %x415 = add i32 %x413, %x414
    %x416 = mul i32 %x415, 1
    %x417 = add i32 %x416, 0
    %x418 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x417
    %x419 = load i32, i32* %x418
    %x420 = icmp ne i32 %x419, 32
    br i1 %x420 ,label %basic_block_85, label %basic_block_86
basic_block_85:
    %x421 = load i32, i32* @i
    %x422 = load i32, i32* @ii
    %x423 = add i32 %x421, %x422
    %x424 = mul i32 %x423, 1
    %x425 = add i32 %x424, 0
    %x426 = getelementptr [10000 x i32],[10000 x i32]* @get2, i32 0, i32 %x425
    %x427 = load i32, i32* %x426
    %x428 = sub i32 %x427, 48
    call void @intadd(i32 %x428)
    %x429 = load i32, i32* @ii
    %x430 = add i32 %x429, 1
    store i32 %x430, i32* @ii
    br label %basic_block_84
basic_block_86:
    %x431 = load i32, i32* @i
    %x432 = load i32, i32* @ii
    %x433 = add i32 %x431, %x432
    %x434 = sub i32 %x433, 1
    store i32 %x434, i32* @i
    br label %basic_block_83
basic_block_83:
    br label %basic_block_81
basic_block_81:
    %x435 = load i32, i32* @i
    %x436 = add i32 %x435, 1
    store i32 %x436, i32* @i
    br label %basic_block_64
basic_block_66:
    %x437 = mul i32 1, 1
    %x438 = add i32 %x437, 0
    %x439 = getelementptr [10000 x i32],[10000 x i32]* @ints, i32 0, i32 %x438
    %x440 = load i32, i32* %x439
    call void @putint(i32 %x440)
    ret i32 0
}
