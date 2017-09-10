

#define _CRT_SECURE_NO_WARNINGS

// memory leak test.
#ifdef _DEBUG 
#include <vld.h>
#endif

// Array idx chk test.
//#define ARRAYS_DEBUG

#define USE_FAST_LOAD_DATA
#include <wiz/ClauText.h>


int main(int argc, char* argv[])
{
	srand(time(nullptr)); // 

	string fileName;


	if (argc == 1) {
		cout << "FileName: ";
		getline(cin, fileName);
	}
	else
	{
		fileName = string(argv[1]);
	}
	wiz::load_data::UserType global;
	try {
			wiz::load_data::LoadData::FastLoadDataFromFile(fileName, global);
			cout << "fileName is " << fileName << endl;
			cout << "excute result is " << excute_module("", &global, ExcuteData(), 0) << endl;
	}
	catch (const char* str) {
		cout << str << endl;
		GETCH();
	}
	catch (const string& str) {
		cout << str << endl;
		GETCH();
	}
	catch (const wiz::Error& e) {
		cout << e << endl;
		GETCH();
	}
#ifndef _DEBUG
	catch (...) {
		cout << "UnKnown Error.." << endl;
		GETCH();
	}
#endif
   	return 0;
}

