#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

enum AM : int64_t { P = 0, I = 1, R = 2 };
class IntCpu {
  private:
    std::unordered_map<int64_t, int64_t> memory;
    int64_t inp = 0;
    int64_t rbp = 0;
    std::array<AM, 3> am = {AM::P, AM::P, AM::P};
    int64_t& get_memory(int64_t c, int off) {
        switch (am[off - 1]) {
        case AM::P:
            return memory[c];
            break;
        case AM::I:
            return memory[inp + off];
            break;
        case AM::R:
            return memory[rbp + c];
            break;
        default:
            break;
        }
    }
    int64_t& C() { return get_memory(memory[inp + 1], 1); }
    int64_t& D() { return get_memory(memory[inp + 2], 2); }
    int64_t& E() { return get_memory(memory[inp + 3], 3); }

    void ADD() {
        E() = C() + D();
        inp += 4;
    }
    void MUL() {
        E() = C() * D();
        inp += 4;
    }
    void INP() {
        std::cout << "Input: ";
        std::cin >> C();
        std::cout << std::endl;
        inp += 2;
    }
    void OUT() {
        std::cout << "Output: " << C() << std::endl;
        inp += 2;
    }
    void SIP() {
        if (C()) {
            inp = D();
        } else {
            inp += 3;
        }
    }
    void JMP() {
        if (!C()) {
            inp = D();
        } else {
            inp += 3;
        }
    }
    void CMP() {
        E() = C() < D();
        inp += 4;
    }
    void EQL() {
        E() = C() == D();
        inp += 4;
    }
    void ARP() {
        rbp += C();
        inp += 2;
    }
    void HALT() { std::exit(-1); }
    int64_t parse_op(int64_t op) {
        auto s = std::string(5 - std::to_string(op).length(), '0') +
                 std::to_string(op);
        for (int i = 0; i < 3; i++) {
            am[2 - i] = static_cast<AM>((int)s[i] - '0');
        }
        return stoi(s.substr(3, 2));
    }

  public:
    IntCpu(std::vector<int64_t> in) {
        int i = 0;
        for (auto v : in) {
            memory[i] = v;
            i++;
        }
    }
    IntCpu() = delete;
    void run() {
        for (;;) {
            int64_t op = parse_op(memory[inp]);
            switch (op) {
            case 1:
                ADD();
                break;
            case 2:
                MUL();
                break;
            case 3:
                INP();
                break;
            case 4:
                OUT();
                break;
            case 5:
                SIP();
                break;
            case 6:
                JMP();
                break;
            case 7:
                CMP();
                break;
            case 8:
                EQL();
                break;
            case 9:
                ARP();
                break;
            case 99:
                HALT();
                break;
            }
        }
    }
};
auto main(int argc, char* argv[]) -> int {
    std::cout << "*** INTCODE ***" << std::endl;
    std::ifstream file(argv[1]);
    std::string buffer;
    std::vector<int64_t> data;
    std::getline(file, buffer);
    std::stringstream s(buffer);
    while (s.good()) {
        std::string subs;
        std::getline(s, subs, ',');
        data.push_back(std::stoi(subs));
    }
    IntCpu cpu(data);
    cpu.run();
    return 0;
}
