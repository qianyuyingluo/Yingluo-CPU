# Yingluo-CPU

一个基于 RISC-V 指令集的 64 位简易 CPU，使用 C++17 软件模拟。

当前实现 RV64I 的 ADDI、ADD 指令子集，并非完整 RV64I CPU。
拥有 32 个 64 位通用寄存器（x0 恒为零）、64 位 PC、默认 4 KiB 小端字节内存。
基础指令长度为 32 位。每次 step() 完成取指、译码、执行、写回，并将 PC 增加 4。
ADDI 对 12 位立即数进行符号扩展；加法按模 2^64 回绕。

## 构建和运行

需要支持 C++17 的 g++ 和 make。

```sh
make
./build/yingluo
make test
```

示例中的机器码对应：

```asm
addi x1, x0, 5
addi x2, x0, 7
add  x3, x1, x2
```

最终输出为 `PC=12 x1=5 x2=7 x3=12`。
示例由宿主程序限制执行三步；当前没有实现停机指令或汇编器。
load_program() 接收 32 位机器码列表，重置寄存器、PC 和内存。
非法/未支持指令和取指越界会抛出异常，失败指令不推进 PC。

## 文件

- src/cpu.hpp：CPU 状态、程序装载和单步执行。
- src/main.cpp：三条指令的演示程序。
- tests/cpu_test.cpp：寄存器、立即数边界、溢出、非法指令和内存边界测试。

下一阶段可增加 SUB、逻辑运算、LD/SD、BEQ 和 JAL，运行带循环的程序。
