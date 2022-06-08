# bgeu

> Branch if greater than or equal unsigned

## 用法

```asm
bgeu rs1, rs2, label
```

若寄存器 rs1 的值大于等于寄存器 rs2 的值(均视为无符号数)，则跳转到 label。

## 例子

```asm
    la a0, 2
    la a1, 1
    bgeu a0, a1, done
```
