#ifndef BITS2BYTE
#define BITS2BYTE

#include <vector>
using namespace std;

class Bits2Byte {
public:
	Bits2Byte();
	~Bits2Byte();
	void addBits(string str);
	void addBits(char c);
	int size();
	int generateByte();
	int readBit();
	int addZero();

private:
	vector<int> bits;
};
#endif 
