#include <stdio.h>
#include <iostream>
#include "cookie_decryptor.h"
#include "sqlite3.h"

using namespace std;

string cookie_path = "C:\\Users\\empty\\AppData\\Roaming\\Opera Software\\Opera GX Stable\\Network";

void decrypt()
{
	sqlite3* db = 0; // хэндл объекта соединение к БД
	char* err = 0;
	//auto res = sqlite3_open(cookie_path.c_str(), &db);
	//cout << res;
}

void test_fuN()
{
	cout << "test" << endl;
}