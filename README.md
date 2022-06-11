# tinyos

> 基于 risc-v 平台的简单操作系统，旨在学习 risc-v 和实践
> 操作系统的设计与实现

## 运行

```sh
cd os && make run
```

## feature

- 内存分页管理
- UART 外设
- 多进程
- 上下文切换
- 外部中断(输入、输出)
- 时钟中断
- 系统调用

## TODO

- 无 bootloader
- 无虚拟内存
- 无用户态

## 代码结构

- [riscv 汇编入门](./asm/)
- [bootloader x86](./bootloader/)
- [riscv 文档](./docs/)
- [riscv os](./riscvos/)
  -- [riscv setup](./riscvos/1-setup/)
  -- [riscv final](./riscvos/final/)

## 参考资料

- https://ica123.com/archives/6318
- https://github.com/plctlab/riscv-operating-system-mooc
- https://notes.eatonphil.com/bootloader-basics.html
- https://0x00sec.org/t/realmode-assembly-writing-bootable-stuff-part-5/3667
