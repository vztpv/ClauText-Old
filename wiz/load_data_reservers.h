
#ifndef LOAD_DATA_RESERVERS_H
#define LOAD_DATA_RESERVERS_H

#include <vector>
#include <fstream>
#include <string>

#include <wiz/cpp_string.h>
#include <wiz/load_data_utility.h>
#include <wiz/queues.h>

namespace wiz {
	namespace load_data {
		class InFileReserver
		{
		private:
			std::ifstream* pInFile;
		public:
			int Num;
		public:
			explicit InFileReserver(std::ifstream& inFile)
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

		class InFileReserver2
		{
		private:
			std::ifstream* pInFile;
		public:
			int Num;
			int CoreNum;
		public:
			explicit InFileReserver2(std::ifstream& inFile, int num_of_core)
			{
				pInFile = &inFile;
				Num = 1;
				CoreNum = num_of_core;
			}
			bool end()const { return pInFile->eof(); }
		public:
			bool operator() (std::vector<ArrayQueue<Token>>& strVec, const wiz::LoadDataOption& option, const int offset)
			{
				return Utility::Reserve2_2(*pInFile, strVec, Num, option, CoreNum, offset).second > 0;
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