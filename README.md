

# 编译原理课程设计 类C编译器设计与实现

#### 1950509 马家昱 2022.6.1

[toc]

## 需求分析

### 实验目的

1. 掌握使用高级程序语言实现一个一遍完成的、简单语言的编译器的方法。

2. 掌握简单的词法分析器、语法分析器、符号表管理、中间代码生成以及目标代码生成的实现方法。

3. 掌握将生成代码写入文件的技术

### 实验要求与完成情况

使用高级程序语言实现一个类C语言的编译器，可以提供词法分析、语法分析、符号表管理、中间代码生成以及目标代码生成等功能。具体要求如下：

1. **【已完成】**使用高级程序语言作为实现语言，实现一个类C语言的编译器。编码实现编译器的组成部分。

2. **【已完成】**要求的类C编译器是个一遍的编译程序，词法分析程序作为子程序，需要的时候被语法分析程序调用。

3. **【已完成】**使用语法制导的翻译技术，在语法分析的同时生成中间代码，并保存到文件中。

4. **【已完成】**要求输入类C语言源程序，输出中间代码表示的程序;

5. **【已完成】**要求输入类C语言源程序，输出目标代码（可汇编执行）的程序。

6. **【已完成】**实现过程、函数调用的代码编译。

7. **【已完成】**拓展类C语言文法，实现包含数组的中间代码以及目标代码生成。

**完成了任务书中的所有要求，包括函数调用与多维数组，最终生成MIPS架构汇编语言，通过任务书中测试代码。**

除此之外，**额外**实现的功能有：

1. 在语法中增加了**AND(&&)**和**OR(||)**运算符，使用**回填技术**实现**IF**和WHILE语句。

2. 实现了一定的**错误处理**，在词法分析与语法分析中提示错误位置。

3. 实现了**图形化**显示，设计了流畅使用的交互界面。

   <img src="image-20220606135132083.png" alt="image-20220606135132083" style="zoom:67%;" />

### 开发环境

**编译器采用C/C++实现使用工具如下**

* MinGW 32bit

* Qt Creator 4.11.0

**运行环境为 windows10**

**将源代码生成为编译器的编译命令如下：**

```shell
//词法分析器
lexical:lex.yy.c
gcc lex.yy.c -o lexical
lex.yy.c:l.l
lex l.l
clean:
rm lex.yy.c

//语法分析器与中间代码生成器
parse intermediate:lex.yy.c y.tab.c y.tab.h
gcc lex.yy.c y.tab.c -o parse
gcc lex.yy.c y.tab.c -o intermediate
lex.yy.c:l.l
lex l.l
y.tab.c:y.y
yacc y.y -d -v --debug
clean:
rm lex.yy.c y.tab.c y.tab.h a

//目标代码生成器
GenMips:GenMips.cpp GenMips.h
g++ GenMips.cpp -o GenMips
```

**使用时，执行/program/myCompiler.exe即可**

## 系统设计

![image-20220606141533696](image-20220606141533696.png)

**编译器采用模块化设计，由总程序对子程序进行调用，子程序输出至文件或其他子程序**：

* **词法分析**：将源代码转换成token流，将其值与类型输入至文件中，或作为语法分析器的输入
* **语法分析**：使用**LL1文法**对token流进行规约，可以直接将结果输入文件，或结合语义动作生成四元式
* **语义分析**：在文法规约时调用语义动作，传递值或地址，生成四元式
* **目标代码生成器**：读入四元式，生成MIPS汇编代码

## 具体实现

### 词法语法分析

**词法分析**采用正则表达式进行匹配，如图所示：

```c
    line_comment	(\/\/.*\n) 
    %x BLOCK_COMMENT 
    letter	[A-Za-z_] 
    digit	[0-9]
    identifier	{letter}({letter}|{digit})* 
    reserveword	"int"|"void"|"if"|"else"|"while"|"return"
    preprocess	^#.*\n	
    integer	{digit}+	
    operater	"*"|"("|")"|"-"|"+"|"="|"=="|"{"|"}"|";"|"<"|">"|","|"/"|"!="|"<="|">="|"["|"]"
    wrong	{integer}{letter}+
    end	^#(\n)*
```

**文法分析**使用LL1文法实现，为了实现**函数调用与数组**，且与语义动作相结合，对任务书中给出的文法进行改造，共有60条规则：

