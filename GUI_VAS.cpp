//
//	NEW FILE STRUCTURE:	
//
//	NICKNAME<ID>:
//	UNAME<ID>:
//	PASSWD<ID>:
//	ID=<ID>	//keeping it the same, hope it doesent bug monkaS
//	RANK<ID>:
//
//	to do list:
//	add support to # $ % ! & *											//	DONE
//
//	check bugs with @													//	SEEMS FIXED
//
//	look into adding tabbing/enter support on menus						// Prolly gonna subclass some controls
//
//	add RiotClient.exe as the main file type on the explorer box		//	DONE
//
//	fix size of nickname controls in main menu, to work with 14chars+	//	Looks good
//	(14 chars is the size of Val's names)								//
//
//	might change encrypt/decrypt again									//
//	having a dynamic IV would be nice, thinking of 3 options for it:	//
//		1-	Random IV that would be saved to file						//
//		2-	Make it so the IV is based on the hashed password			//
//		3-	Make it so the IV is based on the acc ID					//
//

#include <windows.h>
#include "Valorant.h"

#define SETUP_OK 100			//
#define CREATE_ACCOUNT 101		//
#define ADD_ACCOUNT 102			//
#define CHECK_PASSWD 103		//	Will be used for the HMENUS
#define SELECT_FILE_SETUP 104	//
#define RANK_WINDOW 105			//
#define RANKED_LOGIN 106		//
#define CHANGE_ACC_BTN 107		//
#define DELETE_ACC_BTN 108		//
#define CONFIRM_DELETE_BTN 109	//
#define CANCEL_DELETE_BTN 110	//

#define WM_COMPLETE (WM_USER + 2)	//	Used during the login process to update the window
#define WM_COMPLETE2 (WM_USER + 3)	//

void MainMenu(HINSTANCE currentInstance);															//
LRESULT CALLBACK MainMenuWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);	//	Main Menu Functions
void AddControlsMainMenu(HWND hwnd);																//

void SetUpWindow(HINSTANCE currentInstance);													//
LRESULT CALLBACK SetUpWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);	//	Setup Functions
void AddControlsSetUp(HWND hwnd);																//

void CheckPassword(HINSTANCE currentInstance);													//
LRESULT CALLBACK CHKPWWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);	//	Check Password Functions
void AddControlsCheckPass(HWND hwnd);															//

void AddAccWindow(HINSTANCE currentInstance);													//
LRESULT CALLBACK AddAccWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);	//	Add Account Functions
void AddControlsAddAcc(HWND hwnd);																//

string OpenFileBox(HWND hwnd);		//	Openfile to get the path to "RiotClient"
string ConvertToString(HWND input);	//	Converting edit controls to strings
void FillVector();					//	Filling up the vectors

string RemoveLastChar(string str);	//	Removing the last char from a string

void LOGIN(HINSTANCE currentInstance);															//
LRESULT CALLBACK LOGINWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);	//
void AddControlsLOGIN(HWND hwnd);																//	Login Functions
void AddControlsLOGIN2(HWND hwnd);																//
void AddControlsLOGIN3(HWND hwnd);																//

void SelectRankWindow(HINSTANCE currentInstance);												//
LRESULT CALLBACK RankWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);	//	Select Rank Functions
void AddControlsRankWindow(HWND hwnd);															//

void RankedLoginWindow(HINSTANCE currentInstance);														//
LRESULT CALLBACK RankedLoginWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);	//	Ranked Based Login Functions
void AddControlsRankedLogin(HWND hwnd);																	//

void ChangeAccWindow(HINSTANCE currentInstance);													//
LRESULT CALLBACK ChangeAccWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);	//	Change Account Info Functions
void AddControlsChangeAcc(HWND hwnd);																//

void DeleteAccWindow(HINSTANCE currentInstance);													//
LRESULT CALLBACK DeleteAccWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);	//	Confirm Delete Account Functions
void AddControlsDeleteAcc(HWND hwnd);																//

BOOL STAY_IN_MENU = FALSE;			//	Will be used to keep the loop going so it goes back to the main menu
BOOL PassedCheckPW = FALSE;			//	Will be used to go to the main menu if the password is correct
BOOL FILE_PATH_EXIST = FALSE;		//	Will be used to make sure there's a file path
BOOL HAS_RANK = FALSE;				//	Will be used to check if a rank was selected
HWND pw1, pw2;						//	Will store the edit controls for the passwords when setting up
string HASHEDPW = "";				//	Will store the hashed password
HINSTANCE cInstance;				//	Global variable for HINSTANCE
HWND addnick, adduname, addpw;		//	Will store nickname, username and password when adding accounts
HWND chnick, chuname, chpw;			//	Will store nickname, username and password when changing account info
HWND checkpw;						//	Will store the password when starting the program with an existing VASFile
string path = "";					//	Will store the path to RiotClient
string LOGIN_UNAME, LOGIN_PASSWD;	//	Will be used to store username and password for login
vector<UINT> LOGIN_ACC;				//	Vector for the Login buttons HMENUS
vector<UINT> RANK_ACC;				//	Vector for the Change Rank buttons HMENUS
vector<UINT> CHANGES_ACC;			//	Vector for the change account info buttons HMENUS
int LOGIN_ID;						//	Will store the wanted ID
int SELECTED_RANK = -1;				//	Will store the wanted rank
vector<int> ALL_RANKS_BTN = { 200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218 };	//	Vector containg all ranks


int WINAPI WinMain(HINSTANCE currentInstance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdCount) {
	cInstance = currentInstance;	//	Storing the HINSTANCE on the global variable

	if (FileExist() == FALSE) {			//	Check if files exist
		SetUpWindow(currentInstance);	//	Setup menu
		while (STAY_IN_MENU) {			//
			MainMenu(currentInstance);	//	Loop for the main menu
		}								//
	}else {
		CheckPassword(currentInstance);	//	
		if (PassedCheckPW) {				//	Making sure the password was correct
			while (STAY_IN_MENU) {			//
				MainMenu(currentInstance);	//	Loop for the main menu
			}								//
		}									//
	}

	return 0;
}

