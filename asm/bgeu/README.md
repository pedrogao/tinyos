# bgeu

> Branch if greater than or equal unsigned

## 用法

```asm
bgeu rs1, rs2, offset
```

若寄存器 rs1 的值大于等于寄存器 rs2 的值(均视为无符号数)，把 pc 的值设置为当前值加上
符号位扩展的偏移 offset。(注意：是符号位扩展)

## 例子

```asm
    la a0, 2
    la a1, 1
    bgeu a0, a1, 2f            # 如果 a0==a1，则 pc = 2f ;
```

注意：这里的 2f 不是立即数，而是标签！
