#include <cstdint>
#include <iostream>
#include <sstream>

using namespace std;

class Block
{
public:
	string sHash;
	string sPrevHash, sNextHash;

	Block(uint32_t nIndexIn, const string &sDataIn);

	void MineBlock(uint32_t nDifficulty);

	void UpdateRandomness(string requiredHash, uint32_t nDifficulty);

	string CalculateHashNext();

	void UpdateHashNext(string hasNext);

private:
	uint32_t _nIndex;
	uint32_t _nRandomness;
	string _sData;
	time_t _tTime;

	string _CalculateHash() const;
};