
#ifndef LOAD_DATA_UTILITY_H
#define LOAD_DATA_UTILITY_H

#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <algorithm>
using namespace std;

#include <wiz/cpp_string.h>
//#include <wiz/load_data_types.h> /// 

namespace wiz {
	namespace load_data {

		class Utility
		{
		public:
			static bool IsInteger(const string& str) {
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if ('+' == str[i] || '-' == str[i]) {
							state = 0;
						}
						else if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else return false;
						break;
					case 1:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1;
						}
						else return false;
					}
				}
				return 1 == state; /// chk..
			}
			static bool IsNumberInJson(const string& str)
			{
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if ( // '+' == str[i] || // why can`t +
							'-' == str[i]
							) {
							state = 0;
						}
						else if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else { return false; }
						break;
					case 1:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1;
						}
						else if (str[i] == '.') {
							state = 2;
						}
						else { return false; }
						break;
					case 2:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else { return false; }
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else if ('e' == str[i] || 'E' == str[i]) {
							state = 4;
						}
						else { return false; }
						break;
					case 4:
						if (str[i] == '+' || str[i] == '-') {
							state = 5;
						}
						else {
							state = 5;
						}
						break;
					case 5:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 6;
						}
						else {
							return false;
						}
						break;
					case 6:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 6;
						}
						else {
							return false;
						}
					}
				}
				return 3 == state || 6 == state;
			}
			static bool IsDouble(const string& str) {
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if ('+' == str[i] || '-' == str[i]) {
							state = 0;
						}
						else if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else { return false; }
						break;
					case 1:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1;
						}
						else if (str[i] == '.') {
							state = 2;
						}
						else { return false; }
						break;
					case 2:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else { return false; }
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else if ('e' == str[i] || 'E' == str[i]) {
							state = 4;
						}
						else { return false; }
						break;
					case 4:
						if (str[i] == '+' || str[i] == '-') {
							state = 5;
						}
						else {
							state = 5;
						}
						break;
					case 5:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 6;
						}
						else {
							return false;
						}
						break;
					case 6:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 6;
						}
						else {
							return false;
						}
					}
				}
				return 3 == state || 6 == state;
			}
			static bool IsDate(const string& str)
			{
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else return false;
						break;
					case 1:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1;
						}
						else if (str[i] == '.') {
							state = 2;
						}
						else return false;
						break;
					case 2:
						if (str[i] >= '0' && str[i] <= '9') { state = 2; }
						else if (str[i] == '.') {
							state = 3;
						}
						else return false;
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 4; }
						else return false;
						break;
					case 4:
						if (str[i] >= '0' && str[i] <= '9') { state = 4; }
						else return false;
						break;
					}
				}
				return 4 == state;
			}
			static bool IsMinus(const string& str)
			{
				return str.empty() == false && str[0] == '-';
			}
			/// todo bool ,more stable!!
			static void AddSpace(const string& file1Name, const string& file2Name) {
				ifstream inFile;
				ofstream outFile;

				inFile.open(file1Name);
				outFile.open(file2Name);

				string temp;
				vector<string> strVec;

				while (getline(inFile, temp))
				{
					if (temp.empty() || temp == " ") { continue; }

					for (int j = 0; j < temp.size(); ++j)
					{
						if (temp[j] == '{') {
							outFile << " {\n";
						}
						else if (temp[j] == '}') {
							outFile << " }\n";
						}
						else if (temp[j] == '=') {
							outFile << " = ";
						}
						else
						{
							outFile << temp[j];
						}
					}
					outFile << " ";
					outFile << "\n";
				}

				inFile.close();
				outFile.close();
			}
			static bool PassSharp(const string& file1Name, const string& file2Name)
			{
				ifstream inFile;
				ofstream outFile;

				inFile.open(file1Name);
				if (inFile.fail()) { return false; }
				outFile.open(file2Name);
				if (outFile.fail()) { inFile.close(); return false; }

				string temp;

				while (getline(inFile, temp))
				{
					StringTokenizer tokenizer(temp, "#");

					if (tokenizer.countTokens() == 1 && tokenizer.isFindExist())
					{
						//
					}
					else if (tokenizer.countTokens() >= 1)
					{
						temp = tokenizer.nextToken();
						if (false == temp.empty())
						{
							outFile << temp << "\n";
						}
					}
				}

				inFile.close();
				outFile.close();
				return true;
			}

			static pair< bool, bool > PassSharp(ifstream& inFile, ArrayQueue<string>& strVec)
			{
				string temp;
				bool chk = (bool)getline(inFile, temp);
				if (chk) {
					StringTokenizer tokenizer(temp, "#");

					if (tokenizer.countTokens() == 1 && tokenizer.isFindExist())
					{
						//
					}
					else if (tokenizer.countTokens() >= 1)
					{
						temp = tokenizer.nextToken();
						if (false == temp.empty())
						{
							strVec.push(temp);
							return{ true, true };
						}
					}
				}
				return{ chk, false };
			}
		private:
			class DoThread // need to rename!
			{
			private:
				vector<string>* strVec;
				ArrayQueue<string>* aq;
				int strVecStart;
				int strVecEnd;
			public:
				DoThread( vector<string>* strVec, ArrayQueue<string>* aq, int strVecStart, int strVecEnd ) 
					: strVec(strVec), aq(aq), strVecStart(strVecStart), strVecEnd(strVecEnd)
				{
				}
				void operator() () {
					for (int i = strVecStart; i <= strVecEnd; ++i)
					{
						StringTokenizer tokenizer((*strVec)[i]);
						while (tokenizer.hasMoreTokens()) {
							aq->push(tokenizer.nextToken());
						}
					}
				}
			};
		private:
			static string RemoveEndSpace( const string& str )
			{
				string temp;
				int state = 0;

				for (int i = str.size() - 1; i >= 0; --i) {
					if (state == 0 && wiz::isWhitespace(str[i]))
					{
						continue;
					}
					else {
						state = 1;
					}

					temp.push_back(str[i]);
				}
				// reverse..
				string retVal;
				for (int i = temp.size() - 1; i >= 0; --i) {
					retVal.push_back(temp[i]);
				}
				return retVal;
			}
		public:
			static bool ChkExist(const string& str) /// has bug?, unstatble?
			{
				int state = -1;

				for (string::size_type i = 0; i < str.size(); ++i)
				{
					if (0 >= state && i == 0 && '\"' == str[i]) {
						state = 1;
					}
					else if (0 >= state && i > 0 && '\"' == str[i] && '\\' != str[i - 1])
					{
						state = 1;
					}
					else if (1 == state && i > 0 && '\\' != str[i - 1] && '\"' == str[i]) {
						state = 0;
					}
				}

				return 0 == state;
			}
		public:
			static pair<bool, int> Reserve2(ifstream& inFile, ArrayQueue<string>& aq, const int num = 1)
			{
				int count = 0;
				string temp;
				vector<string> strVecTemp;
				ArrayQueue<string> arrayQueue[4];
				
				for (int i = 0; i < num && (getline(inFile,temp)); ++i) {
					//temp = RemoveEndSpace(temp);
					bool chkStr = ChkExist(temp);
					if (chkStr) {
						temp = ChangeStr(temp, "^", "^0"); // 1ÁÙ¿¡ "~~~" ?	
						temp = Utility::ChangeStr(temp, "#", "^5");
					}
					
					temp = PassSharp(temp);
					temp = AddSpace(temp);
					
					if (chkStr) {
						temp = ChangeStr(temp, " ", "^1");
						temp = ChangeStr(temp, "\t", "^2");
						temp = ChangeStr(temp, "\r", "^3");
						temp = ChangeStr(temp, "\n", "^4");
					}

					strVecTemp.push_back(temp);
					count++;
				}

				if (count >= 100) { // 4 ?
					DoThread dtA(&strVecTemp, &arrayQueue[0], 0, count / 4 - 1),
						dtB(&strVecTemp, &arrayQueue[1], count / 4, (count / 4) * 2 - 1),
						dtC(&strVecTemp, &arrayQueue[2], (count / 4) * 2, (count / 4) * 3 - 1),
						dtD(&strVecTemp, &arrayQueue[3], (count / 4) * 3, count - 1);
					std::thread _threadA(dtA), _threadB(dtB), _threadC(dtC), _threadD(dtD);

					_threadA.join();
					_threadB.join();
					_threadC.join();
					_threadD.join();

					for (int i = 0; i < 4; ++i) {
						for (int j = 0; j < arrayQueue[i].size(); ++j) {
							aq.push(arrayQueue[i][j]);
						}
					}
				}
				else if (count > 0) {
					DoThread dtA(&strVecTemp, &aq, 0, count - 1);
					dtA();
				}

				return{ count > 0, count };
			}

			/// must lineNum > 0
			static pair<bool, int> Reserve(ifstream& inFile, ArrayQueue<string>& strVec, const int num = 1)
			{
				string temp;
				int count = 0;

				for (int i = 0; i < num && (inFile >> temp); ++i) {
					strVec.push(temp);
					count++;
				}
				return{ count > 0, count };
			}
			
			static const string& Top(const ArrayQueue<string>& strVec)
			{
				return strVec[0];
			}
			static string Pop(ArrayQueue<string>& strVec)
			{
				return strVec.pop();
			}
			static int GetIndex(const ArrayQueue<string>& strVec, const string& str)
			{
				int idx = -1;

				for (int i = 0; i < strVec.size(); ++i)
				{
					const string x = strVec[i];
					idx++;
					if (x == str)
					{
						return idx;
					}
				}
				return -1;
			}

			static pair<bool, string> LookUp(const ArrayQueue<string>& strVec, const int idx = 1)
			{
				if (strVec.size() <= idx)
				{
					return{ false, "" };
				}
				return{ true, strVec[idx] };
			}
			/// must strVec[start] == up or down
			/// now not use!!
			static pair<bool, int> IsMatched(const ArrayQueue<string>& strVec, const string& up, const string& down, const int start = 0, const int start_num = 0, int* pidx = NULL, int*pnum = NULL)
			{
				int num = start_num;
				int count = 0;
				int state = 0;
				int idx = -1;

				for (int i = start; i < strVec.size(); ++i) {
					if (strVec[i] == up) { state = 1; num++; }
					else if (strVec[i] == down) { state = 1;  num--; }
					if (state == 1 && num == 0)
					{
						if (pidx) { *pidx = i; }
						if (pnum) { *pnum = num; }
						return{ true, count };
					}
					count++;
					idx = i;
				}
				if (pidx) { *pidx = idx; }
				if (pnum) { *pnum = num; }
				return{ false, -1 };
			}

		public:

			// To Do
			// AddSpace : return string
			static string AddSpace(const string& str)
			{
				string temp;

				for (int i = 0; i < str.size(); ++i)
				{
					/// To Do - chabnge to switch statement.
					if ('=' == str[i]) {
						temp.push_back(' ');
						temp.push_back('=');
						temp.push_back(' ');
					}
					else if ('{' == str[i]) {
						temp.push_back(' ');
						temp.push_back('{');
						temp.push_back(' ');
					}
					else if ('}' == str[i]) {
						temp.push_back(' ');
						temp.push_back('}');
						temp.push_back(' ');
					}
					else {
						temp.push_back(str[i]);
					}
				}

				return temp;
			}

			/// need testing!
			static string PassSharp(const string& str) {
				string temp;
				int state = 0;

				for (int i = 0; i < str.size(); ++i) {
					if (str[i] == '#') { state = 1; }
					else if (str[i] == '\n') { state = 0; }

					if (0 == state) {
						temp.push_back(str[i]);
					}
				}
				return temp;
			}
	
			static string ChangeStr(const string& str, const string& changed_str, const string& result_str) {
				string temp;
				int state = 0;

				//temp.reserve(str.size() * 2);

				for (string::size_type i = 0; i < str.size(); ++i)
				{
					if (0 == state && i == 0 && '\"' == str[i]) {
						state = 1;
						temp.push_back(str[i]);
					}
					else if (0 == state && i > 0 && '\"' == str[i] && '\\' != str[i-1])
					{
						state = 1;
						temp.push_back(str[i]);
					}
					else if (1 == state && (wiz::String::Comp( changed_str.c_str(), str.c_str()+i, changed_str.size()))) {
						state = 1;
						temp += result_str;
						i = i + changed_str.size() - 1;
					}
					else if (1 == state && i > 0 && '\\' != str[i-1] && '\"' == str[i]) {
						state = 0;
						temp.push_back('\"');
					}
					else
					{
						temp.push_back(str[i]);
					}
				}

				//temp.shrink_to_fit();
				return temp;
			}

		};

	}
}

#endif