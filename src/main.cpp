#include "cpu.hpp"
#include <iostream>

int main() {
    Cpu cpu;
    cpu.load_program({0x00500093, 0x00700113, 0x002081b3});
    for (unsigned step = 0; step < 3; ++step) {
        cpu.step();
        std::cout << "PC=" << cpu.pc() << " x1=" << cpu.reg(1)
                  << " x2=" << cpu.reg(2) << " x3=" << cpu.reg(3) << '\n';
    }
}