void MainMenu(HINSTANCE currentInstance) {

	STAY_IN_MENU = FALSE;	//	Breking the main menu loop
	
	HAS_RANK = FALSE;		//	Resetting variables
	SELECTED_RANK = -1;		//

	//Registering Window Class
	const char* CLASS_NAME = "Valorant Account Selector - Main Menu";
	WNDCLASS wc{};
	wc.hInstance = currentInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = MainMenuWindowProcessMessages;
	RegisterClass(&wc);


	//creating window
	CreateWindow(CLASS_NAME, "GUI VAS - Main Menu",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,			//style
		CW_USEDEFAULT, CW_USEDEFAULT,				//initial position
		600, 800,									//size
		nullptr, nullptr, nullptr, nullptr);

	//window loop
	MSG msg{};
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK MainMenuWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
	
	switch (msg) {

	case WM_CREATE: {
		AddControlsMainMenu(hwnd);	// Creating controls
		return 0;
	}
	case WM_COMMAND: 
	{
		switch (param) {

		case CREATE_ACCOUNT:
		{
			DestroyWindow(hwnd);		//	Destroy the current window and
			AddAccWindow(cInstance);	//	open the add account window
			return 0;
		}

		case RANKED_LOGIN:
		{
			DestroyWindow(hwnd);			//	Destroy current window
			RankedLoginWindow(cInstance);	//	Open the ranked based login
			return 0;
		}

		default:
		{	
			//
			//	This section is will check if a login, change rank
			//	or change acc info button was used.
			//

			if (GetLastId() != -1) {	//	Checking there are Accounts on the file
				for (int i = 0; i <= GetLastId(); i++) {	//	Loop
					//	Check Login Buttons
					if (LOGIN_ACC.at(i) == param) {
						LOGIN_ID = i;
						DestroyWindow(hwnd);
						LOGIN(cInstance);
						PostQuitMessage(0);	//	making sure to kill the process
						return 0;
					}
					else {
						//	Check Change Rank Buttons
						if (RANK_ACC.at(i) == param) {
							SelectRankWindow(cInstance);
							if (SELECTED_RANK == -1) {	//	Check if a rank was chossen
								//MessageBox(hwnd, "No rank selected", "NO RANK", MB_OK);		//no need for error message here
								return 0;
							}
							else {
								stringstream toint;								//
								toint << SELECTED_RANK;							//	Get the wanted rank
								stringstream changekey;							//	and send the new rank to 
								changekey << "rank" << i << ":";				//	the ChangeAccInfo function
								ChangeAccInfo(changekey.str(), toint.str());	//
								
								DestroyWindow(hwnd);	//	Destroy window and goes back to the main menu
								STAY_IN_MENU = TRUE;	//
								
								return 0;
							}
							return 0;
						}
						else {
							//	Check change acc info buttons
							if (CHANGES_ACC.at(i) == param) {	
								LOGIN_ID = i;						//
								STAY_IN_MENU = TRUE;				//	Gets the acc id, tells the loop to stay in menu after
								DestroyWindow(hwnd);				//	and goes to the ChangeAccWindow function
								ChangeAccWindow(cInstance);			//
								return 0;
							}
						}
					}
				}
			}else {
				//	Default
				return DefWindowProc(hwnd, msg, param, lparam);
			}
			return 0;
		}

		}
		return 0;
	}

	case WM_DESTROY: {	//	Quitting the window
		PostQuitMessage(0);
		return 0;
	}
	default:
		return DefWindowProc(hwnd, msg, param, lparam);
	}
	
}

void AddControlsMainMenu(HWND hwnd) {

	CreateWindow("static", "Accounts:", WS_VISIBLE | WS_CHILD, 5, 10, 100, 20, hwnd, NULL, NULL, NULL);			//
	CreateWindow("static", "Change Rank:", WS_VISIBLE | WS_CHILD , 180, 10, 50, 40, hwnd, NULL, NULL, NULL);	//	Text for the top of the window
	CreateWindow("static", "LOGIN:", WS_VISIBLE | WS_CHILD , 245, 10, 50, 40, hwnd, NULL, NULL, NULL);			//

	CreateWindow("Button", "Add Account", WS_VISIBLE | WS_CHILD | WS_BORDER, 300, 10, 95, 20, hwnd, (HMENU)CREATE_ACCOUNT, NULL, NULL);			//	Buttons
	CreateWindow("Button", "Ranked-Based Login", WS_VISIBLE | WS_CHILD | WS_BORDER , 400, 10, 150, 20, hwnd, (HMENU)RANKED_LOGIN, NULL, NULL);	//
	
	//	Filling up all vectors before starting the loop
	FillVector();

	if (GetLastId() != -1) {	//	Checking if there are any Accounts on the file
		int y = 20;				//	Setting the base y value

		for (int i = 0; i <= GetLastId(); i++ ) {	//	Starting the Loop
			stringstream exc;							//
			exc << "nickname" << i << ":";				//	Making sure the ID actually has an acc
			string exc2 = GetFromFile(exc.str());		//	attached to it
			if (exc2 != "") {							//
				
				stringstream ss;						//
				ss << "nickname" << i << ":";			//	Getting the nickname
				string nick = GetFromFile(ss.str());	//
				string nick2 = decrypt(nick, HASHEDPW);	//

				stringstream ss2;							//
				ss2 << "rank" << i << ":";					//	Getting the rank
				string namerank = GetFromFile(ss2.str());	//

				int c = 0;												//
				for (; c < namerank.length(); c++) {					//
					if (isdigit(namerank[c])) {							//	Was having some trouble to get the int from the file
						break;											//	seems to be working now.
					}													//	int is then used to get the rank string to show on the menu
				}														//
				namerank = namerank.substr(c, namerank.length() - c);	//
				int numberrank = atoi(namerank.c_str());				//		STILL BUGGED (FIXED?)
				y += 25;	//	Adding 25 to the y value to create space between accoutns

				CreateWindow("Static", nick2.c_str(), WS_VISIBLE | WS_CHILD | WS_OVERLAPPED, 5, y, 160, 20, hwnd, NULL, NULL, NULL);	//shows the nick on the menu

				CreateWindow("button", RankToString(numberrank).c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, 180, y, 50, 20, hwnd, (HMENU)(UINT_PTR)RANK_ACC.at(i), NULL, NULL);	//shows the rank on the menu

				CreateWindow("Button", "LOGIN", WS_VISIBLE | WS_CHILD | WS_BORDER, 245, y, 50, 20, hwnd, (HMENU)(UINT_PTR)LOGIN_ACC.at(i), NULL, NULL);	//creates a login button on the menu

				CreateWindow("button", "Configure Acc", WS_VISIBLE | WS_CHILD | WS_BORDER, 315, y, 100, 20, hwnd, (HMENU)(UINT_PTR)CHANGES_ACC.at(i), NULL, NULL);	//creates the configure acc button
			}
		}

	}


}

