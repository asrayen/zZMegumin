#include <iostream>
#include <bitset>
#include <random>

class LFSR
{
private:
    unsigned short state; // （16位寄存器）

public:
    LFSR(unsigned short seed) : state(seed) {}

    // 生成下一个随机数
    bool nextBit()
    {
        // 反馈多项式：x^16 + x^14 + x^13 + x^11 + 1
        bool feedback = ((state >> 15) ^ (state >> 13) ^ (state >> 12) ^ (state >> 10)) & 1;

        // 将当前状态左移1位，并将反馈位添加到最低位
        state = (state << 1) | feedback;

        // 返回生成的最低有效位（LSB）
        return state & 1;
    }

    // 获取当前状态
    unsigned short getState() const
    {
        return state;
    }

    // 打印状态的二进制形式
    void printState() const
    {
        std::bitset<16> binState(state);
        std::cout << "LFSR state: " << binState << std::endl;
    }
};

int main()
{
    std::random_device rd;
    unsigned short seed = rd(); // 随机种子
    LFSR lfsr(seed);
    std::cout << "current seed:" << seed << std::endl;
    std::cout << "Generating 20 random bits:" << std::endl;
    for (int i = 0; i < 20; i++)
    {
        bool randomBit = lfsr.nextBit();
        std::cout << randomBit;
    }
    std::cout << std::endl;

    return 0;
}
