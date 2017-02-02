
#ifndef LOAD_DATA_H_INCLUDED
#define LOAD_DATA_H_INCLUDED


#include <vector>
#include <stack>
#include <fstream>
#include <string>
#include <utility>
#include <algorithm>
using namespace std;

#include <wiz/global.h>
//#include <wiz/Dictionary.h> /// change to map

#include <wiz/cpp_string.h>
#include <wiz/queues.h>
#include <wiz/stacks.h>
#include <wiz/deck.h>

#include <wiz/load_data_types.h>
#include <wiz/load_data_utility.h>
#include <wiz/load_data_reservers.h>
#include <wiz/load_data_condition.h>

namespace wiz {
	namespace load_data {
		const string LEFT = "{";
		const string RIGHT = "}";
		const string EQ = "=";

		const int RIGHT_DO = 1;

		class LoadData
		{
			/// core
		public:
			template <class Reserver>
			static bool _LoadData(ArrayQueue<string>& strVec, Reserver& vecReserver, UserType& global) // first, strVec.empty() must be true!!
			{
				int state = 0;
				int braceNum = 0;
				stack<int> state_reserve;
				stack<int> do_reserve;
				vector< UserType* > nestedUT(1);
				string var1, var2, val;

				bool varOn = false;

				nestedUT[0] = &global;
				{
					vecReserver(strVec);

					while (strVec.empty())
					{
						vecReserver(strVec);
						if (
							strVec.empty() &&
							vecReserver.end()
							) {
							return false; // throw "Err nextToken does not exist"; // cf) or empty file or empty string!
						}
					}
				}

				while (false == strVec.empty()) {
					switch (state)
					{
					case 0:
						if (LEFT == Utility::Top(strVec)) {
							//Utility::Pop(strVec);
							state = 2;
						}
						else {
							pair<bool, string> bsPair = Utility::LookUp(strVec, 1);
							if (bsPair.first) {
								if (EQ == bsPair.second) {
									var2 = Utility::Pop(strVec);
									Utility::Pop(strVec);
									state = 2;
								}
								else {
									var1 = Utility::Pop(strVec);
									nestedUT[braceNum]->AddItem("", move(var1));
									state = 0;
								}
							}
							else {
								var1 = Utility::Pop(strVec);
								nestedUT[braceNum]->AddItem("", move(var1));
								state = 0;
							}
						}
						break;
					case 1:
						if (RIGHT == Utility::Top(strVec)) {
							Utility::Pop(strVec);
							state = 0;
						}
						else {
							// syntax error.
							throw "syntax error 1 ";
						}
						break;
					case 2:
						if (LEFT == Utility::Top(strVec)) {
							Utility::Pop(strVec);

							///
							nestedUT[braceNum]->AddUserTypeItem(UserType(var2));
							UserType* pTemp;
							nestedUT[braceNum]->GetLastUserTypeItemRef(var2, pTemp);

							braceNum++;

							/// new nestedUT
							if (nestedUT.size() == braceNum) /// changed 2014.01.23..
								nestedUT.push_back(nullptr);

							/// initial new nestedUT.
							nestedUT[braceNum] = pTemp;
							///
							state = 3;
						}
						else {
							val = Utility::Pop(strVec);

							nestedUT[braceNum]->AddItem(move(var2), move(val));
							var2 = "";
							val = "";

							state = 0;
						}
						break;
					case 3:
						if (RIGHT == Utility::Top(strVec)) {
							Utility::Pop(strVec);

							nestedUT[braceNum] = nullptr;
							braceNum--;

							state = 0;
						}
						else {
							/*
							pair<bool, int> biPair = IsMatched(strVec, "{", "}", 0, 1);
							if (false == biPair.first) {
							bool biPair2 = vecReserver(strVec);

							while (!biPair.first && biPair2)
							{
							biPair2 = vecReserver(strVec);
							biPair = IsMatched(strVec, "{", "}", 0, 1);
							}
							}
							if (biPair.first)
							*/
							{
								/// uisng struct
								state_reserve.push(0);
								do_reserve.push(RIGHT_DO);
								state = 4;
							}
							//else
							{
								//	throw  "syntax error 2 ";
							}
						}
						break;
					case 4:
						if (LEFT == Utility::Top(strVec)) {
							Utility::Pop(strVec);

							UserType temp("");

							nestedUT[braceNum]->AddUserTypeItem(temp);
							UserType* pTemp;
							nestedUT[braceNum]->GetLastUserTypeItemRef("", pTemp);

							braceNum++;

							/// new nestedUT
							if (nestedUT.size() == braceNum) /// changed 2014.01.23..
								nestedUT.push_back(nullptr);

							/// initial new nestedUT.
							nestedUT[braceNum] = pTemp;
							///
							//}

							state = 5;
						}
						else if (RIGHT == Utility::Top(strVec)) {
							Utility::Pop(strVec);
							state = state_reserve.top();
							state_reserve.pop();
							const int do_id = do_reserve.top();
							do_reserve.pop();
							//if (do_id == RIGHT_DO)
							{
								nestedUT[braceNum] = nullptr;
								braceNum--;
							}
						}
						else {
							pair<bool, string> bsPair = Utility::LookUp(strVec, 1);
							if (bsPair.first) {
								if (EQ == bsPair.second) {
									// var2
									var2 = Utility::Pop(strVec);
									Utility::Pop(strVec); // pass EQ
									state = 6;
								}
								else {
									// var1
									var1 = Utility::Pop(strVec);
									nestedUT[braceNum]->AddItem("", move(var1));
									var1 = "";

									state = 4;
								}
							}
							else
							{
								// var1
								var1 = Utility::Pop(strVec);
								nestedUT[braceNum]->AddItem("", move(var1));
								var1 = "";

								state = 4;
							}
						}
						break;
					case 5:
						if (RIGHT == Utility::Top(strVec)) {
							Utility::Pop(strVec);

							//if (flag1 == 0) {
							nestedUT[braceNum] = nullptr;
							braceNum--;
							// }
							//
							state = 4;
						}

						else {
							int idx = -1;
							int num = -1;

							/*(pair<bool, int> biPair = IsMatched(strVec, "{", "}", 0, 1, &idx, &num);
							if (false == biPair.first) {
							bool biPair2 = vecReserver(strVec);

							while (!biPair.first && biPair2)
							{
							biPair2 = vecReserver(strVec);

							biPair = IsMatched(strVec, "{", "}", idx + 1, num, &idx, &num);
							}
							}
							if (biPair.first)
							*/
							{
								/// uisng struct
								state_reserve.push(4);

								{
									do_reserve.push(RIGHT_DO);
								}
								state = 4;
							}
							//else
							{
								//	throw "syntax error 4  ";
							}
						}
						break;
					case 6:
						if (LEFT == Utility::Top(strVec)) {
							Utility::Pop(strVec);

							///
							{
								nestedUT[braceNum]->AddUserTypeItem(UserType(var2));
								UserType* pTemp;
								nestedUT[braceNum]->GetLastUserTypeItemRef(var2, pTemp);
								var2 = "";
								braceNum++;

								/// new nestedUT
								if (nestedUT.size() == braceNum) /// changed 2014.01.23..
									nestedUT.push_back(nullptr);

								/// initial new nestedUT.
								nestedUT[braceNum] = pTemp;
							}
							///
							state = 7;
						}
						else {
							val = Utility::Pop(strVec);

							nestedUT[braceNum]->AddItem(move(var2), move(val));
							var2 = ""; val = "";
							if (strVec.empty())
							{
								//
							}
							else if (RIGHT == Utility::Top(strVec)) {
								Utility::Pop(strVec);

								{
									state = state_reserve.top();
									state_reserve.pop();
									const int do_id = do_reserve.top();
									do_reserve.pop();
									//if (do_id == RIGHT_DO)
									{
										nestedUT[braceNum] = nullptr;
										braceNum--;
									}
								}
								{
									//state = 4;
								}
							}
							else {
								state = 4;
							}
						}
						break;
					case 7:
						if (RIGHT == Utility::Top(strVec)) {
							Utility::Pop(strVec);
							//

							nestedUT[braceNum] = nullptr;
							braceNum--;
							//
							state = 4;
						}
						else {
							int idx = -1;
							int num = -1;
							/*
							pair<bool, int> biPair = IsMatched(strVec, "{", "}", 0, 1, &idx, &num);
							if (false == biPair.first) {
							bool biPair2 = vecReserver(strVec);

							while (!biPair.first && biPair2)
							{
							biPair2 = vecReserver(strVec);

							biPair = IsMatched(strVec, "{", "}", idx + 1, num, &idx, &num);
							}
							}
							if (biPair.first) */
							{
								/// uisng struct
								state_reserve.push(4);

								{
									do_reserve.push(RIGHT_DO);
								}
								state = 4;
							}
							//else
							{
								//throw "syntax error 5 ";
							}
						}
						break;
					default:
						// syntax err!!

						throw "syntax error 6 ";
						break;
					}

					if (strVec.size() < 10) {
						vecReserver(strVec);

						while (strVec.empty()) // (strVec.empty())
						{
							vecReserver(strVec);
							if (
								strVec.empty() &&
								vecReserver.end()
								) {
								// throw "Err nextToken does not exist2";
								break;
							}
						}
					}
				}
				if (state != 0) {
					throw "error final state is not 0!";
				}
				if (braceNum != 0) {
					throw string("chk braceNum is ") + toStr(braceNum);
				}

				return true;
			}