void SetUpWindow(HINSTANCE currentInstance) {

	
	//Registering Window Class
	const char* CLASS_NAME = "Valorant Account Selector - Set Up";
	WNDCLASS wc{};
	wc.hInstance = currentInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = SetUpWindowProcessMessages;
	RegisterClass(&wc);


	//creating window
	CreateWindow(CLASS_NAME, "GUI VAS - SetUp",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,			//style
		CW_USEDEFAULT, CW_USEDEFAULT,		//initial position
		600, 600,							//size
		nullptr, nullptr, nullptr, nullptr);

	//window loop
	MSG msg2{};
	while (GetMessage(&msg2, nullptr, 0, 0)) {
		TranslateMessage(&msg2);
		DispatchMessage(&msg2);
	}
	
}

LRESULT CALLBACK SetUpWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
	switch(msg) {

	case WM_CREATE:
	{
		AddControlsSetUp(hwnd);	//	Creating the controls
		return 0;
	}
	case WM_COMMAND:
	{
		switch (param) {
		case SETUP_OK:
		{
			if (FILE_PATH_EXIST == TRUE) {	//	Checks if a file path was added

				string pass1, pass2, str1, str2;	//
				pass1 = ConvertToString(pw1);		//Getting the strings from the program
				pass2 = ConvertToString(pw2);		//and getting them ready to be compared and sent to the file
				str1 = hashing(pass1.c_str());		// if correct
				str2 = hashing(pass2.c_str());		//

				if (str1 == str2) {
					STAY_IN_MENU = TRUE;	//	Stay in the main menu loop
					HASHEDPW = str1;								//
					string buffer = encrypt(HASHEDPW, HASHEDPW);	//If the strings are correct
					stringstream ss;								// the global hash variable will become the hash
					ss << "Hash: " << buffer;						// and the encrypted hash will be sent to the 
					string tofile = ss.str();						// file
					SendToFile(tofile);						//

					DestroyWindow(hwnd);	// Destroying the setup window
					return 0;
				}
				else {
					MessageBox(hwnd, "PLEASE MAKE SURE BOTH PASSWORDS ARE EQUAL", "PASSWORDS ARE DIFFERENT", MB_OK); // error passwords are not equal
					return 0;
				}
			}
			else {
				MessageBox(hwnd, "YOU MUST SELECT A FILE PATH", "NO FILE PATH", MB_OK);	//	Error no file path selected
				return 0;
			}
		}
		case SELECT_FILE_SETUP:
		{
			path = OpenFileBox(hwnd);	//	Getting the path to the riot client

			if (path[0] == '\0') {														//
				MessageBox(hwnd, "PLEASE SELECT A FILE", "NO FILE SELECTED", MB_OK);	//	Error no path selected
				return 0;																//
			}else {
				string str;					//
				stringstream ss;			//	
				ss << "Path: " << path;		//	Sending the Path to the file and
				str = ss.str();				//	chaging FILE_PATH_EXIST to true
											//
				SendToFile(str.c_str());	//
				FILE_PATH_EXIST = TRUE;		//
				return 0;
			}
		}

		default:
		{
			return 0;
		}
		}
			
			return 0;
	}
	case WM_DESTROY:
	{
		if (HASHEDPW[0] == '\0') {	//	Handling the exception of a user that registers a path but doesnt 
			remove("VAS.conf");		//	create a password
		}
		PostQuitMessage(0);	//	quitting
		return 0;
	}
	default:
		return DefWindowProc(hwnd, msg, param, lparam);
	}
}

void AddControlsSetUp(HWND hwnd) {
	//
	//	Setup window controls
	//
	CreateWindow("static", "Master Password:", WS_VISIBLE | WS_CHILD, 5, 10, 200, 20, hwnd, NULL, NULL, NULL);
	pw1 = CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_PASSWORD | ES_AUTOHSCROLL, 5, 30, 200, 20, hwnd, NULL, NULL, NULL);

	CreateWindow("static", "Repeat Master Password:", WS_VISIBLE | WS_CHILD, 5, 50, 200, 20, hwnd, NULL, NULL, NULL);
	pw2 = CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_PASSWORD | ES_AUTOHSCROLL, 5, 70, 200, 20, hwnd, NULL, NULL, NULL);

	CreateWindow("static", "Please add your RIOT CLIENT for Valorant: (NOT VALORANT.EXE)", WS_VISIBLE | WS_CHILD | ES_AUTOVSCROLL, 5, 90, 300, 40, hwnd, NULL, NULL, NULL);
	CreateWindow("Button", "Select File.", WS_VISIBLE | WS_CHILD | WS_BORDER, 5, 130, 130, 20, hwnd, (HMENU)SELECT_FILE_SETUP, NULL, NULL);

	CreateWindow("Button", "Confirm.", WS_VISIBLE | WS_CHILD, 5, 160, 80, 20, hwnd, (HMENU)SETUP_OK, NULL, NULL);
}

