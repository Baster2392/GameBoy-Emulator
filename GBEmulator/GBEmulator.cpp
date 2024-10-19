#include <iostream>
#include <cstddef>
#include <cstdint>

using namespace std;

int main()
{
	uint16_t a = 1000;
	uint8_t b = -255;
	uint16_t c = a + uint16_t(b) + 0xFF00;
	printf("a=%d b=%d a+b=%d", a, b, c);
}