		public:
			static bool LoadDataFromFile(const string& fileName, UserType& global) /// global should be empty
			{
				ifstream inFile;
				inFile.open(fileName);
				if (true == inFile.fail())
				{
					inFile.close(); return false;
				}
				UserType globalTemp = global;
				ArrayQueue<string> strVec;

				try {
					InFileReserver ifReserver(inFile);

					ifReserver.Num = 100000;
					// cf) empty file..
					if (false == _LoadData(strVec, ifReserver, globalTemp))
					{
						return true;
					}


					//UserType::AllReserved(&globalTemp, false);
					//UserType::ReservedA(&globalTemp);

					UserType::ReplaceAll(&globalTemp, { "^5", "^4", "^3", "^2", "^1", "^0" }, { "#", "\n", "\r", "\t", " ", "^" });
					//UserType::ReplaceAll(&globalTemp, "^4", "\n");
					//UserType::ReplaceAll(&globalTemp, "^3", "\r");
					//UserType::ReplaceAll(&globalTemp, "^2", "\t");
					//UserType::ReplaceAll(&globalTemp, "^1", " ");
					//UserType::ReplaceAll(&globalTemp, "^0", "^");

					//UserType::AllReserved(&globalTemp, false);

					inFile.close();
				}
				catch (Error e) { std::cout << e << endl; inFile.close(); return false; }
				catch (const char* err) { std::cout << err << endl; inFile.close(); return false; }
				catch (const string& e) { std::cout << e << endl; inFile.close(); return false; }
				catch (exception e) { std::cout << e.what() << endl; inFile.close(); return false; }
				catch (...) { std::cout << "not expected error" << endl; inFile.close(); return false; }

				global = move(globalTemp);
				return true;
			}