```c
    0 $accept: startSym $end

    1 startSym: program
    2         | startSym program
    3         | error

    4 program: type ID_ '(' args ')' block

    5 ID_: ID

    6 args: arg_dec ',' args
    7     | arg_dec
    8     | VOID
    9     | /* empty */

   10 arg_dec: type ID

   11 block: '{' decls statementlist '}'

   12 decls: decls decl
   13      | /* empty */

   14 decl: type ID ';'
   15     | adecl
   16     | dadecl

   17 adecl: type ID '[' INTEGER ']' ';'

   18 dadecl: type ID '[' INTEGER ']' '[' INTEGER ']' ';'

   19 type: INT
   20     | VOID

   21 statementlist: statementlist M statement
   22              | statement

   23 statement: IF '(' boolean ')' M statement ELSE N M statement
   24          | IF '(' boolean ')' M statement
   25          | WHILE M '(' boolean ')' M statement
   26          | assignment ';'
   27          | '{' statementlist '}'
   28          | RETURN expression ';'
   29          | RETURN ';'

   30 assignment: ID '=' boolean
   31           | array '=' boolean
   32           | darray '=' boolean

   33 boolean: boolean OR M boolean
   34        | boolean AND M boolean
   35        | '!' boolean
   36        | '(' boolean ')'
   37        | expression REL expression
   38        | TRUE
   39        | FALSE
   40        | expression

   41 M: /* empty */

   42 N: /* empty */

   43 expression: '(' expression ')'
   44           | expression '+' expression
   45           | expression '-' expression
   46           | expression '*' expression
   47           | expression '/' expression
   48           | '-' expression
   49           | loc
   50           | INTEGER
   51           | callprogram

   52 callprogram: ID '(' expressions ')'

   53 expressions: expressions ',' expression
   54            | expression
   55            | /* empty */

   56 loc: ID
   57    | array
   58    | darray

   59 array: ID '[' INTEGER ']'

   60 darray: ID '[' INTEGER ']' '[' INTEGER ']'
```

### 中间代码生成

由于增加了AND与OR运算符，需采用回填技术，记录真链假链，节点结构设计如下：

```c
typedef struct node
{
    instrlist *truelist, *falselist, *nextlist;//回填用到的链表
    char addr[256];//地址
    char lexeme[256];//值
    char oper[3];
    int instr;
};
```

以if-else语句为例：statement: IF '(' boolean ')' M statement ELSE N M statement 

语义动作如下

```c
{
    backpatch(list, $3.truelist, $5.instr);
    backpatch(list, $3.falselist, $9.instr);
    $6.nextlist = merge($6.nextlist, $8.nextlist);
    $$.nextlist = merge($6.nextlist, $10.nextlist);
}
```

回填与合并操作如下：

```c
int backpatch(codelist *dst, instrlist *list, int instrno)
{
    if (list!=NULL)
    {
        listele *p=list->first;
        char tmp[20];
        sprintf(tmp, ", %d)", instrno);
        while (p!=NULL)
        {
            if (p->instrno<dst->linecnt)
            strcat(dst->code[p->instrno], tmp);
            p=p->next;
        }
    }
    return 0;
}
instrlist* merge(instrlist *list1, instrlist *list2)
{
    instrlist *p;
    if (list1 == NULL)
    	p = list2;
    else
    {
        if (list2!=NULL)
        {
        if (list1->last == NULL)
        {
            list1->first = list2->first;
            list1->last =list2->last;
            }else list1->last->next = list2->first;
            list2->first = list2->last = NULL;
            free(list2);
        }
        p = list1;
    }
    return p;
}
```

#### 函数调用

函数调用也使用了类似回填的操作：

1. 在四元式第一行生成跳转至main函数的命令，但不填写具体地址。
2. 在规约至函数名为main的操作时，回填地址。

```c
strcpy(fun_name,$1.lexeme);
gen_custmise(list,$1.lexeme,"_","_","_");
if(!strcmp($1.lexeme,"main"))
{
    sprintf(tmp,"(j, _, _, %d)",list->linecnt-1);
    strcpy(list->code[0],tmp);
}
```

在声明函数时，同样对其形式参数进行声明：

![image-20220606143814130](image-20220606143814130.png)

函数调用时生成的四元式：

**(call, function_name, _, 返回值地址)**

```c
callprogram:
	ID '(' expressions ')'
	{
		new_temp(&$$, get_temp_index(list));
		gen_custmise(list,"call",$1.lexeme,"_",$$.addr);
	}
```

函数返回时生成的四元式：

**(return, 返回值地址, _, function_name)**

```c
RETURN expression ';'	
	{
		gen_custmise(list,"return",$2.addr,"_",fun_name);
	}
	|
	RETURN ';'
	{
		gen_custmise(list,"return","_","_",fun_name);
	}
```

#### 数组实现

数组调用时，首先根据其维度生成临时变量计算结果，计算规则如下：