string OpenFileBox(HWND hwnd) {

	OPENFILENAME ofn;							//create openfile
	ZeroMemory(&ofn, sizeof(OPENFILENAME));		//clear openfile

	char pathc[512];	//initialize path variable
		
	ofn.lStructSize = sizeof(OPENFILENAME);															//
	ofn.hwndOwner = hwnd;																			//
	ofn.lpstrFile = pathc;																			//defining the openfile properties
	ofn.lpstrFile[0] = '\0';																		//
	ofn.nMaxFile = 512;																				//
	ofn.lpstrFilter = ("RiotClient\0RiotClientServices.exe\0Executables\0*.exe\0All Files\0*.*");	//
	ofn.nFilterIndex = 1; ;																			//
	ofn.Flags = 0x00000008; // OFN_NOCHANGEDIR Flag (makes sure the program stays on this directory)

	GetOpenFileName(&ofn);	//getting the file name

	string pathreturn = pathc;

	return pathreturn;
}

void CheckPassword(HINSTANCE currentInstance) {
	//Registering Window Class
	const char* CLASS_NAME = "CHECKPW - VAS";
	WNDCLASS wc{};
	wc.hInstance = currentInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = CHKPWWindowProcessMessages;
	RegisterClass(&wc);


	//creating window
	CreateWindow(CLASS_NAME, "VAS - CHECKPW",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,			//style
		CW_USEDEFAULT, CW_USEDEFAULT,		//initial position
		400, 400,							//size
		nullptr, nullptr, nullptr, nullptr);

	//window loop
	MSG msg4{};
	while (GetMessage(&msg4, nullptr, 0, 0)) {
		TranslateMessage(&msg4);
		DispatchMessage(&msg4);
	}
}

LRESULT CALLBACK CHKPWWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
	switch(msg)
	{
	case WM_CREATE:
	{
		AddControlsCheckPass(hwnd);	//	Creating controls
		return 0;
	}
	case WM_COMMAND:
	{
		switch (param)
		{
		case CHECK_PASSWD:
		{
			string convertedpw = ConvertToString(checkpw).c_str();					//
			string hashedconvertedpw = hashing(convertedpw);						// Getting the hash from the user and geting it ready to compare to the file
			string encriphashpw = encrypt(hashedconvertedpw, hashedconvertedpw);	//

			string pwfromfile = GetFromFile("Hash: ");	//Getting the string from the file

			if (encriphashpw == pwfromfile) {	
				STAY_IN_MENU = TRUE;			//
				HASHEDPW = hashedconvertedpw;	//
				PassedCheckPW = TRUE;			// If the password is correct the program will continue to the main menu and destroy the current window
				DestroyWindow(hwnd);			//
			}else{
				MessageBox(hwnd, "Please enter the correct password.", "WRONG PASSWORD", MB_OK);	//if the string is wrong the program will wait for a new password
			}
		}
		}
	return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);	//	quitting
		return 0;
	}


	default:
	{
		return DefWindowProc(hwnd, msg, param, lparam);
	}
	}


}

void AddControlsCheckPass(HWND hwnd) {	
	//	Check password controls
	CreateWindow("static", "Please enter your Master Password:", WS_VISIBLE | WS_CHILD, 5, 5, 200, 20, hwnd, NULL, NULL, NULL);
	checkpw = CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_PASSWORD | ES_AUTOHSCROLL, 5, 30, 200, 20, hwnd, NULL, NULL, NULL);
	CreateWindow("button", "Confirm", WS_VISIBLE | WS_CHILD | WS_BORDER, 5, 80, 90, 20, hwnd, (HMENU)CHECK_PASSWD, NULL, NULL);
}

void AddAccWindow(HINSTANCE currentInstance) {
	//Registering Window Class
	const char* CLASS_NAME = "Valorant Account Selector - Add Account";
	WNDCLASS wc{};
	wc.hInstance = currentInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = AddAccWindowProcessMessages;
	RegisterClass(&wc);


	//creating window
	CreateWindow(CLASS_NAME, "GUI VAS - AddAcc",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_GROUP,			//style
		CW_USEDEFAULT, CW_USEDEFAULT,		//initial position
		450, 600,							//size
		nullptr, nullptr, nullptr, nullptr);

	//window loop
	MSG msg3{};
	while (GetMessage(&msg3, NULL, 0, 0)) {
		TranslateMessage(&msg3);
		DispatchMessage(&msg3);
	}

}

