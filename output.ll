declare i32 @getint()
declare i32 @getch()
declare i32 @getarray(i32*)
declare void @putint(i32)
declare void @putch(i32)
declare void @putarray(i32, i32*)
declare void @memset(i32*, i32, i32)
@maxn = dso_local global i32 18

@mod = dso_local global i32 1000000007

@dp = dso_local global [13226976 x i32] zeroinitializer

@list = dso_local global [200 x i32] zeroinitializer


define dso_local i32 @equal(i32 %x1, i32 %x2){
    %x3 = alloca i32
    store i32 %x1, i32* %x3
    %x4 = alloca i32
    store i32 %x2, i32* %x4
    %x5 = load i32, i32* %x3
    %x6 = load i32, i32* %x4
    %x7 = icmp eq i32 %x5, %x6
    br i1 %x7 ,label %basic_block_1, label %basic_block_2
basic_block_1:
    ret i32 1
    br label %basic_block_2
basic_block_2:
    ret i32 0
}

define dso_local i32 @dfs(i32 %x1, i32 %x2, i32 %x3, i32 %x4, i32 %x5, i32 %x6){
    %x7 = alloca i32
    store i32 %x1, i32* %x7
    %x8 = alloca i32
    store i32 %x2, i32* %x8
    %x9 = alloca i32
    store i32 %x3, i32* %x9
    %x10 = alloca i32
    store i32 %x4, i32* %x10
    %x11 = alloca i32
    store i32 %x5, i32* %x11
    %x12 = alloca i32
    store i32 %x6, i32* %x12
    %x13 = load i32, i32* %x7
    %x14 = mul i32 %x13, 734832
    %x15 = add i32 %x14, 0
    %x16 = load i32, i32* %x8
    %x17 = mul i32 %x16, 40824
    %x18 = add i32 %x17, %x15
    %x19 = load i32, i32* %x9
    %x20 = mul i32 %x19, 2268
    %x21 = add i32 %x20, %x18
    %x22 = load i32, i32* %x10
    %x23 = mul i32 %x22, 126
    %x24 = add i32 %x23, %x21
    %x25 = load i32, i32* %x11
    %x26 = mul i32 %x25, 7
    %x27 = add i32 %x26, %x24
    %x28 = load i32, i32* %x12
    %x29 = mul i32 %x28, 1
    %x30 = add i32 %x29, %x27
    %x31 = getelementptr [13226976 x i32],[13226976 x i32]* @dp, i32 0, i32 %x30
    %x32 = load i32, i32* %x31
    %x33 = icmp ne i32 %x32, -1
    br i1 %x33 ,label %basic_block_3, label %basic_block_4
basic_block_3:
    %x34 = load i32, i32* %x7
    %x35 = mul i32 %x34, 734832
    %x36 = add i32 %x35, 0
    %x37 = load i32, i32* %x8
    %x38 = mul i32 %x37, 40824
    %x39 = add i32 %x38, %x36
    %x40 = load i32, i32* %x9
    %x41 = mul i32 %x40, 2268
    %x42 = add i32 %x41, %x39
    %x43 = load i32, i32* %x10
    %x44 = mul i32 %x43, 126
    %x45 = add i32 %x44, %x42
    %x46 = load i32, i32* %x11
    %x47 = mul i32 %x46, 7
    %x48 = add i32 %x47, %x45
    %x49 = load i32, i32* %x12
    %x50 = mul i32 %x49, 1
    %x51 = add i32 %x50, %x48
    %x52 = getelementptr [13226976 x i32],[13226976 x i32]* @dp, i32 0, i32 %x51
    %x53 = load i32, i32* %x52
    ret i32 %x53
    br label %basic_block_4
basic_block_4:
    %x54 = load i32, i32* %x7
    %x55 = load i32, i32* %x8
    %x56 = add i32 %x54, %x55
    %x57 = load i32, i32* %x9
    %x58 = add i32 %x56, %x57
    %x59 = load i32, i32* %x10
    %x60 = add i32 %x58, %x59
    %x61 = load i32, i32* %x11
    %x62 = add i32 %x60, %x61
    %x63 = icmp eq i32 %x62, 0
    br i1 %x63 ,label %basic_block_5, label %basic_block_6
basic_block_5:
    ret i32 1
    br label %basic_block_6
basic_block_6:
    %x64 = alloca i32
    store i32 0, i32* %x64
    %x65 = load i32, i32* %x7
    %x66 = icmp ne i32 %x65, 0
    br i1 %x66 ,label %basic_block_7, label %basic_block_8
basic_block_7:
    %x67 = load i32, i32* %x64
    %x68 = load i32, i32* %x7
    %x69 = load i32, i32* %x12
    %x70 = call i32 @equal(i32 %x69, i32 2)
    %x71 = sub i32 %x68, %x70
    %x72 = load i32, i32* %x7
    %x73 = sub i32 %x72, 1
    %x74 = load i32, i32* %x8
    %x75 = load i32, i32* %x9
    %x76 = load i32, i32* %x10
    %x77 = load i32, i32* %x11
    %x78 = call i32 @dfs(i32 %x73, i32 %x74, i32 %x75, i32 %x76, i32 %x77, i32 1)
    %x79 = mul i32 %x71, %x78
    %x80 = add i32 %x67, %x79
    %x81 = load i32, i32* @mod
    %x82 = srem i32 %x80, %x81
    store i32 %x82, i32* %x64
    br label %basic_block_8
basic_block_8:
    %x83 = load i32, i32* %x8
    %x84 = icmp ne i32 %x83, 0
    br i1 %x84 ,label %basic_block_9, label %basic_block_10
basic_block_9:
    %x85 = load i32, i32* %x64
    %x86 = load i32, i32* %x8
    %x87 = load i32, i32* %x12
    %x88 = call i32 @equal(i32 %x87, i32 3)
    %x89 = sub i32 %x86, %x88
    %x90 = load i32, i32* %x7
    %x91 = add i32 %x90, 1
    %x92 = load i32, i32* %x8
    %x93 = sub i32 %x92, 1
    %x94 = load i32, i32* %x9
    %x95 = load i32, i32* %x10
    %x96 = load i32, i32* %x11
    %x97 = call i32 @dfs(i32 %x91, i32 %x93, i32 %x94, i32 %x95, i32 %x96, i32 2)
    %x98 = mul i32 %x89, %x97
    %x99 = add i32 %x85, %x98
    %x100 = load i32, i32* @mod
    %x101 = srem i32 %x99, %x100
    store i32 %x101, i32* %x64
    br label %basic_block_10
basic_block_10:
    %x102 = load i32, i32* %x9
    %x103 = icmp ne i32 %x102, 0
    br i1 %x103 ,label %basic_block_11, label %basic_block_12
basic_block_11:
    %x104 = load i32, i32* %x64
    %x105 = load i32, i32* %x9
    %x106 = load i32, i32* %x12
    %x107 = call i32 @equal(i32 %x106, i32 4)
    %x108 = sub i32 %x105, %x107
    %x109 = load i32, i32* %x7
    %x110 = load i32, i32* %x8
    %x111 = add i32 %x110, 1
    %x112 = load i32, i32* %x9
    %x113 = sub i32 %x112, 1
    %x114 = load i32, i32* %x10
    %x115 = load i32, i32* %x11
    %x116 = call i32 @dfs(i32 %x109, i32 %x111, i32 %x113, i32 %x114, i32 %x115, i32 3)
    %x117 = mul i32 %x108, %x116
    %x118 = add i32 %x104, %x117
    %x119 = load i32, i32* @mod
    %x120 = srem i32 %x118, %x119
    store i32 %x120, i32* %x64
    br label %basic_block_12
basic_block_12:
    %x121 = load i32, i32* %x10
    %x122 = icmp ne i32 %x121, 0
    br i1 %x122 ,label %basic_block_13, label %basic_block_14
basic_block_13:
    %x123 = load i32, i32* %x64
    %x124 = load i32, i32* %x10
    %x125 = load i32, i32* %x12
    %x126 = call i32 @equal(i32 %x125, i32 5)
    %x127 = sub i32 %x124, %x126
    %x128 = load i32, i32* %x7
    %x129 = load i32, i32* %x8
    %x130 = load i32, i32* %x9
    %x131 = add i32 %x130, 1
    %x132 = load i32, i32* %x10
    %x133 = sub i32 %x132, 1
    %x134 = load i32, i32* %x11
    %x135 = call i32 @dfs(i32 %x128, i32 %x129, i32 %x131, i32 %x133, i32 %x134, i32 4)
    %x136 = mul i32 %x127, %x135
    %x137 = add i32 %x123, %x136
    %x138 = load i32, i32* @mod
    %x139 = srem i32 %x137, %x138
    store i32 %x139, i32* %x64
    br label %basic_block_14
basic_block_14:
    %x140 = load i32, i32* %x11
    %x141 = icmp ne i32 %x140, 0
    br i1 %x141 ,label %basic_block_15, label %basic_block_16
basic_block_15:
    %x142 = load i32, i32* %x64
    %x143 = load i32, i32* %x11
    %x144 = load i32, i32* %x7
    %x145 = load i32, i32* %x8
    %x146 = load i32, i32* %x9
    %x147 = load i32, i32* %x10
    %x148 = add i32 %x147, 1
    %x149 = load i32, i32* %x11
    %x150 = sub i32 %x149, 1
    %x151 = call i32 @dfs(i32 %x144, i32 %x145, i32 %x146, i32 %x148, i32 %x150, i32 5)
    %x152 = mul i32 %x143, %x151
    %x153 = add i32 %x142, %x152
    %x154 = load i32, i32* @mod
    %x155 = srem i32 %x153, %x154
    store i32 %x155, i32* %x64
    br label %basic_block_16
basic_block_16:
    %x156 = load i32, i32* %x7
    %x157 = mul i32 %x156, 734832
    %x158 = add i32 %x157, 0
    %x159 = load i32, i32* %x8
    %x160 = mul i32 %x159, 40824
    %x161 = add i32 %x160, %x158
    %x162 = load i32, i32* %x9
    %x163 = mul i32 %x162, 2268
    %x164 = add i32 %x163, %x161
    %x165 = load i32, i32* %x10
    %x166 = mul i32 %x165, 126
    %x167 = add i32 %x166, %x164
    %x168 = load i32, i32* %x11
    %x169 = mul i32 %x168, 7
    %x170 = add i32 %x169, %x167
    %x171 = load i32, i32* %x12
    %x172 = mul i32 %x171, 1
    %x173 = add i32 %x172, %x170
    %x174 = getelementptr [13226976 x i32],[13226976 x i32]* @dp, i32 0, i32 %x173
    %x175 = load i32, i32* %x64
    %x176 = load i32, i32* @mod
    %x177 = srem i32 %x175, %x176
    store i32 %x177, i32* %x174
    %x178 = load i32, i32* %x7
    %x179 = mul i32 %x178, 734832
    %x180 = add i32 %x179, 0
    %x181 = load i32, i32* %x8
    %x182 = mul i32 %x181, 40824
    %x183 = add i32 %x182, %x180
    %x184 = load i32, i32* %x9
    %x185 = mul i32 %x184, 2268
    %x186 = add i32 %x185, %x183
    %x187 = load i32, i32* %x10
    %x188 = mul i32 %x187, 126
    %x189 = add i32 %x188, %x186
    %x190 = load i32, i32* %x11
    %x191 = mul i32 %x190, 7
    %x192 = add i32 %x191, %x189
    %x193 = load i32, i32* %x12
    %x194 = mul i32 %x193, 1
    %x195 = add i32 %x194, %x192
    %x196 = getelementptr [13226976 x i32],[13226976 x i32]* @dp, i32 0, i32 %x195
    %x197 = load i32, i32* %x196
    ret i32 %x197
}
@cns = dso_local global [20 x i32] zeroinitializer


