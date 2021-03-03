#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include "cryptlib.h"
#include "sha.h"
#include "hex.h"
#include "files.h"
#include "aes.h"
#include "modes.h"

#ifdef _WIN32
	#include <windows.h>
	#define sleep(x) Sleep(1000* (x))
#else
	#error "System not Supported"
#endif

using namespace std;

inline string encrypt(string msg, string hkey) {

	// this encryption/decryption uses a fixed iv and a different key (based on user input).
	// i know this is not a secure way of doing, encryption is just here as a simple barrier. 
	// this program should only run locally on a trusted machine.
	// if someone has access to your machine and wants your info they would be able to get it.

	using namespace CryptoPP;

	byte key[AES::MAX_KEYLENGTH]; //creating a 32Bytes/256bits key
	byte iv[AES::BLOCKSIZE];
	memcpy(key, hkey.c_str(), AES::MAX_KEYLENGTH);
	memset(iv, 0x20, AES::BLOCKSIZE);

	string cipheredtext;

	CryptoPP::AES::Encryption aesEncryption(key, AES::MAX_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);

	StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(cipheredtext));
	stfEncryptor.Put(reinterpret_cast<const unsigned char*>(msg.c_str()), msg.length() + 1);
	stfEncryptor.MessageEnd();

	//cout << "String out of encryption: " << cipheredtext << endl;

	string encodedcipheredtext;
	StringSource ss(cipheredtext, true, new HexEncoder(new StringSink(encodedcipheredtext)));

	//cout << "String after transformation: " << encodedcipheredtext << endl;

	return encodedcipheredtext;
}