LRESULT CALLBACK AddAccWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {

	switch (msg) {
	case WM_CREATE:
	{
		AddControlsAddAcc(hwnd);	//	creating controls
		return 0;
	}
	case WM_COMMAND:
	{
		switch(param) {
		
		case ADD_ACCOUNT:
		{
			if (HAS_RANK) {
				string uname, pass, nick;				//
				nick = ConvertToString(addnick);		//getting the credentials
				pass = ConvertToString(addpw);			//
				uname = ConvertToString(adduname);		//

				string nick2f = encrypt(nick.c_str(), HASHEDPW);			//
				string uname2f = encrypt(uname.c_str(), HASHEDPW);			//encrypting the credentials
				string pass2f = encrypt(pass.c_str(), HASHEDPW);			//

				AddAcc(nick2f, uname2f, pass2f, SELECTED_RANK);	//sending the credentials to the file

				DestroyWindow(hwnd);	//destroying the window
				return 0;
			}
			else {
				MessageBox(hwnd, "NO RANK", "Account Must Have a Rank", MB_OK);	//	Error no rank added
				return 0;
			}
		}

		case RANK_WINDOW:
		{
			SelectRankWindow(cInstance);																										//	Creates rank window
			if (SELECTED_RANK != -1) {																											//	Checks if a rank was selected
				CreateWindow("static", RankToString(SELECTED_RANK).c_str(), WS_VISIBLE | WS_CHILD, 105, 155, 95, 20, hwnd, NULL, NULL, NULL);	//	Show the user what the selected rank was
				HAS_RANK = TRUE;																												//	Changes HAS_RANK to true, allowing the account to be added
			}
			return 0;
		}

		default:
		{
			return 0;
		}

		}
		return 0;
	}

	case WM_DESTROY:
	{
		STAY_IN_MENU = TRUE;		//
		PostQuitMessage(0);			//destroying this window and going back to the main menu
		return 0;					//
	}

	default:
	{
		return DefWindowProc(hwnd, msg, param, lparam);
	}

	}
}

void AddControlsAddAcc(HWND hwnd) {
	//	controls for the add account menu
	CreateWindow("Static", "ACC Nickname(not you Val username):", WS_VISIBLE | WS_CHILD , 5, 5, 250, 20, hwnd, NULL, NULL, NULL);
	addnick = CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD, 5, 30, 250, 20, hwnd, NULL, NULL, NULL);

	CreateWindow("Static", "Username:", WS_VISIBLE | WS_CHILD, 5, 55, 250, 20, hwnd, NULL, NULL, NULL);
	adduname = CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD, 5, 80, 250, 20, hwnd, NULL, NULL, NULL);

	CreateWindow("Static", "Password:", WS_VISIBLE | WS_CHILD, 5, 105, 250, 20, hwnd, NULL, NULL, NULL);
	addpw = CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_PASSWORD, 5, 130, 250, 20, hwnd, NULL, NULL, NULL);

	CreateWindow("Button", "Add Rank.", WS_VISIBLE | WS_CHILD | WS_BORDER, 5, 155, 95, 20, hwnd, (HMENU)RANK_WINDOW, NULL, NULL);

	CreateWindow("Button", "Add Account.", WS_VISIBLE | WS_CHILD | WS_BORDER, 5, 180, 95, 20, hwnd, (HMENU)ADD_ACCOUNT, NULL, NULL);
}

string ConvertToString(HWND input) {
	int len = GetWindowTextLength(input) + 1;						//	get the length of the input string and adding 1 to it
	string output;		//	creates the output string
	GetWindowText(input, const_cast<char*>(output.c_str()), len);	//	Getting the input string and storing on the output string
	return output;	//	returning the output string
}

void FillVector() {

	if (GetLastId() == -1) {	//	making sure there are accounts on the file
		return;
	}
	else {
		//clear vectors
		LOGIN_ACC.clear();
		RANK_ACC.clear();
		CHANGES_ACC.clear();

		for (UINT i = 0; i <= (UINT)GetLastId(); i++) {
			LOGIN_ACC.push_back(i);		//
			UINT i2 = (300 + i);		//	Adding values to all vectors
			RANK_ACC.push_back(i2);		//
			UINT i3 = (400 + i);		//
			CHANGES_ACC.push_back(i3);	//
		}
	}
}

void LOGIN(HINSTANCE currentInstance) {
	//Registering Window Class
	const char* CLASS_NAME = "Valorant Account Selector - LOGIN";
	WNDCLASS wc{};
	wc.hInstance = currentInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = LOGINWindowProcessMessages;
	RegisterClass(&wc);


	//creating window
	CreateWindow(CLASS_NAME, "GUI VAS - LOGIN",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,			//style
		CW_USEDEFAULT, CW_USEDEFAULT,				//initial position
		400, 500,									//size
		nullptr, nullptr, nullptr, nullptr);

	//window loop
	MSG msg4{};
	while (GetMessage(&msg4, nullptr, 0, 0)) {
		TranslateMessage(&msg4);
		DispatchMessage(&msg4);
	}
}

LRESULT CALLBACK LOGINWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
	switch (msg)
	{
	case WM_CREATE:
	{
		AddControlsLOGIN(hwnd);	//	creating initial controls
		return 0;
	}
	case WM_COMPLETE:
	{
		AddControlsLOGIN2(hwnd);	//	creating controls after the game starts opening
		return 0;
	}
	case WM_COMPLETE2:
	{
		AddControlsLOGIN3(hwnd);	//	creating controls after the game is done open
		DestroyWindow(hwnd);		//	destroying the window
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);	//	quitting
		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, msg, param, lparam);
	}
	}

}

void AddControlsLOGIN(HWND hwnd) {
	//	creating the controls for the login window
	path = GetFromFile("Path: ");				//
	stringstream ss, ss2;						//	Getting:
	ss << "uname" << LOGIN_ID << ":";			//	-Path
	ss2 << "passwd" << LOGIN_ID << ":";			//	-Username
												//	-Password
	string unamefile = GetFromFile(ss.str());	//	From the file
	string passwdfile = GetFromFile(ss2.str());	//

	LOGIN_UNAME = decrypt(unamefile, HASHEDPW);		//	Decrypting the username and password
	LOGIN_PASSWD = decrypt(passwdfile, HASHEDPW);	//

	LOGIN_UNAME = RemoveLastChar(LOGIN_UNAME);		//	Removing their last char
	LOGIN_PASSWD = RemoveLastChar(LOGIN_PASSWD);	//	(was generating a 2 at the end)

	opengame(path);	//	opening up the game

	CreateWindow("static", "BOOTING UP VALORANT", WS_VISIBLE | WS_CHILD, 5, 10, 350, 20, hwnd, NULL, NULL, NULL);	//
	UpdateWindow(hwnd);																								//	Tells the user the game is booting up
	PostMessage(hwnd, WM_COMPLETE, NULL, NULL);																		//
}

