
#ifndef LOAD_DATA_RESERVERS_H
#define LOAD_DATA_RESERVERS_H

#include <fstream>
using namespace std;

#include <wiz/load_data_utility.h>
#include <wiz/queues.h>

namespace wiz {
	namespace load_data {
		class InFIleReserver
		{
		private:
			ifstream* pInFile;
		public:
			int Num;
		public:
			explicit InFIleReserver(ifstream& inFile)
			{
				pInFile = &inFile;
				Num = 1;
			}
			bool end()const { return pInFile->eof(); }
		public:
			bool operator() (ArrayQueue<string>& strVec)
			{
				return Utility::Reserve2(*pInFile, strVec, Num).second > 0;
			}
		};
		class NoneReserver
		{
		private:
			int count;
		public:
			explicit NoneReserver() : count(0) { }
			bool operator() (ArrayQueue<string>& strVec)
			{
				count = 1;
				return false;
			}
			bool end()const { return 1 == count; }
		};

	}
}

#endif