

#define _CRT_SECURE_NO_WARNINGS

//#include <vld.h>

//#define ARRAYS_DEBUG
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <string>
#include <ctime>
#include <cstdlib>
using namespace std;

#include <conio.h>

#include <wiz/load_data.h>
#include <wiz/stacks.h>
#include <wiz/cpp_string.h>

#include <Windows.h>


inline int GETCH() /// To Do - Replace..
{
	return _getch();
}

inline void FFLUSH()  // to  DO - Ãß°¡!!
{
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail(), '\n');
}

inline void gotoxy(short x, short y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

inline void setcolor(int color, int bgcolor)
{
	color = color & 0xF;
	bgcolor = bgcolor & 0xF;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		(bgcolor << 4) | color);
}

class MData
{
public:
	bool isDir;
	string varName;
	int no; /// for UserType that has same name.!, rename?
public:
	explicit MData(const bool isDir = false, const string& varName = "", const int no = 0) : isDir(isDir), varName(varName), no(no)
	{

	}
};

void MStyleTest(wiz::load_data::UserType* pUt)
{
	std::vector<wiz::load_data::ItemType<wiz::load_data::UserType*>> utVec;
	std::vector<MData> mdVec;
	//std::vector<vector<MData>> mdVec2;
	wiz::load_data::ItemType<wiz::load_data::UserType*> global;
	wiz::load_data::UserType* utTemp = pUt;
	std::vector<wiz::load_data::UserType*> utVec2;
	std::vector<int> idxVec; //for store before idx
	std::vector<string> strVec; // for value..
	int braceNum = 0;
	int idx = 0;
	bool isFirst = true;
	bool isReDraw = true;
	int sizeOfWindow = 30;
	int Start = 0;
	int End = 0;
	int state = 0;

	global.Push(utTemp);

	utVec.push_back(global);

	utVec2.push_back(utTemp);

	system("cls");

	int count_userType = 0;
	int count_item = 0;

	while (true) {
		if (isFirst) {
			mdVec = std::vector<MData>();
			count_userType = 0;
			count_item = 0;

			for (int h = 0; h < utVec[braceNum].size(); ++h) {
				for (int i = 0; i < utVec[braceNum].Get(h)->GetUserTypeListSize(); ++i) {
					MData mdTemp{ true, utVec[braceNum].Get(h)->GetUserTypeList(i)->GetName(), h };
					if (mdTemp.varName.empty()) {
						mdTemp.varName = " ";
					}

					mdVec.push_back(mdTemp);
					count_userType++;
				}
			}
			for (int h = 0; h < utVec[braceNum].size(); ++h) {
				for (int i = 0; i < utVec[braceNum].Get(h)->GetItemListSize(); ++i) {
					MData mdTemp{ false, utVec[braceNum].Get(h)->GetItemList(i).GetName(), h };
					if (mdTemp.varName.empty()) {
						mdTemp.varName = " ";
					}
					mdVec.push_back(mdTemp);
					count_item++;
				}
			}
			isFirst = false;
		}
		if (isReDraw) {
			setcolor(0, 0);
			system("cls");

			End = min(Start + sizeOfWindow - 1, mdVec.size() - 1);
			if (mdVec.empty()) {
				End = Start - 1;
			}
			// draw mdVec and cursor - chk!!
			else {
				for (int i = Start; i <= End; ++i) {
					if (mdVec[i].isDir) { setcolor(0, 10); }
					else { setcolor(0, 7); }
					if (false == mdVec[i].varName.empty()) {
						cout << "  " << mdVec[i].varName;
					}
					else
					{
						cout << "  " << " ";
					}
					if (i != mdVec.size() - 1) { cout << endl; }
				}

				gotoxy(0, idx - Start);

				setcolor(0, 12);
				cout << "¡Ü";
				setcolor(0, 0);
				gotoxy(0, 0);
			}

			isReDraw = false;
		}

		// move and chk enterkey. - todo!!
		{
			char ch = GETCH();
			FFLUSH();

			if ('q' == ch) { return; }

			// todo - add, remove, save
			if (strVec.empty() && Start <= End && idx > 0 && ('w' == ch || 'W' == ch))
			{
				// draw mdVec and cursor - chk!!
				if (idx == Start) {
					system("cls");

					int count = 0;
					int newEnd = Start - 1;
					int newStart = max(0, newEnd - sizeOfWindow + 1);

					Start = newStart; End = newEnd;
					idx--;

					for (int i = Start; i <= End; ++i) {
						if (mdVec[i].isDir) { setcolor(0, 10); }
						else { setcolor(0, 7); }
						cout << "  " << mdVec[i].varName;
						if (mdVec[i].varName.empty()) { cout << " "; }
						if (i != mdVec.size() - 1) { cout << endl; }
						count++;
					}
					gotoxy(0, idx - Start);
					setcolor(0, 12);
					cout << "¡Ü";
					setcolor(0, 0);
				}
				else {
					gotoxy(0, idx - Start);
					setcolor(0, 0);
					cout << "  ";
					idx--;

					gotoxy(0, idx - Start);
					setcolor(0, 12);
					cout << "¡Ü";
					setcolor(0, 0);
				}
			}
			else if (
				strVec.empty() && Start <= End && (idx < mdVec.size() - 1)
				&& ('s' == ch || 'S' == ch)
				)
			{
				if (idx == End) {
					system("cls");

					int count = 0;
					int newStart = End + 1;
					int newEnd = min(newStart + sizeOfWindow - 1, mdVec.size() - 1);

					Start = newStart; End = newEnd;
					idx++;

					for (int i = Start; i <= End; ++i) {
						if (mdVec[i].isDir) { setcolor(0, 10); }
						else { setcolor(0, 7); }
						cout << "  " << mdVec[i].varName;
						if (i != mdVec.size() - 1) { cout << endl; }
						count++;
					}
					gotoxy(0, 0);
					setcolor(0, 12);
					cout << "¡Ü";
					setcolor(0, 0);
				}
				else {
					gotoxy(0, idx - Start);
					setcolor(0, 0);
					cout << "  ";
					idx++;

					gotoxy(0, idx - Start);
					setcolor(0, 12);
					cout << "¡Ü";
					setcolor(0, 0);
				}
			}
			if (!strVec.empty() && Start <= End && idx > 0 && ('w' == ch || 'W' == ch))
			{
				// draw mdVec and cursor - chk!!
				if (idx == Start) {
					system("cls");

					int count = 0;
					int newEnd = Start - 1;
					int newStart = max(0, newEnd - sizeOfWindow + 1);

					Start = newStart; End = newEnd;
					idx--;

					for (int i = Start; i <= End; ++i) {
						setcolor(0, 7);
						cout << "  " << strVec[i];
						if (i != strVec.size() - 1) { cout << endl; }
						count++;
					}
					gotoxy(0, idx - Start);
					setcolor(0, 12);
					cout << "¡Ü";
					setcolor(0, 0);
				}
				else {
					gotoxy(0, idx - Start);
					setcolor(0, 0);
					cout << "  ";
					idx--;

					gotoxy(0, idx - Start);
					setcolor(0, 12);
					cout << "¡Ü";
					setcolor(0, 0);
				}
			}
			else if (
				!strVec.empty() && Start <= End && (idx < strVec.size() - 1)
				&& ('s' == ch || 'S' == ch)
				)
			{
				if (idx == End) {
					setcolor(0, 0);
					system("cls");

					int count = 0;
					int newStart = End + 1;
					int newEnd = min(newStart + sizeOfWindow - 1, strVec.size() - 1);

					Start = newStart; End = newEnd;
					idx++;

					for (int i = Start; i <= End; ++i) {
						setcolor(0, 7);
						cout << "  " << strVec[i];
						if (i != strVec.size() - 1) { cout << endl; }
						count++;
					}
					gotoxy(0, 0);
					setcolor(0, 12);
					cout << "¡Ü";
					setcolor(0, 0);
				}
				else {
					gotoxy(0, idx - Start);
					setcolor(0, 0);
					cout << "  ";
					idx++;

					gotoxy(0, idx - Start);
					setcolor(0, 12);
					cout << "¡Ü";
					setcolor(0, 0);
				}
			}
			else if ('\r' == ch || '\n' == ch) {
				/// To Do..
				gotoxy(0, 0);
				if (count_userType == 0 && count_item == 0)
				{
					//
				}
				else if (state == 0 && strVec.empty() && idx < count_userType) { // utVec[braceNum].Get(mdVec[idx].no)->GetUserTypeListSize()) {
					setcolor(0, 0);
					system("cls");

					// usertypelist
					braceNum++;
					idxVec.push_back(idx); /// idx?

					if (braceNum >= utVec.size()) {
						utVec.push_back(wiz::load_data::ItemType<wiz::load_data::UserType*>());
						utVec2.push_back(NULL);
					}

					wiz::load_data::ItemType< wiz::load_data::UserType*> ref;
					ref.Push(NULL);
					string strTemp = mdVec[idxVec[braceNum - 1]].varName;
					if (strTemp == " ")
					{
						strTemp = "";
					}

					if (utVec[braceNum - 1].Get(mdVec[idxVec[braceNum - 1]].no)->GetUserTypeItemRef(idxVec[braceNum - 1], ref.Get(0)))
					{
						//
					}
					utVec[braceNum] = ref;

					utVec2[braceNum - 1]->GetUserTypeItemRef(idxVec[braceNum - 1], ref.Get(0));
					utVec2[braceNum] = ref.Get(mdVec[idxVec[braceNum - 1]].no);

					Start = 0;
					idx = 0;
					isFirst = true;
					isReDraw = true;
				}
				else
				{
					if (
						!mdVec.empty() &&
						strVec.empty()
						)
					{
						setcolor(0, 0);
						system("cls");

						string strTemp = mdVec[idx].varName;
						if (strTemp == " ") { strTemp = ""; }
						const int count = 1; // utVec[braceNum].Get(mdVec[idx].no)->GetItem(strTemp).size();
						setcolor(0, 7);

						for (int i = 0; i < count; ++i) {
							setcolor(0, 7);

							auto x = utVec[braceNum].Get(mdVec[idx].no)->GetItemList(idx - count_userType);
							string temp = x.Get(0);
							cout << "  " << temp;
							strVec.push_back(temp);
							//}
							if (i != count - 1) { cout << endl; }
						}
					}

					if (state == 0) {
						gotoxy(0, 0);
						state = 1;

						idxVec.push_back(idx);

						idx = 0;
						Start = 0;

						setcolor(0, 12);
						cout << "¡Ü";
						setcolor(0, 0);
					}
					else if (state == 1) { /// cf) state = 2;
						gotoxy(0, idx); /// chk..
						Start = idx;
					}


					// idx = 0;
					End = min(Start + sizeOfWindow - 1, strVec.size() - 1);
					if (strVec.empty()) { End = Start - 1; }


					// chk
					count_userType = 0;
					count_item = 1;
				}
			}
			else if (0 == state && 'f' == ch)
			{
				string temp;
				int x = 0;
				system("cls");
				setcolor(0, 7);
				cout << "row input : ";
				cin >> temp;
				FFLUSH();

				if (wiz::load_data::Utility::IsInteger(temp)) {
					x = stoi(temp); // toInt
					if (x < 0) { x = 0; }
					else if (x >= mdVec.size()) {
						x = mdVec.size() - 1;
					}

					// chk!! ToDo ?
					idx = x;
					x = max(0, x - sizeOfWindow / 2);
					Start = x;
					isReDraw = true; /// chk!! To Do - OnlyRedraw? Reset? // int + changed?
				}
				else
				{
					isReDraw = true; /// OnlyDraw = true?, no search?
				}
			}
			else if (0 == state && '1' == ch)
			{
				int index = idx;
				string temp = mdVec[idx].varName;

				for (int i = idx - 1; i >= 0; --i) {
					if (mdVec[i].varName == temp) {
						index = i;
					}
					else
					{
						break;
					}
				}

				idx = index;
				Start = max(0, idx - sizeOfWindow / 2);
				isReDraw = true;
			}
			else if (0 == state && '2' == ch)
			{
				int index = idx;
				string temp = mdVec[idx].varName;

				for (int i = idx + 1; i < mdVec.size(); ++i) {
					if (mdVec[i].varName == temp) {
						index = i;
					}
					else
					{
						break;
					}
				}

				idx = index;
				Start = max(0, idx - sizeOfWindow / 2);
				isReDraw = true;

			}
			else {
				if ('q' == ch) { return; } // quit
				else if ('b' == ch && braceNum > 0 && strVec.empty() && state == 0) {  // back
					braceNum--;

					setcolor(0, 0);
					system("cls");

					isFirst = true;
					isReDraw = true;
					//Start = idxVec.back();

					idx = idxVec.back();
					idxVec.pop_back();

					if (0 <= idx - sizeOfWindow / 2)
					{
						Start = idx - sizeOfWindow / 2;
					}
					else {
						Start = 0;
					}
				}
				else if ('b' == ch && !idxVec.empty()) /// state == 1 ?
				{
					idx = idxVec.back();
					idxVec.pop_back();

					if (0 <= idx - sizeOfWindow / 2)
					{
						Start = idx - sizeOfWindow / 2;
					}
					else {
						Start = 0;
					}

					state = 0;
					setcolor(0, 0);
					system("cls");
					strVec.clear();

					isFirst = true;
					isReDraw = true;
				}
				else if ('e' == ch) {
					setcolor(0, 0);
					system("cls");
					setcolor(7, 0);

					cout << "edit mode" << endl;
					cout << "add - a, change - c, remove - r, save - s" << endl;
					ch = GETCH();
					FFLUSH();

					if ('a' == ch) { // add
						int select = -1;
						string var;
						string val;

						setcolor(0, 7);
						// need more test!!
						cout << "add UserType : 1, add Item : 2, add usertype that name is \"\": 3 your input : ";
						cin >> select;
						FFLUSH();

						// add userType?
						if (1 == select) {
							// name of UserType.
							cout << "what is new UserType name? : ";
							cin >> var;
							FFLUSH();
							utVec2[braceNum]->AddUserTypeItem(wiz::load_data::UserType(var));
						}
						// addd Item?
						else if (2 == select) {
							// var, val /// state¿¡ µû¶ó?
							cout << "var : ";
							cin >> var;
							cout << "val : ";
							FFLUSH();
							getline(cin, val);
							utVec2[braceNum]->AddItem(var, val);
						}
						else if (3 == select)
						{
							utVec2[braceNum]->AddUserTypeItem(wiz::load_data::UserType(""));
						}
					}
					else if ('c' == ch && Start <= End) { // change var or value
														  // idx
						if (idx < count_userType) {
							string temp;
							setcolor(0, 7);
							cout << "change userType name : ";

							FFLUSH();
							getline(cin, temp);

							int count = 0;
							for (int h = 0; h < utVec[braceNum].size(); ++h) {
								for (int i = 0; i < utVec[braceNum].Get(h)->GetUserTypeListSize(); ++i) {
									if (count == idx) {
										utVec[braceNum].Get(h)->GetUserTypeList(i)->SetName(temp);
									}
									count++;
								}
							}
						}
						else {
							string temp;
							setcolor(0, 7);

							string name, value;
							if (1 == state) { // val
								cout << "change val : " << endl;
								getline(cin, temp);

								value = temp;

								int count = 0;
								int count_userType = 0;
								for (int h = 0; h < utVec[braceNum].size(); ++h) {
									for (int i = 0; i < utVec[braceNum].Get(h)->GetUserTypeListSize(); ++i) {
										count++;
										count_userType++;
									}
								}
								for (int h = 0; h < utVec[braceNum].size(); ++h) {
									for (int i = 0; i < utVec[braceNum].Get(h)->GetItemListSize(); ++i) {
										if (idxVec.back() == count) {
											utVec[braceNum].Get(h)->GetItemList(i).Get(idx) = value;
										}
										count++;
									}
								}
								idx = idxVec.back();
								idxVec.pop_back();
								// max!
								if (0 <= idx - sizeOfWindow / 2)
								{
									Start = idx - sizeOfWindow / 2;
								}
								else {
									Start = 0;
								}
								strVec.clear();
								state = 0;
							}
							else if (0 == state) { // var
								cout << "change var : " << endl;
								cin >> temp;
								FFLUSH();
								name = temp;
								int count = 0;
								for (int h = 0; h < utVec[braceNum].size(); ++h) {
									for (int i = 0; i < utVec[braceNum].Get(h)->GetUserTypeListSize(); ++i) {
										count++;
									}
								}
								for (int h = 0; h < utVec[braceNum].size(); ++h) {
									for (int i = 0; i < utVec[braceNum].Get(h)->GetItemListSize(); ++i) {
										if (idx == count) {
											utVec[braceNum].Get(h)->GetItemList(i).SetName(name);
										}
										count++;
									}
								}
							}
						}
					}
					else if ('r' == ch && Start <= End) { // remove
						if (idx < count_userType)
						{
							int count = 0;
							for (int h = 0; h < utVec[braceNum].size(); ++h) {
								for (int i = 0; i < utVec[braceNum].Get(h)->GetUserTypeListSize(); ++i) {
									if (count == idx) {
										string temp = mdVec[idx].varName;
										if (temp == " ") {
											temp = "";
										}
										utVec[braceNum].Get(h)->RemoveUserTypeList(temp);
									}
									count++;
								}
							}
							idx = 0;
							Start = 0;
						}
						else {
							if (state == 0) {
								int count = 0;
								int count_ut = 0;
								for (int h = 0; h < utVec[braceNum].size(); ++h) {
									for (int i = 0; i < utVec[braceNum].Get(h)->GetUserTypeListSize(); ++i) {
										count++;
										count_ut++;
									}
								}
								for (int h = 0; h < utVec[braceNum].size(); ++h) {
									for (int i = 0; i < utVec[braceNum].Get(h)->GetItemListSize(); ++i) {
										if (count == idx) {
											string temp = mdVec[idx].varName;
											if (temp == " ") { temp = ""; }

											utVec[braceNum].Get(h)->RemoveItemList(temp);
										}
										count++;
									}
								}
								idx = 0;
								Start = 0;
							}
							else if (1 == state)
							{
								int count = 0;
								int count_ut = 0;
								for (int h = 0; h < utVec[braceNum].size(); ++h) {
									for (int i = 0; i < utVec[braceNum].Get(h)->GetUserTypeListSize(); ++i) {
										count++;
										count_ut++;
									}
								}
								for (int h = 0; h < utVec[braceNum].size(); ++h) {
									for (int i = 0; i < utVec[braceNum].Get(h)->GetItemListSize(); ++i) {
										if (count == idxVec.back()) {
											string temp = mdVec[idxVec.back()].varName;
											if (temp == " ") { temp = ""; }

											utVec[braceNum].Get(h)->GetItemList(count - count_ut).Remove(0);
											if (utVec[braceNum].Get(h)->GetItemList(count - count_ut).size() == 0) {
												utVec[braceNum].Get(h)->GetItemList(count - count_ut).Remove();
												utVec[braceNum].Get(h)->RemoveEmptyItem();
											}
										}
										count++;
									}
								}

								idxVec.pop_back();
								idx = 0;
								// max!
								if (0 <= idx - sizeOfWindow / 2)
								{
									Start = idx - sizeOfWindow / 2;
								}
								else {
									Start = 0;
								}
								strVec.clear();
								state = 0;
							}
						}
					}
					else if ('s' == ch) { // save total data.
						string temp;

						setcolor(0, 7);
						cout << "save file name : ";
						FFLUSH();
						getline(cin, temp);

						wiz::load_data::LoadData::SaveWizDB(*utTemp, temp, "1");
					}

					if (1 == state)
					{
						idxVec.back();
						idxVec.pop_back();
						idx = 0;
						// max!
						if (0 <= idx - sizeOfWindow / 2)
						{
							Start = idx - sizeOfWindow / 2;
						}
						else {
							Start = 0;
						}
						strVec.clear();
						state = 0;
					}

					/// else if( l? reload?
					isFirst = true; // 
					isReDraw = true; //
				}
				else if ('t' == ch  && braceNum == 0) { // pass???
					isFirst = true;
					isReDraw = true;
					setcolor(0, 0);
					system("cls");

					setcolor(7, 0);
					cout << "text edit mode" << endl;

					// Add, AddUserType, Set, Remove, RemoveAll ?.
					string temp;
					FFLUSH();
					getline(cin, temp);

					wiz::StringTokenizer tokenizer(temp, "|");
					vector<string> strVecTemp;

					while (tokenizer.hasMoreTokens()) {
						strVecTemp.push_back(tokenizer.nextToken());
					}
					if (!strVecTemp.empty()) {
						try {
							if ("add" == strVecTemp[0])
							{
								if (false == wiz::load_data::LoadData::AddData(*utTemp, strVecTemp[1], strVecTemp[2], strVecTemp[3]))
								{
									cout << "faile to add" << endl; /// To Do to following code.
								}
							}
							else if ("addusertype" == strVecTemp[0])
							{
								wiz::load_data::LoadData::AddUserType(*utTemp, strVecTemp[1], strVecTemp[2], strVecTemp[3], strVecTemp[4]);
							}
							else if ("set" == strVecTemp[0])
							{
								wiz::load_data::LoadData::SetData(*utTemp, strVecTemp[1], strVecTemp[2], strVecTemp[3], strVecTemp[4]);
							}
							else if ("remove" == strVecTemp[0])
							{
								wiz::load_data::LoadData::Remove(*utTemp, strVecTemp[1], strVecTemp[2], strVecTemp[3]);
							}
							//else if ("removenonameitem" == strVecTemp[0])
							//{
							//	wiz::load_data::LoadData::RemoveNoNameItem(*utTemp, strVecTemp[1], strVecTemp[2]);
							//}
							else if ("removeall" == strVecTemp[0])
							{
								wiz::load_data::LoadData::Remove(*utTemp, strVecTemp[1], strVecTemp[2]);
							}
							else if ("searchitem" == strVecTemp[0])
							{
								cout << wiz::load_data::LoadData::SearchItem(*utTemp, strVecTemp[1], strVecTemp[2]) << endl;
								GETCH();
							}
							else if ("searchusertype" == strVecTemp[0])
							{
								cout << wiz::load_data::LoadData::SearchUserType(*utTemp, strVecTemp[1], strVecTemp[2]) << endl;
								GETCH();
							}
						}
						catch (exception& e) {}
						catch (wiz::Error& e) {}
						catch (const char* e) {}
						catch (const string& e) {}
					}
					//
					idx = 0;
					Start = 0;
				}
			}
		}
	}
}