void AddControlsLOGIN2(HWND hwnd) {
	CreateWindow("static", "WAITING FOR VALORANT TO OPEN", WS_VISIBLE | WS_CHILD, 5, 35, 350, 20, hwnd, NULL, NULL, NULL);	//
	UpdateWindow(hwnd);																										//	Tells the user the program is now waiting for Valorant
	waitgame();																												//	to open up
	PostMessage(hwnd, WM_COMPLETE2, NULL, NULL);																			//
}

void AddControlsLOGIN3(HWND hwnd) {
	CreateWindow("static", "GENERATING INPUT", WS_VISIBLE | WS_CHILD, 5, 60, 350, 20, hwnd, NULL, NULL, NULL);	//	Tells the user the program is about to
	UpdateWindow(hwnd);																							//	generate input
	
	sleep(3);	//	waits 3 seconds to make sure the UI is done loading

	type(LOGIN_UNAME);		//
	tabbing();				//	Generates input
	type(LOGIN_PASSWD);		//
	ENTER();				//
}

string RemoveLastChar(string str) {
	string str2 = str.substr(0, str.size() - 1);	//	taking away the last char on the string
	return str2;
}

void SelectRankWindow(HINSTANCE currentInstance) {
	//Registering Window Class
	const char* CLASS_NAME = "Valorant Account Selector - Select a Rank";
	WNDCLASS wc{};
	wc.hInstance = currentInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = RankWindowProcessMessages;
	RegisterClass(&wc);


	//creating window
	CreateWindow(CLASS_NAME, "GUI VAS - RANKS",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,			//style
		CW_USEDEFAULT, CW_USEDEFAULT,				//initial position
		300, 300,									//size
		nullptr, nullptr, nullptr, nullptr);

	//window loop
	MSG msg5{};
	while (GetMessage(&msg5, nullptr, 0, 0)) {
		TranslateMessage(&msg5);
		DispatchMessage(&msg5);
	}

}

LRESULT CALLBACK RankWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
	switch (msg) {
	case WM_CREATE:
	{
		AddControlsRankWindow(hwnd);	//	creating controls
		return 0;
	}
	case WM_COMMAND:
	{
		for (int i = 0; i < 19; i++) {
			if (ALL_RANKS_BTN[i] == param) {				//	checking what rank button was used
				SELECTED_RANK = (ALL_RANKS_BTN[i] - 200);	//	sending the rank to the global variable
			}
		}
		if (SELECTED_RANK != -1) {
			DestroyWindow(hwnd);	//	Destroys window if no rank was selected
		}
		else {
		}
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);	//	quitting
		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, msg, param, lparam);
	}
	}
}

void AddControlsRankWindow(HWND hwnd) {
	//	controls for the rank window
	CreateWindow("button", "IR1", WS_VISIBLE | WS_CHILD | WS_BORDER, 5,		5, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[0], NULL, NULL);
	CreateWindow("button", "IR2", WS_VISIBLE | WS_CHILD | WS_BORDER, 60,	5, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[1], NULL, NULL);
	CreateWindow("button", "IR3", WS_VISIBLE | WS_CHILD | WS_BORDER, 115,	5, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[2], NULL, NULL);
	CreateWindow("button", "BR1", WS_VISIBLE | WS_CHILD | WS_BORDER, 5,		40, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[3], NULL, NULL);
	CreateWindow("button", "BR2", WS_VISIBLE | WS_CHILD | WS_BORDER, 60,	40, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[4], NULL, NULL);
	CreateWindow("button", "BR3", WS_VISIBLE | WS_CHILD | WS_BORDER, 115,	40, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[5], NULL, NULL);
	CreateWindow("button", "SI1", WS_VISIBLE | WS_CHILD | WS_BORDER, 5,		75, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[6], NULL, NULL);
	CreateWindow("button", "SI2", WS_VISIBLE | WS_CHILD | WS_BORDER, 60,	75, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[7], NULL, NULL);
	CreateWindow("button", "SI3", WS_VISIBLE | WS_CHILD | WS_BORDER, 115,	75, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[8], NULL, NULL);
	CreateWindow("button", "GO1", WS_VISIBLE | WS_CHILD | WS_BORDER, 5,		110, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[9], NULL, NULL);
	CreateWindow("button", "GO2", WS_VISIBLE | WS_CHILD | WS_BORDER, 60,	110, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[10], NULL, NULL);
	CreateWindow("button", "GO3", WS_VISIBLE | WS_CHILD | WS_BORDER, 115,	110, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[11], NULL, NULL);
	CreateWindow("button", "PL1", WS_VISIBLE | WS_CHILD | WS_BORDER, 5,		145, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[12], NULL, NULL);
	CreateWindow("button", "PL2", WS_VISIBLE | WS_CHILD | WS_BORDER, 60,	145, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[13], NULL, NULL);
	CreateWindow("button", "PL3", WS_VISIBLE | WS_CHILD | WS_BORDER, 115,	145, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[14], NULL, NULL);
	CreateWindow("button", "DI1", WS_VISIBLE | WS_CHILD | WS_BORDER, 5,		180, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[15], NULL, NULL);
	CreateWindow("button", "DI2", WS_VISIBLE | WS_CHILD | WS_BORDER, 60,	180, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[16], NULL, NULL);
	CreateWindow("button", "DI3", WS_VISIBLE | WS_CHILD | WS_BORDER, 115,	180, 50, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[17], NULL, NULL);
	CreateWindow("button", "IM/RAD", WS_VISIBLE | WS_CHILD | WS_BORDER, 5,	215, 160, 30, hwnd, (HMENU)(UINT_PTR)ALL_RANKS_BTN[18], NULL, NULL);
}

