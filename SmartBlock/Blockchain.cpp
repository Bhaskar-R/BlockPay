#include "Blockchain.h"

Blockchain::Blockchain(uint32_t nDifficulty, string secretKey) : _nDifficulty(nDifficulty), _secretKey(secretKey)
{
	_vChain.emplace_back(Block(0, "Genesis Block"));
}

void Blockchain::AddBlock(Block bNew)
{
	bNew.sPrevHash = _GetLastBlock().sHash;
	bNew.MineBlock(_nDifficulty);
	_vChain.push_back(bNew);
	Block blockToBeUpdated = *(std::prev(_vChain.end(), 2));
	blockToBeUpdated.UpdateHashNext(bNew.CalculateHashNext());
	blockToBeUpdated.UpdateRandomness(bNew.sPrevHash, _nDifficulty);
}

Block Blockchain::_GetLastBlock() const
{
	return _vChain.back();
}
