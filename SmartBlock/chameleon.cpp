#include <iostream>
#include <string>
#include <cstring>
#include <openssl/evp.h>

using namespace std;

string chameleonHash(string msg, string key)
{
	// Set up the hashing context
	EVP_MD_CTX *mdctx;
	mdctx = EVP_MD_CTX_new();
	const EVP_MD *md = EVP_get_digestbyname("SHA256");

	// Initialize the context with the key
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, key.c_str(), key.size());

	// Hash the message and finalize the context
	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hash_len;
	EVP_DigestUpdate(mdctx, msg.c_str(), msg.size());
	EVP_DigestFinal_ex(mdctx, hash, &hash_len);

	// Convert the hash to a string and return it
	string result;
	char buf[3];
	for (int i = 0; i < hash_len; i++)
	{
		sprintf(buf, "%02x", hash[i]);
		result += buf;
	}
	EVP_MD_CTX_free(mdctx);
	return result;
}

bool verifyChameleonHash(string msg, string key, string hash)
{
	// Compute the expected hash using the original key
	string expected_hash = chameleonHash(msg, key);

	// Compare the expected hash with the computed hash
	if (expected_hash == hash)
		return true;
	return false;
}

// int main()
// {
// 	string msg = "bhaskar";
// 	string key = "secret";
// 	string hash = chameleonHash(msg, key);
// 	cout << "Message: " << msg << endl;
// 	cout << "Key: " << key << endl;
// 	cout << "Hash: " << hash << endl;
// 	// Verify the hash
// 	bool is_valid = verifyChameleonHash(msg, key, hash);

// 	// Print the result of the verification
// 	if (is_valid)
// 		cout << "The hash is valid." << endl;
// 	else
// 		cout << "The hash is not valid." << endl;
// 	return 0;
// }