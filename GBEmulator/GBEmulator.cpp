#include <iostream>
#include <cstddef>
#include <cstdint>
#include "CPU.h"

using namespace std;

int main()
{
	CPU cpu = CPU();
	cpu.A = 10;
	cpu.B = 10;
	cpu.ADD_A_r8(cpu.B);
	printf("A=%d", cpu.A);
}