

#define _CRT_SECURE_NO_WARNINGS

// memory leak test.
#ifdef _DEBUG 
#include <vld.h>
#endif

// Array idx chk test.
//#define ARRAYS_DEBUG

//#define USE_FAST_LOAD_DATA // no use?
#include <wiz/ClauText.h>



int main(int argc, char* argv[])
{
	srand(time(nullptr)); // 

	std::string fileName;


	if (argc == 1) {
		std::cout << "FileName: ";
		std::getline(std::cin, fileName);
	}
	else
	{
		fileName = std::string(argv[1]);
	}
	wiz::load_data::UserType global;
	try {
		wiz::load_data::LoadData::LoadDataFromFile(fileName, global);
		std::cout << "fileName is " << fileName << std::endl;
		std::cout << "excute result is " << excute_module("", &global, ExcuteData(), 0) << std::endl;
	}
	catch (const char* str) {
		std::cout << str << std::endl;
		GETCH();
	}
	catch (const std::string& str) {
		std::cout << str << std::endl;
		GETCH();
	}
	catch (const wiz::Error& e) {
		std::cout << e << std::endl;
		GETCH();
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
		GETCH();
	}
#ifndef _DEBUG
//	catch (...) {
//		std::cout << "UnKnown Error.." << std::endl;
//		GETCH();
//	}
#endif

   	return 0;
}

