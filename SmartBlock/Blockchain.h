#include <cstdint>
#include <vector>
#include "Block.h"

using namespace std;

class Blockchain
{
public:
	Blockchain(uint32_t nDifficulty, string secretKey);

	void AddBlock(Block bNew);

private:
	uint32_t _nDifficulty;
	vector<Block> _vChain;
	string _secretKey;
	Block _GetLastBlock() const;
};