inline string decrypt(string msg, string hkey) {

	// this encryption/decryption uses a fixed iv and a different key (based on user input).
	// i know this is not a secure way of doing, encryption is just here as a simple barrier. 
	// this program should only run locally on a trusted machine.
	// if someone has access to your machine and wants your info they would be able to get it.

	using namespace CryptoPP;
	string decryptedtext;

	//cout << "hex msg: " << msg << endl;

	string decodedmsg;
	StringSource ss(msg, true, new HexDecoder(new StringSink(decodedmsg)));

	//cout << "cyphered text: " << decodedmsg << endl;

	byte key[AES::MAX_KEYLENGTH]; //creating a 32Bytes/256bits key
	byte iv[AES::BLOCKSIZE];
	memcpy(key, hkey.c_str(), AES::MAX_KEYLENGTH);
	memset(iv, 0x20, AES::BLOCKSIZE);

	AES::Decryption aesDecrypt(key, AES::MAX_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecrypt(aesDecrypt, iv);

	StreamTransformationFilter stfDecrypt(cbcDecrypt, new CryptoPP::StringSink(decryptedtext));
	stfDecrypt.Put(reinterpret_cast<const unsigned char*>(decodedmsg.c_str()), decodedmsg.size());
	stfDecrypt.MessageEnd();

	//cout << "decrypted text: " << decryptedtext << endl;

	return decryptedtext;

}

inline void checkcaps() {
	//input event
	INPUT ip;

	//generic keyboard event
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	//getting keyboard layout
	HKL kbl = GetKeyboardLayout(0);

	//check if capslock is on
	if (GetKeyState(0x14)) {

		//generating capslock input
		ip.ki.wVk = 0x14;					//capslock code
		ip.ki.dwFlags = 0;					//indicating key-down
		SendInput(1, &ip, sizeof(ip));		//sending input
		ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
		SendInput(1, &ip, sizeof(ip));		//sending input;
	}
}

inline void type(string msg){
	//variable for uppercased letters
	bool upper = 0;
	
	//creating input event
	INPUT ip;

	//generic keyboard event
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	//getting keyboard layout	
	HKL kbl = GetKeyboardLayout(0);

	checkcaps();

	HWND val_hwnd = FindWindowW(NULL, L"Riot Client");
	SetForegroundWindow(val_hwnd);

	//generating input input
	for(size_t i=0; i < msg.length(); i++){
		char c = msg[i];

		HWND val_hwnd = FindWindowW(NULL, L"Riot Client");
		SetForegroundWindow(val_hwnd);

			switch(c){

			case '!':
			{

				ip.ki.wVk = 0x00A0;					//shift code
				ip.ki.dwFlags = 0;					//indicating key-down
				SendInput(1, &ip, sizeof(ip));		//sending input

				ip.ki.wVk = 0x0031;					//1 code
				ip.ki.dwFlags = 0;					//indicating key-down
				SendInput(1, &ip, sizeof(ip));

				ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
				SendInput(1, &ip, sizeof(ip));		//sending input

				ip.ki.wVk = 0x00A0;					//shift code			
				ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
				SendInput(1, &ip, sizeof(ip));		//sending input
				break;
			}

			case '@':
			{

				ip.ki.wVk = 0x00A0;					//shift code
				ip.ki.dwFlags = 0;					//indicating key-down
				SendInput(1, &ip, sizeof(ip));		//sending input

				ip.ki.wVk = 0x0032;					//2 code
				ip.ki.dwFlags = 0;					//indicating key-down
				SendInput(1, &ip, sizeof(ip));

				ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
				SendInput(1, &ip, sizeof(ip));		//sending input

				ip.ki.wVk = 0x00A0;					//shift code			
				ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
				SendInput(1, &ip, sizeof(ip));		//sending input
				break;
			}

			case '#':
			{

				ip.ki.wVk = 0x00A0;					//shift code
				ip.ki.dwFlags = 0;					//indicating key-down
				SendInput(1, &ip, sizeof(ip));		//sending input

				ip.ki.wVk = 0x0033;					//3 code
				ip.ki.dwFlags = 0;					//indicating key-down
				SendInput(1, &ip, sizeof(ip));

				ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
				SendInput(1, &ip, sizeof(ip));		//sending input

				ip.ki.wVk = 0x00A0;					//shift code			
				ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
				SendInput(1, &ip, sizeof(ip));		//sending input
				break;
			}

			case '$':
			{

				ip.ki.wVk = 0x00A0;					//shift code
				ip.ki.dwFlags = 0;					//indicating key-down
				SendInput(1, &ip, sizeof(ip));		//sending input

				ip.ki.wVk = 0x0034;					//4 code
				ip.ki.dwFlags = 0;					//indicating key-down
				SendInput(1, &ip, sizeof(ip));

				ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
				SendInput(1, &ip, sizeof(ip));		//sending input

				ip.ki.wVk = 0x00A0;					//shift code			
				ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
				SendInput(1, &ip, sizeof(ip));		//sending input
				break;
			}

			case '%':
			{

				ip.ki.wVk = 0x00A0;					//shift code
				ip.ki.dwFlags = 0;					//indicating key-down
				SendInput(1, &ip, sizeof(ip));		//sending input

				ip.ki.wVk = 0x0035;					//5 code
				ip.ki.dwFlags = 0;					//indicating key-down
				SendInput(1, &ip, sizeof(ip));

				ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
				SendInput(1, &ip, sizeof(ip));		//sending input

				ip.ki.wVk = 0x00A0;					//shift code			
				ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
				SendInput(1, &ip, sizeof(ip));		//sending input
				break;
			}

			case '&':
			{

				ip.ki.wVk = 0x00A0;					//shift code
				ip.ki.dwFlags = 0;					//indicating key-down
				SendInput(1, &ip, sizeof(ip));		//sending input

				ip.ki.wVk = 0x0037;					//7 code
				ip.ki.dwFlags = 0;					//indicating key-down
				SendInput(1, &ip, sizeof(ip));

				ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
				SendInput(1, &ip, sizeof(ip));		//sending input

				ip.ki.wVk = 0x00A0;					//shift code			
				ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
				SendInput(1, &ip, sizeof(ip));		//sending input
				break;
			}

			case '*':
			{

				ip.ki.wVk = 0x00A0;					//shift code
				ip.ki.dwFlags = 0;					//indicating key-down
				SendInput(1, &ip, sizeof(ip));		//sending input

				ip.ki.wVk = 0x0038;					//8 code
				ip.ki.dwFlags = 0;					//indicating key-down
				SendInput(1, &ip, sizeof(ip));

				ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
				SendInput(1, &ip, sizeof(ip));		//sending input

				ip.ki.wVk = 0x00A0;					//shift code			
				ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
				SendInput(1, &ip, sizeof(ip));		//sending input
				break;
			}
			
			default:
			{
				//checking if the key should be uppercase		
				if (isupper(c)) {
					//printf("%c\n",c);
					//generating capslock input
					ip.ki.wVk = 0x14;					//caps code
					ip.ki.dwFlags = 0;					//indicating key-down
					SendInput(1, &ip, sizeof(ip));		//sending input
					ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
					SendInput(1, &ip, sizeof(ip));		//sending input

					upper = 1;
				}

				ip.ki.wVk = VkKeyScanExA(c, kbl);	//getting the key's code
				ip.ki.dwFlags = 0;					//indicating key-down
				SendInput(1, &ip, sizeof(ip));		//sending input
				ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
				SendInput(1, &ip, sizeof(ip));		//sending input		

				if (upper == 1) {
					//generating capslock input
					ip.ki.wVk = 0x14;					//caps code
					ip.ki.dwFlags = 0;					//indicating key-down
					SendInput(1, &ip, sizeof(ip));		//sending input
					ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
					SendInput(1, &ip, sizeof(ip));		//sending input

					upper = 0;
				}
				break;
			}
			}
	}
}

inline int GetLastId() {
	fstream VASFile;
	string key = "ID=";
	VASFile.open("VAS.conf", ios::in);
	string stringfromfile = "";
	if (!VASFile) {
		cout << "Fail to open file." << endl;
	}
	else {
		string line;
		int i = 0;
		while (getline(VASFile, line)) {
			i++;
			size_t found = line.rfind(key);
			if (found != std::string::npos) {
				line.replace(found, key.length(), "");
				stringfromfile = line;
				line.clear();
			}
		}
	}
	VASFile.close();
	int ID = -1;
	if (stringfromfile != "") {
		ID = std::stoi(stringfromfile);
	}
	return ID;
}

inline string GetFromFile(string key) {
	fstream VASFile;
	VASFile.open("VAS.conf", ios::in);
	string stringfromfile;
	if (!VASFile) {
		cout << "Fail to open file." << endl;
	}
	else {
		string line;
		while (getline(VASFile, line)) {
			size_t found = line.rfind(key);
			if (found != std::string::npos) {
				line.replace(found, key.length(), "");
				stringfromfile = line;
				break;
			}
		}
	}
	VASFile.close();
	return stringfromfile;
}

inline int comparerank(int therank){
	//	NEW RULE: 02/MARCH/2021
	//	IRON 1 - BRONZE 3 CAN PLAY WITH UP TO SILVER 3	
	//	SILVER 1 - PLAT 3 SAME RULE AS IT WAS
	//	RADIANT CAN ONLY PLAY WITH IMORTALS
	//	IMORTAL CAN PLAY WITH DIAMOND
	int high, low;

	if (GetLastId() != -1) {
		//	Checking if the rank < silver
		if (therank < 6) {
			//	=< silver ranks can play with iron 1 to silver 3
			low = 0;
			high = 8;
		}
		else {
			// > silver ranks can only play with ppl 3 ranks above or below
			if (therank > 5 && therank < 9) {
				low = 0;
			}
			else {
				low = therank - 4;
			}
			high = therank + 4;
		}

		for (int i = 0; i <= GetLastId(); i++) {
			stringstream ss;
			ss << "rank" << i << ":";
			string info = GetFromFile(ss.str());
			int intinfo = atoi(info.c_str());

			if (intinfo > low && intinfo < high) {
				//	make sure the account is not Radiant
				if (intinfo == 19) {
					//	radiant accounts can only play with imortals
					if (therank == 18) {
						return i;
					}
				}
				else {
					return i;
				}
			}
		}
		return -1;
	}
	else {
		return -1;
	}
}

inline void opengame(string game){
	stringstream ss;
	ss << "start \"\" \"" << game << "\" --launch-product=valorant --launch-patchline=live";
	string command = ss.str();
	system(command.c_str());
}

inline void tabbing(){
	//input event
	INPUT ip;

	//generic keyboard event
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	
	//getting keyboardlayout
	HKL kbl = GetKeyboardLayout(0);
	
	//tabbing
	ip.ki.wVk = 0x09;					//tab code
	ip.ki.dwFlags = 0;					//indicating key-down
	SendInput(1, &ip, sizeof(ip));		//sending input
	ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
	SendInput(1, &ip, sizeof(ip));		//sending input
}

inline void ENTER() {
	//input event
	INPUT ip;

	//generic keyboard event
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	//getting keyboardlayout
	HKL kbl = GetKeyboardLayout(0);

	//tabbing
	ip.ki.wVk = 0x0D;					//enter code
	ip.ki.dwFlags = 0;					//indicating key-down
	SendInput(1, &ip, sizeof(ip));		//sending input
	ip.ki.dwFlags = KEYEVENTF_KEYUP;	//indicating key-up
	SendInput(1, &ip, sizeof(ip));		//sending input
}

inline void waitgame(){
	bool ValOpen = 0; //variable that will be used for the do-while loop
	
	printf("Waiting for Valorant to Open...\n");
	do{
		HWND val_hwnd = FindWindowW(NULL, L"Riot Client"); //checking if the login window for Valorant is open
		
		if(val_hwnd != 0){
			ValOpen = 1;	 //braking the loop
			sleep(5);		//not the best solution to solve the timing problem of the login screen but should work fine
		}
		sleep(1);	//here just so the program isn't constantly checking, checking once every second should be fine

	} while(ValOpen == 0);
}

inline string RankToString(int rank) {
	string str;
	switch (rank) {
	case 0:
	{
		str = "IR1";
		break;
	}
	case 1:
	{
		str = "IR2";
		break;
	}
	case 2:
	{
		str = "IR3";
		break;
	}
	case 3:
	{
		str = "BR1";
		break;
	}
	case 4:
	{
		str = "BR2";
		break;
	}
	case 5:
	{
		str = "BR3";
		break;
	}
	case 6:
	{
		str = "SI1";
		break;
	}
	case 7:
	{
		str = "SI2";
		break;
	}
	case 8:
	{
		str = "SI3";
		break;
	}
	case 9:
	{
		str = "GO1";
		break;
	}
	case 10:
	{
		str = "GO2";
		break;
	}
	case 11:
	{
		str = "GO3";
		break;
	}
	case 12:
	{
		str = "PL1";
		break;
	}
	case 13:
	{
		str = "PL2";
		break;
	}
	case 14:
	{
		str = "PL3";
		break;
	}
	case 15:
	{
		str = "DI1";
		break;
	}
	case 16:
	{
		str = "DI2";
		break;
	}
	case 17:
	{
		str = "DI3";
		break;
	}
	case 18:
	{
		str = "IM";
		break;
	}

	case 19:
	{
		str = "RAD";
		break;
	}
	default:
	{
		str = "";
		break;
	}
	}
	return str;
}

inline string hashing(string msg) {
	
	std::stringstream output;
	std::string outputstring;
	std::string digest;
	using namespace CryptoPP;
	HexEncoder encoder = (new FileSink(output));
	SHA256 hash;
	hash.Update((const byte*)msg.data(), msg.size());
	digest.resize(hash.DigestSize());
	hash.Final((byte*)&digest[0]);

	StringSource(digest, true, new Redirector(encoder));

	outputstring = output.str();

	return outputstring;
}

inline bool FileExist() {
	ifstream FILE("VAS.conf");		//opening file
	if (FILE.good() == TRUE) {		//checking if file exists
		cout << "File Exists" << endl;
		FILE.close();	//always close your files kids
	}
	else {
		cout << "File Does Not Exist" << endl;
		FILE.close();	//always close your files kids
	}
	return FILE.good();	//sending the answer back to main
}

inline void SendToFile(string msg){
	fstream VASFile;
	VASFile.open("VAS.conf", std::fstream::out | std::fstream::app);
	if (!VASFile) {
		cout << "Fail to Open File." << endl;
	}else {
		VASFile << msg <<endl;
	}
	VASFile.close();
}

inline void AddAcc(string nickname, string uname,  string passwd, int rank) {
	int id = GetLastId();
	if (id < 0) {
		id = 0;
	}
	else {
		id++;
	}

	for (int i = 0; i < 5; i++) {
		string msg;
		stringstream ss;
		switch (i){
			case 0: {
				ss << "nickname" << id << ":" << nickname;
				break;
			}
			case 1: {
				ss << "uname" << id << ":" << uname;
				break;
			}
			case 2: {
				ss << "passwd" << id << ":" << passwd;
				break;
			}
			case 3:{
				ss << "ID=" << id;
				break;
			}
			case 4: {
				ss << "rank" << id << ":" << rank;
				break;
			}
		}
		msg = ss.str();

		SendToFile(msg);
	}

}

inline void DeleteAcc(int id) {
	stringstream sskey, sskey2;
	
	sskey << "nickname" << id << ":";
	sskey2 << "rank" << id << ":";

	fstream VASFile;
	fstream temp;
	
	VASFile.open("VAS.conf", ios::in);
	temp.open("tempVAS.conf", ios::out);

	if (!VASFile) {
		cout << "Failed to open file." << endl;
		temp.close();
		VASFile.close();
	}
	else {
		BOOL Deleting = FALSE;
		BOOL StopDeleting = FALSE;
		string line;
		while (getline(VASFile, line)) {
			size_t found = line.rfind(sskey.str());
			if (found != std::string::npos) {
				Deleting = TRUE;
			}
			if (StopDeleting) {
				Deleting = FALSE;
			}
			if (Deleting) {
				size_t found2 = line.rfind(sskey2.str());
				if (found2 != std::string::npos) {
					StopDeleting = TRUE;
				}
			}
			else {
				temp << line << endl;
			}
		}	
		temp.close();
		VASFile.close();
		remove("VAS.conf");
		rename("tempVAS.conf", "VAS.conf");
	}

}

inline void ChangeAccInfo(string infochange, string newinfo) {
	string key;
	stringstream sskey;
	BOOL changing = FALSE;
	sskey << infochange;
	key = sskey.str();
	
	fstream VASFile;
	fstream temp;
	VASFile.open("VAS.conf", ios::in);
	temp.open("tempVAS.conf", ios::out);
	if (!VASFile) {
		cout << "Fail to open file." << endl;
		temp.close();
		VASFile.close();
	}
	else {
		string line;
		while (getline(VASFile, line)) {
			size_t found = line.rfind(infochange);
			if (found != std::string::npos) {
				changing = TRUE;
			}
			else {
				temp << line << endl;
			}
			
			if (changing) {
					stringstream ss;
					ss << infochange << newinfo;
					line = ss.str();
					temp << line << endl;
					changing = FALSE;
			}
		}
		temp.close();
		VASFile.close();
		remove("VAS.conf");
		rename("tempVAS.conf", "VAS.conf");
	}
}

