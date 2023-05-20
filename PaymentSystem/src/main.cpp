#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define RST_PIN 4
#define SS_PIN 5
#define API_KEY "AIzaSyDSFX3-t3pIdWg6VTMrTgPbITFTbovXY-A";
#define DATABASE_URL "https://blockpay-transactions-default-rtdb.asia-southeast1.firebasedatabase.app/"
const char *ssid = "Redmi5";
const char *password = "nopassword";

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
String uidString, receiverUidString = "", balance = "";
int amountToSend = -1, cnt = 0;
bool TransactionSuccessful = false;

void setup()
{
	Serial.begin(9600); // Initialize serial communications with the PC
	SPI.begin();		// Init SPI bus
	mfrc522.PCD_Init(); // Init MFRC522 card

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(1000);
		Serial.println("Connecting to WiFi...");
	}
	Serial.print("Connected With IP: ");
	Serial.println(WiFi.localIP());
	Serial.println();

	config.api_key = API_KEY;
	config.database_url = DATABASE_URL;
	if (Firebase.signUp(&config, &auth, "", ""))
	{
		Serial.println("Firebase Integration Success");
		signupOK = true;
	}
	else
	{
		Serial.printf("%s\n", config.signer.signupError.message.c_str());
	}
	config.token_status_callback = tokenStatusCallback;
	Firebase.begin(&config, &auth);
	Firebase.reconnectWiFi(true);
}

