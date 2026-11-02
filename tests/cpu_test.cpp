#include "cpu.hpp"
#include <iostream>
#include <limits>

void check(bool condition) {
    if (!condition) throw std::runtime_error("Test failed");
}
void expect_error(Cpu& cpu) {
    const auto pc = cpu.pc();
    try { cpu.step(); } catch (const std::runtime_error&) {
        check(cpu.pc() == pc);
        return;
    }
    throw std::runtime_error("Expected CPU error");
}
int main() {
    Cpu cpu;
    cpu.load_program({0x00500093, 0x00700113, 0x002081b3});
    for (int i = 0; i < 3; ++i) cpu.step();
    check(cpu.reg(1) == 5 && cpu.reg(2) == 7 && cpu.reg(3) == 12 && cpu.pc() == 12);
    cpu.load_program({0xfff00093, 0x00108113, 0x001081b3, 0x00500013});
    for (int i = 0; i < 4; ++i) cpu.step();
    check(cpu.reg(1) == std::numeric_limits<std::uint64_t>::max());
    check(cpu.reg(2) == 0 && cpu.reg(3) == std::numeric_limits<std::uint64_t>::max() - 1);
    check(cpu.reg(0) == 0);
    cpu.load_program({0x80000093, 0x7ff00113});
    cpu.step(); cpu.step();
    check(cpu.reg(1) == std::uint64_t{0} - 2048 && cpu.reg(2) == 2047);
    for (auto word : {0u, 0x402081b3u, 0x00101093u}) {
        cpu.load_program({word}); expect_error(cpu);
    }
    Cpu small(4);
    small.load_program({0x00000013}); small.step(); expect_error(small);
    Cpu empty(0); expect_error(empty);
    bool rejected = false;
    try { small.load_program({0, 0}); } catch (const std::runtime_error&) { rejected = true; }
    check(rejected);
    cpu.load_program({}); check(cpu.pc() == 0 && cpu.reg(1) == 0);
    std::cout << "All CPU tests passed\n";
}
