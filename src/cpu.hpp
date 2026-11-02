#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>

// RV64I subset. Memory is byte-addressed and little-endian.
class Cpu {
public:
    explicit Cpu(std::size_t memory_size = 4096) : memory_(memory_size) {}
    std::uint64_t pc() const { return pc_; }
    std::uint64_t reg(std::size_t index) const { return registers_.at(index); }

    void load_program(const std::vector<std::uint32_t>& words) {
        if (words.size() > memory_.size() / 4)
            throw std::runtime_error("Program exceeds memory");
        registers_.fill(0);
        pc_ = 0;
        std::fill(memory_.begin(), memory_.end(), 0);
        for (std::size_t i = 0; i < words.size(); ++i)
            for (unsigned byte = 0; byte < 4; ++byte)
                memory_[i * 4 + byte] = static_cast<std::uint8_t>(words[i] >> (byte * 8));
    }

    void step() {
        if (pc_ % 4 != 0) throw std::runtime_error("Misaligned instruction address");
        if (pc_ > memory_.size() || memory_.size() - pc_ < 4)
            throw std::runtime_error("Instruction fetch out of bounds");
        std::uint32_t instruction = 0;
        for (unsigned byte = 0; byte < 4; ++byte)
            instruction |= std::uint32_t(memory_[pc_ + byte]) << (byte * 8);
        const auto opcode = instruction & 0x7f;
        const auto rd = (instruction >> 7) & 31;
        const auto funct3 = (instruction >> 12) & 7;
        const auto rs1 = (instruction >> 15) & 31;
        const auto rs2 = (instruction >> 20) & 31;
        const auto funct7 = instruction >> 25;
        std::uint64_t result;
        if (opcode == 0x13 && funct3 == 0) { // ADDI
            std::uint64_t immediate = instruction >> 20;
            if (immediate & 0x800) immediate |= ~std::uint64_t{0xfff};
            result = registers_[rs1] + immediate;
        } else if (opcode == 0x33 && funct3 == 0 && funct7 == 0) { // ADD
            result = registers_[rs1] + registers_[rs2];
        } else {
            throw std::runtime_error("Unsupported or illegal instruction");
        }
        // Unsigned arithmetic implements RISC-V modulo-2^64 addition.
        if (rd != 0) registers_[rd] = result;
        pc_ += 4;
    }

private:
    std::array<std::uint64_t, 32> registers_{};
    std::uint64_t pc_ = 0;
    std::vector<std::uint8_t> memory_;
};
