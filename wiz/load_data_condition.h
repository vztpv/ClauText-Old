
#ifndef LOAD_DATA_CONDITION_H
#define LOAD_DATA_CONDITION_H

#include <vector>
#include <string>
#include <algorithm>
using namespace std;

#include <wiz/stacks.h>
#include <wiz/load_data_types.h>
#include <wiz/load_data_utility.h>

namespace wiz {
	namespace load_data {
		class Condition {
		private:
			int braceNum;
			std::vector<string> tokenVec;
			ArrayStack<string> tokenStack;
			string condition;
			int i;
			UserType* position; // need set, get ter!!
			UserType* global;
			int option;
		public:
			explicit Condition(const string& condition, UserType* position, UserType* global, const int option = 0)
				: braceNum(0), condition(condition), i(0), position(position), option(option)
			{
				Init(condition);
			}
		private:
			string reverse(string str) { /// to std::reverse ?
				std::reverse(str.begin(), str.end());
				return str;
			}
			
			string GetType(const string& str) {
				if (Utility::IsInteger(str)) { return "INTEGER"; }
				else if (Utility::IsDouble(str)) { return "DOUBLE"; }
				else if (Utility::IsDate(str)) { return "DATE"; }
				else return "STRING";
			}
			string Compare(const string& str1, const string& str2, const int type = 0)
			{
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
							for (int i = 0; i < y.size() - x.size(); ++i) {
								x.push_back('0');
							}
						}
						else {
							for (int i = 0; i < x.size() - y.size(); ++i) {
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
					if ("= 0" == z)
					{
						x = tokenizer1.nextToken();
						y = tokenizer2.nextToken();

						if (x.size() < y.size()) {
							for (int i = 0; i < y.size() - x.size(); ++i) {
								x.push_back('0');
							}
						}
						else {
							for (int i = 0; i < x.size() - y.size(); ++i) {
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

				return "ERROR";
			}

			auto Get(const string& var, const string& position, UserType* utPosition, UserType* global) {
				string valTemp = position;
				StringTokenizer tokenizer(position, "/");
				UserType* utTemp;

				if (false == tokenizer.hasMoreTokens()) {
					utTemp = utPosition;
				}
				else {
					if (tokenizer.nextToken() != "root") {
						utTemp = utPosition;
					}
					else {
						valTemp = string(position.c_str() + 5);
						utTemp = global;
					}
				}
				return UserType::Find(utTemp, valTemp);
			}
			string GetValue(const string& op, const string& var, const string& val, UserType* utPosition, UserType* global, const string& option = "0")
			{
				/// ExistItem, ExistUserType
				if (NULL == utPosition) { return "ERROR"; }
				if ("EXISTITEM" == op) { /// option == 1?	
					auto x = Get(var, val, utPosition, global);
					if (x.first) {
						//if (x.second.size() > 1) { return "ERROR"; } ///
						for (int i = 0; i < x.second.size(); ++i) {
							if (x.second[i]->GetItem(var).size() > 0)
							{
								return "TRUE";
							}
						}
						return "FALSE";
					}
					return "FALSE";
				}
				else if ("EXISTUSERTYPE" == op) { /// option == 1?	
					auto x = Get(var, val, utPosition, global);
					if (x.first) {
						//if (x.second.size() > 1) { return "ERROR"; } ///
						for (int i = 0; i < x.second.size(); ++i) {
							if (x.second[i]->GetUserTypeItem(var).size() > 0)
							{
								return "TRUE";
							}
						}
						return "FALSE";
					}
					return "FALSE";
				}
				else if ("NOTEXISTITEM" == op) { /// option == 2 ?
					auto x = Get(var, val, utPosition, global);
					if (x.first) {
						//if (x.second.size() > 1) { return "ERROR"; } ///

						for (int i = 0; i < x.second.size(); ++i) {
							if (0 < x.second[i]->GetItem(var).size())
							{
								return "FALSE";
							}
						}
						return "TRUE";
					}
					return "FALSE";
				}
				else if ("NOTEXISTUSERTYPE" == op) { /// option == 2 ?
					auto x = Get(var, val, utPosition, global);
					if (x.first) {
						//if (x.second.size() > 1) { return "ERROR"; } ///

						for (int i = 0; i < x.second.size(); ++i) {
							if (0 < x.second[i]->GetUserTypeItem(var).size())
							{
								return "FALSE";
							}
						}
						return "TRUE";
					}
					return "FALSE";
				}

				return "ERROR";
			}
			string GetValue(const string& op, const string& var1, const string& position1, const string& var2, const string& position2,
				UserType* utPosition, UserType* global, const string& option = "0")
			{
				// COMP<, COMP>, EQ, NOTEQ
				if (NULL == utPosition) { return "ERROR"; }

				auto x = Get(var1, position1, utPosition, global);
				auto y = Get(var2, position2, utPosition, global);

				if ("~" != position1 && false == x.first)
				{
					return "ERROR GV1";
				}
				if ("~" != position2 && false == y.first)
				{
					return "ERROR GV2";
				}
				//
				if (((x.first && x.second.size() > 1) || (y.first && y.second.size() > 1))) {
					return "ERROR GV3";
				}

				vector<ItemType<string>> value1;  // Item<string> <- 
				vector<ItemType<string>> value2;
				
				// added..
				if (position1 != "~") {
					value1 = x.second[0]->GetItem(var1);
				}
				if (position2 != "~") {
					value2 = y.second[0]->GetItem(var2);
				}
				//
				if (position1 == "~" ) {
					value1.push_back(ItemType<string>("")); // var1
					value1[0].Push(var1);
				}
				if (position2 == "~") {
					value2.push_back(ItemType<string>("")); // var2
					value2[0].Push(var2);
				}

				//
				if (value1.size() == 0) {
					return "ERROR GV4";
				}
				if (value2.size() == 0) {
					return "ERROR GV5";
				}

				if (option == "0" && (value1.size() > 1 || value2.size() > 1)) {
					return "ERROR GV6";
				}

				if ("COMP<" == op) {
					if ("0" != option) { /// ToDo.. // 0. just 1-1, // 1. for any case // 2. for all case
						for (int i = 0; i < value1.size(); ++i) {
							for (int j = 0; j < value2.size(); ++j) {
								if (Compare(value1[i].Get(0), value2[j].Get(0)) == "< 0") {
									if ("1" == option) { return "TRUE"; }
								}
								else {
									if ("2" == option) { return "FALSE"; }
								}
							}
						}
						if ("1" == option) { return "FALSE"; }
						else if ("2" == option) {
							return "TRUE";
						}
					}
					else {
						if (Compare(value1[0].Get(0), value2[0].Get(0)) == "< 0") {
							return "TRUE";
						}
					}
					return "FALSE";
				}
				else if ("COMP<EQ" == op) {
					if ("0" != option) { /// ToDo.. // 0. just 1-1, // 1. for any case // 2. for all case
						for (int i = 0; i < value1.size(); ++i) {
							for (int j = 0; j < value2.size(); ++j) {
								string temp = Compare(value1[i].Get(0), value2[j].Get(0));
									if (temp == "< 0" || temp == "== 0") {
										if ("1" == option) { return "TRUE"; }
									}
									else {
										if ("2" == option) { return "FALSE"; }
									}
							}
						}
						if ("1" == option) { return "FALSE"; }
						else if ("2" == option) {
							return "TRUE";
						}
					}
					else {
						string temp = Compare(value1[0].Get(0), value2[0].Get(0));
						if (temp == "< 0" || temp == "== 0") {
							return "TRUE";
						}
					}
					return "FALSE";
				}
				else if ("COMP>" == op) {
					if ("0" != option) { /// ToDo.. // 0. just 1-1, // 1. for any case // 2. for all case
						for (int i = 0; i < value1.size(); ++i) {
							for (int j = 0; j < value2.size(); ++j) {
								if (Compare(value1[i].Get(0), value2[j].Get(0)) == "> 0") {
									if ("1" == option) { return "TRUE"; }
								}
								else {
									if ("2" == option) { return "FALSE"; }
								}
							}
						}
						if ("1" == option) { return "FALSE"; }
						else if ("2" == option) {
							return "TRUE";
						}
					}
					else {
						if (Compare(value1[0].Get(0), value2[0].Get(0)) == "> 0") {
							return "TRUE";
						}
					}
					return "FALSE";
				}
				else if ("COMP>EQ" == op) {
					if ("0" != option) { /// ToDo.. // 0. just 1-1, // 1. for any case // 2. for all case
						for (int i = 0; i < value1.size(); ++i) {
							for (int j = 0; j < value2.size(); ++j) {
								string temp = Compare(value1[i].Get(0), value2[j].Get(0));
								if (temp == "> 0" || temp == "== 0") {
									if ("1" == option) { return "TRUE"; }
								}
								else {
									if ("2" == option) { return "FALSE"; }
								}
							}
						}
						if ("1" == option) { return "FALSE"; }
						else if ("2" == option) {
							return "TRUE";
						}
					}
					else {
						string temp = Compare(value1[0].Get(0), value2[0].Get(0));
						if (temp == "> 0" || temp == "== 0") {
							return "TRUE";
						}
					}
					return "FALSE";
				}
				else if ("EQ" == op) {
					if ("0" != option) { /// ToDo.. // 0. just 1-1, // 1. for any case // 2. for all case
						for (int i = 0; i < value1.size(); ++i) {
							for (int j = 0; j < value2.size(); ++j) {
								if (Compare(value1[i].Get(0), value2[j].Get(0)) == "== 0") {
									if ("1" == option) { return "TRUE"; }
								}
								else {
									if ("2" == option) { return "FALSE"; }
								}
							}
						}
						if ("1" == option) { return "FALSE"; }
						else if ("2" == option) {
							return "TRUE";
						}
					}
					else {
						if (Compare(value1[0].Get(0), value2[0].Get(0)) == "== 0") {
							return "TRUE";
						}
					}
					return "FALSE";
				}
				else if ("NOTEQ" == op) {
					if ("0" != option) { /// ToDo.. // 0. just 1-1, // 1. for any case // 2. for all case
						for (int i = 0; i < value1.size(); ++i) {
							for (int j = 0; j < value2.size(); ++j) {
								if (Compare(value1[i].Get(0), value2[j].Get(0)) != "== 0") {
									if ("1" == option) { return "TRUE"; }
								}
								else {
									if ("2" == option) { return "FALSE"; }
								}
							}
						}
						if ("1" == option) { return "FALSE"; }
						else if ("2" == option) {
							return "TRUE";
						}
					}
					else {
						if (Compare(value1[0].Get(0), value2[0].Get(0)) != "== 0") {
							return "TRUE";
						}
					}
					return "FALSE";
				}

				return "ERROR GV7";
			}
			string BoolOperation(const string& op, const string& x, const string& y)
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

			void Init(const string& condition)
			{
				bool evalue = false;

				string str = Utility::AddSpace(condition);
				StringTokenizer tokenizer(str, { " ", "\t", "\n", "\r" });

				while (tokenizer.hasMoreTokens()) {
					string temp = tokenizer.nextToken();
					if (temp == "^") {
						temp = "";
					}
					tokenVec.push_back(temp);
				}
			}
		public:
			bool Next() {
				try {
					if (i >= tokenVec.size()) { return false; }
					if (condition.empty()) {
						return false;
					}
					{
						if ("=" != tokenVec[i]
							&& "{" != tokenVec[i]
							&& "}" != tokenVec[i]) {
							tokenStack.push(tokenVec[i]);
						}

						if ("{" == tokenVec[i]) {
							braceNum++;
						}
						else if ("}" == tokenVec[i]) {
							braceNum--;

							// COMP<, COMP<EQ, COMP>, COMP>EQ, EQ, NOTEQ
							if (tokenStack.size() >= 6 && ("COMP<" == tokenStack[tokenStack.size() - 6]
								|| "COMP<EQ" == tokenStack[tokenStack.size() - 6]
								|| "COMP>" == tokenStack[tokenStack.size() - 6]
								|| "COMP>EQ" == tokenStack[tokenStack.size() - 6]
								|| "EQ" == tokenStack[tokenStack.size() - 6]
								|| "NOTEQ" == tokenStack[tokenStack.size() - 6]))
							{
								const string op = tokenStack[tokenStack.size() - 6];
								const string var1 = tokenStack[tokenStack.size() - 5];
								const string position1 = tokenStack[tokenStack.size() - 4];
								const string var2 = tokenStack[tokenStack.size() - 3];
								const string position2 = tokenStack[tokenStack.size() - 2];
								const string option = tokenStack[tokenStack.size() - 1];
								tokenStack.pop();
								tokenStack.pop();
								tokenStack.pop();
								tokenStack.pop();
								tokenStack.pop();
								tokenStack.pop();

								tokenStack.push(GetValue(op, var1, position1, var2, position2, position, global, option));
							}
							else if (tokenStack.size() >= 3 && "NOT" != tokenStack[tokenStack.size() - 2]) {
								string op = tokenStack[tokenStack.size() - 3];
								const string operand1 = tokenStack[tokenStack.size() - 2];
								const string operand2 = tokenStack[tokenStack.size() - 1];

								if (op == "AND" || op == "OR") {
									tokenStack.pop();
									tokenStack.pop();
									tokenStack.pop();

									tokenStack.push(BoolOperation(op, operand1, operand2));
								}
								else { // EXIST, NOTEXIST for ( ITEM or USERTYPE ) 
									tokenStack.pop();
									tokenStack.pop();
									tokenStack.pop();

									tokenStack.push(GetValue(op, operand1, operand2, position, global));
								}
							}
							else if (tokenStack.size() >= 2 && "NOT" == tokenStack[tokenStack.size() - 2]) {
								const string op = tokenStack[tokenStack.size() - 2];
								const string operand1 = tokenStack[tokenStack.size() - 1];

								tokenStack.pop();
								tokenStack.pop();
								tokenStack.push(BoolOperation(op, operand1, operand1));
							}
						}
						++i;
					}
				}
				catch (const char* err) { cout << err << endl; return false; }
				catch (Error& e) { cout << e << endl; return false; }
				return true;
			}
		public:
			int NowIndex()const { return i; }
			const ArrayStack<string>& Now()const { return tokenStack; }
			ArrayStack<string>& Now() { return tokenStack; }
			bool IsNowImportant() const { // ??
				return "}" == tokenVec[i] && "NOT" != tokenStack[tokenStack.size() - 2];
			}
		};
	}
}

#endif
