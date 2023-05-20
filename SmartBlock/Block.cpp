#include "Block.h"
#include "sha256.h"
#include "blake.cpp"
#include "chameleon.cpp"
#include <chrono>

Block::Block(uint32_t nIndexIn, const string &sDataIn) : _nIndex(nIndexIn), _sData(sDataIn)
{
	_nRandomness = 0;
	_tTime = time(nullptr);

	sHash = _CalculateHash();
}

void Block::MineBlock(uint32_t nDifficulty)
{
	// auto start = std::chrono::high_resolution_clock::now();
	char cstr[nDifficulty + 1];
	for (uint32_t i = 0; i < nDifficulty; ++i)
	{
		cstr[i] = '0';
	}
	cstr[nDifficulty] = '\0';

	string str(cstr);
	do
	{
		_nRandomness++;
		sHash = _CalculateHash();
	} while (sHash.substr(0, nDifficulty) != str);
	// auto stop = std::chrono::high_resolution_clock::now();

	// // Calculate the time taken
	// auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	cout << "Block " << _nIndex << " mined : " << sHash << endl;

	// cout << "Time taken: " << duration.count() / 1000000.0 << " seconds" << std::endl;
}

inline string Block::_CalculateHash() const
{
	stringstream ss;
	ss << _nIndex << sPrevHash << _tTime << _sData << sNextHash << _nRandomness;
	srand((unsigned int)time(NULL));
	string SecretKey = to_string(rand() % sizeof(int));
	return chameleonHash(ss.str(), SecretKey);
	// return blake_hash(ss.str());
	// return sha256(ss.str());
}

void Block::UpdateHashNext(string hashNext)
{
	cout << "--------------------------------" << endl;
	cout << "Old HashNext : " << sNextHash << endl;
	sNextHash = hashNext;
	cout << "New HashNext : " << sNextHash << endl;
	cout << "HashNext of the Block " << _nIndex << " has been updated" << endl;
	cout << "--------------------------------" << endl
		 << endl;
}

string Block::CalculateHashNext()
{
	stringstream ss;
	ss << _nIndex << sPrevHash << _tTime << _sData << sNextHash;
	return blake_hash(ss.str());
}

void Block::UpdateRandomness(string requiredHash, uint32_t nDifficulty)
{

	cout << "--------------------------------" << endl;
	cout << "Old Randomness : " << _nRandomness << endl;
	string tempHash;
	_nRandomness = 0;
	char cstr[nDifficulty + 1];
	for (uint32_t i = 0; i < nDifficulty; ++i)
	{
		cstr[i] = '0';
	}
	cstr[nDifficulty] = '\0';

	string str(cstr);
	do
	{
		_nRandomness++;
		tempHash = _CalculateHash();
	} while ((tempHash != requiredHash) and (tempHash.substr(0, nDifficulty) != str));

	cout << "New Randomness : " << _nRandomness << endl;
	cout << "Randomness of the Block " << _nIndex << " has been Updated" << endl;
	cout << "Hash of the Block " << _nIndex << " is " << sHash << endl;
	cout << "--------------------------------" << endl
		 << endl;
	cout << "==================================================================" << endl;
}