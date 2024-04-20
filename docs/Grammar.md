# Grammar

things that are missing in czechtina syntax:

- array types
- static arrays
- match flow
- exact match flow
- defined for loop

```c
use -> "use" identifier ("::" identifier)* semicolon
package -> "package" identifier ("::" identifier)* semicolon

var_definition -> identifier ":" type


function
//   -> identifier var_definition? (',' var_definition)* "->" expression
    -> identifier var_definition? (',' var_definition)* "{" type lines "}"

struct
    -> "struct" identifier "{" (var_definition | function)* "}"

type
    -> "pointer" "<" type ">"
    -> "&" type
    -> "?" type
    -> primitive_type

flow
    -> "if" expression "{" lines "}" ("else" "if" "{" lines "}")* ("else" "{" lines "}")?
    -> "while" expression "{" lines "}"
    -> "return" expression? ";"
    -> "break" ";"
    -> "continue" ";"

for
//    -> "for" var_definition = expression ";" expression ";" expression "{" lines "}"

    -> "for" var_definition "->" expression ("do" | "po") expression "{" lines "}"

//done
enum
    -> "enum" identifier "{" identifier /*("=" numeric_literal)?*/ ("," identifier /*("=" numeric_literal)*/?)* "}"


lines
    -> lines lines
    -> flow
    -> for
    -> single_exp assign right_side ";"
    -> single_exp unary_assign ";"
    -> expression ";"
    -> var_definition ("=" right_side)? ":"

right_side
    -> expression
    -> new_node

new_node
    -> "new" type
    -> "new" type, expression

assign
    -> "="
    -> "+="
    -> "-="
    -> "*="
    -> "/="
    -> "%="

unary_assign
    -> "++"
    -> "--"


expression
    -> comparation (("&&" | "||") comparation)*


comparation_operator
    -> "==" | "!=" | "<" | ">" | "<=" | ">="

compariton
    -> terms (comparison_operator terms)*

terms
    -> factor (("+" | "-") factor)*

factor
    -> unary_exp (("*" | "/" | "%") unary_exp)*


unary_exp
    -> "!" unary_exp
    -> "-" unary_exp
    -> "(" expression ")"
    -> left_single expression ("," expression)*
    -> ("call" | "zavolej") left_single
    -> single_exp
    -> literal


single_exp
    -> "&" single_exp
    -> left_single

left_single
    -> left_single "[" expression "]"
    -> left_single "." identifier
    -> identifier


```
