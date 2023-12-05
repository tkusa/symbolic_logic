# Symbolic Logic

Analyze symbolic logic

## Compile
```
make
```

## Usage
```
$ ./logic 'A && B'
AB : A && B
00 : 0
10 : 0
01 : 0
11 : 1

$ ./logic '(A -> !B) -> (B->!A)'
AB : (A -> !B) -> (B->!A)
00 : 1
10 : 1
01 : 1
11 : 1
```