define dso_local i32 @main(){
    %x1 = alloca i32
    %x2 = call i32 @getint()
    store i32 %x2, i32* %x1
    %x3 = alloca i32
    store i32 0, i32* %x3
    br label %basic_block_17
basic_block_17:
    %x4 = load i32, i32* %x3
    %x5 = load i32, i32* @maxn
    %x6 = icmp slt i32 %x4, %x5
    %x7 = zext i1 %x6 to i32
    %x8 = icmp ne i32 %x7, 0
    br i1 %x8 ,label %basic_block_18, label %basic_block_19
basic_block_18:
    %x9 = alloca i32
    store i32 0, i32* %x9
    br label %basic_block_20
basic_block_20:
    %x10 = load i32, i32* %x9
    %x11 = load i32, i32* @maxn
    %x12 = icmp slt i32 %x10, %x11
    %x13 = zext i1 %x12 to i32
    %x14 = icmp ne i32 %x13, 0
    br i1 %x14 ,label %basic_block_21, label %basic_block_22
basic_block_21:
    %x15 = alloca i32
    store i32 0, i32* %x15
    br label %basic_block_23
basic_block_23:
    %x16 = load i32, i32* %x15
    %x17 = load i32, i32* @maxn
    %x18 = icmp slt i32 %x16, %x17
    %x19 = zext i1 %x18 to i32
    %x20 = icmp ne i32 %x19, 0
    br i1 %x20 ,label %basic_block_24, label %basic_block_25
basic_block_24:
    %x21 = alloca i32
    store i32 0, i32* %x21
    br label %basic_block_26
basic_block_26:
    %x22 = load i32, i32* %x21
    %x23 = load i32, i32* @maxn
    %x24 = icmp slt i32 %x22, %x23
    %x25 = zext i1 %x24 to i32
    %x26 = icmp ne i32 %x25, 0
    br i1 %x26 ,label %basic_block_27, label %basic_block_28
basic_block_27:
    %x27 = alloca i32
    store i32 0, i32* %x27
    br label %basic_block_29
basic_block_29:
    %x28 = load i32, i32* %x27
    %x29 = load i32, i32* @maxn
    %x30 = icmp slt i32 %x28, %x29
    %x31 = zext i1 %x30 to i32
    %x32 = icmp ne i32 %x31, 0
    br i1 %x32 ,label %basic_block_30, label %basic_block_31
basic_block_30:
    %x33 = alloca i32
    store i32 0, i32* %x33
    br label %basic_block_32
basic_block_32:
    %x34 = load i32, i32* %x33
    %x35 = icmp slt i32 %x34, 7
    %x36 = zext i1 %x35 to i32
    %x37 = icmp ne i32 %x36, 0
    br i1 %x37 ,label %basic_block_33, label %basic_block_34
basic_block_33:
    %x38 = load i32, i32* %x3
    %x39 = mul i32 %x38, 734832
    %x40 = add i32 %x39, 0
    %x41 = load i32, i32* %x9
    %x42 = mul i32 %x41, 40824
    %x43 = add i32 %x42, %x40
    %x44 = load i32, i32* %x15
    %x45 = mul i32 %x44, 2268
    %x46 = add i32 %x45, %x43
    %x47 = load i32, i32* %x21
    %x48 = mul i32 %x47, 126
    %x49 = add i32 %x48, %x46
    %x50 = load i32, i32* %x27
    %x51 = mul i32 %x50, 7
    %x52 = add i32 %x51, %x49
    %x53 = load i32, i32* %x33
    %x54 = mul i32 %x53, 1
    %x55 = add i32 %x54, %x52
    %x56 = getelementptr [13226976 x i32],[13226976 x i32]* @dp, i32 0, i32 %x55
    store i32 -1, i32* %x56
    %x57 = load i32, i32* %x33
    %x58 = add i32 %x57, 1
    store i32 %x58, i32* %x33
    br label %basic_block_32
basic_block_34:
    %x59 = load i32, i32* %x27
    %x60 = add i32 %x59, 1
    store i32 %x60, i32* %x27
    br label %basic_block_29
basic_block_31:
    %x61 = load i32, i32* %x21
    %x62 = add i32 %x61, 1
    store i32 %x62, i32* %x21
    br label %basic_block_26
basic_block_28:
    %x63 = load i32, i32* %x15
    %x64 = add i32 %x63, 1
    store i32 %x64, i32* %x15
    br label %basic_block_23
basic_block_25:
    %x65 = load i32, i32* %x9
    %x66 = add i32 %x65, 1
    store i32 %x66, i32* %x9
    br label %basic_block_20
basic_block_22:
    %x67 = load i32, i32* %x3
    %x68 = add i32 %x67, 1
    store i32 %x68, i32* %x3
    br label %basic_block_17
basic_block_19:
    store i32 0, i32* %x3
    br label %basic_block_35
basic_block_35:
    %x69 = load i32, i32* %x3
    %x70 = load i32, i32* %x1
    %x71 = icmp slt i32 %x69, %x70
    %x72 = zext i1 %x71 to i32
    %x73 = icmp ne i32 %x72, 0
    br i1 %x73 ,label %basic_block_36, label %basic_block_37
basic_block_36:
    %x74 = load i32, i32* %x3
    %x75 = mul i32 %x74, 1
    %x76 = add i32 %x75, 0
    %x77 = getelementptr [200 x i32],[200 x i32]* @list, i32 0, i32 %x76
    %x78 = call i32 @getint()
    store i32 %x78, i32* %x77
    %x79 = load i32, i32* %x3
    %x80 = mul i32 %x79, 1
    %x81 = add i32 %x80, 0
    %x82 = getelementptr [200 x i32],[200 x i32]* @list, i32 0, i32 %x81
    %x83 = load i32, i32* %x82
    %x84 = mul i32 %x83, 1
    %x85 = add i32 %x84, 0
    %x86 = getelementptr [20 x i32],[20 x i32]* @cns, i32 0, i32 %x85
    %x87 = load i32, i32* %x3
    %x88 = mul i32 %x87, 1
    %x89 = add i32 %x88, 0
    %x90 = getelementptr [200 x i32],[200 x i32]* @list, i32 0, i32 %x89
    %x91 = load i32, i32* %x90
    %x92 = mul i32 %x91, 1
    %x93 = add i32 %x92, 0
    %x94 = getelementptr [20 x i32],[20 x i32]* @cns, i32 0, i32 %x93
    %x95 = load i32, i32* %x94
    %x96 = add i32 %x95, 1
    store i32 %x96, i32* %x86
    %x97 = load i32, i32* %x3
    %x98 = add i32 %x97, 1
    store i32 %x98, i32* %x3
    br label %basic_block_35
basic_block_37:
    %x99 = alloca i32
    %x100 = mul i32 1, 1
    %x101 = add i32 %x100, 0
    %x102 = getelementptr [20 x i32],[20 x i32]* @cns, i32 0, i32 %x101
    %x103 = load i32, i32* %x102
    %x104 = mul i32 2, 1
    %x105 = add i32 %x104, 0
    %x106 = getelementptr [20 x i32],[20 x i32]* @cns, i32 0, i32 %x105
    %x107 = load i32, i32* %x106
    %x108 = mul i32 3, 1
    %x109 = add i32 %x108, 0
    %x110 = getelementptr [20 x i32],[20 x i32]* @cns, i32 0, i32 %x109
    %x111 = load i32, i32* %x110
    %x112 = mul i32 4, 1
    %x113 = add i32 %x112, 0
    %x114 = getelementptr [20 x i32],[20 x i32]* @cns, i32 0, i32 %x113
    %x115 = load i32, i32* %x114
    %x116 = mul i32 5, 1
    %x117 = add i32 %x116, 0
    %x118 = getelementptr [20 x i32],[20 x i32]* @cns, i32 0, i32 %x117
    %x119 = load i32, i32* %x118
    %x120 = call i32 @dfs(i32 %x103, i32 %x107, i32 %x111, i32 %x115, i32 %x119, i32 0)
    store i32 %x120, i32* %x99
    %x121 = load i32, i32* %x99
    call void @putint(i32 %x121)
    ret i32 0
}
