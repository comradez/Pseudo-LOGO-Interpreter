# 程序设计基础大作业-C++ 实现 LOGO 解释器

## 小组成员

软件93 徐文博

软件91 刘明旸

软件92 张楚炎

## 运行环境

MinGW GCC/G++ 4.8.1

Python 3.6.0+

​	opencv-python 4.1.1.26

Windows10 (1909)

## 小组分工

徐文博：LOGO 伪代码的逻辑处理，包括了循环、变量与函数定义、语法检查等操作

刘明旸：LOGO 编辑器 GUI 创建、必做任务及自定义任务的logo代码、LOGO 语言的公式支持（$\log,\ \sin\ ,\arcsin$，四则运算支持括号等）

张楚炎：LOGO 语言的基础操作函数（`MOVE`、`PAINT`、`TURN`等的 C++ 实现）、文档撰写与视频制作

（徐文博和张楚炎的代码及刘明旸的 expression.cpp 在一起编译，而刘明旸的代码主要基于 Python，与编译后的 exe 文件交互）

## 语法说明

（对原伪 LOGO 的语法进行了少许修改，请务必仔细阅读）

注：下文简单的把变量看作 C++ 中的左值，表达式的值、字面量看作 C++ 中的右值，于是 `[variable]` 表示一个左值，`[expression]` 表示一个右值

注：允许海龟走出图片像素范围，但此时不会在图片上留下任何轨迹

### 值类型

| 类型        | 对应 C++ 实现 |
| :---------- | :------------ |
| `BOOLEAN`   | `bool`        |
| `CHARACTER` | `char`        |
| `FRACTION`  | `double`      |
| `INTEGER`   | `long long`   |

- 任意时刻，变量和表达式的值有确定的类型，为上述四种之一
- 变量的类型是动态的，进行赋值时变量的类型会变为表达式的值的类型
- 表达式的值的类型为参与运算的值的类型中精度最高的类型，精度大小关系：`BOOLEAN`<`CHARACTER`<`INTEGER`<`FRACTION`
  - `BOOLEAN` 类型参与运算时只有 0 和 1 两种值，故而有 `TRUE` 和 `TRUE` 相加得 `TRUE`、相减得 `FALSE` 等，但不建议这样使用

#### 字面量

