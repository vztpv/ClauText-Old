

// change to char chk. (from addspace, chk #, chk " ") - 2016.02.17 

#ifndef LOAD_DATA_UTILITY_H
#define LOAD_DATA_UTILITY_H

#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include <algorithm>
using namespace std;

#include <wiz/cpp_string.h>

namespace wiz {
	namespace load_data {
		class UserType;

		class Utility
		{
		public:
			static bool IsInteger(string str) {
				if (str.size() > 2 && str[0] == str.back() && str[0] == '\"') {
					str = str.substr(1, str.size() - 2);
				}
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
			static bool IsNumberInJson(string str)
			{
				if (str.size() > 2 && str[0] == str.back() && str[0] == '\"') {
					str = str.substr(1, str.size() - 2);
				}
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
			static bool IsDouble(string str) {
				if (str.size() > 2 && str[0] == str.back() && str[0] == '\"') {
					str = str.substr(1, str.size() - 2);
				}
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
			static bool IsDate(string str) /// chk!!
			{
				if (str.size() > 2 && str[0] == str.back() && str[0] == '\"') {
					str = str.substr(1, str.size() - 2);
				}
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
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else return false;
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else if (str[i] == '.') {
							state = 4;
						}
						else return false;
						break;
					case 4:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else return false;
						break;
					case 5:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else return false;
						break;
					}
				}
				return 5 == state;
			}
			static bool IsDateTimeA(string str) // yyyy.MM.dd.hh
			{
				if (str.size() > 2 && str[0] == str.back() && str[0] == '\"') {
					str = str.substr(1, str.size() - 2);
				}
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
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else return false;
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else if (str[i] == '.') {
							state = 4;
						}
						else return false;
						break;
					case 4:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else return false;
						break;
					case 5:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else if (str[i] == '.') { state = 6; }
						else return false;
						break;
					case 6:
						if (str[i] >= '0' && str[i] <= '9') { state = 7; }
						else return false;
						break;
					case 7:
						if (str[i] >= '0' && str[i] <= '9') { state = 7; }
						else return false;
						break;
					}
				}
				return 7 == state;
			}
			static bool IsDateTimeB(string str) // yyyy.MM.dd.hh.mm
			{
				if (str.size() > 2 && str[0] == str.back() && str[0] == '\"') {
					str = str.substr(1, str.size() - 2);
				}
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
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else return false;
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else if (str[i] == '.') {
							state = 4;
						}
						else return false;
						break;
					case 4:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else return false;
						break;
					case 5:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else if (str[i] == '.') { state = 6; }
						else return false;
						break;
					case 6:
						if (str[i] >= '0' && str[i] <= '9') { state = 7; }
						else return false;
						break;
					case 7:
						if (str[i] >= '0' && str[i] <= '9') { state = 7; }
						else if (str[i] == '.') {
							state = 8;
						}
						else return false;
						break;
					case 8:
						if (str[i] >= '0' && str[i] <= '9') { state = 9; }
						else return false;
						break;
					case 9:
						if (str[i] >= '0' && str[i] <= '9') { state = 9; }
						else return false;
						break;
					}
				}
				return 9 == state;
			}
			static bool IsMinus(string str)
			{
				if (str.size() > 2 && str[0] == str.back() && str[0] == '\"') {
					str = str.substr(1, str.size() - 2);
				}
				return str.empty() == false && str[0] == '-';
			}

			static string reverse(string str) { /// to std::reverse ?
				std::reverse(str.begin(), str.end());
				return str;
			}
			static string GetType(string str) {
				if (IsInteger(str)) { return "INTEGER"; }
				else if (IsDouble(str)) { return "DOUBLE"; }
				else if (IsDateTimeB(str)) { return "DATETIMEB"; }
				else if (IsDateTimeA(str)) { return "DATETIMEA"; }
				else if (IsDate(str)) { return "DATE"; }
				else return "STRING";
			}
			static string Compare(string str1, string str2, const int type = 0)
			{
				if (str1.size() > 2 && str1[0] == str1.back() && str1[0] == '\"')
				{
					str1 = str1.substr(1, str1.size() - 2);
				}
				if (str2.size() > 2 && str2[0] == str2.back() && str2[0] == '\"')
				{
					str2 = str2.substr(1, str2.size() - 2);
				}

				string type1 = GetType(str1);
				string type2 = GetType(str2);

				if (type1 != type2) {
					return "ERROR";
				}

				if ("STRING" == type1 || type == 1)
				{
					if (str1 < str2) {
						return "< 0";
					}
					else if (str1 == str2) {
						return "== 0";
					}
					return "> 0";
				}
				else if ("INTEGER" == type1)
				{
					if (Utility::IsMinus(str1) && !Utility::IsMinus(str2)) { return "< 0"; }
					else if (!Utility::IsMinus(str1) && Utility::IsMinus(str2)) { return "> 0"; }

					const bool minusComp = Utility::IsMinus(str1) && Utility::IsMinus(str2);

					if (false == minusComp) {
						string x = reverse(str1);
						string y = reverse(str2);

						if (x[0] == '+') { x = string(x.c_str() + 1); }
						if (y[0] == '+') { y = string(y.c_str() + 1); }

						if (x.size() < y.size()) {
							while (x.size() < y.size()) {
								x.push_back('0');
							}
						}
						else {
							while (y.size() < x.size()) {
								y.push_back('0');
							}
						}
						return Compare(reverse(x), reverse(y), 1);
					}
					else {
						return Compare(string(str2.c_str() + 1), string(str1.c_str() + 1));
					}
				}
				else if ("DOUBLE" == type1)
				{
					StringTokenizer tokenizer1(str1, ".");
					StringTokenizer tokenizer2(str2, ".");

					string x = tokenizer1.nextToken();
					string y = tokenizer2.nextToken();

					string z = Compare(x, y);
					if ("== 0" == z)
					{
						x = tokenizer1.nextToken();
						y = tokenizer2.nextToken();

						if (x.size() < y.size()) {
							while (x.size() < y.size()) {
								x.push_back('0');
							}
						}
						else {
							while (y.size() < x.size()) {
								y.push_back('0');
							}
						}
						return Compare(x, y, 1);
					}
					else
					{
						return z;
					}
				}
				else if ("DATE" == type1)
				{
					StringTokenizer tokenizer1(str1, ".");
					StringTokenizer tokenizer2(str2, ".");

					for (int i = 0; i < 3; ++i) {
						const string x = tokenizer1.nextToken();
						const string y = tokenizer2.nextToken();

						const string comp = Compare(x, y);

						if (comp == "< 0") { return comp; }
						else if (comp == "> 0") { return comp; }
					}
					return "== 0";
				}
				else if ("DATETIMEA" == type1) {
					StringTokenizer tokenizer1(str1, ".");
					StringTokenizer tokenizer2(str2, ".");

					for (int i = 0; i < 4; ++i) {
						const string x = tokenizer1.nextToken();
						const string y = tokenizer2.nextToken();

						const string comp = Compare(x, y);

						if (comp == "< 0") { return comp; }
						else if (comp == "> 0") { return comp; }
					}
					return "== 0";
				}
				else if ("DATETIMEB" == type2) {
					StringTokenizer tokenizer1(str1, ".");
					StringTokenizer tokenizer2(str2, ".");

					for (int i = 0; i < 5; ++i) {
						const string x = tokenizer1.nextToken();
						const string y = tokenizer2.nextToken();

						const string comp = Compare(x, y);

						if (comp == "< 0") { return comp; }
						else if (comp == "> 0") { return comp; }
					}
					return "== 0";
				}
				return "ERROR";
			}

			static string BoolOperation(const string& op, const string& x, const string& y)
			{
				if (x == "ERROR" || y == "ERROR") { return "ERROR"; }
				if ("NOT" == op) { return x == "TRUE" ? "FALSE" : "TRUE"; }
				else if ("AND" == op) {
					if (x == "TRUE" && y == "TRUE") { return "TRUE"; }
					else {
						return "FALSE";
					}
				}
				else if ("OR" == op) {
					if (x == "TRUE" || y == "TRUE") { return "TRUE"; }
					else {
						return "FALSE";
					}
				}
				else {
					return "ERROR";
				}
			}

		private:
			class DoThread // need to rename!
			{
			private:
				vector<string>* strVec;
			public:
				list<Token>* aq;
				//int strVecStart;
				//int strVecEnd;
			public:
				DoThread(vector<string>* strVec, list<Token>* aq) //, list<string>* aq)//, int strVecStart, int strVecEnd)
					: strVec(strVec), aq(aq) // , strVecStart(strVecStart), strVecEnd(strVecEnd)
				{
					//
				}
				void operator() (const tbb::blocked_range<size_t>& r) {
					vector<string>* strVecTemp = strVec; // enterkey 기준으로 나뉘어져있다고 가정한다.

					for (size_t x = r.begin(); x != r.end(); ++x)
					{
						//StringTokenizer tokenizer(std::move( (*strVecTemp)[x] ) );
						//while (tokenizer.hasMoreTokens()) {
						//	aq.push(tokenizer.nextToken());
						//}
						string statement = (*strVecTemp)[x];
						int token_first = 0, token_last = 0; // idx of token in statement.
						int state = 0;


						for (int i = 0; i < statement.size(); ++i) {
							if (0 == state && '\"' == statement[i]) {
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									string temp = statement.substr(token_first, token_last - token_first + 1);
									if (!temp.empty()) {
										aq->emplace_back(move(temp));
									}
								}
								state = 1;
								token_first = i; token_last = i;
							}
							else if (1 == state && '\"' == statement[i]) {
								state = 0; token_last = i;

								aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
								token_first = i + 1;
							}

							if (0 == state && '=' == statement[i]) {
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									string temp = statement.substr(token_first, token_last - token_first + 1);
									if (!temp.empty()) {
										aq->emplace_back(move(temp));
									}
								}
								aq->emplace_back(("="));
								token_first = i + 1;
							}
							else if (0 == state && isWhitespace(statement[i])) { // isspace ' ' \t \r \n , etc... ?
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									string temp = statement.substr(token_first, token_last - token_first + 1);
									if (!temp.empty()) {
										aq->emplace_back((move(temp)));
									}
								}
								token_first = i + 1;
							}
							else if (0 == state && '{' == statement[i]) {
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									string temp = statement.substr(token_first, token_last - token_first + 1);
									if (!temp.empty()) {
										aq->emplace_back((move(temp)));
									}
								}
								aq->emplace_back(("{"));
								token_first = i + 1;
							}
							else if (0 == state && '}' == statement[i]) {
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									string temp = statement.substr(token_first, token_last - token_first + 1);
									if (!temp.empty()) {
										aq->emplace_back((move(temp)));
									}
								}
								aq->emplace_back(("}"));
								token_first = i + 1;
							}

							if (0 == state && '#' == statement[i]) { // different from load_data_from_file
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									aq->emplace_back((statement.substr(token_first, token_last - token_first + 1)));
								}
								int j = 0;
								for (j = i; j < statement.size(); ++j) {
									if (statement[j] == '\n') // cf) '\r' ?
									{
										break;
									}
								}
								--j; // "before enter key" or "before end"
								
								if (j - i + 1 > 0) {
									aq->emplace_back(statement.substr(i, j - i + 1), true);
								}
								token_first = j + 2;
								i = token_first - 1;
							}
						}

						if (token_first < statement.size())
						{
							string temp = statement.substr(token_first);
							if (!temp.empty()) {
								aq->emplace_back((move(temp)));
							}
						}
					}
				}
				//
				//DoThread(DoThread& other, tbb::split) : strVec(other.strVec) // , aq(other.aq)
				//{
					//
				//}
				//void join(DoThread& other) 
				//{
				//	aqpush(std::move(other.aq));
				//}
			};
		public:
			static pair<bool, int> Reserve2(ifstream& inFile, list<Token>& aq, const int num = 1)
			{
				int count = 0;
				string temp;
				vector<string> strVecTemp;

				for (int i = 0; i < num && (getline(inFile, temp)); ++i) {
					if (temp.empty()) { continue; }
					strVecTemp.push_back(temp);
					count++;
				}

				
				DoThread doThread(&strVecTemp, &aq);
				
				doThread(tbb::blocked_range<size_t>(0, count));
				
				//tbb::parallel_reduce(tbb::blocked_range<size_t>(0, count), doThread);
				//aq.push(std::move(doThread.aq));
				
				return{ count > 0, count };
			}

			/// must lineNum > 0
			static pair<bool, int> Reserve(ifstream& inFile, list<string>& strVec, const int num = 1)
			{
				string temp;
				int count = 0;

				for (int i = 0; i < num && (inFile >> temp); ++i) {
					strVec.push_back(temp);
					count++;
				}
				return{ count > 0, count };
			}

			template<class Reserver>
			static const string& Top(list<Token>& strVec, wiz::load_data::UserType* ut, Reserver reserver)
			{
				do {
					while (!strVec.empty()) {
						if ((strVec.begin())->isComment) {
							ut->PushComment(move((strVec.begin())->str));
							strVec.pop_front(); // pop_front
						}
						else {
							return (strVec.begin())->str;
						}
					}
					reserver(strVec);
					while (strVec.empty()) // (strVec.empty())
					{
						reserver(strVec);
						if (
							strVec.empty() &&
							reserver.end()
							) {
							return ""; //throw wiz::Error("Error1 in Top function in load_data_utility.h");
						}
					}
				} while (true);
				throw wiz::Error("Error2 in Top function in load_data_utility.h");
			}
			template <class Reserver>
			static bool Pop(list<Token>& strVec, string* str, wiz::load_data::UserType* ut, Reserver reserver)
			{
				do {
					while (!strVec.empty()) {
						if ((strVec.begin())->isComment) {
							ut->PushComment(move((strVec.begin())->str));
							strVec.pop_front(); // pop_front
						}
						else {
							Token token = strVec.front();
							strVec.pop_front();

							//cout << token.str << endl;

							if (str) {
								*str = move(token.str);
							}
							return true;
						}
					}
					reserver(strVec);
					while (strVec.empty()) // (strVec.empty())
					{
						reserver(strVec);
						if (
							strVec.empty() &&
							reserver.end()
							) {
							return false; //throw wiz::Error("Error1 in Pop function in load_data_utility.h");
						}
					}
				} while (true);
				throw wiz::Error("Error2 in Pop function in load_data_utility.h");
			}
		
			template <class Reserver>
			static pair<bool, Token> LookUp(list<Token>& strVec, const int offset, wiz::load_data::UserType* ut, Reserver reserver)
			{
				int count = 0;
				list<Token>::iterator x;

				do {
					x = strVec.begin();
					while (strVec.size() > offset) {
						if (x->isComment) {
							ut->PushComment(move(x->str));
							x = strVec.erase(x);
						}
						else if (offset == count) {
							return{ true, *x };
						}
						else {
							count++;
							++x;
						}
					}
					reserver(strVec);
					while (strVec.size() <= offset) // (strVec.empty())
					{
						reserver(strVec);
						if (
							strVec.size() <= offset &&
							reserver.end()
							) {
							return{ false, Token() };
						}
					}
				} while (true);
				throw wiz::Error("Error in LookUp function in load_data_utility.h");
			}
		public:
			//
			static bool ChkExist(const string& str) // for \"
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
					else if (0 >= state && str[i] == '#') {
						break;
					}
				}

				return 0 == state; // exist and valid !! chk - todo!
			} 

			// AddSpace : return string
			static void AddSpace(const string& str, string& temp)
			{
				temp = "";

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

				//return temp;
			}

			/// need testing!
			static void PassSharp(const string& str, string& temp) { // txt file로 들어온다?
				temp = "";
				int state = 0;

				for (int i = 0; i < str.size(); ++i) {
					if (str[i] == '#') { state = 1; continue; }
					else if (str[i] == '\n') { state = 0; continue; }

					if (0 == state) {
						temp.push_back(str[i]);
					}
				}
				//return temp;
			}

			static bool _ChangeStr(const string& str, const vector<string>& changed_str, const vector<string>& result_str, string::size_type& i, int& state, string& temp) {
				for (string::size_type j = 0; j < changed_str.size(); ++j) {
					if (wiz::String::Comp(changed_str[j].c_str(), str.c_str() + i, changed_str[j].size())) {
						state = 1;
						temp.append(result_str[j]);
						i = i + changed_str[j].size() - 1;
						return true;
					}
				}
				return false;
			}
			// 길이가 긴 문자열이 먼저 나와야 한다?
			static void ChangeStr(const string& str, const vector<string>& changed_str, const vector<string>& result_str, string& temp) {
				temp = "";
				int state = 0;


				for (string::size_type i = 0; i < str.size(); ++i)
				{
					if (0 == state && i == 0 && '\"' == str[i]) {
						state = 1;
						temp.push_back(str[i]);
					}
					else if (0 == state && i > 0 && '\"' == str[i] && '\\' != str[i - 1])
					{
						state = 1;
						temp.push_back(str[i]);
					}
					else if (1 == state  && _ChangeStr(str, changed_str, result_str, i, state, temp)) {
						//
					}
					else if ((1 == state && i > 0 && '\\' != str[i - 1] && '\"' == str[i])) {
						state = 0;
						temp.push_back('\"');
					}
					else
					{
						temp.push_back(str[i]);
					}
				}

				//return temp;
			}
		};
	}
}

#endif