class EventInfo
{
public:
	wiz::load_data::UserType* eventUT;
	wiz::ArrayStack< wiz::load_data::UserType* > nowUT; //
	int item_idx;
	wiz::ArrayStack<int> userType_idx;
	vector<pair<string, string>> parameters;
	map<string, string> locals;
	string id; //
	wiz::ArrayStack<string> conditionStack;
	wiz::ArrayStack<int> state;
	string return_value;
	string option;
public:
	EventInfo() : eventUT(NULL), return_value("")
	{

	}
};

class SortInfo // need to rename
{
public:
	string name;
	int iElement; // 2 : userType, // 1 : item
	size_t idx;
public:
	SortInfo() { }
	SortInfo(const string& name, int iElement, size_t idx)
		: name(name), iElement(iElement), idx(idx)
	{

	}
	// for sorting..
	bool operator<(const SortInfo& info) const
	{
		return this->name < info.name;
	}
};
// only one exist or do not exist
inline wiz::load_data::ItemType<string> GetItem(const wiz::load_data::UserType* ut, const string& name) {
	return ut->GetItem(name)[0];
}

inline vector<wiz::load_data::UserType*> GetUserType(const wiz::load_data::UserType* ut, const string& name)
{
	return ut->GetUserTypeItem(name);
}
string Find(wiz::load_data::UserType* ut, const string& str)
{
	int count = 0;
	int idx = -1;
	for (int i = str.size() - 1; i >= 0; --i) {
		if ('/' == str[i]) {
			if (count == 0) {
				idx = i;
			}
			count++;
		}
	}

	if (count == 1)
	{
		return "";
	}
	else {
		auto x = wiz::load_data::UserType::Find(ut,
			wiz::String::substring(str, 0, idx));
		if (x.first == false) { return ""; }
		else if (x.second[0]->GetItem(wiz::String::substring(str, idx + 1)).empty())
		{
			if ((x = wiz::load_data::UserType::Find(ut, str)).first)
			{
				return str;
			}
			else
			{
				return "";
			}
		}
		else { return x.second[0]->GetItem(wiz::String::substring(str, idx + 1))[0].Get(0); }
	}
}
inline bool Exist(wiz::load_data::UserType* ut, const string& dir)
{
	auto x = wiz::load_data::UserType::Find(ut, dir);
	return x.first;
}
// to do - rename!
pair<string, string> Find2(wiz::load_data::UserType* ut, const string& str)
{
	int idx = -1;
	for (int i = str.size() - 1; i >= 0; --i) {
		if ('/' == str[i]) {
			idx = i;
			break;
		}
	}
	return{ wiz::String::substring(str, 0, idx), wiz::String::substring(str,idx + 1) };
}
string reverse(string str) { /// to std::reverse 
	std::reverse(str.begin(), str.end());
	return str;
}