void RankedLoginWindow(HINSTANCE currentInstance) {
	STAY_IN_MENU = TRUE;
	//Registering Window Class
	const char* CLASS_NAME = "Valorant Account Selector - LOGIN RANK";
	WNDCLASS wc{};
	wc.hInstance = currentInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = RankedLoginWindowProcessMessages;
	RegisterClass(&wc);


	//creating window
	CreateWindow(CLASS_NAME, "GUI VAS - RANK LOGIN",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,			//style
		CW_USEDEFAULT, CW_USEDEFAULT,				//initial position
		300, 300,									//size
		nullptr, nullptr, nullptr, nullptr);

	//window loop
	MSG msg6{};
	while (GetMessage(&msg6, nullptr, 0, 0)) {
		TranslateMessage(&msg6);
		DispatchMessage(&msg6);
	}
}

LRESULT CALLBACK RankedLoginWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
	switch (msg) {
	case WM_CREATE:
	{
		AddControlsRankedLogin(hwnd);	//	adding controls
		return 0;
	}
	case WM_COMMAND:
	{
		switch (param) {
		case RANK_WINDOW:
		{
			SelectRankWindow(cInstance);	//	creates the choose rank window
			if (SELECTED_RANK == -1) {		//	checks if a rank was selected
				MessageBox(hwnd, "Select a rank", "No rank", MB_OK);	//	Error no rank selected
				return 0;
			}
			else {
				CreateWindow("static", RankToString(SELECTED_RANK).c_str(), WS_VISIBLE | WS_CHILD, 5, 75, 75, 20, hwnd, NULL, NULL, NULL);		//
				CreateWindow("button", "Confirm.", WS_VISIBLE | WS_CHILD | WS_BORDER, 5, 100, 75, 20, hwnd, (HMENU)RANKED_LOGIN, NULL, NULL);	//	Creating controls to show the user what rank was choosen
				UpdateWindow(hwnd);																												//	and creating the confirm button
				return 0;
			}
			return 0;
		}
		case RANKED_LOGIN:
		{
			int gotid = comparerank(SELECTED_RANK);	//	getting the id of the account that can play
			if (gotid == -1) {
				MessageBox(hwnd, "No account found in the rank range specified.", "No account", MB_OK);	//	Error, no account on that rank range
				DestroyWindow(hwnd);	//	Destroying window
				STAY_IN_MENU = TRUE;	//	and going back to the main menu
				return 0;
			}
			else {
				STAY_IN_MENU = FALSE;	//	breaking the main menu loop
				LOGIN_ID = gotid;		//	storing the account id on the global variable
				DestroyWindow(hwnd);	//	destroying window
				LOGIN(cInstance);	//	calling the login function
				PostQuitMessage(0);	//making sure to kill the process
				return 0;
			}
		}
		default:
		{
			return 0;
		}
		}
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);	//	quitting
		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, msg, param, lparam);
	}
	}
}

void AddControlsRankedLogin(HWND hwnd) {
	//	adding controls for the ranked based login window
	CreateWindow("static", "Select the lowest rank on your party.", WS_VISIBLE | WS_CHILD, 5, 5, 200, 40, hwnd, NULL, NULL, NULL);
	CreateWindow("button", "Select Rank.", WS_VISIBLE | WS_CHILD, 5, 50, 75, 20, hwnd, (HMENU)RANK_WINDOW, NULL, NULL);
}

void ChangeAccWindow(HINSTANCE currentInstance) {
	//Registering Window Class
	const char* CLASS_NAME = "Valorant Account Selector - ACC INFO";
	WNDCLASS wc{};
	wc.hInstance = currentInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = ChangeAccWindowProcessMessages;
	RegisterClass(&wc);


	//creating window
	CreateWindow(CLASS_NAME, "GUI VAS - Change Acc Info",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,			//style
		CW_USEDEFAULT, CW_USEDEFAULT,				//initial position
		400, 300,									//size
		nullptr, nullptr, nullptr, nullptr);

	//window loop
	MSG msg7{};
	while (GetMessage(&msg7, nullptr, 0, 0)) {
		TranslateMessage(&msg7);
		DispatchMessage(&msg7);
	}
}

LRESULT CALLBACK ChangeAccWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
	switch (msg) {
	case WM_CREATE:
	{
		STAY_IN_MENU = TRUE;		//	making sure it goes back to the main menu
		AddControlsChangeAcc(hwnd);	//	creating controls
	}
	case WM_COMMAND:
	{
		switch (param) {
		case CHANGE_ACC_BTN:
		{
			BOOL CNK = FALSE, CNM = FALSE, CPW = FALSE;	//	Variables to check what information was changed
			string nick = ConvertToString(chnick);		//
			string uname = ConvertToString(chuname);	//	Getting the strings
			string passwd = ConvertToString(chpw);		//
			if (nick[0] != '\0') {	//	checking if nickname changed
				stringstream ss;											//
				ss << "nickname" << LOGIN_ID << ":";						//	Changing the info on the file
				ChangeAccInfo(ss.str(), encrypt(nick.c_str(),HASHEDPW));	//	and setting CNK as true
				CNK = TRUE;													//
			}
			if (uname[0] != '\0') {	//	checking if username changed
				stringstream ss;											//
				ss << "uname" << LOGIN_ID << ":";							//	Changing the info on the file
				ChangeAccInfo(ss.str(), encrypt(uname.c_str(), HASHEDPW));	//	and setting CNM as true
				CNM = TRUE;													//
			}
			if (passwd[0] != '\0') {	//	Checking if password changed
				stringstream ss;											//
				ss << "passwd" << LOGIN_ID << ":";							//	Changing the info on the file
				ChangeAccInfo(ss.str(), encrypt(passwd.c_str(),HASHEDPW));	//	and setting CPW as true
				CPW = TRUE;													//
			}
			if (CNK == CNM && CNM == CPW) {	//	checking if all variables are the same
				if (CNK) {
					MessageBox(hwnd, "All infos were changed", "Changed Info", MB_OK);	//	if all information changed
				}
				else {
					MessageBox(hwnd, "NO CHANGES WERE MADE", "Changed Info", MB_OK);	//	if nothing changed
				}
			}
			else {	//	if all variables are not the same
				stringstream ssop;					//
				BOOL INNK = FALSE, INNM = FALSE;	//	creating variables
				ssop << "Changes were made to: ";	//
				if (CNK) {	//	checking if nickname changed
					ssop << "nickname";
					INNK = TRUE;
				}
				if (CNM) {	//	checking if username changed
					if (INNK) {	//	checking if a coma is necessary
						ssop << ", ";
					}
					ssop << "username";
					INNM = TRUE;
				}
				if (CPW) {	//	checking if password changed
					if (INNK || INNM) {	//	checking if a coma is necessary
						ssop << ", ";
					}
					ssop << "password";
				}
				MessageBox(hwnd, ssop.str().c_str(), "Changed Info", MB_OK);	//	telling the user what information changed
			}
			DestroyWindow(hwnd);
			return 0;
		}
		case DELETE_ACC_BTN:
		{
			DestroyWindow(hwnd);		//	Destroying this window
			DeleteAccWindow(cInstance);	//	and creating the delete account window
			return 0;
		}
		default:
		{
			return 0;
		}
		}
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);	//	quitting
		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, msg, param, lparam);
	}
	}
}

