#include "Bits2Byte.h"
#include <string>
#include <iostream>
using namespace std;

Bits2Byte::Bits2Byte() {}

void Bits2Byte::addBits(string str)
{
	for (auto i : str) {
		if (i == '1')
			bits.push_back(1);
		else
			bits.push_back(0);
	}
}

void Bits2Byte::addBits(char c) {
	for (int i = 7; i >= 0; i--) {
		bits.push_back(((c >> i) & 1));
	}
}

Bits2Byte::~Bits2Byte()
{
	bits.clear();
}

int Bits2Byte::size()
{
	return bits.size();
}

int Bits2Byte::generateByte()
{
	int num = 0;
	for (int i = 0; i < 8; i++) {
		int p = 1;
		for (int j = 1; j <= 7 - i; j++) 
			p *= 2;
		num += bits[i] * p;
	}
	
	for (int i = 0; i < 8; i++) 
		bits.erase(bits.begin());
	return num;
}

int Bits2Byte::readBit() {
	if (bits.size() == 0)
		return -1;
	int bit = bits[0];
	bits.erase(bits.begin());
	return bit;
}

int Bits2Byte::addZero()
{
	while (bits.size() % 8 != 0) {
		bits.push_back(0);
	}
	return 0;
}
