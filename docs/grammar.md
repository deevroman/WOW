```
file_input: [suite] ENDMARKER
suite: (NEWLINE+ stmt)+

funcdef: 'def' NAME parameters ':' suite
parameters: '(' [funcargslist] ')'
funcargslist: NAME (',' NAME)*

stmt: simple_stmt | compound_stmt
simple_stmt: expr_stmt | del_stmt | pass_stmt | flow_stmt | import_stmt
expr_stmt: test (augassign test | ('=' test)* )
augassign: ('+='|'-='|'*='|'/='|'%='|'**='|'//='|'&='|'|='|'^='|'<<='|'>>=')

del_stmt: 'del' expr
pass_stmt: 'pass'
flow_stmt: break_stmt | continue_stmt | return_stmt
break_stmt: 'break'
continue_stmt: 'continue'
return_stmt: 'return' [test]

import_stmt: 'import' NAME ['as' NAME]

compound_stmt: if_stmt | while_stmt | for_stmt | funcdef | classdef
if_stmt: 'if' test ':' suite ('elif' test ':' suite)* ['else' ':' suite]
while_stmt: 'while' test ':' suite ['else' ':' suite]
for_stmt: 'for' NAME 'in' test ':' suite ['else' ':' suite]


test: and_test ('or' and_test)*
and_test: not_test ('and' not_test)*
not_test: 'not' not_test | comparison
comparison: expr (comp_op expr)*
comp_op: '<'|'>'|'=='|'>='|'<='|'!='

expr: xor_expr ('|' xor_expr)*
xor_expr: and_expr ('^' and_expr)*
and_expr: shift_expr ('&' shift_expr)*
shift_expr: arith_expr (('<<' | '>>') arith_expr)*
arith_expr: term (('+'|'-') term)*
term: factor (('*'|'/'|'%'|'//') factor)*
factor: ('+'|'-'|'~') factor | power
power: name_expr ('**' factor)*
name_expr: ('(' test ')' | NAME | NUMBER | STRING | 'None' | 'True' | 'False') trailer*
trailer: '(' [arglist] ')' | '[' test ']' | '.' NAME

arglist: test (',' test)*

classdef: 'class' NAME ':' suite

```