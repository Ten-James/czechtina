<h1 align="center">
	<img src="misc/banner.png" alt="CZECHTINA BANNER">
    
</h1>

Version 0.9.1

<hr />

Compiler for **czechtina** written in **czechtina**

## Why Czechtina?

Czechtina is programming language based on C and czech language. Czechtina should be faster to write than c and with additional features, easier to maintain. lot of features are still missing, but it is still in development.

## Installation

### Requirements

- having c compiler installed on your system with cc command

### using shell script

for stable version for your system

```sh
  ./install.sh --stable
```

## Syntax showcase

```cz
zpracuj histo:pointer<int>, minimum:int { void
    c:int = 0;
    scanf "%d", adresa c;
    for i:int -> 0 do 9 {
        if c == i+minimum {
            histo[i] += 1;
            return;
        }
    }
    histo[9] = histo[9] + 1;
}

main {
    t:char;
    histo:pointer<int> = [0,0,0,0,0,0,0,0,0,0];
    scanf "%c", adresa t;
    if 'v' != t != 'h' {
        println "Neplatny mod vykresleni";
        return 1;
    }
    n:int;
    minimum:int;
    scanf "%d", adresa n;
    scanf "%d", adresa minimum;
    for i:int -> 0 do n {
        zpracuj histo, minimum;
    }
    return 0;
}
```

## Upcoming Features

- [ ] array type (generic type)
- [ ] redesign virtual functions
- [ ] prelude in czechtina
- [ ] type checking
- [ ] memory management
- [ ] modularization
  - each module compile into one C file

## after 1.0.0

- [ ] function overloading
- [ ] operator overloading
- [ ] enums as types
  - type can be now enum
  - enum will have its own print function
  - int and enums cant be mixed
  - exact match will be used to work with enums
- [ ] match and exact match
  - instead of if else
  - exact match will be used to work with enums for now

## Changes comparing to C language

### syntax

Czechtina -> C

- variable declaration
  - `name:type;` -> `type name;`
  - `name:type = value;` -> `type name = value;`
  - `name:type = [value1, value2, ...];` -> `type name = {value1, value2, ...};`
- function definition:
  - `name args { type` -> `type name(args) {`
- function call:
  - `zavolej name;` or `call name;` -> `name();`
  - `name arg;` -> `name(arg);`
  - `name arg1, arg2, ...;` -> `name(arg1, arg2, ...);`
    - if there is expression in arguments, it will be evaluated before function call
    - `name arg1, name2 arg2;` -> `name(arg1,name2(arg2));`
    - `name arg1, 5 == false;` -> `name(arg1, 5 == false);`
- for syntax
  - `for i:int -> 0 do 10` -> `for(int i = 0; i < 10; i++)`
  - `for i:int -> 0 az 10` -> `for(int i = 0; i <= 10; i++)`
- if and else if are without brackets
  - `if condition` -> `if(condition)`
  - `else if condition` -> `else if(condition)`
- after `if`, `else if`, `else`, `for`, `while` there is block or `-> one line of code`
  - `if 5 == 5 {...}` -> `if(5 == 5) {...}`
  - `if 5 == 5 -> return 0;` -> `if(5 == 5) {return 0;}`

### few virtual functions

- `InC <data>`
  - data has to be string literal
  - -> data will be wrtitten to C source file;
- `throw <data>`
  - data has to be string literal
  - -> print error message and exit program
- `adresa <var>`
  - -> return address of var
- `hodnota <var>`
  - -> return value of pointer var
- `typeof <expr>`
  - -> return type of var as string
- `sizeof <expr>`
  - for identifier return sizeof(void\*)
  - any other is handled by c
  - -> return sizeof(expr)
- `printf <format>, <...args>`
  - -> fprintf(czStdOut, format, args)
- `println <...args>`
  - -> print all args to czStdOut then print newline
- `print <...args>`
  - -> print all args to czStdOut
