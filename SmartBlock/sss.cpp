#include <iostream>
#include <random>
#include <vector>

using namespace std;

// Function to generate a random polynomial of degree k-1
vector<int> generate_polynomial(int secret, int threshold, int num_shares)
{
	vector<int> coeffs(threshold);

	random_device rd;
	mt19937 gen(rd());
	int size = log10(secret) - 1;
	uniform_int_distribution<> distrib(1, pow(10, size));

	coeffs[0] = secret;
	for (int i = 1; i < threshold; i++)
	{
		coeffs[i] = distrib(gen);
	}

	vector<int> shares(num_shares);
	for (int i = 1; i <= num_shares; i++)
	{
		int x = i;
		shares[i - 1] = coeffs[threshold - 1];
		for (int j = threshold - 2; j >= 0; j--)
		{
			shares[i - 1] = shares[i - 1] * x + coeffs[j];
		}
	}

	return shares;
}

// Function to reconstruct the secret from a set of shares
int reconstruct_secret(const vector<int> &shares, int threshold)
{
	int secret = 0;
	for (int i = 0; i < threshold; i++)
	{
		int xi = i + 1;
		int yi = shares[i];
		int numerator = 1;
		int denominator = 1;
		for (int j = 0; j < threshold; j++)
		{
			if (j != i)
			{
				int xj = j + 1;
				numerator *= -xj;
				denominator *= (xi - xj);
			}
		}
		secret += yi * numerator / denominator;
	}
	return secret;
}

int main()
{
	// Choose a secret and a threshold
	int secret = 4356789;
	int threshold = 4; // No. of members required to reconstruct the secret key

	// Generate shares
	vector<int> shares = generate_polynomial(secret, threshold, 5);

	// Reconstruct the secret
	int reconstructed_secret = reconstruct_secret(shares, threshold);

	// Print the results
	cout << "Secret: " << secret << endl;
	cout << "Shares: ";
	for (int i = 0; i < shares.size(); i++)
		cout << shares[i] << " ";
	cout << endl;
	cout << "Reconstructed secret: " << reconstructed_secret << endl;

	return 0;
}
