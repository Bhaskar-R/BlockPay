#include <iostream>
#include <string>
#include <random>

// Simple block structure
struct Block
{
	uint32_t index;
	std::string data;
	uint32_t nonce;
	std::string previousHash;
	std::string hash;
};

// Function to calculate block hash
std::string calculateHash(Block block)
{
	// Simple hash function, just concatenating block fields
	std::string hashString = std::to_string(block.index) + block.data + std::to_string(block.nonce) + block.previousHash;
	return hashString;
}

// Proof-of-work function
void mineBlock(Block &block, uint32_t difficulty)
{
	// Create a random_device to seed the generator
	std::random_device rd;

	// Create a mersenne_twister_engine for generating random bits
	std::mt19937_64 gen(rd());

	// Create a distribution for generating random 32-bit integers
	std::uniform_int_distribution<uint32_t> dist(0, std::numeric_limits<uint32_t>::max());

	// Set the target hash based on the difficulty
	std::string targetHash(difficulty, '0');

	// Loop until the hash meets the target difficulty
	do
	{
		// Generate a random nonce
		block.nonce = dist(gen);

		// Calculate the block hash
		block.hash = calculateHash(block);
	} while (block.hash.substr(0, difficulty) != targetHash);
}

int main()
{
	// Create a sample block
	Block block;
	block.index = 0;
	block.data = "Hello, world!";
	block.nonce = 0;
	block.previousHash = "0000000000000000000000000000000000000000000000000000000000000000";

	// Mine the block with a difficulty of 2 (leading zeros in hash)
	mineBlock(block, 63);

	// Print the block contents
	std::cout << "Block contents:" << std::endl;
	std::cout << "Index: " << block.index << std::endl;
	std::cout << "Data: " << block.data << std::endl;
	std::cout << "Nonce: " << block.nonce << std::endl;
	std::cout << "Previous hash: " << block.previousHash << std::endl;
	std::cout << "Hash: " << block.hash << std::endl;

	return 0;
}
