# lisp-like-interpreter
Interpreter development in C++ for LISP-like language:
- LISP-like program contains variable definitions, input/output instructions, conditional and loop statements and is limited to operations involving integer-type variables
- Interpreter operations:
  - Read a file containing the program to be interpreted; the syntax of the program is defined by an unambiguous context-free grammar
  - Execute the program contained in the source file
  - Ask through console for any data input provided by the input instructions and display on the console the result of expressions provided by the output instructions

<hr>

### USAGE
```
./lispInterpreter.exe [path to the file with lisp code]
```

<hr>

### GRAMMAR
**PROGRAM**
<pre>
program        → stmt_block 
stmt_block     → statement | ( **BLOCK** statement_list ) 
statement_list → statement statement_list | statement 
</pre>

**STATEMENT**
<pre>
statement → variable_def | io_stmt | cond_stmt | loop_stmt

variable_def → ( **SET** variable_id num_expr )
io_stmt      → ( **PRINT** num_expr ) | ( **INPUT** variable_id )
cond_stmt    → ( **IF** bool_expr stmt_block stmt_block )
loop_stmt    → ( **WHILE** bool_expr stmt_block )
</pre>

**NUMERICAL EXPRESSION**
<pre>
num_expr → ( **ADD** num_expr num_expr ) | ( **SUB** num_expr num_expr ) | ( **MUL** num_expr num_expr ) 
         | ( **DIV** num_expr num_expr ) | **number** | **variable_id**
</pre>

**BOOLEAN EXPRESSION**
<pre>
bool_expr → ( **LT** num_expr num_expr ) | ( **GT** num_expr num_expr ) | ( **EQ** num_expr num_expr ) 
          | ( **AND** bool_expr bool_expr ) | ( **OR** bool_expr bool_expr ) | ( **NOT** bool_expr ) | **TRUE** | **FALSE**
</pre>

**VARIABLE**
<pre>
variable_id → alpha_list
alpha_list  → alpha alpha_list | alpha
alpha       → **a** | **b** | **c** | . . . | **z** | **A** | **B** | **C** | . . . | **Z**
</pre>

**NUMBERS**
<pre>
number    → - posnumber | posnumber
posnumber → **0** | sigdigit rest
sigdigit  → **1** | . . . | **9**
rest      → digit rest | **ε**
digit     → **0** | sigdigit
</pre>

<hr>

### UML
![UML](./imgs/uml.png)

<hr>

### FACTORIAL EXAMPLE
![Factorial Example](./imgs/example_factorial.PNG)
