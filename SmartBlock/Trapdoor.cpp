#include <bits/stdc++.h>
using namespace std;

string random_gen(int bits)
{
	string s = "";
	for (int i = 0; i < bits; i++)
	{
		s += 48 + rand() % 10;
	}
	return s;
}

string multiply(string num1, string num2)
{
	int len1 = num1.size();
	int len2 = num2.size();
	if (len1 == 0 || len2 == 0)
		return "0";
	vector<int> result(len1 + len2, 0);
	int i_n1 = 0;
	int i_n2 = 0;
	for (int i = len1 - 1; i >= 0; i--)
	{
		int carry = 0;
		int n1 = num1[i] - '0';
		i_n2 = 0;
		for (int j = len2 - 1; j >= 0; j--)
		{
			int n2 = num2[j] - '0';
			int sum = n1 * n2 + result[i_n1 + i_n2] + carry;
			carry = sum / 10;
			result[i_n1 + i_n2] = sum % 10;
			i_n2++;
		}
		if (carry > 0)
			result[i_n1 + i_n2] += carry;
		i_n1++;
	}
	int i = result.size() - 1;
	while (i >= 0 && result[i] == 0)
		i--;
	if (i == -1)
		return "0";
	string s = "";
	while (i >= 0)
		s += std::to_string(result[i--]);
	return s;
}

string Division(string number, long long int divisor)
{
	string ans;
	long long int idx = 0;
	long long int temp = number[idx] - '0';
	while (temp < divisor)
		temp = temp * 10 + (number[++idx] - '0');
	while (number.size() > idx)
	{
		ans += (temp / divisor) + '0';
		temp = (temp % divisor) * 10 + number[++idx] - '0';
	}
	if (ans.length() == 0)
		return "0";
	return ans;
}

string addition(string a, string b)
{
	if (a.length() > b.length())
		swap(a, b);
	string sum = "";
	int len1 = a.length();
	int len2 = b.length();
	int digitDiff = len2 - len1;
	int carry = 0;
	int intSum;
	for (int i = len1 - 1; i >= 0; i--)
	{
		intSum = ((a[i] - '0') + (b[i + digitDiff] - '0') + carry);
		sum.push_back(intSum % 10 + '0');
		carry = intSum / 10;
	}
	for (int i = digitDiff - 1; i >= 0; i--)
	{
		intSum = ((b[i] - '0') + carry);
		sum.push_back(intSum % 10 + '0');
		carry = intSum / 10;
	}
	if (carry)
		sum.push_back(carry + '0');
	reverse(sum.begin(), sum.end());
	return sum;
}

bool isSmaller(string a, string b)
{
	int n1 = a.length(), n2 = b.length();

	if (n1 < n2)
		return true;
	if (n2 < n1)
		return false;

	for (int i = 0; i < n1; i++)
		if (a[i] < b[i])
			return true;
		else if (a[i] > b[i])
			return false;
	return false;
}

string difference(string a, string b)
{
	if (isSmaller(a, b))
		swap(a, b);
	string str = "";
	int n1 = a.length(), n2 = b.length();
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	int carry = 0;
	for (int i = 0; i < n2; i++)
	{
		int sub = ((a[i] - '0') - (b[i] - '0') - carry);
		if (sub < 0)
		{
			sub = sub + 10;
			carry = 1;
		}
		else
			carry = 0;
		str.push_back(sub + '0');
	}
	for (int i = n2; i < n1; i++)
	{
		int sub = ((a[i] - '0') - carry);
		if (sub < 0)
		{
			sub = sub + 10;
			carry = 1;
		}
		else
			carry = 0;
		str.push_back(sub + '0');
	}
	reverse(str.begin(), str.end());
	const regex pattern("^0+(?!$)");
	str = regex_replace(str, pattern, "");
	return str;
}

vector<pair<string, string>> generate_keys()
{
	srand(time(0));
	string secret_key;
	cout << "Enter Secret Key : ";
	cin >> secret_key;

	int parts = 4, required = 2;

	string b = random_gen(secret_key.size());
	vector<pair<string, string>> keys(parts);
	for (int i = 0; i < parts; i++)
	{
		// y=a+bx
		string x = to_string(i + 1);
		string y = addition(secret_key, multiply(b, x));
		keys[i] = make_pair(x, y);
	}
	return keys;
}

string secret_phrase()
{
	int pts, required = 2;
	cout << "Enter Number of Points : ";
	cin >> pts;
	while (pts < 2)
	{
		cout << "Minimum Number of Points required are " << required << endl;
		cout << "Enter Number of points : ";
		cin >> pts;
	}
	vector<pair<int, string>> points(pts);
	cout << "Enter " << pts << " Points :" << endl;
	for (int i = 0; i < pts; i++)
	{
		int x;
		string y;
		cin >> x >> y;
		points[i] = make_pair(x, y);
	}
	string dy = difference((points[1].second), (points[0].second));
	int dx = (points[1].first) - (points[0].first);
	string b = Division(dy, dx);
	return difference(points[0].second, multiply(b, to_string(points[0].first)));
}

int main()
{
	int opt;
	cout << "Choose one\n1.To Generate keys \n2.To Generate Secret Phrase\n";
	cin >> opt;
	while (opt != 1 and opt != 2)
	{
		cout << "Choose Proper option\n1.To Generate keys \n2.To Generate Secret Phrase\n";
		cin >> opt;
	}

	if (opt == 1)
	{
		vector<pair<string, string>> keys = generate_keys();
		cout << "\nFour Secret Keys " << endl;
		for (auto &i : keys)
			cout << "(" << i.first << "," << i.second << ")" << endl;
	}
	else
	{
		string res = secret_phrase();
		cout << "Secret Key : " << res << endl;
	}
	return 0;
}