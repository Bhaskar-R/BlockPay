#include <openssl/evp.h>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>

using namespace std;

string blake_hash(const string &data)
{
	unsigned char digest[32]; // Use a 32-byte (256-bit) digest
	size_t digest_len = sizeof(digest);

	// Initialize the OpenSSL library
	OpenSSL_add_all_digests();

	// Initialize the BLAKE2s hash context
	const EVP_MD *md = EVP_blake2s256(); // Use the 256-bit variant
	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(mdctx, md, NULL);

	// Add data to the hash context
	EVP_DigestUpdate(mdctx, data.c_str(), data.size());

	// Finalize the hash and get the digest
	unsigned int digest_len_actual = 0;
	EVP_DigestFinal_ex(mdctx, digest, &digest_len_actual);

	// Verify that the actual digest length matches the expected length
	if (digest_len_actual != digest_len)
	{
		std::cerr << "Error: Unexpected digest length " << digest_len_actual << ", expected " << digest_len << std::endl;
	}

	// Clean up
	EVP_MD_CTX_free(mdctx);

	ostringstream stream;
	stream << hex << setfill('0');
	for (size_t i = 0; i < digest_len; ++i)
		stream << setw(2) << static_cast<unsigned>(static_cast<unsigned char>(digest[i]));
	return stream.str();
}