void AddControlsChangeAcc(HWND hwnd) {
	//	change account information controls
	CreateWindow("static", "Change whatever info you want or just keep it blank to keep it the same", WS_VISIBLE | WS_CHILD, 5, 5, 300, 40, hwnd, NULL, NULL, NULL);
	
	CreateWindow("static", "Nickname:", WS_VISIBLE | WS_CHILD, 5, 50, 80, 20, hwnd, NULL, NULL, NULL);
	chnick = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD, 90, 50, 250, 20, hwnd, NULL, NULL, NULL);

	CreateWindow("static", "Username:", WS_VISIBLE | WS_CHILD, 5, 75, 80, 20, hwnd, NULL, NULL, NULL);
	chuname = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD, 90, 75, 250, 20, hwnd, NULL, NULL, NULL);

	CreateWindow("static", "Password:", WS_VISIBLE | WS_CHILD, 5, 100, 80, 20, hwnd, NULL, NULL, NULL);
	chpw = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | ES_PASSWORD, 90, 100, 250, 20, hwnd, NULL, NULL, NULL);

	CreateWindow("button", "Confirm.", WS_VISIBLE | WS_CHILD | WS_BORDER, 5, 125, 75, 20, hwnd, (HMENU)CHANGE_ACC_BTN, NULL, NULL);

	CreateWindow("button", "DELETE ACC", WS_VISIBLE | WS_CHILD | WS_BORDER, 250, 125, 105, 20, hwnd, (HMENU)DELETE_ACC_BTN, NULL, NULL);
}

void DeleteAccWindow(HINSTANCE currentInstance) {
	//Registering Window Class
	const char* CLASS_NAME = "Valorant Account Selector - DELETE ACC";
	WNDCLASS wc{};
	wc.hInstance = currentInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = DeleteAccWindowProcessMessages;
	RegisterClass(&wc);


	//creating window
	CreateWindow(CLASS_NAME, "GUI VAS - DELETE ACC",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,			//style
		CW_USEDEFAULT, CW_USEDEFAULT,				//initial position
		300, 300,									//size
		nullptr, nullptr, nullptr, nullptr);

	//window loop
	MSG msg8{};
	while (GetMessage(&msg8, nullptr, 0, 0)) {
		TranslateMessage(&msg8);
		DispatchMessage(&msg8);
	}
}

LRESULT CALLBACK DeleteAccWindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
	switch (msg) {
	case WM_CREATE:
		{
			AddControlsDeleteAcc(hwnd);	//	creating controls
			return 0;
		}
	case WM_COMMAND:
	{
		switch (param) {
		case CONFIRM_DELETE_BTN:
		{
			DeleteAcc(LOGIN_ID);	//	Deleting the account
			DestroyWindow(hwnd);	//	Destroying the window
			return 0;
		}
		case CANCEL_DELETE_BTN:
		{
			DestroyWindow(hwnd);	//	Does not delete the account, just destroys this window
			return 0;
		}
		default:
		{
			return 0;
		}
		}
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);	//	quitting
		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, msg, param, lparam);
	}
	}
}

void AddControlsDeleteAcc(HWND hwnd) {
	//	controls for delete account window
	stringstream ss, ssf;														//
	ssf << "nickname" << LOGIN_ID << ":";										//	Getting the nickname of the account that will be deleted
	ss << "Account nickname = " << decrypt(GetFromFile(ssf.str()), HASHEDPW);	//
	CreateWindow("static", "ARE YOU SURE YOU WANT TO DELETE THIS ACCOUNT?", WS_VISIBLE | WS_CHILD, 5, 5, 270, 40, hwnd, NULL, NULL, NULL);	//
	CreateWindow("static", ss.str().c_str(), WS_VISIBLE | WS_CHILD, 5, 50, 295, 20, hwnd, NULL, NULL, NULL);								//	Showing what account will be deleted for the user
	CreateWindow("button", "CONFIRM", WS_VISIBLE | WS_CHILD | WS_BORDER, 5, 75, 75, 20, hwnd, (HMENU)CONFIRM_DELETE_BTN, NULL, NULL);	//	Making sure the user wants to delete the account
	CreateWindow("button", "CANCEL", WS_VISIBLE | WS_CHILD | WS_BORDER, 90, 75, 75, 20, hwnd, (HMENU)CANCEL_DELETE_BTN, NULL, NULL);	//
}