string GetType(const string& str) {
	if (wiz::load_data::Utility::IsInteger(str)) { return "INTEGER"; }
	else if (wiz::load_data::Utility::IsDouble(str)) { return "DOUBLE"; }
	else if (wiz::load_data::Utility::IsDate(str)) { return "DATE"; }
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
		if (wiz::load_data::Utility::IsMinus(str1) && !wiz::load_data::Utility::IsMinus(str2)) { return "< 0"; }
		else if (!wiz::load_data::Utility::IsMinus(str1) && wiz::load_data::Utility::IsMinus(str2)) { return "> 0"; }

		const bool minusComp = wiz::load_data::Utility::IsMinus(str1) && wiz::load_data::Utility::IsMinus(str2);

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
		wiz::StringTokenizer tokenizer1(str1, ".");
		wiz::StringTokenizer tokenizer2(str2, ".");

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
		wiz::StringTokenizer tokenizer1(str1, ".");
		wiz::StringTokenizer tokenizer2(str2, ".");

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

inline string FindParameters(const vector<pair<string, string>>& parameters, const string& operand)
{
	for (int i = 0; i < parameters.size(); ++i) {
		if (wiz::String::startsWith(operand, "$parameter.")
			&& wiz::String::substring(operand, 11) == parameters[i].first) {
			return parameters[i].second;
		}
	}
	return "";
}
inline string FindLocals(map<string, string>& locals, const string& operand)
{
	if (wiz::String::startsWith(operand, "$local.") && locals.end() != locals.find(wiz::String::substring(operand, 7)))
	{
		return locals[wiz::String::substring(operand, 7)];
	}
	return "";
}
//need to renewal. add $AND $OR $NOT
/// remove /, parameter chk!!
void operation(wiz::load_data::UserType& global, const vector<pair<string, string>>& parameters, const string& str, wiz::ArrayStack<string>& operandStack, EventInfo  info)
{
	if (!operandStack.empty() && operandStack.top() == "ERROR") { return; }

	if ("$EQ" == str) {
		string x, y;
		int idx = -1;
		x = operandStack.pop();
		y = operandStack.pop();

		if (x == y) {
			operandStack.push("TRUE");
		}
		else {
			operandStack.push("FALSE");
		}
	}
	else if ("$NOTEQ" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if (x != y) {
			operandStack.push("TRUE");
		}
		else {
			operandStack.push("FALSE");
		}
	}
	else if ("$AND" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if (x == "TRUE" && y == "TRUE") {
			operandStack.push("TRUE");
		}
		else {
			operandStack.push("FALSE");
		}
	}
	else if ("$OR" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if (x == "TRUE" || y == "TRUE") {
			operandStack.push("TRUE");
		}
		else {
			operandStack.push("FALSE");
		}
	}
	else if ("$NOT" == str)
	{
		string x;
		x = operandStack.pop();

		if (x == "TRUE") {
			operandStack.push("FALSE");
		}
		else {
			operandStack.push("TRUE");
		}
	}
	else if ("$COMP<" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if (Compare(x, y) == "< 0") {
			operandStack.push("TRUE");
		}
		else
		{
			operandStack.push("FALSE");
		}
	}
	else if ("$COMP>" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if (Compare(x, y) == "> 0") {
			operandStack.push("TRUE");
		}
		else
		{
			operandStack.push("FALSE");
		}
	}
	else if ("$COMP<EQ" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if (Compare(x, y) == "< 0" || Compare(x, y) == "== 0") {
			operandStack.push("TRUE");
		}
		else
		{
			operandStack.push("FALSE");
		}
	}
	else if ("$COMP>EQ" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if (Compare(x, y) == "> 0" || Compare(x, y) == "== 0") {
			operandStack.push("TRUE");
		}
		else
		{
			operandStack.push("FALSE");
		}
	}
	else if ("$add" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if (GetType(x) == GetType(y) && (GetType(y) == "INTEGER")) { /// only integer -> BigInteger
			operandStack.push(wiz::toStr(atoll(x.c_str()) + atoll(y.c_str())));
		}
		else if (GetType(x) == GetType(y) && GetType(y) == "DOUBLE") {
			operandStack.push(wiz::_toString(std::stod(x) + std::stod(y)));
		}
		else
		{
			operandStack.push("ERROR");
		}
	}
	else if ("$multiple" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();



		if (GetType(x) == GetType(y) && (GetType(y) == "INTEGER")) { /// only integer -> BigInteger
			operandStack.push(wiz::toStr(atoll(x.c_str()) * atoll(y.c_str())));
		}
		else if (GetType(x) == GetType(y) && GetType(y) == "DOUBLE") {
			operandStack.push(wiz::_toString(std::stod(x) * std::stod(y)));  /// chk?
		}
		else
		{
			operandStack.push("ERROR");
		}
	}
	else if ("$divide" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if (GetType(x) == GetType(y) && (GetType(y) == "INTEGER")) { /// only integer -> BigInteger
			operandStack.push(wiz::toStr(atoll(x.c_str()) / atoll(y.c_str())));
		}
		else if (GetType(x) == GetType(y) && GetType(y) == "DOUBLE") {
			operandStack.push(wiz::_toString(std::stod(x) / std::stod(y)));
		}
		else
		{
			operandStack.push("ERROR");
		}
	}
	else if ("$modular" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if (GetType(x) == GetType(y) && (GetType(y) == "INTEGER")) { /// only integer -> BigInteger
			operandStack.push(wiz::toStr(atoll(x.c_str()) % atoll(y.c_str())));
		}
		else
		{
			operandStack.push("ERROR");
		}
	}
	else if ("$rand" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		if (GetType(x) == GetType(y) && (GetType(y) == "INTEGER")) { /// only integer -> BigInteger
			long long _x = atoll(x.c_str());
			long long _y = atoll(y.c_str());
			long long _z = rand() % (_y - _x + 1) + _x;
			operandStack.push(wiz::toStr(_z));
		}
		else
		{
			operandStack.push("ERROR");
		}
	}
	else if ("$concat" == str)
	{
		string x, y;
		x = operandStack.pop();
		y = operandStack.pop();

		operandStack.push(x + y);
	}
	else if ("$return_value" == str)
	{
		operandStack.push(info.return_value);
	}
	// cf) empty test!!
	///ToDo - GetList -> // GetItemListIdxByIListIdx, GetUserTypeLisIdxtByIListIdx ?
	else if ("$back" == str) // ex) for x  = { 0 1 2 3 .. }, for usertaypelist? and mixed? and need more test!
	{
		string x = operandStack.pop();

		string value = wiz::load_data::LoadData::GetItemListData(global, x, "TRUE");
		wiz::load_data::UserType ut;
		wiz::load_data::LoadData::LoadDataFromString(value, ut);

		if (ut.IsItemList(ut.GetIList().size()-1))
		{
			auto x = (wiz::load_data::ItemType<string>*)ut.GetList(ut.GetIList().size() - 1);
			operandStack.push(x->Get(0));
		}
		else {
			auto x = (wiz::load_data::UserType*)ut.GetList(ut.GetIList().size() - 1);
			operandStack.push("\"" + x->ToString() + "\"");
		}
	}
	// pop_back or front - remove this function?
	else if ("$pop_back" == str) // and for usertypelist? and mixed?, usertype-> "~"
	{
		string x = operandStack.pop();
		string name;
		for (int i = x.size() - 1; i >= 0; --i)
		{
			if (x[i] == '/' && i != 0) {
				name = wiz::String::substring(x, i + 1);
				x = wiz::String::substring(x, 0, i - 1);
				break;
			}
		}

		if (x.empty()) { x = "."; }

		wiz::load_data::UserType* ut = NULL;
		auto finded = wiz::load_data::UserType::Find(&global, x);
		ut = finded.second[0];

		if (ut->IsItemList(ut->GetIList().size() - 1))
		{
			auto x = (wiz::load_data::ItemType<string>*)ut->GetList(ut->GetIList().size() - 1);
			operandStack.push(x->Get(0));
			ut->RemoveItemList(ut->GetItemListSize() - 1);
		}
		else {
			auto x = (wiz::load_data::UserType*)ut->GetList(ut->GetIList().size() - 1);
			operandStack.push("\"" + x->ToString() + "\"");
			ut->RemoveUserTypeList(ut->GetUserTypeListSize() - 1);
		}
	}
	// todo - $front, pop-front.
	else if ("$front" == str)
	{
		string x = operandStack.pop();

		string value = wiz::load_data::LoadData::GetItemListData(global, x, "TRUE");
		wiz::load_data::UserType ut;
		wiz::load_data::LoadData::LoadDataFromString(value, ut);

		if (ut.IsItemList(0))
		{
			auto x = (wiz::load_data::ItemType<string>*)ut.GetList(0);
			operandStack.push(x->Get(0));
		}
		else {
			auto x = (wiz::load_data::UserType*)ut.GetList(0);
			operandStack.push("\"" + x->ToString() + "\"");
		}
	}
	else if ("$pop_front" == str)
	{
		string x = operandStack.pop();
		string name;
		for (int i = x.size() - 1; i >= 0; --i)
		{
			if (x[i] == '/' && i != 0) {
				name = wiz::String::substring(x, i + 1);
				x = wiz::String::substring(x, 0, i - 1);
				break;
			}
		}

		if (x.empty()) { x = "."; }

		wiz::load_data::UserType* ut = NULL;
		auto finded = wiz::load_data::UserType::Find(&global, x);
		ut = finded.second[0];

		if (ut->IsItemList(0))
		{
			auto x = (wiz::load_data::ItemType<string>*)ut->GetList(0);
			operandStack.push(x->Get(0));
			ut->RemoveItemList(0);
		}
		else {
			auto x = (wiz::load_data::UserType*)ut->GetList(0);
			operandStack.push("\"" + x->ToString() + "\"");
			ut->RemoveUserTypeList(0);
		}
	}
	else if ("$get" == str)
	{
		string x = operandStack.pop();

		if ('/' == x[0])
		{
			string temp = Find(&global, x); if (!temp.empty()) { x = temp; }
		}
		{
			string temp = FindParameters(parameters, x);
			if (!temp.empty()) { x = temp; }
		}
		{
			string temp = FindLocals(info.locals, x);
			if (!temp.empty()) { x = temp; }
		}
		operandStack.push(x);
	}
	else if ("$size" == str)
	{
		string x = operandStack.pop();

		if ('/' == x[0])
		{
			wiz::load_data::UserType* ut = wiz::load_data::UserType::Find(&global, x).second[0];
			x = wiz::toStr(ut->GetItemListSize());
		}
		else
		{
			x = "FALSE";
		}

		operandStack.push(x);
	}
	else if ("$size2" == str)
	{
		string x = operandStack.pop();

		if ('/' == x[0])
		{
			wiz::load_data::UserType* ut = wiz::load_data::UserType::Find(&global, x).second[0];
			x = wiz::toStr(ut->GetUserTypeListSize());
		}
		else
		{
			x = "FALSE";
		}

		operandStack.push(x);
	}
	else if ("$element" == str) // for list
	{
		string x = operandStack.pop(); // list_name
		string y = operandStack.pop(); // idx
		long long idx = atoll(y.c_str());

		if ('/' == x[0])
		{
			wiz::load_data::UserType* ut = wiz::load_data::UserType::Find(&global, x).second[0];
			x = ut->GetItemList(idx).Get(0);
		}
		else
		{
			x = "NONE";
		}

		operandStack.push(x);
	}
}

string ToBool3(wiz::load_data::UserType& global, const vector<pair<string, string>>& parameters, const string& temp, EventInfo info) /// has bug!
{
	wiz::StringTokenizer tokenizer(temp, vector<string>{ "/" });
	vector<string> tokenVec;
	string result;

	while (tokenizer.hasMoreTokens()) {
		tokenVec.push_back(tokenizer.nextToken());
	}

	for (int i = 0; i < tokenVec.size(); ++i)
	{
		result = result + "/";
		if (wiz::String::startsWith(tokenVec[i], "$parameter.")) {
			int last = -1;
			for (int j = 0; j < tokenVec[i].size(); ++j)
			{
				if (tokenVec[i][j] == ' ' || tokenVec[i][j] == '{' || tokenVec[i][j] == '}' || tokenVec[i][j] == '=') {
					last = j - 1;
					break;
				}
			}
			if (last != -1)
			{
				string temp = FindParameters(parameters, wiz::String::substring(tokenVec[i], 0, last));

				if (!temp.empty()) {
					tokenVec[i] = wiz::String::replace(wiz::String::substring(tokenVec[i], 0, last), wiz::String::substring(tokenVec[i], 0, last), temp)
						+ wiz::String::substring(tokenVec[i], last + 1);
				}
			}
			else
			{
				string temp = FindParameters(parameters, tokenVec[i]);
				if (!temp.empty()) {
					tokenVec[i] = temp;
				}
			}
		}
		else if (wiz::String::startsWith(tokenVec[i], "$local.")) {
			int last = -1;
			for (int j = 0; j < tokenVec[i].size(); ++j)
			{
				if (tokenVec[i][j] == ' ' || tokenVec[i][j] == '{' || tokenVec[i][j] == '}' || tokenVec[i][j] == '=') {
					last = j - 1;
					break;
				}
			}
			if (last != -1)
			{
				string temp = FindLocals(info.locals, wiz::String::substring(tokenVec[i], 0, last));

				if (!temp.empty()) {
					tokenVec[i] = wiz::String::replace(wiz::String::substring(tokenVec[i], 0, last), wiz::String::substring(tokenVec[i], 0, last), temp)
						+ wiz::String::substring(tokenVec[i], last + 1);
				}
			}
			else
			{
				string temp = FindLocals(info.locals, tokenVec[i]);
				if (!temp.empty()) {
					tokenVec[i] = temp;
				}
			}
		}

		result = result + tokenVec[i];
	}
	return result;
}
string ToBool4(wiz::load_data::UserType& global, const vector<pair<string, string>>& parameters, const string& temp, EventInfo  info)
{
	string result = temp;
	wiz::ArrayStack<string> resultStack;
	wiz::load_data::UserType ut;
	bool chk = false;

	bool flag_A = false;
	if (result.size() > 1 && result[0] == '/')
	{
		flag_A = true;
	}
	result = ToBool3(global, parameters, result, info);
	if (result.empty()) { return ""; }
	if (!flag_A) {
		result = string(result.c_str() + 1);
	}
	wiz::load_data::LoadData::LoadDataFromString(result, ut);
	result = ut.ToString();
	if (result.empty()) { return result; }

	if (ut.empty()) {
		return "";
	}
	if (ut.GetUserTypeListSize() == 0 && ut.GetItemListSize() == 1) /// chk
	{
		if ('/' == result[0] && result.size() > 1)
		{
			string temp = Find(&global, result);
			if (!temp.empty()) {
				result = temp;
			}
		}
		else if (wiz::String::startsWith(result, "$local.")) {
			string _temp = FindLocals(info.locals, result);
			if (!_temp.empty()) {
				result = _temp;
			}
		}
		else if (wiz::String::startsWith(result, "$parameter.")) {
			string _temp = FindParameters(parameters, result);
			if (!_temp.empty()) {
				result = _temp;
			}
		}
		return result;
	}

	{
		chk = wiz::load_data::Utility::ChkExist(result);
		if (chk) {
			result = wiz::load_data::Utility::ChangeStr(result, "^", "^0"); // in " "
			result = wiz::load_data::Utility::ChangeStr(result, " ", "^1");
			result = wiz::load_data::Utility::ChangeStr(result, "\t", "^2");
			result = wiz::load_data::Utility::ChangeStr(result, "\r", "^3");
			result = wiz::load_data::Utility::ChangeStr(result, "\n", "^4");
			result = wiz::load_data::Utility::ChangeStr(result, "#", "^5");
		}
	}

	{ // chk..
		wiz::StringTokenizer tokenizer(result, { " ", "\n", "\t", "\r", "{", "=", "}" });
		wiz::StringTokenizer tokenizer2(result, { " ", "\n", "\t", "\r" });
		vector<string> tokenVec;
		vector<string> tokenVec2;

		while (tokenizer.hasMoreTokens()) {
			tokenVec.push_back(tokenizer.nextToken());
		}

		for (int i = tokenVec.size() - 1; i >= 0; --i)
		{
			string before = tokenVec[i];
			if ('/' == tokenVec[i][0] && tokenVec[i].size() > 1)
			{
				string _temp = Find(&global, tokenVec[i]);
				if ("" != _temp) {
					tokenVec[i] = _temp;
				}


			}
			else if (wiz::String::startsWith(tokenVec[i], "$local.")) {
				string _temp = FindLocals(info.locals, tokenVec[i]);
				if (!_temp.empty()) {
					tokenVec[i] = _temp;
				}
			}
			else if (wiz::String::startsWith(tokenVec[i], "$parameter.")) {
				string _temp = FindParameters(parameters, tokenVec[i]);
				if (!_temp.empty()) {
					tokenVec[i] = _temp;
				}
			}
		}

		while (tokenizer2.hasMoreTokens()) {
			tokenVec2.push_back(tokenizer2.nextToken());
		}

		result = "";

		int j = tokenVec.size();
		for (int i = tokenVec2.size() - 1; i >= 0; --i) {
			if (tokenVec2[i] == "{" || tokenVec2[i] == "}" || tokenVec2[i] == "=") {
				continue;
			}
			else {
				--j;
				tokenVec2[i] = tokenVec[j];
			}
		}

		for (int i = 0; i < tokenVec2.size(); ++i) {
			if (i != 0) { result = result + " "; }
			result = result + tokenVec2[i];
		}
	}

	//
	wiz::ArrayStack<string> operandStack; // ?¼ì°?°ì
	wiz::ArrayStack<string> operatorStack; // ?°ì°??
	wiz::StringTokenizer tokenizer(result, { " ", "\n", "\t", "\r" });
	vector<string> tokenVec;

	while (tokenizer.hasMoreTokens()) {
		tokenVec.push_back(tokenizer.nextToken());
	}

	for (int i = tokenVec.size() - 1; i >= 0; --i)
	{
		if ('/' == tokenVec[i][0] && tokenVec[i].size() > 1)
		{
			string temp = Find(&global, tokenVec[i]);
			if (!temp.empty()) {
				tokenVec[i] = temp;
			}
		}

		if ('$' != tokenVec[i][0] || ('$' == tokenVec[i][0] && tokenVec[i].size() == 1)) {
			operandStack.push(tokenVec[i]);
		}
		else
		{
			operandStack.pop();
			operandStack.pop();
			operatorStack.push(tokenVec[i]);
			operation(global, parameters, tokenVec[i], operandStack, info);
			operandStack[operandStack.size() - 2] = operandStack[operandStack.size() - 1];
			operandStack.pop();
		}
	}

	// ex) A = { B = 1 C = { 3 } } D = { E }
	// =>  A = { B = 1 C = 3  }  D = E

	//result = "";
	vector<string> strVec;
	stack<int> chkBrace;

	chkBrace.push(0);

	for (int i = operandStack.size() - 1; i >= 0; --i)
	{
		if (operandStack[i] == "}") {
			chkBrace.top()++;
			if (chkBrace.top() == 2)
			{
				string temp = strVec.back();
				strVec.pop_back();
				strVec.pop_back();
				strVec.push_back(temp);
				
				chkBrace.pop();
				continue;
			}
			chkBrace.pop();
		}
		else if (operandStack[i] == "{") {
			chkBrace.top()++;
			chkBrace.push(0);
		}
		else {
			chkBrace.top()++;
		}
		strVec.push_back(operandStack[i]);
	}

	result = "";
	for (int i = 0; i < strVec.size(); ++i) {
		if (i != 0) { result = result + " "; }
		result = result + strVec[i];
	}

	{
		if (chk) {
			result = wiz::load_data::Utility::ChangeStr(result, "^5", "#");
			result = wiz::load_data::Utility::ChangeStr(result, "^4", "\n");
			result = wiz::load_data::Utility::ChangeStr(result, "^3", "\r");
			result = wiz::load_data::Utility::ChangeStr(result, "^2", "\t");
			result = wiz::load_data::Utility::ChangeStr(result, "^1", " ");
			result = wiz::load_data::Utility::ChangeStr(result, "^0", "^");
		}
	}
	return result;
}


string excute_module(wiz::load_data::UserType& global)
{
	map<string, string> objectMap;
	map<string, wiz::load_data::UserType> moduleMap;
	string module_value = "";
	// data, event load..
	wiz::ArrayStack<EventInfo> eventStack;
	map<string, int> convert;
	auto _events = global.GetCopyUserTypeItem("Event");
	wiz::load_data::UserType events;
	for (int i = 0; i < _events.size(); ++i) {
		events.LinkUserType(_events[i]);
	}
	global.RemoveUserTypeList("Event");

	if (global.GetUserTypeItem("Main").empty())
	{
		cout << "do not exist Main" << endl;
		return "ERROR -1";
	}
	auto _Main = global.GetCopyUserTypeItem("Main")[0]; /// todo - main???ê°?¬ì¼ë§??ë¤. Main???ì¼ë©??ë¬..!
	wiz::load_data::UserType Main;
	Main.LinkUserType(_Main);
	global.RemoveUserTypeList("Main");

	// event table setting
	for (int i = 0; i < events.GetUserTypeListSize(); ++i)
	{
		auto x = events.GetUserTypeList(i)->GetItem("id");
		if (!x.empty()) {
			//cout <<	x[0] << endl;
			convert.insert(pair<string, int>(x[0].Get(0), i));
		}
		else {
			// error
		}
	}

	// start from Main.
	{
		EventInfo info;
		info.eventUT = Main.GetUserTypeList(0);
		info.item_idx = 0;
		info.userType_idx.push(0);
		info.parameters.push_back(
			make_pair("id", info.eventUT->GetUserTypeItem("$call")[0]->GetItem("id")[0].Get(0))
		);
		info.id = info.parameters[0].second;

		const int no = convert.at(info.id);
		for (int i = 0; i < events.GetUserTypeList(no)->GetUserTypeListSize(); ++i) {
			if (events.GetUserTypeList(no)->GetUserTypeList(i)->GetName() == "$local") {
				for (int j = 0; j < events.GetUserTypeList(no)->GetUserTypeList(i)->GetItemListSize(); ++j) {
					string name = events.GetUserTypeList(no)->GetUserTypeList(i)->GetItemList(j).Get(0);
					string value = "";
					info.locals.insert(make_pair(name, value));
				}
				break;
			}
		}

		eventStack.push(info);
	}

	// main loop
	while (!eventStack.empty())
	{
		EventInfo info = eventStack.top();
		string str;

		for (int i = 0; i < info.parameters.size(); ++i) {
			if (info.parameters[i].first == "id") {
				str = info.parameters[i].second;
				break;
			}
		}

		int no = convert.at(str);

		int state = 0;

		if (info.userType_idx.size() == 1 && info.userType_idx[0] >= events.GetUserTypeList(no)->GetUserTypeListSize())
		{
			if (eventStack.size() == 1)
			{
				module_value = eventStack.top().return_value;
			}

			eventStack.pop();
			continue;
		}

		{ /// has bug!! WARNNING!!
			wiz::load_data::UserType* val = NULL;
			if (eventStack.top().userType_idx.size() == 1) {
				if (events.GetUserTypeList(no)->GetUserTypeListSize() > eventStack.top().userType_idx.top()) {
					val = events.GetUserTypeList(no)->GetUserTypeList(eventStack.top().userType_idx.top());

					if (eventStack.top().userType_idx.top() >= 1 && val->GetName() == "$else"
						&& events.GetUserTypeList(no)->GetUserTypeList(eventStack.top().userType_idx.top() - 1)->GetName() != "$if") {
						return "ERROR not exist $if, front $else.";
					}
					if (eventStack.top().userType_idx.top() == 0 && val->GetName() == "$else") {
						return "ERROR not exist $if, front $else.";
					}
				}
				else {
					val = NULL;
				}
			}
			else
			{
				// # of userType_idx == nowUT.size() + 1, and nowUT.size() == conditionStack.size()..
				while (!eventStack.top().nowUT.empty() && eventStack.top().nowUT.top()->GetUserTypeListSize() <= eventStack.top().userType_idx.top())
				{
					eventStack.top().nowUT.pop();
					eventStack.top().userType_idx.pop();
					eventStack.top().conditionStack.pop();
				}

				if (!eventStack.top().nowUT.empty() && eventStack.top().nowUT.top()->GetUserTypeListSize() > eventStack.top().userType_idx.top()) {
					val = eventStack.top().nowUT.top()->GetUserTypeList(eventStack.top().userType_idx.top());

					if (eventStack.top().userType_idx.top() >= 1 && val->GetName() == "$else"
						&& eventStack.top().nowUT.top()->GetUserTypeList(eventStack.top().userType_idx.top() - 1)->GetName() != "$if") {
						return "ERROR not exist $if, front $else.";
					}
					if (eventStack.top().userType_idx.top() == 0 && val->GetName() == "$else") {
						return "ERROR not exist $if, front $else.";
					}
				}
				else // same to else if( eventSTack.top().nowUT.empty() ), also same to else if ( 1 == eventStack.top().userType_idx.size() )
				{
					if (events.GetUserTypeList(no)->GetUserTypeListSize() > eventStack.top().userType_idx.top()) {
						val = events.GetUserTypeList(no)->GetUserTypeList(eventStack.top().userType_idx.top());

						if (eventStack.top().userType_idx.top() >= 1 && val->GetName() == "$else"
							&& events.GetUserTypeList(no)->GetUserTypeList(eventStack.top().userType_idx.top() - 1)->GetName() != "$if") {
							return "ERROR not exist $if, front $else.";
						}
						if (eventStack.top().userType_idx.top() == 0 && val->GetName() == "$else") {
							return "ERROR not exist $if, front $else.";
						}
					}
					else {
						val = NULL;
					}
				}
			}

			while (val != NULL) 
			{
				if ("$edit_mode" == val->GetName())
				{
					MStyleTest(&global);

					system("cls");
					
					gotoxy(0, 0);
					setcolor(7, 0);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$save" == val->GetName()) // save data, event, main!
				{
					//todo
					// "filename" save_option(0~2)
					string fileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					string option = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());
					
					wiz::load_data::LoadData::SaveWizDB(global, fileName, option, "");
					wiz::load_data::LoadData::SaveWizDB(Main, fileName, option, "APPEND");
					wiz::load_data::LoadData::SaveWizDB(events, fileName, option, "APPEND");

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$save_data_only" == val->GetName())
				{
					//todo
					// "filename" save_option(0~2)
					string fileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					string option = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());
					
					wiz::load_data::LoadData::SaveWizDB(global, fileName, option, "");

					eventStack.top().userType_idx.top()++;
					break;
				}

				else if ("$save_data_only2" == val->GetName()) 
				{
					//todo
					// "filename" save_option(0~2)
					string dirName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					string fileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					string option = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top());

					// todo - for? auto x = global.GetUserTypeItem(dirName);
					wiz::load_data::UserType* utTemp = global.GetUserTypeItem(dirName)[0];
					wiz::load_data::LoadData::SaveWizDB(*utTemp, fileName, option, "");


					eventStack.top().userType_idx.top()++;
					break;
				}
				/// module name or object name -> must "~" .
				else if ("$register_module" == val->GetName())
				{
					string moduleFileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					moduleFileName = wiz::String::substring(moduleFileName, 1, moduleFileName.size() - 2);

					wiz::load_data::UserType moduleUT;
					wiz::load_data::LoadData::LoadDataFromFile(moduleFileName, moduleUT);

					//moduleMap.insert(make_pair(moduleFileName, moduleUT));
					moduleMap[moduleFileName] = move(moduleUT);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$call_registered_module" == val->GetName())
				{
					string moduleFileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					string input;

					if (val->GetUserTypeListSize() >= 2) {
						input = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());
					}

					moduleFileName = wiz::String::substring(moduleFileName, 1, moduleFileName.size() - 2);

					wiz::load_data::UserType moduleUT = moduleMap.at(moduleFileName);
					wiz::load_data::LoadData::AddData(moduleUT, "", input, "TRUE");

					eventStack.top().return_value = excute_module(moduleUT);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$module" == val->GetName())
				{
					string moduleFileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					string input;

					if (val->GetUserTypeListSize() >= 2) {
						input = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());
					}

					moduleFileName = wiz::String::substring(moduleFileName, 1, moduleFileName.size() - 2);

					wiz::load_data::UserType moduleUT;
					wiz::load_data::LoadData::LoadDataFromFile(moduleFileName, moduleUT);
					wiz::load_data::LoadData::AddData(moduleUT, "", input, "TRUE");

					eventStack.top().return_value = excute_module(moduleUT);

					eventStack.top().userType_idx.top()++;
					break;
				}
				// todo - register module from file
				// todo	- register module from string
				// todo - call registered module.  $registered_module = { name = { ~ } input = { input = { n = 1 } } }

				else if ("$register_object" == val->GetName()) {
					string objectFileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);

					wiz::load_data::UserType objectUT;
					wiz::load_data::LoadData::LoadDataFromFile(objectFileName, objectUT);

					string data = objectUT.ToString();

					//objectMap.insert(make_pair(objectFileName, data));
					objectMap[objectFileName] = move(data);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$call_registered_object" == val->GetName()) {
					string objectFileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);
					string parameter;

					parameter = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());
					
					string data = objectMap.at(objectFileName);
					data = data + " Main = { $call = { id = 0 } } Event = { id = 0 $call = { " + parameter + "  } } ";
					
					wiz::load_data::UserType objectUT;
					wiz::load_data::LoadData::LoadDataFromString(data, objectUT);

					eventStack.top().return_value = excute_module(objectUT);

					eventStack.top().userType_idx.top()++;
					break;
				}
				/// object of class?
				else if ("$object" == val->GetName()) {
					string objectFileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);
					string parameter;
					
					parameter = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());
					

					wiz::load_data::UserType objectUT;
					wiz::load_data::LoadData::LoadDataFromFile(objectFileName, objectUT);

					string data = objectUT.ToString();

					data = data + " Main = { $call = { id = 0 } } Event = { id = 0 $call = { " + parameter + "  } } ";
					
					objectUT.Remove();
					wiz::load_data::LoadData::LoadDataFromString(data, objectUT);

					eventStack.top().return_value = excute_module(objectUT);

					eventStack.top().userType_idx.top()++;
					break;
				}
				// todo - register object from file.
				//		~.ToString() + "Main = { $call = { id = 0 } } Event = { id = 0 $call = { id = " + id_val + " " + param_name1 + " = " + param_val1 + "  } } "
				// todo - register object from string.
				// todo - call registered object.  $registered_object = { name = { "ex2.txt" } parameter = { id = 1 i = 1 j = 1 } }  
				else if ("$option" == val->GetName()) // first
				{
					eventStack.top().option = ToBool4(global, eventStack.top().parameters, val->ToString(), eventStack.top());

					eventStack.top().userType_idx.top()++;
					break;
				}
				// done - ($push_back-insert!) $pop_back, $push_front, $pop_front ($front?, $back?)
				else if ("$pop_back" == val->GetName()) {
					string dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					wiz::load_data::UserType* ut = NULL;
					auto finded = wiz::load_data::UserType::Find(&global, dir);
					ut = finded.second[0];

					string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 2) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());
					}
					wiz::load_data::LoadData::Remove(global, dir, ut->GetIList().size() - 1, condition);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$push_front" == val->GetName()) {
					string value = val->GetUserTypeList(1)->ToString();
					string dir;
					if (val->GetUserTypeList(0)->GetItemListSize() > 0) {
						dir = val->GetUserTypeList(0)->GetItemList(0).Get(0);
						dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top());
					}
					else ///val->Ge
					{
						dir = string(val->GetUserTypeList(0)->ToString());
						dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top());
					}


					value = ToBool4(global, eventStack.top().parameters, value, eventStack.top());
					
					string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 3) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top());
					}
					wiz::load_data::LoadData::AddDataAtFront(global, dir, value, condition);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$pop_front" == val->GetName()) {
					string dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					
					string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 2) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());
					}
					wiz::load_data::LoadData::Remove(global, dir, 0, condition);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$call" == val->GetName()) {
					if (!val->GetItem("id").empty()) {
						info.id = val->GetItem("id")[0].Get(0);
					}
					else {
						info.id = ToBool4(global, eventStack.top().parameters, val->GetUserTypeItem("id")[0]->ToString(), eventStack.top());
					}
					// cf) id =  { $local.i }
					// ì¶ê? todo

					info.eventUT = events.GetUserTypeList(no);
					info.userType_idx.clear();
					info.userType_idx.push(0);
					info.return_value.clear();
					info.nowUT.clear();

					EventInfo info2;
					info2 = info;

					if (info.id != eventStack.top().id) {
						info.parameters.clear();
					}
					info.conditionStack.clear();

					//
					if (info.id != eventStack.top().id) {
						for (int j = 0; j < val->GetItemListSize(); ++j) {
							if (val->GetItemListSize() > 0) {
								string temp = ToBool4(global, info2.parameters, val->GetItemList(j).Get(0), info2);
								info.parameters.push_back(make_pair(val->GetItemList(j).GetName(), temp));
							}
						}
						for (int j = 0; j < val->GetUserTypeListSize(); ++j) {
							if (val->GetUserTypeListSize() > 0) {
								string temp = ToBool4(global, info2.parameters, val->GetUserTypeList(j)->ToString(), info2);
								info.parameters.push_back(make_pair(val->GetUserTypeList(j)->GetName(), temp));
							}
						}
						eventStack.top().userType_idx.top()++;
					}
					else {
						if (val->GetItemListSize() > 0) {
							for (int j = 0; j < val->GetItemListSize(); ++j) {
								string temp = ToBool4(global, info2.parameters, val->GetItemList(j).Get(0), info);
								for (int k = 0; k < info.parameters.size(); ++k)
								{
									if (info.parameters[k].first == val->GetItemList(j).GetName())
									{
										info.parameters[k].second = temp;
									}
								}
								// debug 
								//cout << temp << endl;
							}
						}
						if (val->GetUserTypeListSize() > 0) {
							for (int j = 0; j < val->GetUserTypeListSize(); ++j) {
								string temp = ToBool4(global, info2.parameters, val->GetUserTypeList(j)->ToString(), info);
								for (int k = 0; k < info.parameters.size(); ++k)
								{
									if (info.parameters[k].first == val->GetUserTypeList(j)->GetName())
									{
										info.parameters[k].second = temp;
									}
								}
								// debug 
								//cout << temp << endl;
							}
						}

						eventStack.top().userType_idx.top()++;

						// remove remove_now_event_stack_a?
						if (eventStack.top().option == "REMOVE_NOW_EVENT_STACK_A" || "REMOVE_IF_CALL_ONESELF_EVENT" == eventStack.top().option) //
						{
							eventStack.pop();
						}
					}

					if (eventStack.top().option == "REMOVE_IF_CALL_ANY_EVENT")
					{
						eventStack.pop();
					}


					info.locals.clear();
					const int no = convert.at(info.id);
					for (int i = 0; i < events.GetUserTypeList(no)->GetUserTypeListSize(); ++i) {
						if (events.GetUserTypeList(no)->GetUserTypeList(i)->GetName() == "$local") {
							for (int j = 0; j < events.GetUserTypeList(no)->GetUserTypeList(i)->GetItemListSize(); ++j) {
								string name = events.GetUserTypeList(no)->GetUserTypeList(i)->GetItemList(j).Get(0);
								string value = "";
								info.locals.insert(make_pair(name, value));
							}
							break;
						}
					}

					eventStack.push(info);

					break;
				}
				
				else if ("$assign" == val->GetName()) /// -> assign2?
				{
					pair<string, string> dir = Find2(&global, val->GetItemList(0).Get(0));
					string data = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					if (dir.first == "" && wiz::String::startsWith(dir.second, "$local."))
					{
						eventStack.top().locals[wiz::String::substring(dir.second, 7)] = data;
					}
					else {
						wiz::load_data::LoadData::SetData(global, dir.first, dir.second, data, "TRUE");
					}
					eventStack.top().userType_idx.top()++;
					break;
				}
				
				else if ("$assign2" == val->GetName())
				{
					pair<string, string> dir = Find2(&global, ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top()));
					string data = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());
					
					{
						if (dir.first == "" && wiz::String::startsWith(dir.second, "$local."))
						{
							eventStack.top().locals[wiz::String::substring(dir.second, 7)] = data;
						}
						else {
							wiz::load_data::LoadData::SetData(global, dir.first, dir.second, data, "TRUE");
						}
					}
					
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$assign_local" == val->GetName()) /// no condition, 
				{
					pair<string, string> dir = Find2(&global, ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top()));
					string data = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());

					{
						if (dir.first == "" && dir.second.size() > 1 && dir.second[0] == '@')
						{
							dir.second.erase(dir.second.begin());
						}
						if (dir.first == "" && wiz::String::startsWith(dir.second, "$local."))
						{
							eventStack.top().locals[wiz::String::substring(dir.second, 7)] = data;
						}
						else {
							// throw error??
						}
					}

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$assign_global" == val->GetName()) // ì£¼ì!! dir=> dir/name ( dir= { name = val } } , @ë¥??ì ë¶ì¬???ë¤. 
				{
					pair<string, string> dir = Find2(&global, ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top()));
					string data = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());

					string condition;
					if (val->GetUserTypeListSize() >= 3) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top());
					}
					wiz::load_data::LoadData::SetData(global, dir.first, dir.second, data, condition);

					// chk local?

					eventStack.top().userType_idx.top()++;
					break;
				}
				/// cf) insert3? - any position?
				else if ("push_back" == val->GetName() || "$insert" == val->GetName() || "$insert2" == val->GetName())
				{
					string value = val->GetUserTypeList(1)->ToString();
					string dir;
					if (val->GetUserTypeList(0)->GetItemListSize() > 0) {
						dir = val->GetUserTypeList(0)->GetItemList(0).Get(0);
						dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top());
					}
					else ///val->Ge
					{
						dir = string(val->GetUserTypeList(0)->ToString());
						dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top());
					}

					value = ToBool4(global, eventStack.top().parameters, value, eventStack.top());

					string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 3) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top());
					}
					wiz::load_data::LoadData::AddData(global, dir, value, condition);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$make" == val->GetName()) // To Do? - make2? or remake?
				{
					string dir;
					bool is2 = false;
					if (val->GetItemListSize() > 0) {
						dir = val->GetItemList(0).Get(0);
						dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top());
					}
					else // 
					{
						dir = string(val->ToString());
						dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top());
						is2 = true;
					}

					string name;
					for (int i = dir.size() - 1; i >= 0; --i)
					{
						if (dir[i] == '/') {
							name = wiz::String::substring(dir, i + 1);
							dir = wiz::String::substring(dir, 0, i - 1);
							break;
						}
					}
					if (dir.empty()) { dir = "."; }

					string condition = "TRUE";
					if (is2 && val->GetUserTypeListSize() >= 2) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());
					}
					else if (false == is2 && val->GetUserTypeListSize() >= 1 ) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					}

					wiz::load_data::LoadData::AddUserType(global, dir, name, "", condition);


					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$findIndex" == val->GetName()) // For list : { 1 2  3 4 5 }
				{
					string dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					string value = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());

					wiz::load_data::UserType ut;
					wiz::load_data::LoadData::LoadDataFromString(wiz::load_data::UserType::Find(&global, dir).second[0]->ToString(), ut);

					for (int i = 0; i < ut.GetItemListSize(); ++i) {
						if (ut.GetItemList(i).Get(0) == value) {
							eventStack.top().return_value = wiz::toStr(i);
							break;
						}
					}

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$remove" == val->GetName()) // remove by dir., remove all?
				{
					string dir = string(val->GetItemList(0).Get(0).c_str()); // item -> userType

					dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top());

					string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 2) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());
					}
					wiz::load_data::LoadData::Remove(global, dir, condition);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$remove2" == val->GetName()) // remove /dir/name
				{
					string dir = string(val->GetItemList(0).Get(0).c_str()); // item -> userType
					dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top());
					string name;
					for (int i = dir.size() - 1; i >= 0; --i)
					{
						if (dir[i] == '/') {
							name = wiz::String::substring(dir, i + 1);
							dir = wiz::String::substring(dir, 0, i - 1);
							break;
						}
					}

					string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 2) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());
					}
					wiz::load_data::LoadData::Remove(global, dir, name, condition);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$remove3" == val->GetName()) /// remove itemlist by idx.
				{
					string dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					string value = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());

					long long idx = atoll(value.c_str());  // long long -> int?

					string condition = "TRUE";

					if (val->GetUserTypeListSize() >= 3) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top());
					}

					wiz::load_data::LoadData::Remove(global, dir, idx, condition);
						// remove -> UserType::Find(&global, dir).second[0]->RemoveItemList(idx); /// change ?

					eventStack.top().userType_idx.top()++;
					break;
				}
				
				else if ("$setElement" == val->GetName())
				{
					string dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					string idx = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());
					string value = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top());

					long long _idx = stoll(idx);
					wiz::load_data::UserType::Find(&global, dir).second[0]->SetItem(_idx, value);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$swap" == val->GetName()) // $swap2
				{
					string dir = string(val->GetItemList(0).Get(0).c_str()); // + 0
					string value1 = val->GetUserTypeList(0)->ToString();
					string value2 = val->GetUserTypeList(1)->ToString();

					value1 = ToBool4(global, eventStack.top().parameters, value1, eventStack.top());
					value2 = ToBool4(global, eventStack.top().parameters, value2, eventStack.top());
					if (value1 != value2) {
						long long x = atoll(value1.c_str());
						long long y = atoll(value2.c_str());

						string temp = wiz::load_data::UserType::Find(&global, dir).second[0]->GetItemList(x).Get(0);
						string temp2 = wiz::load_data::UserType::Find(&global, dir).second[0]->GetItemList(y).Get(0);

						wiz::load_data::LoadData::SetData(global, dir, x, temp2, "TRUE");
						wiz::load_data::LoadData::SetData(global, dir, y, temp, "TRUE");
					}

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$print" == val->GetName()) /// has many bugs..!?, for print list or print item?.
				{
					if (val->GetUserTypeListSize() == 1
						&& val->GetUserTypeList(0)->GetItemListSize() == 1)
					{
						string listName = val->GetUserTypeList(0)->GetItemList(0).Get(0);

						if (listName.size() >= 2 && listName[0] == '\"' && listName.back() == '\"')
						{
							listName = wiz::String::substring(listName, 1, listName.size() - 2);
							cout << listName;
						}
						else if (listName.size() == 2 && listName[0] == '\\' && listName[1] == 'n')
						{
							cout << endl;
						}
						else if (wiz::String::startsWith(listName, "$local.")
							|| wiz::String::startsWith(listName, "$parameter.")
							)
						{
							string temp = ToBool4(global, eventStack.top().parameters, listName, eventStack.top());
							if (temp.empty()) {
								cout << "EMPTY";
							}
							else {
								cout << temp;
							}
						}
						else if (wiz::String::startsWith(listName, "/") && listName.size() > 1)
						{
							string temp = ToBool4(global, eventStack.top().parameters, listName, eventStack.top());
							if (temp != listName) // chk 
							{
								cout << temp;
							}
							else {
								wiz::load_data::UserType* ut = wiz::load_data::UserType::Find(&global, listName).second[0];
								if (ut->GetItemList(0).GetName().empty()) {
									cout << ut->GetItemList(0).Get(0);
								}
							}
						}
						else
						{
							wiz::load_data::UserType* ut = wiz::load_data::UserType::Find(&global, listName).second[0];
							if (ut->GetItemList(0).GetName().empty()) {
								cout << ut->GetItemList(0).Get(0);
							}
						}
					}
					else if (val->GetUserTypeListSize() == 1
						&& val->GetUserTypeList(0)->GetItemListSize() == 0
						&& val->GetUserTypeList(0)->GetUserTypeListSize() == 1)
					{
						string name = ToBool4(global, eventStack.top().parameters,
							val->GetUserTypeList(0)->ToString(), eventStack.top());
						cout << name;
					}
					else
					{
						string start = val->GetUserTypeList(1)->ToString();
						string last = val->GetUserTypeList(2)->ToString();

						start = ToBool4(global, eventStack.top().parameters, start, eventStack.top());
						last = ToBool4(global, eventStack.top().parameters, last, eventStack.top());

						string listName = val->GetUserTypeList(0)->GetItemList(0).Get(0);
						long long _start = atoll(start.c_str());
						long long _last = atoll(last.c_str());
						wiz::load_data::UserType* ut = wiz::load_data::UserType::Find(&global, listName).second[0];
						for (int i = _start; i <= _last; ++i)
						{
							if (i != _start) { cout << " "; }
							cout << ut->GetItemList(i).Get(0);
						}
					}

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$print2" == val->GetName()) /// for print usertype.ToString();
				{
					string dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					auto x = wiz::load_data::UserType::Find(&global, dir);
					
					for (auto& ut : x.second) {
						cout << ut->ToString();
						cout << endl;
					}

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$load" == val->GetName())
				{
					// to do, load data and events from other file!
					for (int i = 0; i < val->GetItemListSize(); ++i) {
						wiz::load_data::UserType ut;
						string fileName = ToBool4(global, eventStack.top().parameters, val->GetItemList(i).Get(0), eventStack.top());
						fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);

						if (wiz::load_data::LoadData::LoadDataFromFile(fileName, ut)) {
							{
								int item_count = 0;
								int userType_count = 0;

								for (int i = 0; i < ut.GetIList().size(); ++i) {
									if (ut.IsItemList(i)) {
										global.AddItem(std::move(ut.GetItemList(item_count).GetName()),
											std::move(ut.GetItemList(item_count).Get(0)));
										item_count++;
									}
									else {
										global.AddUserTypeItem(std::move(*ut.GetUserTypeList(userType_count)));
										userType_count++;
									}
								}
							}

							auto _Main = ut.GetUserTypeItem("Main");
							if (!_Main.empty())
							{
								// error!
								cout << "err" << endl;
								
								return "ERROR -2"; /// exit?
							}
						}
						else {
							// error!
						}
					}

					{
						convert.clear();
						auto _events = global.GetCopyUserTypeItem("Event");
						for (int i = 0; i < _events.size(); ++i) {
							events.LinkUserType(_events[i]);
						}
						global.RemoveUserTypeList("Event");

						// event table setting
						for (int i = 0; i < events.GetUserTypeListSize(); ++i)
						{
							auto x = events.GetUserTypeList(i)->GetItem("id");
							if (!x.empty()) {
								//cout <<	x[0] << endl;
								convert.insert(pair<string, int>(x[0].Get(0), i));
							}
							else {
								// error
							}
						}

						// update no
						no = convert[str];
					}

					eventStack.top().userType_idx.top()++;
					break;

				}
				else if ("$load_only_data" == val->GetName()) // $load2?
				{
					// to do, load data and events from other file!
					wiz::load_data::UserType ut;
					string fileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top());
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					string dirName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top());
					wiz::load_data::UserType* utTemp = global.GetUserTypeItem(dirName)[0];
						
					if (wiz::load_data::LoadData::LoadDataFromFile(fileName, ut)) {
						{
							int item_count = 0;
							int userType_count = 0;

							for (int i = 0; i < ut.GetIList().size(); ++i) {
								if (ut.IsItemList(i)) {
									utTemp->AddItem(std::move(ut.GetItemList(item_count).GetName()),
										std::move(ut.GetItemList(item_count).Get(0)));
									item_count++;
								}
								else {
									utTemp->AddUserTypeItem(std::move(*ut.GetUserTypeList(userType_count)));
									userType_count++;
								}
							}
						}

					//	auto _Main = ut.GetUserTypeItem("Main");
					//	if (!_Main.empty())
					//	{
							// error!
					//		cout << "err" << endl;

				//			return "ERROR -2"; /// exit?
				//		}
					}
					else {
						// error!
					}

					eventStack.top().userType_idx.top()++;
					break;

				}
				else if ("$clear_screen" == val->GetName())
				{
					system("cls");
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$_getch" == val->GetName())
				{
					_getch();
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$input" == val->GetName())
				{
					string str;
					cin >> str;
					eventStack.top().return_value = str;
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$return" == val->GetName())
				{
					eventStack.top().userType_idx.top()++;
					if (eventStack.size() > 1)
					{
						string temp = ToBool4(global, eventStack.top().parameters, val->ToString(), eventStack.top());
						/// if temp just one
						eventStack[eventStack.size() - 2].return_value = temp;
					}

					if (eventStack.size() == 1)
					{
						string temp = ToBool4(global, eventStack.top().parameters, val->ToString(), eventStack.top());

						module_value = temp;
					}
					eventStack.pop();
					break;
				}
				else if ("$parameter" == val->GetName())
				{
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$local" == val->GetName())
				{
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$sort" == val->GetName()) {
					// todo
					vector<SortInfo> siVec;
					wiz::load_data::UserType* utTemp = 
						wiz::load_data::UserType::Find(&global, 
							ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top())).second[0];
	
					vector<wiz::load_data::Type*> temp;


					int item_count = 0, ut_count = 0;
					for (int i = 0; i < utTemp->GetIList().size(); ++i) {
						if (utTemp->GetIList()[i] == 1) {
							temp.push_back(&(utTemp->GetItemList(item_count)));
							siVec.emplace_back(utTemp->GetItemList(item_count).GetName(), 1, i);
							item_count++;
						}
						else {
							temp.push_back(utTemp->GetUserTypeList(ut_count));
							siVec.emplace_back(utTemp->GetUserTypeList(ut_count)->GetName(), 2, i);
							ut_count++;
						}
					}

					std::sort(siVec.begin(), siVec.end());
					

					wiz::load_data::UserType ut;
					for (int i = 0; i < temp.size(); ++i)
					{
						if (siVec[i].iElement == 1) {
							ut.AddItem(siVec[i].name, static_cast<wiz::load_data::ItemType<string>*>(temp[siVec[i].idx])->Get(0));
						}
						else {
							ut.AddUserTypeItem(*(static_cast<wiz::load_data::UserType*>(temp[siVec[i].idx])));
						}
					}

					utTemp->Remove();

					wiz::load_data::LoadData::AddData(*(utTemp), "", ut.ToString(), "TRUE");
					

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$stable_sort" == val->GetName()) {
					// todo
					// todo
					vector<SortInfo> siVec;
					wiz::load_data::UserType* utTemp =
						wiz::load_data::UserType::Find(&global,
							ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top())).second[0];

					vector<wiz::load_data::Type*> temp;


					int item_count = 0, ut_count = 0;
					for (int i = 0; i < utTemp->GetIList().size(); ++i) {
						if (utTemp->GetIList()[i] == 1) {
							temp.push_back(&(utTemp->GetItemList(item_count)));
							siVec.emplace_back(utTemp->GetItemList(item_count).GetName(), 1, i);
							item_count++;
						}
						else {
							temp.push_back(utTemp->GetUserTypeList(ut_count));
							siVec.emplace_back(utTemp->GetUserTypeList(ut_count)->GetName(), 2, i);
							ut_count++;
						}
					}

					std::stable_sort(siVec.begin(), siVec.end());


					wiz::load_data::UserType ut;
					for (int i = 0; i < temp.size(); ++i)
					{
						if (siVec[i].iElement == 1) {
							ut.AddItem(siVec[i].name, static_cast<wiz::load_data::ItemType<string>*>(temp[siVec[i].idx])->Get(0));
						}
						else {
							ut.AddUserTypeItem(*(static_cast<wiz::load_data::UserType*>(temp[siVec[i].idx])));
						}
					}

					utTemp->Remove();

					wiz::load_data::LoadData::AddData(*(utTemp), "", ut.ToString(), "TRUE");


					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$if" == val->GetName()) // ToDo!!
				{
					string temp = val->GetUserTypeList(0)->ToString();
					temp = ToBool4(global, eventStack.top().parameters, temp, eventStack.top());

					if (!eventStack.top().conditionStack.empty())
					{
						if ("TRUE" == temp && eventStack.top().conditionStack.top() == "FALSE")
						{
							temp = "FALSE";
						}
						else if ("FALSE" == temp && eventStack.top().conditionStack.top() == "FALSE")
						{
							temp = "FALSE";
						}
						else if (!eventStack.top().nowUT.empty() && eventStack.top().userType_idx.top() + 1 < eventStack.top().nowUT.top()->GetUserTypeListSize()
							&& (eventStack.top().nowUT.top()->GetUserTypeList(eventStack.top().userType_idx.top() + 1)->GetName() == "$else"))
						{
							eventStack.top().conditionStack.push(temp);
						}
						else if ("TRUE" == temp)
						{
							eventStack.top().conditionStack.push(temp);
						}
					}
					else
					{
						if (eventStack.top().userType_idx.top() + 1 < events.GetUserTypeList(no)->GetUserTypeListSize() &&
							events.GetUserTypeList(no)->GetUserTypeList(eventStack.top().userType_idx.top() + 1)->GetName() == "$else")
						{
							eventStack.top().conditionStack.push(temp);
						}
						else if ("TRUE" == temp)
						{
							eventStack.top().conditionStack.push(temp);
						}
					}

					if ("TRUE" == temp)
					{
						eventStack.top().nowUT.push(val->GetUserTypeList(1));
						//val = eventStack.top().nowUT.top()->GetUserTypeList(0); 
						eventStack.top().userType_idx.top()++;
						eventStack.top().userType_idx.push(0);
						//eventStack.top().state.push(1);
						//state = 1;
						break;
					}//
					else if ("FALSE" == temp)
					{
						eventStack.top().userType_idx.top()++;
						break;
					}
					else
					{
						// debug..
						std::cout << "Error Debug : " << temp << endl;
						return "ERROR -3";
					}
				}
				else if ("$else" == val->GetName())
				{
					if (!eventStack.top().conditionStack.empty() && "FALSE" == eventStack.top().conditionStack.top())
					{
						eventStack.top().conditionStack.top() = "TRUE";
						eventStack.top().nowUT.push(val->GetUserTypeList(0));
						//val = eventStack.top().nowUT.top()->GetUserTypeList(0); // empty chk
						eventStack.top().userType_idx.top()++;
						eventStack.top().userType_idx.push(0);
						//eventStack.top().state.push(2);
						//state = 2; //
						break;
					}
					else
					{
						eventStack.top().userType_idx.top()++;
						break;
					}
				}

				{
				//	if (chkFunc(eventStack, &val)) {
				//		eventStack.top().userType_idx.top()++;
				//		break;
				//	}
				}
			}
		}
	}

	return module_value;
}


int main(int argc, char* argv[])
{
	srand(time(NULL)); // 

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
	wiz::load_data::LoadData::LoadDataFromFile(fileName, global);
	
	cout << "fileName is " << fileName << endl;
	cout << "excute result is " << excute_module(global) << endl;
	//_getch(); // pause..

	return 0;
}

