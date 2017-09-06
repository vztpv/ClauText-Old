
#ifndef LOAD_DATA_RESERVERS_H
#define LOAD_DATA_RESERVERS_H

#include <vector>
#include <fstream>
#include <string>
using namespace std;

#include <wiz/cpp_string.h>
#include <wiz/load_data_utility.h>
#include <wiz/queues.h>

namespace wiz {
	namespace load_data {
		class InFileReserver
		{
		private:
			ifstream* pInFile;
		public:
			int Num;
		public:
			explicit InFileReserver(ifstream& inFile)
			{
				pInFile = &inFile;
				Num = 1;
			}
			bool end()const { return pInFile->eof(); }
		public:
			bool operator() (ArrayQueue<Token>& strVec, const wiz::LoadDataOption& option)
			{
				return Utility::Reserve2(*pInFile, strVec, Num, option).second > 0;
			}
		};
		class NoneReserver
		{
		private:
			int count;
		public:
			explicit NoneReserver() : count(0) { }
			bool operator() (ArrayQueue<Token>& strVec, const wiz::LoadDataOption&)
			{
				count = 1;
				return false;
			}
			bool end()const { return 1 == count; }
		};
	}
}

#endif