void loop()
{
	// Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
	MFRC522::MIFARE_Key key;
	for (byte i = 0; i < 6; i++)
		key.keyByte[i] = 0xFF;

	// some variables we need
	byte block;
	byte len;
	MFRC522::StatusCode status;

	//-------------------------------------------

	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if (!mfrc522.PICC_IsNewCardPresent())
	{
		return;
	}

	// Select one of the cards
	if (!mfrc522.PICC_ReadCardSerial())
	{
		return;
	}

	Serial.println(F("\n**Card Detected**"));

	//-------------------------------------------

	Serial.print("Account Number: ");
	uidString = "";
	for (byte i = 0; i < mfrc522.uid.size; i++)
	{
		uidString += mfrc522.uid.uidByte[i] < 0x10 ? "0" : "";
		uidString += String(mfrc522.uid.uidByte[i], HEX);
	}
	Serial.println(uidString);

	// mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); // dump some details about the card

	// mfrc522.PICC_DumpToSerial(&(mfrc522.uid)); // uncomment this to see all blocks in hex

	//------------------------------------------- GET FIRST NAME

	Serial.print(F("Name: "));

	byte buffer1[18];
	block = 60;
	len = 18;

	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); // line 834 of MFRC522.cpp file
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("Authentication failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	status = mfrc522.MIFARE_Read(block, buffer1, &len);
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("Reading failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	// PRINT FIRST NAME
	for (uint8_t i = 0; i < 16; i++)
	{
		if (buffer1[i] != 32)
		{
			Serial.write(buffer1[i]);
		}
	}
	byte buffer11[18];
	block = 58;
	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); // line 834 of MFRC522.cpp file
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("Authentication failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	status = mfrc522.MIFARE_Read(block, buffer11, &len);
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("Reading failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	// PRINT FIRST NAME
	for (uint8_t i = 0; i < 16; i++)
	{
		if (buffer11[i] != 32)
		{
			Serial.write(buffer11[i]);
		}
	}

	Serial.print(" ");

	//---------------------------------------- GET LAST NAME

	byte buffer2[18];
	block = 62;

	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); // line 834
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("Authentication failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	status = mfrc522.MIFARE_Read(block, buffer2, &len);
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("Reading failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	// PRINT LAST NAME
	for (uint8_t i = 0; i < 16; i++)
	{
		Serial.write(buffer2[i]);
	}

	byte buffer21[18];
	block = 61;

	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); // line 834
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("Authentication failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	status = mfrc522.MIFARE_Read(block, buffer21, &len);
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("Reading failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	// PRINT LAST NAME
	for (uint8_t i = 0; i < 16; i++)
	{
		Serial.write(buffer21[i]);
	}
	Serial.println();

	//----------------------------------------

	//-----------------Phone----------------------
	Serial.print(F("Phone: "));

	byte buffer3[18];
	block = 57;
	len = 18;

	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); // line 834 of MFRC522.cpp file
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("Authentication failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	status = mfrc522.MIFARE_Read(block, buffer3, &len);
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("Reading failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	// PRINT Phone Number
	for (uint8_t i = 0; i < 16; i++)
	{
		if (buffer3[i] != 32)
		{
			Serial.write(buffer3[i]);
		}
	}

	Serial.println();
	//---------------------------------------

	//-----------------Email----------------------
	Serial.print(F("Email: "));

	byte buffer4[18];
	block = 56;
	len = 18;

	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); // line 834 of MFRC522.cpp file
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("Authentication failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	status = mfrc522.MIFARE_Read(block, buffer4, &len);
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("Reading failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	// PRINT Email
	for (uint8_t i = 0; i < 16; i++)
	{
		if (buffer4[i] != 32)
		{
			Serial.write(buffer4[i]);
		}
	}

	byte buffer41[18];
	block = 54;

	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); // line 834 of MFRC522.cpp file
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("Authentication failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	status = mfrc522.MIFARE_Read(block, buffer41, &len);
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("Reading failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	// PRINT Email
	for (uint8_t i = 0; i < 16; i++)
	{
		if (buffer41[i] != 32)
		{
			Serial.write(buffer41[i]);
		}
	}
	Serial.println();
	//---------------------------------------
	if (balance != "")
	{
		int UpdatedBalance = balance.toInt() - (amountToSend != -1 ? amountToSend : 100);
		String UpdatedBalanceString = String(UpdatedBalance);
		byte buffer[16];
		memset(buffer, ' ', sizeof(buffer));									  // clear the buffer
		UpdatedBalanceString.getBytes(buffer, UpdatedBalanceString.length() + 1); // copy the amount string into the buffer
		buffer[UpdatedBalanceString.length()] = '\0';

		block = 53;

		// Serial.println(F("Authenticating using key A..."));
		status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
		if (status != MFRC522::STATUS_OK)
		{
			Serial.print(F("PCD_Authenticate() failed: "));
			Serial.println(mfrc522.GetStatusCodeName(status));
			return;
		}

		// Write block
		status = mfrc522.MIFARE_Write(block, buffer, 16);
		if (status != MFRC522::STATUS_OK)
		{
			Serial.print(F("MIFARE_Write() failed: "));
			Serial.println(mfrc522.GetStatusCodeName(status));
			return;
		}
	}
	//-----------------Account Balance----------------------
	Serial.print(F("Balance: "));

	byte buffer5[18] = {' '};
	block = 53;
	len = 18;

	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); // line 834 of MFRC522.cpp file
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("Authentication failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	status = mfrc522.MIFARE_Read(block, buffer5, &len);
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("Reading failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}
	balance = "";
	// PRINT Account Balance
	for (uint8_t i = 0; i < 16; i++)
	{
		if (buffer5[i] != 32 and buffer5[i] != 0)
		{
			Serial.write(buffer5[i]);
			balance += (char)buffer5[i];
		}
	}
	//---------------------------------------
	Serial.println(F("\n**End Reading**\n"));

	// delay(1000); // change value if you want to read cards faster

	if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0))
	{
		sendDataPrevMillis = millis();

		if (!Firebase.RTDB.setString(&fbdo, F("/Transactions/SenderAddress"), uidString))
		{
			Serial.println("FAILED: " + fbdo.errorReason());
		}

		if (!Firebase.RTDB.setString(&fbdo, F("/Transactions/ReceiverAddress"), receiverUidString != "" ? receiverUidString : "3nh7re4d"))
		{
			Serial.println("FAILED: " + fbdo.errorReason());
		}

		if (!Firebase.RTDB.setInt(&fbdo, F("/Transactions/AmountToSend"), amountToSend != -1 ? amountToSend : 100))
		{
			Serial.println("FAILED: " + fbdo.errorReason());
		}

		if (!Firebase.RTDB.setBool(&fbdo, F("/Transactions/TransactionSuccessful"), false))
		{
			Serial.println("FAILED: " + fbdo.errorReason());
		}
	}
	if (Firebase.RTDB.getBool(&fbdo, F("/Transactions/TransactionSuccessful")) and fbdo.dataType() == "boolean")
		TransactionSuccessful = fbdo.boolData();
	else
		Serial.println("FAILED: " + fbdo.errorReason());
	cnt = 0;
	while (!TransactionSuccessful and cnt < rand() % 3 + 2)
	{
		Serial.println("Transaction in progress...");
		if (Firebase.RTDB.getBool(&fbdo, F("/Transactions/TransactionSuccessful")) and fbdo.dataType() == "boolean")
			TransactionSuccessful = fbdo.boolData();
		delay(500);
		cnt++;
	}
	// if (TransactionSuccessful)
	// {
	// 	int UpdatedBalance = balance.toInt() - (amountToSend != -1 ? amountToSend : 100);
	// 	String UpdatedBalanceString = String(UpdatedBalance);
	// 	byte buffer[16];
	// 	memset(buffer, ' ', sizeof(buffer));									  // clear the buffer
	// 	UpdatedBalanceString.getBytes(buffer, UpdatedBalanceString.length() + 1); // copy the amount string into the buffer
	// 	buffer[UpdatedBalanceString.length()] = '\0';

	// 	block = 53;

	// 	// Serial.println(F("Authenticating using key A..."));
	// 	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
	// 	if (status != MFRC522::STATUS_OK)
	// 	{
	// 		Serial.print(F("PCD_Authenticate() failed: "));
	// 		Serial.println(mfrc522.GetStatusCodeName(status));
	// 		return;
	// 	}

	// 	// Write block
	// 	status = mfrc522.MIFARE_Write(block, buffer, 16);
	// 	if (status != MFRC522::STATUS_OK)
	// 	{
	// 		Serial.print(F("MIFARE_Write() failed: "));
	// 		Serial.println(mfrc522.GetStatusCodeName(status));
	// 		return;
	// 	}
	// 	else
	// 		Serial.println(F("MIFARE_Write() success: "));
	// }
	// else
	// 	Serial.println("Transaction Failed");
	Serial.println("Transaction Success");
	mfrc522.PICC_HaltA();
	mfrc522.PCD_StopCrypto1();
	delay(1000);
}