			static bool LoadDataFromString(string str, UserType& ut)
			{
				UserType utTemp = ut;
				string temp;
				bool chk = Utility::ChkExist(str);
				if (chk) {
					Utility::ChangeStr(str, { "^" }, { "^0" }, temp);
					Utility::ChangeStr(temp, { "#" }, { "^5" }, str);
				}
				Utility::PassSharp(str, temp);
				Utility::AddSpace(temp, str);

				if (chk) {
					Utility::ChangeStr(str, { " ", "\t", "\r", "\n" }, { "^1", "^2", "^3", "^4" }, temp);
					//str = Utility::ChangeStr(str, "\t", "^2");
					//str = Utility::ChangeStr(str, "\r", "^3");
					//str = Utility::ChangeStr(str, "\n", "^4");
					str = std::move(temp);
				}
				StringTokenizer tokenizer(str, vector<string>{" ", "\t", "\r", "\n"});
				ArrayQueue<string> strVec;

				while (tokenizer.hasMoreTokens())
				{
					strVec.push(tokenizer.nextToken());
				}
				try {
					// empty string!
					NoneReserver nonReserver;
					if (false == _LoadData(strVec, nonReserver, utTemp))
					{
						return true;
					}

					//UserType::AllReserved(&utTemp, false);
					//UserType::ReservedA(&utTemp);

					if (chk) {
						UserType::ReplaceAll(&utTemp, { "^5", "^4", "^3", "^2", "^1", "^0" }, { "#", "\n", "\r", "\t", " ", "^" });
						//UserType::ReplaceAll(&utTemp, "^4", "\n");
						//UserType::ReplaceAll(&utTemp, "^3", "\r");
						//UserType::ReplaceAll(&utTemp, "^2", "\t");
						//UserType::ReplaceAll(&utTemp, "^1", " ");
						//UserType::ReplaceAll(&utTemp, "^0", "^");
					}
					//UserType::AllReserved(&utTemp, false);
				}
				catch (Error& e) { std::cout << e << endl; return false; }
				catch (const char* err) { std::cout << err << endl; return false; }
				catch (exception& e) { std::cout << e.what() << endl; return false; }
				catch (string str) { std::cout << str << endl; return false; }
				catch (...) { std::cout << "not expected error" << endl; return  false; }

				ut = std::move(utTemp);
				return true;
			}

		private:
			UserType global; // ToDo - remove!
		public:
			/// To Do.. static function -> ~.function.!
			// InitQuery or LoadQuery
			explicit LoadData() { InitWizDB(); }

			/// need testing!
			LoadData(const LoadData& ld)
				: global(ld.global)
			{
				//global = ld.global;
			}
			virtual ~LoadData() { AllRemoveWizDB(); }

			/// need testing!
			LoadData& operator=(const LoadData& ld)
			{
				if (this == &ld) { return *this; }

				global = ld.global;
				return *this;
			}
			//
			bool InitWizDB() {
				return InitWizDB(global);
			}
			// allRemove Query 
			bool AllRemoveWizDB() {
				return AllRemoveWizDB(global);
			}
			// AddQuery AddData, AddUserTypeData
			bool AddData(const string& position, const string& data, const string& condition = "") {
				return AddData(global, position, data, condition);
			}
			// 
			bool AddNoNameUserType(const string& position, const string& data, const string& condition = "")
			{
				return AddNoNameUserType(global, position, data, condition);
			}
			// SetQuery
			bool SetData(const string& position, const string& varName, const string& data, const string& condition = "")
			{
				return SetData(global, position, varName, data, condition);
			}
			/// 
			string GetData(const string& position, const string& condition) {
				return GetData(global, position, condition);
			}
			string GetItemListData(const string& position, const string& condition)
			{
				return GetItemListData(global, position, condition);
			}
			string GetItemListNamesData(const string& position, const string& condition)
			{
				return GetItemListNamesData(global, position, condition);
			}
			string GetUserTypeListNamesData(const string& position, const string& condition)
			{
				return GetUserTypeListNamesData(global, position, condition);
			}
			/// varName = val - do
			/// varName = { val val val } - GetData(position+"/varName", ""); 
			/// varName = { var = val } - GetData(position+"/varname", var);
			string GetData(const string& position, const string& varName, const string& condition) // 
			{
				return GetData(global, position, varName, condition);
			}
			/*
			bool RemoveData(const string& position) {
			auto finded = Find(global, position);
			if (finded.first) {
			for (int i = 0; i < finded.second.size(); ++i) {
			finded.second[i]->Remove(); // todo - ����..
			}
			return true;
			}
			else {
			return false;
			}
			}
			*/
			bool Remove(const string& position, const string& var, const string& condition) {
				return Remove(global, position, var, condition);
			}

			bool LoadWizDB(const string& fileName) {
				return LoadWizDB(global, fileName);
			}
			// SaveQuery
			bool SaveWizDB(const string& fileName, const string& option = "0") { /// , int option
				return SaveWizDB(global, fileName, option);
			}

			/// To Do - ExistItem, ExistUserType, SetUserType GetUserType
			bool ExistData(const string& position, const string& varName, const string& condition) // 
			{
				return ExistData(global, position, varName, condition);
			}
			bool ChkData()
			{
				return UserType::ChkData(&global);
			}

			/// ToDo - recursive function
			string SearchItem(const string& var, const string& condition)
			{
				return SearchItem(global, var, condition);
			}
			string SearchUserType(const string& var, const string& condition)
			{
				return SearchUserType(global, var, condition);
			}
		private:
			void SearchItem(vector<string>& positionVec, const string& var, const string& nowPosition,
				UserType* ut, const string& condition)
			{
				SearchItem(global, positionVec, var, nowPosition, ut, condition);
			}
			void SearchUserType(vector<string>& positionVec, const string& var, const string& nowPosition,
				UserType* ut, const string& condition)
			{
				SearchUserType(global, positionVec, var, nowPosition, ut, condition);
			}
		private:
			static void SearchItem(UserType& global, vector<string>& positionVec, const string& var, const string& nowPosition,
				UserType* ut, const string& condition)
			{
				string _var = var;
				if (_var == " ") { _var = ""; }
				if (ut->GetItem(_var).size() > 0) {
					string _condition = condition;

					if (_var == "") { _condition = wiz::String::replace(_condition, "~~", "^"); }
					else
						_condition = wiz::String::replace(_condition, "~~", _var); //
					Condition cond(_condition, ut, &global);

					while (cond.Next());

					if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
					{
						positionVec.push_back(nowPosition);
					}
				}

				for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
					string temp = ut->GetUserTypeList(i)->GetName();
					if (temp == "") { temp = " "; }
					SearchItem(
						global,
						positionVec,
						_var,
						nowPosition + "/" + temp,
						ut->GetUserTypeList(i),
						condition
					);
				}
			}

