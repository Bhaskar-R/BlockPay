#include <iostream>
#include <string>
#include <map>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "Blockchain.h"
#include <ctime>

using namespace std;
using json = nlohmann::json;

string url = "https://blockpay-transactions-default-rtdb.asia-southeast1.firebasedatabase.app/Transactions.json";

static size_t callback(void *data, size_t size, size_t nmemb, void *userp)
{
	((string *)userp)->append((char *)data, size * nmemb);
	return size * nmemb;
}

string getFirebaseData()
{
	CURL *curl;
	CURLcode res;
	string response;

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, NULL);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			cerr << "Error: " << curl_easy_strerror(res) << endl;

		curl_easy_cleanup(curl);
	}

	return response;
}

void setFirebaseData(string path, string data)
{
	CURL *curl;
	CURLcode res;
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	curl = curl_easy_init();
	string tempurl = url;
	tempurl = tempurl.erase(tempurl.size() - 5) + "/" + path + ".json";
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, tempurl.c_str());
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
			cerr << "Error: " << curl_easy_strerror(res) << endl;
		else
			cout << " Data set successfully in Firebase!\n"
				 << endl;

		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
	}
}

int main()
{
	srand((unsigned int)time(NULL));
	string SecretKey = to_string(rand() % sizeof(int));
	uint32_t Difficulty = 4;
	map<string, string> TransactionsMap;
	cout << "Initializing SmartBlock..." << endl
		 << endl;
	Blockchain bChain = Blockchain(Difficulty, SecretKey);
	int counter = 0;
	cout << "SmartBlock is Initialized." << endl
		 << endl;
	cout << "Waiting for Transactions..." << endl
		 << endl;
	;
	while (true)
	{
		// Get data from Firebase
		string firebaseData = getFirebaseData();
		json j = json::parse(firebaseData);
		for (auto it = j.begin(); it != j.end(); ++it)
			TransactionsMap[it.key()] = it.value().dump();

		if (TransactionsMap["TransactionSuccessful"] == "false")
		{
			time_t current_time = time(nullptr);
			char *time_str = ctime(&current_time);
			cout << "\nTransaction is recorded from " << TransactionsMap["SenderAddress"] << " to " << TransactionsMap["ReceiverAddress"] << endl;
			cout << "\nMining Block " << counter << "..." << endl;
			string blockData = TransactionsMap["ReceiverAddress"] + "%" + TransactionsMap["SenderAddress"] + "%" + TransactionsMap["AmountToSend"] + "%" + time_str;
			bChain.AddBlock(Block(counter, blockData));
			setFirebaseData("TransactionSuccessful", "true");
			counter++;
		}
	}
	return 0;
}