| 类型        | 说明                                                         |
| ----------- | ------------------------------------------------------------ |
| `BOOLEAN`   | 只有 `TRUE` 和 `FALSE` 两种                                  |
| `CHARACTER` | 形如 `'x'`，`x` 为单个字符或[转义序列](https://zh.cppreference.com/w/cpp/language/escape "标准参照 cppreference") |
| `FRACTION`  | 形如 `1.234`、`.0` 等，需要包含 `.` 以显式区别于 `INTEGER` 类型字面量 |
| `INTEGER`   | 形如 `1234`、`01` 等不含小数点的可以以 `-` 开头的数字串      |

- 超出对应类型表示范围的字面量的值是未定义的

### 类型转换

- `[type] [variable]`
  - 将变量 `[variable]` 的类型转变为 `[type]`，`[type]` 为前文四种类型之一
  - 变量转换的 C++ 实现即为强制类型转换，故效果与 C++ 中的完全相同

### 变量声明与赋值

+ `DEF [variable] [expression]`
  + 声明一个名称为 `[variable]` 的变量，赋值为 `[expression]`
  + 变量名称由小写字母组成，支持作用域屏蔽（相应地，变量也具有生命周期和作用域）
+ `SET [variable] [expression]`

  + 将变量 `[variable]` 赋值为 `[expression]`

### 运算符

- 一元运算：`[operator] [expression] [variable]`
  - 将变量 `[variable]` 的值赋为表达式 `[operator] [expression]` 的值
  - 目前只有 `NOT` 是一元运算符，其它均为二元运算符

- 二元运算：`[operator] [expression1] [expression2] [variable] `
  - 将变量 `[variable]` 的值赋为表达式 `[expression1] [operator] [expression2]` 的值

#### 算术运算符

- `[operator]` 为以下几种之一：`ADD`、`SUB`、`MUL`、`DIV`、`MOD`，分别代表 C++ 中的：`+`、`-`、`*`、`/`、`%`
- `DIV` 和 `MOD` 运算中 `[expression2]` 的值不可以为 0（解释器会进行报错）
- `FRACTION` 类型参与的 `MOD` 运算效果相当于 C++ 中的 `std::fmod`

#### 位运算符

- `[operator]` 为以下几种之一：`AND`、`OR`、`XOR`、`NOT`，分别代表 C++ 中的：`&`、`|`、`^`、`~`
- `FRACTION` 类型不允许参与位运算（解释器会进行报错）
- 只有 `BOOLEAN` 类型参与时，运算符等效于逻辑运算中的：与、或、异或、非

#### 关系运算符

+ `[operator]` 为以下几种之一：`EQ`、`NEQ`、`LT`、`LEQ`、`GT`、`GEQ`，分别代表 C++ 中的：`==`、`!=`、`<`、`<=`、`>`、`>=`
  + 运算符名称来源为 $\LaTeX$，即：equal、not equal、less than、less or equal、greater than、greater or equal
+ 表达式的值的类型永远为 `BOOLEAN`，不同类型间的关系运算规则等同于对应 C++ 实现

#### 表达式

- `{...}` 表示一个表达式
  - `...` 表示一个 C++ 风格的表达式，支持 `+`、`-`、`*`、`/`、`%`、`&`、`|`、`^`，以及函数 `std::sin(double)`、`std::cos(double)`、`std::log(double)`、`std::tan(double)`、`std::exp(double)`、`std::asin(double)`、`std::acos(double)`、`std::atan(double)`、`std::atan2(double)`、`rand()`（使用 `std::mt19937_64` 及 `uniform_int_distribution<int>` 实现 ）
  - 由于实现问题，表达式的类型一定为 `[FRACTION]`
  - 目前错误检查功能不够完善

### 文件头

- 合法的 logo 代码前四行必须为如下格式：

  ```logo
  @SIZE [w] [h] 
  @BACKGROUND [r] [g] [b] 
  @POSITION [x] [y]
  @FRAME [z]
  ```

  - `[...]` 表示一个整数字面量
  - `[w]` 和 `[h]` 代表图片大小（宽 × 高）（单位为像素），最大支持 1920×1080
  - `[r]`、`[g]`、`[b]` 为 0~255 的整数字面量，表示初始背景颜色
  - `[x]`、`[y]` 表示海龟的初始位置
  - `[z]` 表示帧率，大小代表每修改 `[z]` 个像素点输出一帧，为 0 则不输出中间帧

- 其它参数的初始设定

  - 海龟颜色初始为 (0,0,0)
  - 海龟轨迹粗细初始为 1 像素
  - 海龟朝向初始为 y 轴正方向

### 基本操作

+ `CLOAK`
  + 将海龟的可视性改为否
+ `COLOR [r] [g] [b]`
  + 将海龟的颜色修改为 RGB 通道 (`[r]`,`[g]`,`[b]`)，并将海龟的可视性修改为是
  + 隐式地将 `[r]`、`[g]`、`[b]` 的值的类型转为 `INTEGER`，并将值 $\bmod{256}$
+ `MOVE [step]`
  + 海龟以当前方向前进 `[step]` 个像素
  + 隐式地将类型转为 `INTEGER`
+ `TURN [angle]`
  + 海龟顺时针方向旋转 `[angle]` 度
  + 隐式地将类型转为 `FRACTION`

#### 附加功能

+ `PAINT`
  + 将与海龟目前所在的像素同色且联通的所有像素染色为海龟的颜色，类似画图工具中的油漆桶
+ `CIRCLE [direction] [radius]`

  + 从海龟当前所在点画一个半径为 `[radius]` 的圆，`[direction]` 标定旋转方向，0 和 1 分别代表逆时针和顺时针
  + 隐式地将 `[direction]` 的值的类型转为 `INTEGER`，`[radius]` 的值的类型转为 `FRACTION`
  + `[direction]` 不为 0 或 1 是未定义行为
+ `ARC [direction] [radius] [angle]`
  + 从海龟当前所在点画一段半径为 `[radius]`，圆心角为 `[angle]` 度的圆弧，`[direction]` 标定旋转方向，0 和 1 分别代表逆时针和顺时针
  + 隐式地将 `[direction]` 的值的类型转为 `INTEGER`，`[radius]` 和 `[angle]` 的值的类型转为 `FRACTION`
  + `[direction]` 不为 0 或 1 是未定义行为
+ `TREE [number] [trunk_length] [angle] [delta] [depth]`
  + 从海龟当前所在点出发绘制一棵分形树，`[number]` 表示每层树枝数目，`[trunk_length]` 表示树干长度，`[angle]` 表示每层最外侧的树枝之间的夹角，`[delta]` 表示下一层树干长度与本层树干长度的比值，`[depth]` 表示树的深度
  + 隐式地将 `[number]` 和 `[depth]` 的值的类型转为 `[INTEGER]`，`[trunk_length]`、`[angle]`、`[delta]` 的值的类型转为 `[FRACTION]`
+ `POS [x] [y]`
  + 将海龟的位置设为 (`[x]`,`[y]`)，过程中不产生轨迹
  + 隐式地将 `[x]` 和 `[y]` 的值的类型转为 `[FRACTION]`
+ `ARG [angle]`
  + 将海龟的方向角设为 `[angle]`（相对于x轴正向，顺时针方向）
  + 隐式地将 `[angle]` 的值的类型转为 `[FRACTION]`
+ `THICK [expression]`
  - 将乌龟的轨迹宽度像素值设置为 `[expression]`，`[expression]` 为 0 时相当于不修改，`[expression]` 为负数时取绝对值
  - 隐式地将 `[expression]` 的值的类型转为 `[INTEGER]`

### 控制语句

#### 循环

- `CONTINUE`

  + 直接从循环体开始处执行下一次循环

- `BREAK`

  - 推出当前循环体，从循环体结束的下一行继续执行

- `WHILE` 循环

  ```logo
  WHILE [expression]
  	...
  END WHILE
  ```

  - 当 `[expression]` 为真（隐式转换为 `BOOLEAN`）时执行 `WHILE` 到 `END WHILE` 之间的代码，每次执行完成后进行一次检查

- `REPEAT` 循环

  ```logo
  REPEAT [expression]
  	...
  END REPEAT
  ```

  - 执行 `REPEAT` 到 `END REPEAT` 之间的代码 `[expression]` 次（隐式转换为 `INTEGER` ）
  - `REPEAT` 循环次数在首次进入循环时确定，如果 `[expression]` 为变量，且值在 `REPEAT` 循环内部被修改，`REPEAT` 循环的次数不受影响

#### 函数

##### 定义

```logo
FUNC [Function]([parameter1],[parameter2],...)
	...
END FUNC
```

+ 定义一个名称为 `[Function]` 的函数，形参为 `[parameter1],[parameter2],...`
+ 函数名称必须首字母大写，其余字母小写
+ `RETURN [expression]`
+ 函数退出并返回`[expression]`，无返回值则在 `[expression]` 处写 `VOID`
  + 若函数体末尾隐含了一个 `RETURN VOID`，故函数可以无 `RETURN` 语句

##### 调用

+ `CALL [Function] ([argument1],[argument2],...) [variable]`
+ 调用函数 `Function` ，实参为`[argument1],[argument2],...`，并将返回值赋给 `[variable]`，若不打算赋值则在 `[variable]` 处写 `VOID`
    + 若函数本身返回 `VOID` 则对 `[variable]` 无任何影响，即此时将 `[variable]` 赋值为 `VOID` 是合法的，但无任何效果

#### 条件语句

+ `IF` 判断语句
  
  ```logo
  IF [expression]
  	...
  ELSE
  	...
  END IF
  ```
  
  + 如果 `[expression]` 为 `TRUE` （隐式的转换为 `BOOLEAN`）则执行 `IF` 到 `ELSE` 之间的代码，否则执行 `ELSE` 到 `END IF` 之间的代码
    + 其中 `ELSE` 语句为可选的

### 注释

+ 单行注释
  
  ```logo
  
  ```
### This is a comment.

  ```
  
  + 注释以 `#  ` 开头，后接至少一个空白字符，`#` 前不应当有任何非空白字符
  
  - 注释语句将被忽略，不会进行语法检查和执行

### 调试功能

该部分功能较为羸弱，但是对于调试 logo 代码，有时比 gdb 调试还是稍微方便一些

##### 监视语句

- `$ ...`
  - 以 `$` 开头，后接至少一个空白字符，`...` 为正常的一行 logo 代码（不包括文件头）
  - 表示每执行到该语句，会向代码所在目录的 debug.log 文件输出一次该行行号及代码，可用于确定 logo 代码的具体执行情况
  - 函数定义的起始语句 `FUNC [Function]([parameter1],[parameter2],...)` 以及查看变量语句 `$WATCH [variable]$` 将不会被输出，虽然可以监视

##### 查看变量

- `$WATCH [variable]`
  - 表示每执行到该行会向代码所在目录的 debug.log 文件输出一次该行行号、变量名称及对应值，`CHARACTER` 类型的不可见字符会以转义序列形式输出
  - 该语句的语法错误不会导致程序中途结束，但 debug.logo 文件会有相应错误信息
  ```



## 编辑器

1. 编辑器支持基础的保存、另存为、打开等文件读写操作
2. 编辑器支持语法**实时**高亮与代码一键**格式化**
3. 编辑器支持**自动缩进**与**代码补齐**