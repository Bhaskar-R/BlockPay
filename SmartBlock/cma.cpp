#include <bits/stdc++.h>

using namespace std;
int hashValue;
void getVRFHash(int vhash)
{
	hashValue = vhash;
}

bool isCommitteeMember(int g, int threshold)
{
	if (g < threshold)
		return true;
	return false;
}
int main()
{
	int n, t, thres = 0.5;
	// getVRFHash(vhash);
	int g = hashValue / pow(2, log2(hashValue) + 1);
	isCommitteeMember(g, thres);
	return 0;
}