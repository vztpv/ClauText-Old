
#ifndef LOAD_DATA_UTILITY_H
#define LOAD_DATA_UTILITY_H

#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <algorithm>
using namespace std;

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
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else return false;
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else return false;
						break;
					}
				}
				return 3 == state;
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
			static pair<bool, int> Reserve2(ifstream& inFile, ArrayQueue<string>& aq, const int num = 1)
			{
				int count = 0;
				string temp;
				vector<string> strVecTemp;
				ArrayQueue<string> arrayQueue[4];
				
				for (int i = 0; i < num && (getline(inFile,temp)); ++i) {
					//temp = RemoveEndSpace(temp);
					temp = PassSharp(temp);
					temp = AddSpace(temp);
					temp = ChangeSpace(temp, '^'); 

					strVecTemp.push_back(temp);
					count++;
				}

				if (count >= 4) {
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
			/*
			pair< bool, int > Reserve(ifstream& inFile, ArrayQueue<string>& strVec, const int lineNum = 1)
			{
			int count = 0; // string num : not valid line num!
			pair<bool, bool> prTemp(false, false);

			for (int i = 0; i < lineNum; ++i) {
			ArrayQueue<string> temp;
			bool b1 = prTemp.first;
			bool b2 = prTemp.second;
			pair<bool, bool> bbTemp = PassSharp(inFile, temp);
			if (bbTemp.second) {
			for (int i = 0; i < temp.size(); ++i) {
			const string str = temp[i];
			StringTokenizer tokenizer(str, vector<string>{ " ", "\t", "\r", "\n" });
			for (int k = 0; k < tokenizer.countTokens(); ++k) {
			string str = tokenizer.nextToken();
			strVec.push(str);
			}
			count = count + tokenizer.countTokens();
			}
			}
			prTemp.first = b1 || bbTemp.first;
			}

			return{ prTemp.first, count };
			}
			*/
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
			/// must strVec[start] == up or down?
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
			// AddSpace : return string?
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
			/// need to rename!, has bug.., line ±âÁØ..
			static string ChangeSpace(const string& str, const char result_ch) {
				string temp;
				int state = 0;

				for (int i = 0; i < str.size(); ++i)
				{
					if (0 == state && '\"' == str[i]) {
						state = 1;
						temp.push_back(str[i]);
					}
					else if (1 == state && '\"' != str[i] && (' ' == str[i] || '\t' == str[i])) {
						state = 1;
						temp.push_back(result_ch);
					}
					else if (1 == state && '\"' == str[i]) {
						state = 0;
						temp.push_back('\"');
					}
					else
					{
						temp.push_back(str[i]);
					}
				}

				return temp;
			}

			static void ChangeCharInString(string& str, const char target_ch, const char result_ch)
			{
				for (int i = 0; i < str.size(); ++i)
				{
					if (str[i] == target_ch)
					{
						str[i] = result_ch;
					}
				}
			}

		};

	}
}

#endif