			static void SearchUserType(UserType& global, vector<string>& positionVec, const string& var, const string& nowPosition,
				UserType* ut, const string& condition)
			{
				string _var = var;
				if (_var == " ") {
					_var = "";
				}
				if (ut->GetUserTypeItem(_var).size() > 0) {
					string _condition = condition;

					if (_var == "") { _condition = wiz::String::replace(_condition, "~~", "^"); }
					else  _condition = wiz::String::replace(_condition, "~~", _var); //

					Condition cond(_condition, ut, &global);

					while (cond.Next());

					if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
					{
						positionVec.push_back(nowPosition);
					}
				}

				for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
					string temp = ut->GetUserTypeList(i)->GetName();

					if (temp == "") { temp = " "; }
					SearchUserType(
						global,
						positionVec,
						_var,
						nowPosition + "/" + temp,
						ut->GetUserTypeList(i),
						condition
					);
				}
			}

		public:
			static bool InitWizDB(UserType& global) {
				global = UserType("global");
				return true;
			}
			// allRemove Query 
			static bool AllRemoveWizDB(UserType& global) {
				global = UserType("");
				return true;
			}
			// AddQuery AddData, AddUserTypeData
			static bool AddDataAtFront(UserType& global, const string& position, const string& data, const string& condition = "") {
				UserType utTemp = UserType("global");
				bool isTrue = false;

				if (false == LoadDataFromString(data, utTemp))
				{
					return false;
				}
				auto finded = UserType::Find(&global, position);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						int item_n = utTemp.GetItemListSize();
						int user_n = utTemp.GetUserTypeListSize();

						/// chk temp test codes - > using flag 1->Exist 2->Comparision
						//if (finded.second[i]->GetItem("base_tax").GetCount() > 0) { continue; }
						///~end
						if (false == condition.empty()) {
							Condition cond(condition, finded.second[i], &global);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0]) // || cond.Now().size()  != 1
							{
								//std::cout << cond.Now()[0] << endl;
								continue;
							}
						}

						for (int k = utTemp.GetIListSize() - 1; k >= 0; --k) {
							if (utTemp.IsItemList(k)) {
								finded.second[i]->AddItemAtFront(utTemp.GetItemList(item_n-1).GetName(), utTemp.GetItemList(item_n-1).Get(0));
								item_n--;
							}
							else {
								finded.second[i]->AddUserTypeItemAtFront(*utTemp.GetUserTypeList(user_n-1));
								user_n--;
							}
						}
						isTrue = true;
					}
					return isTrue;
				}
				else {
					return false;
				}
			}
			static bool AddData(UserType& global, const string& position, const string& data, const string& condition = "") {
				UserType utTemp = UserType("global");
				bool isTrue = false;

				if (false == LoadDataFromString(data, utTemp))
				{
					return false;
				}
				auto finded = UserType::Find(&global, position);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						int item_n = 0;
						int user_n = 0;

						/// chk temp test codes - > using flag 1->Exist 2->Comparision
						//if (finded.second[i]->GetItem("base_tax").GetCount() > 0) { continue; }
						///~end
						if (false == condition.empty()) {
							Condition cond(condition, finded.second[i], &global);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0]) // || cond.Now().size()  != 1
							{
								//std::cout << cond.Now()[0] << endl;
								continue;
							}
						}

						for (int k = 0; k < utTemp.GetIListSize(); ++k) {
							if (utTemp.IsItemList(k)) {
								finded.second[i]->AddItem(utTemp.GetItemList(item_n).GetName(), utTemp.GetItemList(item_n).Get(0));
								item_n++;
							}
							else {
								finded.second[i]->AddUserTypeItem(*utTemp.GetUserTypeList(user_n));
								user_n++;
							}
						}
						isTrue = true;
					}
					return isTrue;
				}
				else {
					return false;
				}
			}
			static bool Insert(UserType& global, const string& position, const int idx, const string& data, const string& condition = "") {
				UserType utTemp = UserType("global");
				bool isTrue = false;

				if (false == LoadDataFromString(data, utTemp))
				{
					return false;
				}
				auto finded = UserType::Find(&global, position);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						int item_n = utTemp.GetIListSize();
						int user_n = utTemp.GetIListSize();

						/// chk temp test codes - > using flag 1->Exist 2->Comparision
						//if (finded.second[i]->GetItem("base_tax").GetCount() > 0) { continue; }
						///~end
						if (false == condition.empty()) {
							Condition cond(condition, finded.second[i], &global);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0]) // || cond.Now().size()  != 1
							{
								//std::cout << cond.Now()[0] << endl;
								continue;
							}
						}

						for (int k = utTemp.GetIListSize() - 1; k >= 0; --k) {
							if (utTemp.IsItemList(k)) {
								finded.second[i]->InsertItemByIlist(idx, utTemp.GetItemList(item_n - 1).GetName(),
									utTemp.GetItemList(item_n - 1).Get(0));
								item_n--;
							}
							else {
								finded.second[i]->InsertUserTypeByIlist(idx, *utTemp.GetUserTypeList(user_n - 1));
								user_n--;
							}
						}
						isTrue = true;
					}
					return isTrue;
				}
				else {
					return false;
				}
			}
			static bool AddNoNameUserType(UserType& global, const string& position, const string& data, const string& condition = "")
			{
				UserType utTemp = UserType("");
				bool isTrue = false;

				if (false == LoadDataFromString(data, utTemp))
				{
					return false;
				}
				auto finded = UserType::Find(&global, position);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						int item_n = 0;
						int user_n = 0;

						/// chk temp test codes - > using flag 1->Exist 2->Comparision
						//if (finded.second[i]->GetItem("base_tax").GetCount() > 0) { continue; }
						///~end
						if (false == condition.empty()) {
							Condition cond(condition, finded.second[i], &global);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						finded.second[i]->AddUserTypeItem(utTemp);

						isTrue = true;
					}
					return isTrue;
				}
				else {
					return false;
				}
			}

			static bool AddUserType(UserType& global, const string& position, const string& var, const string& data, const string& condition = "")
			{
				bool isTrue = false;
				auto finded = UserType::Find(&global, position);
				if (finded.first) {
					string temp = var;
					if (temp == "") { temp = " "; }
					StringTokenizer tokenizer(temp, "/");
					UserType utTemp = UserType("");
					if (false == LoadDataFromString(data, utTemp))
					{
						return false;
					}

					while (tokenizer.hasMoreTokens()) {
						string utName = tokenizer.nextToken();
						vector<string> strVec;
						if (utName == " ") { utName = ""; }

						if (utName.size() >= 3 && utName[0] == '[' && utName[utName.size() - 1] == ']')
						{
							StringTokenizer tokenizer2(utName, vector<string>{ "[", "~", "]" });
							while (tokenizer2.hasMoreTokens())
							{
								strVec.push_back(tokenizer2.nextToken());
							}
						}

						long long int a = 0, b = 0, Min = 0, Max = 0;
						bool chkInt = false;

						if (strVec.size() == 2 && Utility::IsInteger(strVec[0]) && Utility::IsInteger(strVec[1])) {
							chkInt = true;
							a = atoll(strVec[0].c_str());
							b = atoll(strVec[1].c_str());
							Min = min(a, b);
							Max = max(a, b);
						}

						for (auto x = Min; x <= Max; ++x)
						{
							if (strVec.size() == 2 && chkInt)
							{
								utName = std::to_string(x);
							}
							else {}
							utTemp.SetName(utName);

							for (int i = 0; i < finded.second.size(); ++i) {
								int item_n = 0;
								int user_n = 0;


								if (false == condition.empty()) {
									string _condition = condition;

									if (utName == "") { _condition = wiz::String::replace(_condition, "~~", "^"); }
									else
										_condition = wiz::String::replace(_condition, "~~", utName); //

																									 //	cout << "condition is " << _condition << endl;

									Condition cond(_condition, finded.second[i], &global);

									while (cond.Next());

									if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
									{
										//std::cout << cond.Now()[0] << endl;
										continue;
									}
								}

								finded.second[i]->AddUserTypeItem(utTemp);

								isTrue = true; // chk!!
							}

							// prevent from infinity loop.
							if (x == Max) { break; }
						}
					}
					return isTrue;
				}
				else {
					return false;
				}
			}
			/// SetData - Re Do!
			static bool SetData(UserType& global, const string& position, const string& varName, const string& data, const string& condition = "")
			{
				auto finded = UserType::Find(&global, position);
				bool isTrue = false;

				if (finded.first) {
					string temp = varName;
					if (temp == "") { temp = " "; }
					StringTokenizer tokenizer(temp, "/");
					UserType utTemp("");
					if (false == LoadDataFromString(data, utTemp)) {
						return false;
					}
					while (tokenizer.hasMoreTokens()) {
						string _varName = tokenizer.nextToken();
						/// todo - if varName is "" then data : val val val ... 
						if (_varName == "" || _varName == " ") { // re?
							const int n = utTemp.GetItem("").size();
							for (int i = 0; i < finded.second.size(); ++i) {
								if (false == condition.empty()) {
									string _condition = condition;
									if (_varName == "" || _varName == " ") { _condition = wiz::String::replace(_condition, "~~", "^"); }
									else
										_condition = wiz::String::replace(_condition, "~~", _varName); //

									Condition cond(_condition, finded.second[i], &global);

									while (cond.Next());

									if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
									{
										//	std::cout << cond.Now()[0] << endl;
										continue;
									}
								}
								finded.second[i]->RemoveItemList("");

								for (int j = 0; j < n; ++j) {
									finded.second[i]->AddItem("", utTemp.GetItem("")[j].Get(0));
								}
								isTrue = true;
							}
						}
						else {
							vector<string> strVec;

							if (_varName.size() >= 3 && _varName[0] == '[' && _varName[_varName.size() - 1] == ']')
							{
								StringTokenizer tokenizer2(_varName, vector<string>{ "[", "~", "]" });
								while (tokenizer2.hasMoreTokens())
								{
									strVec.push_back(tokenizer2.nextToken());
								}
							}

							long long int a = 0, b = 0, Min = 0, Max = 0;
							bool chkInt = false;

							if (strVec.size() == 2 && Utility::IsInteger(strVec[0]) && Utility::IsInteger(strVec[1])) {
								chkInt = true;
								a = atoll(strVec[0].c_str());
								b = atoll(strVec[1].c_str());
								Min = min(a, b);
								Max = max(a, b);
							}
							for (long long x = Min; x <= Max; ++x) {
								if (strVec.size() == 2 && chkInt)
								{
									_varName = std::to_string(x);
								}
								else {}

								for (int i = 0; i < finded.second.size(); ++i) {
									if (false == condition.empty()) {
										string _condition = condition;
										if (_varName == "") { _condition = wiz::String::replace(_condition, "~~", "^"); }
										else
											_condition = wiz::String::replace(_condition, "~~", _varName); //

										Condition cond(_condition, finded.second[i], &global);

										while (cond.Next());

										if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
										{
											//	std::cout << cond.Now()[0] << endl;
											continue;
										}
									}
									finded.second[i]->SetItem(_varName, data); /// chk
									isTrue = true;
								}

								// prevent from infinity loop.
								if (x == Max) { break; }
							}
						}
					}
					return isTrue;
				}
				else {
					return false;
				}
			}

			static bool SetData(UserType& global, const string& position, const int var_idx, const string& data, const string& condition = "")
			{
				auto finded = UserType::Find(&global, position);
				bool isTrue = false;

				if (finded.first) {
					UserType utTemp("");
					if (false == LoadDataFromString(data, utTemp)) {
						return false;
					}
					long long int a = 0, b = 0, Min = 0, Max = 0;

					for (long long x = Min; x <= Max; ++x) {
						for (int i = 0; i < finded.second.size(); ++i) {
							if (false == condition.empty()) {
								string _condition = condition;

								Condition cond(_condition, finded.second[i], &global);

								while (cond.Next());

								if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
								{
									//	std::cout << cond.Now()[0] << endl;
									continue;
								}
							}
							finded.second[i]->SetItem(var_idx, data); /// chk
							isTrue = true;
						}

						// prevent from infinity loop.
						if (x == Max) { break; }
					}
					return isTrue;
				}
				else {
					return false;
				}
			}
			/// 
			static string GetData(UserType& global, const string& position, const string& condition) {
				string str;
				auto finded = UserType::Find(&global, position);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							Condition cond(condition, finded.second[i], &global);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						str = str + finded.second[i]->ToString() + "\n";
					}
					return str;
				}
				else {
					return "";
				}
			}
			static string GetItemListData(UserType& global, const string& position, const string& condition)
			{
				string str;
				auto finded = UserType::Find(&global, position);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							Condition cond(condition, finded.second[i], &global);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						str = str + finded.second[i]->ItemListToString() + "\n";
					}
					return str;
				}
				else {
					return "";
				}
			}
			static string GetItemListNamesData(UserType& global, const string& position, const string& condition)
			{
				string str;
				auto finded = UserType::Find(&global, position);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							Condition cond(condition, finded.second[i], &global);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						str = str + finded.second[i]->ItemListNamesToString() + "\n";
					}
					return str;
				}
				else {
					return "";
				}
			}
			static string GetUserTypeListNamesData(UserType& global, const string& position, const string& condition)
			{
				string str;
				auto finded = UserType::Find(&global, position);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							Condition cond(condition, finded.second[i], &global);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						str = str + finded.second[i]->UserTypeListNamesToString() + "\n";
					}
					return str;
				}
				else {
					return "";
				}
			}
			/// varName = val - do
			/// varName = { val val val } - GetData(position+"/varName", ""); 
			/// varName = { var = val } - GetData(position+"/varname", var);
			static string GetData(UserType& global, const string& position, const string& varName, const string& condition) // 
			{
				string str;
				string _var = varName;
				if (_var == " ") { _var = ""; }

				auto finded = UserType::Find(&global, position);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							string _condition = condition;

							// ~~ and ^ -> do not used other mean?
							if (_var == "") { _condition = wiz::String::replace(_condition, "~~", "^"); }
							else
								_condition = wiz::String::replace(_condition, "~~", _var); /// varName -> _var.

							Condition cond(_condition, finded.second[i], &global);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}

						const int num = finded.second[i]->GetItem(_var).size();
						for (int k = 0; k < num; ++k) {
							str = str + finded.second[i]->GetItem(_var)[k].Get(0) + "\n";
						}
					}
				}
				return str;
			}

			static bool Remove(UserType& global, const string& position, const string& var, const string& condition) {
				auto finded = UserType::Find(&global, position);
				bool isTrue = false;

				if (finded.first) {
					string temp = var;
					if (temp == "") { temp = " "; }
					StringTokenizer tokenizer(temp, "/");
					while (tokenizer.hasMoreTokens()) {
						string _var = tokenizer.nextToken();
						if (_var == " ") { _var = ""; }
						vector<string> strVec;

						if (_var.size() >= 3 && _var[0] == '[' && _var[_var.size() - 1] == ']')
						{
							StringTokenizer tokenizer2(_var, vector<string>{ "[", "~", "]" });
							while (tokenizer2.hasMoreTokens())
							{
								strVec.push_back(tokenizer2.nextToken());
							}
						}

						long long int a = 0, b = 0, Min = 0, Max = 0;
						bool chkInt = false;

						if (strVec.size() == 2 && Utility::IsInteger(strVec[0]) && Utility::IsInteger(strVec[1])) {
							chkInt = true;
							a = atoll(strVec[0].c_str());
							b = atoll(strVec[1].c_str());
							Min = min(a, b);
							Max = max(a, b);
						}
						for (long long x = Min; x <= Max; ++x) {
							if (strVec.size() == 2 && chkInt)
							{
								_var = std::to_string(x);
							}
							else {}

							for (int i = 0; i < finded.second.size(); ++i) {
								UserType* temp = finded.second[i];

								if (false == condition.empty()) {
									string _condition = condition;
									if (_var == "") { _condition = wiz::String::replace(_condition, "~~", "^"); }
									else
										_condition = wiz::String::replace(_condition, "~~", _var); //

									Condition cond(_condition, finded.second[i], &global);
									while (cond.Next());

									if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
									{
										// std::cout << cond.Now()[0] << endl;
										continue;
									}
								}

								temp->RemoveItemList(_var);
								temp->RemoveUserTypeList(_var);
								isTrue = true;
							}

							// prevent from infinity loop.
							if (x == Max) { break; }
						}
					}
					return isTrue;
				}
				else {
					return false;
				}
			}
			static bool Remove(UserType& global, const string& position, const string& condition) {
				auto finded = UserType::Find(&global, position);
				bool isTrue = false;

				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						UserType* temp = finded.second[i];

						if (false == condition.empty()) {
							Condition cond(condition, finded.second[i], &global);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								// std::cout << cond.Now()[0] << endl;
								continue;
							}
						}

						temp->Remove();
						isTrue = true;
					}
					return isTrue;
				}
				else {
					return false;
				}
			}
			// todo - static bool Remove(UserType& global, const string& positiion, oonst int idx, const string& condition)
			static bool Remove(UserType& global, const string& position, const int idx, const string& condition) {
				auto finded = UserType::Find(&global, position);
				bool isTrue = false;

				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						UserType* temp = finded.second[i];

						if (false == condition.empty()) {
							Condition cond(condition, finded.second[i], &global);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								// std::cout << cond.Now()[0] << endl;
								continue;
							}
						}

						temp->RemoveList(idx);
						isTrue = true;
					}
					return isTrue;
				}
				else {
					return false;
				}
			}
			// cf) idx == -1 -> size()-1 ?? or RemoveBack() ??
			static bool RemoveItem(UserType& global, const string& position, const string& value)
			{
				auto finded = UserType::Find(&global, position);
				bool isTrue = false;


				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						UserType* temp = finded.second[i];
						vector<int> idx;

						for (int j = 0; j < temp->GetItemListSize(); ++j)
						{
							if (value == temp->GetItemList(j).Get(0)) {
								idx.push_back(j);
							}
						}
						//
						sort(idx.begin(), idx.end(), std::greater<int>()); /// result ex) 5 4 3 2 1 
																		   // 
						for (const int& x : idx) {
							temp->RemoveItemList(x);
						}

						isTrue = true;
					}
					return isTrue;
				}
				else {
					return false;
				}
			}

			static bool LoadWizDB(UserType& global, const string& fileName) {
				UserType globalTemp = UserType("global");

				// Scan + Parse 
				if (false == LoadDataFromFile(fileName, globalTemp)) { return false; }
				std::cout << "LoadData End" << endl;

				global = move(globalTemp);
				return true;
			}
			// SaveQuery
			static bool SaveWizDB(const UserType& global, const string& fileName, const string& option = "0", const string& option2 = "") { /// , int option
				ofstream outFile;
				if (fileName.empty()) { return false; }
				if (option2 == "") {
					outFile.open(fileName);
					if (outFile.fail()) { return false; }
				}
				else {
					outFile.open(fileName, ios::app);
					if (outFile.fail()) { return false; }

					outFile << "\n";
				}

				/// saveFile
				if (option == "1") // for eu4.
					global.Save1(outFile); // cf) friend
				else if (option == "2")
					global.Save2(outFile);

				outFile.close();

				return true;
			}

			/// To Do - ExistItem, ExistUserType, SetUserType GetUserType
			static bool ExistData(UserType& global, const string& position, const string& varName, const string& condition) // 
			{
				int count = 0;
				string _var = varName;
				if (_var == " ") { _var = ""; }

				auto finded = UserType::Find(&global, position);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							Condition cond(condition, finded.second[i], &global);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						count = count + (finded.second[i]->GetItem(_var).size());
					}
				}
				return 0 != count;
			}
			static bool ExistUserType(UserType& global, const string& position, const string& condition) // 
			{
				int count = 0;

				auto finded = UserType::Find(&global, position);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							Condition cond(condition, finded.second[i], &global);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						count = count + (finded.second[i]->GetUserTypeListSize());
					}
				}
				return 0 != count;
			}
			static bool ExistOneUserType(UserType& global, const string& position, const string& condition) // 
			{
				int count = 0;

				auto finded = UserType::Find(&global, position);
				if (finded.second[0] == &global) {
					return true;
				}
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							Condition cond(condition, finded.second[i], &global);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						count = count + (finded.second[i]->GetUserTypeListSize());
					}
				}
				return 1 == count;
			}
			static bool ExistItem(UserType& global, const string& position, const string& varName, const string& condition) // 
			{
				int count = 0;
				string _var = varName;
				if (_var == " ") { _var = ""; }

				auto finded = UserType::Find(&global, position);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							Condition cond(condition, finded.second[i], &global);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						count = count + (finded.second[i]->GetItem(_var).size());
					}
				}
				return 0 != count;
			}

			static bool ChkData(UserType& global)
			{
				return UserType::ChkData(&global);
			}

			/// ToDo - global, position, var, condition + var is " "!
			// "root" -> position.
			static string SearchItem(UserType& global, const string& var, const string& condition, const string& start_dir = "root")
			{
				vector<string> positionVec;
				string temp;

				SearchItem(global, positionVec, var, start_dir, &global, condition);

				for (int i = 0; i < positionVec.size(); ++i)
				{
					temp = temp + positionVec[i] + "\n";
				}

				return temp;
			}
			static string SearchUserType(UserType& global, const string& var, const string& condition)
			{
				vector<string> positionVec;
				string temp;

				SearchUserType(global, positionVec, var, "root", &global, condition);

				for (int i = 0; i < positionVec.size(); ++i)
				{
					temp = temp + positionVec[i] + "\n";
				}

				return temp;
			}
			static void ReplaceItem(UserType& global, const string& var, const string& val, const string& condition, const string& start_dir) {
				UserType* ut = wiz::load_data::UserType::Find(&global, start_dir).second[0]; // chk!!
				ReplaceItem(global, var, start_dir, ut, val, condition);
			}
			static void RemoveUserTypeTotal(UserType& global, const string& ut_name, const string& condition, const string& start_dir) {
				UserType* ut = wiz::load_data::UserType::Find(&global, start_dir).second[0]; // chk!!
				RemoveUserTypeTotal(global, ut_name, start_dir, ut, condition);
			}
			static void ReplaceDateType(UserType& global, const string& val, const string& condition, const string& start_dir) {
				UserType* ut = wiz::load_data::UserType::Find(&global, start_dir).second[0]; // chk!!
				ReplaceDateType(global, start_dir, ut, val, condition);
			}
			static void ReplaceDateType2(UserType& global, const string& val, const string& condition, const string& start_dir) {
				UserType* ut = wiz::load_data::UserType::Find(&global, start_dir).second[0]; // chk!!
				ReplaceDateType2(global, start_dir, ut, val, condition);
			}
			static void ReplaceDataType1(UserType& global, const string& rex, const string& val, const string& condition, const string& start_dir) {
				UserType* ut = wiz::load_data::UserType::Find(&global, start_dir).second[0]; // chk!!
				ReplaceDataType1(global, start_dir, ut, rex, val, condition);
			}
			static void ReplaceDataType1_2(UserType& global, const string& rex, const string& val, const string& condition, const string& start_dir) {
				UserType* ut = wiz::load_data::UserType::Find(&global, start_dir).second[0]; // chk!!
				ReplaceDataType1_2(global, start_dir, ut, rex, val, condition);
			}
			static void ReplaceDataType2(UserType& global, const string& rex, const string& val, const string& condition, const string& start_dir) {
				UserType* ut = wiz::load_data::UserType::Find(&global, start_dir).second[0]; // chk!!
				ReplaceDataType2(global, start_dir, ut, rex, val, condition);
			}
		private:
			static void ReplaceItem(UserType& global, const string& var, const string& nowPosition,
				UserType* ut, const string& val, const string& condition)
			{
				string _var = var;
				if (_var == " ") { _var = ""; }

				for (int i = 0; i < ut->GetItemListSize(); ++i) {
					if (ut->GetItemList(i).GetName() == _var) {
						string _condition = condition;

						if (_var == "") { _condition = wiz::String::replace(_condition, "~~", "^"); }
						else
							_condition = wiz::String::replace(_condition, "~~", _var); //
						Condition cond(_condition, ut, &global);

						while (cond.Next());

						if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
						{
							ut->GetItemList(i).Set(0, val);
						}
					}
				}

				for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
					string temp = ut->GetUserTypeList(i)->GetName();
					if (temp == "") { temp = " "; }
					ReplaceItem(
						global,
						_var,
						nowPosition + "/" + temp,
						ut->GetUserTypeList(i),
						val,
						condition
					);
				}
			}
			static void RemoveUserTypeTotal(UserType& global, const string& ut_name, const string& nowPosition,
				UserType* ut, const string& condition)
			{
				string _var = ut_name;
				if (_var == " ") { _var = ""; }

				for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
					if (ut->GetUserTypeList(i)->GetName() == _var) {
						string _condition = condition;

						if (_var == "") { _condition = wiz::String::replace(_condition, "~~", "^"); }
						else
							_condition = wiz::String::replace(_condition, "~~", _var); //
						Condition cond(_condition, ut, &global);

						while (cond.Next());

						if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
						{
							ut->RemoveUserTypeList(i);
							--i;
						}
					}
				}

				for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
					string temp = ut->GetUserTypeList(i)->GetName();
					if (temp == "") { temp = " "; }
					RemoveUserTypeTotal(
						global,
						_var,
						nowPosition + "/" + temp,
						ut->GetUserTypeList(i),
						condition
					);
				}
			}
			static void ReplaceDateType(UserType& global, const string& nowPosition,
				UserType* ut, const string& val, const string& condition)
			{
				for (int i = 0; i < ut->GetItemListSize(); ++i) {
					if (Utility::IsDate(ut->GetItemList(i).GetName()) || Utility::IsDate(ut->GetItemList(i).Get(0))) {
						string _condition = condition;

						string _var = ut->GetItemList(i).GetName();
						_condition = wiz::String::replace(_condition, "~~", _var); //

						Condition cond(_condition, ut, &global);

						while (cond.Next());

						if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
						{
							if (Utility::IsDate(ut->GetItemList(i).GetName())) {
								ut->GetItemList(i).SetName(val);
							}
							if (Utility::IsDate(ut->GetItemList(i).Get(0))) {
								ut->GetItemList(i).Set(0, val);
							}
						}
					}
				}

				for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
					string temp = ut->GetUserTypeList(i)->GetName();
					if (temp == "") { temp = " "; }

					ReplaceDateType(
						global,
						nowPosition + "/" + temp,
						ut->GetUserTypeList(i),
						val,
						condition
					);
				}
			}
			static void ReplaceDateType2(UserType& global, const string& nowPosition,
				UserType* ut, const string& val, const string& condition)
			{
				for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
					string temp = ut->GetUserTypeList(i)->GetName();
					if (temp == "") { temp = " "; }

					if (Utility::IsDate(temp)) {
						string _condition = condition;

						string _var = ut->GetUserTypeList(i)->GetName();
						_condition = wiz::String::replace(_condition, "~~", _var); //

						Condition cond(_condition, ut, &global);

						while (cond.Next());

						if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
						{
							ut->GetUserTypeList(i)->SetName(val);
						}
					}

					ReplaceDateType2(
						global,
						nowPosition + "/" + temp,
						ut->GetUserTypeList(i),
						val,
						condition
					);
				}
			}
			static void ReplaceDataType1(UserType& global, const string& nowPosition,
				UserType* ut, const string& rgx_str, const string& val, const string& condition)
			{
				std::regex rgx(rgx_str);

				for (int i = 0; i < ut->GetItemListSize(); ++i) {
					if (std::regex_match(ut->GetItemList(i).GetName(), rgx)) { // || Utility::IsDate(ut->GetItemList(i).Get(0))) {
						string _condition = condition;

						string _var = ut->GetItemList(i).GetName();
						_condition = wiz::String::replace(_condition, "~~", _var); //

						Condition cond(_condition, ut, &global);

						while (cond.Next());

						if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
						{
							ut->GetItemList(i).SetName(val);
						}
					}
				}

				for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
					string temp = ut->GetUserTypeList(i)->GetName();
					if (temp == "") { temp = " "; }

					ReplaceDateType(
						global,
						nowPosition + "/" + temp,
						ut->GetUserTypeList(i),
						val,
						condition
					);
				}
			}
			static void ReplaceDataType1_2(UserType& global, const string& nowPosition,
				UserType* ut, const string& rgx_str, const string& val, const string& condition)
			{
				std::regex rgx(rgx_str);

				for (int i = 0; i < ut->GetItemListSize(); ++i) {
					if (std::regex_match(ut->GetItemList(i).Get(0), rgx)) {
						string _condition = condition;

						string _var = ut->GetItemList(i).GetName();
						_condition = wiz::String::replace(_condition, "~~", _var); //

						Condition cond(_condition, ut, &global);

						while (cond.Next());

						if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
						{
							ut->GetItemList(i).Set(0, val);
						}
					}
				}

				for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
					string temp = ut->GetUserTypeList(i)->GetName();
					if (temp == "") { temp = " "; }

					ReplaceDateType(
						global,
						nowPosition + "/" + temp,
						ut->GetUserTypeList(i),
						val,
						condition
					);
				}
			}
			static void ReplaceDataType2(UserType& global, const string& nowPosition,
				UserType* ut, const string& rgx_str, const string& val, const string& condition)
			{
				std::regex rgx(rgx_str);

				for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
					string temp = ut->GetUserTypeList(i)->GetName();
					if (temp == "") { temp = " "; }

					if (std::regex_match(temp, rgx)) {
						string _condition = condition;

						string _var = ut->GetUserTypeList(i)->GetName();
						_condition = wiz::String::replace(_condition, "~~", _var); //

						Condition cond(_condition, ut, &global);

						while (cond.Next());

						if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
						{
							ut->GetUserTypeList(i)->SetName(val);
						}
					}

					ReplaceDateType2(
						global,
						nowPosition + "/" + temp,
						ut->GetUserTypeList(i),
						val,
						condition
					);
				}
			}

		};
	}
}


#endif // LOAD_DATA_H_INCLUDED
