#include "cpu.hpp"
#include <iostream>

int main() {
    Cpu cpu;
    // x2 = 5 + 4 + 3 + 2 + 1.
    cpu.load_program({
        0x00500093, // addi x1, x0, 5
        0x00110133, // add  x2, x2, x1
        0xfff08093, // addi x1, x1, -1
        0xfe009ce3  // bne  x1, x0, -8
    });
    constexpr unsigned max_steps = 100;
    unsigned steps = 0;
    while (cpu.pc() != 16 && steps < max_steps) {
        cpu.step();
        ++steps;
        std::cout << "PC=" << cpu.pc() << " x1=" << cpu.reg(1)
                  << " x2=" << cpu.reg(2) << '\n';
    }
    if (cpu.pc() != 16) {
        std::cerr << "Step limit exceeded\n";
        return 1;
    }
}