* **一维数组**：i*4
* **二维数组：i*一维长度+j\*4**
* **……**

```c
array:
	ID '[' INTEGER ']'
	{
		new_temp(&$$, get_temp_index(list)); 
		gen_custmise(list, "*", $3.lexeme,"4", $$.addr);
		strcpy($$.lexeme,$1.lexeme);
	}

darray:
	ID '[' INTEGER ']' '[' INTEGER ']'
	{
		sprintf(temp, "t%d", get_temp_index(list));
		gen_custmise(list, "*", $3.lexeme,"8", temp);
		sprintf(temp1, "t%d", get_temp_index(list));
		gen_custmise(list, "*", $6.lexeme,"4", temp1);
		new_temp(&$$, get_temp_index(list)); 
		gen_custmise(list, "+", temp,temp1, $$.addr);
		strcpy($$.lexeme,$1.lexeme);
	}
```

对数组进行取址时的四元式：**(=[], 数组首地址, 偏移量, 临时变量)**

对数组进行赋值时的四元式：**([]=, 右值地址, 偏移量, 数组首地址)**

### MIPS汇编生成

首先读取四元式，四元式结构如下：

```c++
struct Quaternion
{
	int index;//标号
	string operator;//操作类型
	string arg1;//操作数1
	string arg2;//操作数2
	string result;//结果
};
```

MIPS架构CPU共有32个寄存器，分别是

```shell
"$at", //$1 保留给汇编器
"$v0","$v1", //$2-$3 函数调用返回值
"$a0","$a1","$a2","$a3", //$4-$7 函数调用参数
"$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7", //$8-$15 临时使用
"$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7", //$16-$23 需保存
"$t8","$t9", //$24-$25 临时使用
"$k0","k1",//操作系统／异常处理保留
"$gp", //$28 全局指针
"$sp", //$29 堆栈指针
"$fp", //$30 帧指针
"$ra"//$31 返回地址
```

使用两个变量分别记录：

* Rvalue某个寄存器中存储的变量
* Avalue某个变量所在的寄存器堆

在开始分析四元式前，首先需要先为\$sp赋值esp，为$fp赋值ebp

```assembly
addi $sp,$sp,268697600
addi $fp,$fp,268697596
```

开始分析四元式，主要分为以下几种情况：

* 函数调用与返回（call、return）
  * 创建栈帧并跳转，分配寄存器
  * 销毁栈帧并跳转，返回值放在v1寄存器中
* 赋值（=）
  * 立即数赋值
  * 变量赋值
* 形参与实参声明（defpar、param）
  * 实参变量压栈
  * 为形参分配空间
* 跳转与计算（立即数或寄存器）（j、j>、j>=……）
  * 执行相应命令

在执行上述过程时，需要不断判断变量值是否已经在内存中（通过Rvalue与Avalue），若变量不在内存中，需要执行lw命令移入空闲寄存器。

分配寄存器时，使用到了LRU算法，保存每个寄存器未使用的时间，使用最久未被使用的临时寄存器。

## 执行界面与运行结果

使用任务书中的测试代码：

选择文件读入testcode.c

词法分析，输出结果与统计

<img src="image-20220606151321998.png" alt="image-20220606151321998" style="zoom:67%;" />

文法分析，输出代码结构

<img src="image-20220606151346020.png" alt="image-20220606151346020" style="zoom:67%;" />

四元式

<img src="image-20220606151353241.png" alt="image-20220606151353241" style="zoom:67%;" />

汇编

<img src="image-20220606151425445.png" alt="image-20220606151425445" style="zoom:67%;" />

错误提示

<img src="image-20220606151246358.png" alt="image-20220606151246358" style="zoom:67%;" />

## 遇到问题与心得体会

本次课设中遇到的难点主要为函数调用和多维数组的四元式生成，以及汇编代码生成中寄存器分配的问题。对于前者，通过对上学期编译原理课程进行复习，重新学习函数调用与数组调用的语义动作，完成了整个函数调用以及数组调用的过程。对于在生成目标代码时出现的问题，主要参考了网上的资料，动手实现了LRU算法对寄存器进行分配，同时复习了操作系统知识，模拟了函数调用时栈帧的实现过程。

通过本次课程设计，对编译原理这一课程有了融会贯通的理解，即掌握了从词法分析到目标代码生成的全过程。与此同时巩固了相关课程的知识，如汇编代码、操作系统、计算机组成原理等，对整个计算机知识体系有了更加全面的理解。在此次课设中遇到的许多困难，通过参考其他资料进行解决，无论是对漏洞进行修改和对方法进行重新设计，都极大的帮助个人代码能力的提升。