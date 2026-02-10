# AiSD1
# Stack Processor Interpreter

An interpreter of a simple stack-based processor written in C++.
The processor operates on a stack of character lists and supports its own instruction set.

## Description of Operatio

- The program reads the source code from **the first line of input**.  
- **The second input line** contains characters that will be provided to the program’s standard input (the . instruction).  
- The interpreter executes the program step by step, modifying the stack and printing data to standard output according to the instructions.  

### Input Format
1. First line – the program to execute (a sequence of instructions without spaces). 
2. Second line – input data (a sequence of characters, max. 20,000).  

### Output Format
Everything that the processor prints to standard output during program execution.

---

## Instruction Set

| Instruction | Name            | Description |
|------------|------------------|-----------|
| `'`        | apostrophe       | push an empty list onto the stack |
| `,`        | comma            | pop a list from the stack |
| `:`        | colon            | push a copy of the list from the top of the stack |
| `;`        | semicolon        | swap the two top lists on the stack |
| `@`        | at               | pop a number A and push a copy of the list at position A on the stack (0 = top) |
| `.`        | dot              | read a character from input and prepend it to the list on top of the stack |
| `>`        | greater-than     | print the first character of the list on top of the stack and pop that list |
| `!`        | exclamation mark | logical negation – replace an empty list or '0' with '1', otherwise replace with '0' |
| `<`        | less-than        | pop A and B; if B < A, push `1`, otherwise `0` |
| `=`        | equals           | pop A and B; if B = A, push `1`, otherwise `0` |
| `~`        | tilde            | push the index of the current instruction onto the stack |
| `?`        | question mark    | pop T and W; if W ≠ empty/0, set the instruction pointer to T |
| `-`        | minus            | if the list ends with `-`, remove it; otherwise append `-` to the end|
| `^`        | caret            | remove the trailing `-` from the list (absolute value) |
| `$`        | dollar           | detach the first character of the list on top of the stack and push it as a new list |
| `#`        | hash             | pop list A and append it to the end of the list on top of the stack |
| `+`        | plus             | pop numbers A and B, push A + B |
| `&`        | ampersand        | print the entire stack in the format: `n: list … 0: list` |
| `]`        | right square bracket | pop number A, push the ASCII character with code A|
| `[`        | left square bracket | pop list A, push the ASCII code of the first character of the list |
| others     | other characters     | append the character to the beginning of the list on top of the stack |

---

## Compilation

A standard C++17 (or newer) compiler is sufficient.

## Examples

Example 1

Input:

'123'-456&+&


Output:

1: 321

0: 654-

0: 333-


Explanation:
The program creates lists on the stack, manipulates them using stack operations and arithmetic instructions, and prints the stack state using the & instruction.

Example 2

Input:

'...&$&
123


Output:

0: 321

1: 21

0: 3


Explanation:
Characters from the input stream are read using the . instruction and added to the list.
The $ instruction separates the first character into a new list, and & prints the current contents of the stack.
