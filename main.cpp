

#define _CRT_SECURE_NO_WARNINGS

// memory leak test.
#ifdef _DEBUG 
#include <vld.h>
#endif

// Array idx chk test.
//#define ARRAYS_DEBUG

#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <string>
#include <ctime>
#include <cstdlib>
#include <memory> // for smart pointer.

#include <regex> //
using namespace std;

#include <conio.h>


#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>

#include <wiz/load_data.h>
#include <wiz/stacks.h>
#include <wiz/cpp_string.h>

#include <Windows.h>

//


inline int GETCH() /// To Do - Replace..
{
	return _getch();
}

inline void FFLUSH()  // to  DO - √ﬂ∞°!!
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
				cout << "°‹";
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
					cout << "°‹";
					setcolor(0, 0);
				}
				else {
					gotoxy(0, idx - Start);
					setcolor(0, 0);
					cout << "  ";
					idx--;

					gotoxy(0, idx - Start);
					setcolor(0, 12);
					cout << "°‹";
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
					cout << "°‹";
					setcolor(0, 0);
				}
				else {
					gotoxy(0, idx - Start);
					setcolor(0, 0);
					cout << "  ";
					idx++;

					gotoxy(0, idx - Start);
					setcolor(0, 12);
					cout << "°‹";
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
					cout << "°‹";
					setcolor(0, 0);
				}
				else {
					gotoxy(0, idx - Start);
					setcolor(0, 0);
					cout << "  ";
					idx--;

					gotoxy(0, idx - Start);
					setcolor(0, 12);
					cout << "°‹";
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
					cout << "°‹";
					setcolor(0, 0);
				}
				else {
					gotoxy(0, idx - Start);
					setcolor(0, 0);
					cout << "  ";
					idx++;

					gotoxy(0, idx - Start);
					setcolor(0, 12);
					cout << "°‹";
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
						utVec2.push_back(nullptr);
					}

					wiz::load_data::ItemType< wiz::load_data::UserType*> ref;
					ref.Push(nullptr);
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
						cout << "°‹";
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
							// var, val /// stateø° µ˚∂Û?
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
								count_userType = 0; // int ~ ?
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
									cout << "fail to add" << endl; /// To Do to following code.
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

string excute_module(wiz::load_data::UserType* _global, wiz::load_data::UserType* pEvents = nullptr, 
	const std::shared_ptr<wiz::load_data::EventInfo>& pInfo = nullptr)
{
	wiz::load_data::UserType& global = *_global;
	vector<thread*> waits;
	map<string, string> objectMap;
	map<string, wiz::load_data::UserType> moduleMap;
	string module_value = "";
	// data, event load..
	wiz::ArrayStack<wiz::load_data::EventInfo> eventStack;
	map<string, int> convert;
	vector<wiz::load_data::UserType*> _events;
	wiz::load_data::UserType events;
	wiz::load_data::UserType Main;

	if (nullptr == pEvents) {
		_events = global.GetCopyUserTypeItem("Event");
		for (int i = 0; i < _events.size(); ++i) {
			events.LinkUserType(_events[i]);
		}
		global.RemoveUserTypeList("Event");
	}
	else {
		events = *pEvents;
	}
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
	if (nullptr == pInfo) {
		if (global.GetUserTypeItem("Main").empty())
		{
			cout << "do not exist Main" << endl;
			return "ERROR -1";
		}

		auto _Main = global.GetCopyUserTypeItem("Main")[0];

		Main.LinkUserType(_Main);
		global.RemoveUserTypeList("Main");

		wiz::load_data::EventInfo info;
		info.eventUT = Main.GetUserTypeList(0);
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
	else {
		eventStack.push(*pInfo);
	}

	// main loop
	while (!eventStack.empty())
	{
		wiz::load_data::EventInfo info = eventStack.top();
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
			wiz::load_data::UserType* val = nullptr;
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
					val = nullptr;
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
						val = nullptr;
					}
				}
			}

			while (val != nullptr)
			{
				if ("$do" == val->GetName()) { // chk?
					wiz::load_data::UserType subGlobal;
					wiz::load_data::LoadData::LoadDataFromString(val->GetUserTypeList(1)->ToString(), subGlobal);
					wiz::load_data::UserType inputUT;
					wiz::load_data::LoadData::LoadDataFromString(ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap), inputUT);


					//ChkBool4(&inputUT, global, eventStack.top().parameters, eventStack.top(), objectMap);

					wiz::load_data::LoadData::AddData(subGlobal, "/./", inputUT.ToString(), "TRUE");


					eventStack.top().return_value = excute_module(&subGlobal); // return ?


					eventStack.top().userType_idx.top()++;
					break;
				}
				// add option! for "".support eu3, eu4.
				else if ("$replace_datatype1" == val->GetName()) { // name
					string rex = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					rex = rex.substr(1, rex.size() - 2);
					string sval = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);

					string scondition = "TRUE";
					string start_dir = "root";

					if (val->GetUserTypeListSize() >= 3)
					{
						scondition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top(), objectMap);
					}
					if (val->GetUserTypeListSize() >= 4) {
						start_dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(3)->ToString(), eventStack.top(), objectMap);
					}

					wiz::load_data::LoadData::ReplaceDataType1(global, rex, sval, scondition, start_dir, objectMap);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$replace_datatype1_2" == val->GetName()) { //val
					string rex = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					rex = rex.substr(1, rex.size() - 2);
					string sval = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);

					string scondition = "TRUE";
					string start_dir = "root";

					if (val->GetUserTypeListSize() >= 3)
					{
						scondition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top(), objectMap);
					}
					if (val->GetUserTypeListSize() >= 4) {
						start_dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(3)->ToString(), eventStack.top(), objectMap);
					}

					wiz::load_data::LoadData::ReplaceDataType1_2(global, rex, sval, scondition, start_dir, objectMap);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$replace_datatype2" == val->GetName()) { // usertype name
					string rex = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					rex = rex.substr(1, rex.size() - 2);
					//cout << val->GetUserTypeList(1)->ToString() << endl;
					string sval = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);

					string scondition = "TRUE";
					string start_dir = "root";

					if (val->GetUserTypeListSize() >= 3)
					{
						scondition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top(), objectMap);
					}
					if (val->GetUserTypeListSize() >= 4) {
						start_dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(3)->ToString(), eventStack.top(), objectMap);
					}

					//cout << scondition << endl;
					//cout << "sval " << sval << endl;
					wiz::load_data::LoadData::ReplaceDataType2(global, rex, sval, scondition, start_dir, objectMap);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$replace_datetype" == val->GetName()) {
					string sval = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					
					string scondition = "TRUE";
					string start_dir = "root";

					if (val->GetUserTypeListSize() >= 2)
					{
						scondition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);
					}
					if (val->GetUserTypeListSize() >= 3) {
						start_dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top(), objectMap);
					}

					wiz::load_data::LoadData::ReplaceDateType(global, sval, scondition, start_dir);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$replace_datetype2" == val->GetName()) {
					string sval = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					string scondition = "TRUE";
					string start_dir = "root";

					
					if (val->GetUserTypeListSize() >= 2)
					{
						scondition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);
					}
					if (val->GetUserTypeListSize() >= 3) {
						start_dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top(), objectMap);
					}

					wiz::load_data::LoadData::ReplaceDateType2(global, sval, scondition, start_dir);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$remove_usertype_total" == val->GetName()) {
					string ut_name = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					string condition = "TRUE";
					string start_dir = "root";

					if (val->GetUserTypeListSize() >= 2)
					{
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);
					}
					if (val->GetUserTypeListSize() >= 3) {
						start_dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top(), objectMap);
					}

					wiz::load_data::LoadData::RemoveUserTypeTotal(global, ut_name, condition, start_dir);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$replace_item" == val->GetName()) {
					string svar = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					string sval = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);
					string scondition = "TRUE";
					string start_dir = "root";

					if (val->GetUserTypeListSize() >= 3)
					{
						scondition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top(), objectMap);
					}
					if (val->GetUserTypeListSize() >= 4) {
						start_dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(3)->ToString(), eventStack.top(), objectMap);
					}

					wiz::load_data::LoadData::ReplaceItem(global, svar, sval, scondition, start_dir);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$edit_mode" == val->GetName())
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
					string fileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					string option = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);

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
					string fileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					string option = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);

					wiz::load_data::LoadData::SaveWizDB(global, fileName, option, "");

					eventStack.top().userType_idx.top()++;
					break;
				}

				else if ("$save_data_only2" == val->GetName())
				{
					//todo
					// "filename" save_option(0~2)
					string dirName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					string fileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					string option = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top(), objectMap);

					// todo - for? auto x = global.GetUserTypeItem(dirName);
					wiz::load_data::UserType* utTemp = global.GetUserTypeItem(dirName)[0];
					wiz::load_data::LoadData::SaveWizDB(*utTemp, fileName, option, "");


					eventStack.top().userType_idx.top()++;
					break;
				}
				/// module name or object name -> must "~" .
				else if ("$register_module" == val->GetName())
				{
					string moduleFileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
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
					string moduleFileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					string input;

					if (val->GetUserTypeListSize() >= 2) {
						input = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);
					}

					moduleFileName = wiz::String::substring(moduleFileName, 1, moduleFileName.size() - 2);

					wiz::load_data::UserType moduleUT = moduleMap.at(moduleFileName);
					wiz::load_data::LoadData::AddData(moduleUT, "", input, "TRUE");

					eventStack.top().return_value = excute_module(&moduleUT);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$module" == val->GetName())
				{
					string moduleFileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					string input;

					if (val->GetUserTypeListSize() >= 2) {
						input = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);
					}

					moduleFileName = wiz::String::substring(moduleFileName, 1, moduleFileName.size() - 2);

					wiz::load_data::UserType moduleUT;
					wiz::load_data::LoadData::LoadDataFromFile(moduleFileName, moduleUT);
					wiz::load_data::LoadData::AddData(moduleUT, "", input, "TRUE");

					eventStack.top().return_value = excute_module(&moduleUT);

					eventStack.top().userType_idx.top()++;
					break;
				}
				// todo - register module from file
				// todo	- register module from string
				// todo - call registered module.  $registered_module = { name = { ~ } input = { input = { n = 1 } } }

				else if ("$register_object" == val->GetName()) {
					string objectFileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					
					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);

					wiz::load_data::UserType objectUT;
					wiz::load_data::LoadData::LoadDataFromFile(objectFileName, objectUT);

					string data = objectUT.ToString();

					//objectMap.insert(make_pair(objectFileName, data));
					objectMap[objectFileName] = move(data);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$register_object_from_string" == val->GetName()) {
					string objectName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					string objectData = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);
					objectName = wiz::String::substring(objectName, 1, objectName.size() - 2);
					objectData = wiz::String::substring(objectData, 1, objectData.size() - 2);
					wiz::load_data::UserType objectUT;
					wiz::load_data::LoadData::LoadDataFromString(objectData, objectUT); // error chk?

					string data = objectUT.ToString();

					//objectMap.insert(make_pair(objectFileName, data));
					objectMap[objectName] = move(data);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$call_registered_object" == val->GetName()) {
					string objectFileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);
					string parameter;
					string data = objectMap.at(objectFileName);

					parameter = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);

					data = data + " Main = { $call = { id = 0 } } Event = { id = 0 $call = { " + parameter + "  } } ";

					wiz::load_data::UserType objectUT;
					wiz::load_data::LoadData::LoadDataFromString(data, objectUT);

					eventStack.top().return_value = excute_module(&objectUT);

					eventStack.top().userType_idx.top()++;
					break;
				}
				/// object of class?
				else if ("$object" == val->GetName()) { // "fileName"
					string objectFileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);
					string parameter;

					parameter = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);


					wiz::load_data::UserType objectUT;
					wiz::load_data::LoadData::LoadDataFromFile(objectFileName, objectUT);

					string data = objectUT.ToString();

					data = data + " Main = { $call = { id = 0 } } Event = { id = 0 $call = { " + parameter + "  } } ";

					objectUT.Remove();
					wiz::load_data::LoadData::LoadDataFromString(data, objectUT);

					eventStack.top().return_value = excute_module(&objectUT);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$object_from_string" == val->GetName()) {
					string data = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					string parameter;

					parameter = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);


					wiz::load_data::UserType objectUT;
					wiz::load_data::LoadData::LoadDataFromString(data, objectUT);

					string str = objectUT.ToString();

					str = str + " Main = { $call = { id = 0 } } Event = { id = 0 $call = { " + parameter + "  } } ";

					objectUT.Remove();
					wiz::load_data::LoadData::LoadDataFromString(str, objectUT);

					eventStack.top().return_value = excute_module(&objectUT);

					eventStack.top().userType_idx.top()++;
					break;
				}
				// todo - register object from file.
				//		~.ToString() + "Main = { $call = { id = 0 } } Event = { id = 0 $call = { id = " + id_val + " " + param_name1 + " = " + param_val1 + "  } } "
				// todo - register object from string.
				// todo - call registered object.  $registered_object = { name = { "ex2.txt" } parameter = { id = 1 i = 1 j = 1 } }  
				else if ("$option" == val->GetName()) // first
				{
					eventStack.top().option = ToBool4(global, eventStack.top().parameters, val->ToString(), eventStack.top(), objectMap);

					eventStack.top().userType_idx.top()++;
					break;
				}
				// done - ($push_back-insert!) $pop_back, $push_front, $pop_front ($front?, $back?)
				else if ("$pop_back" == val->GetName()) {
					string dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					wiz::load_data::UserType* ut = nullptr;
					auto finded = wiz::load_data::UserType::Find(&global, dir);
					ut = finded.second[0];

					string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 2) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);
					}
					wiz::load_data::LoadData::Remove(global, dir, ut->GetIListSize() - 1, condition);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$push_front" == val->GetName()) {
					string value = val->GetUserTypeList(1)->ToString();
					string dir;
					if (val->GetUserTypeList(0)->GetItemListSize() > 0) {
						dir = val->GetUserTypeList(0)->GetItemList(0).Get(0);
						dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top(), objectMap);
					}
					else ///val->Ge
					{
						dir = string(val->GetUserTypeList(0)->ToString());
						dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top(), objectMap);
					}


					value = ToBool4(global, eventStack.top().parameters, value, eventStack.top(), objectMap);

					string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 3) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top(), objectMap);
					}
					wiz::load_data::LoadData::AddDataAtFront(global, dir, value, condition);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$pop_front" == val->GetName()) {
					string dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);

					string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 2) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);
					}
					wiz::load_data::LoadData::Remove(global, dir, 0, condition);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$wait" == val->GetName()) {
					for (int i = 0; i < waits.size(); ++i) {
						waits[i]->join();
						delete waits[i];
					}
					waits.resize(0);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$call" == val->GetName()) {
					if (!val->GetItem("id").empty()) {
						info.id = val->GetItem("id")[0].Get(0);
					}
					else {
						info.id = ToBool4(global, eventStack.top().parameters, val->GetUserTypeItem("id")[0]->ToString(), eventStack.top(), objectMap);
					}

					info.eventUT = events.GetUserTypeList(no);
					info.userType_idx.clear();
					info.userType_idx.push(0);
					info.return_value.clear();
					info.nowUT.clear();

					wiz::load_data::EventInfo info2; //
					info2 = info;

					if (info.id != eventStack.top().id) {
						info.parameters.clear();
					}
					info.conditionStack.clear();

					//
					if (info.id != eventStack.top().id) {
						for (int j = 0; j < val->GetItemListSize(); ++j) {
							if (val->GetItemListSize() > 0) {
								string temp = ToBool4(global, info2.parameters, val->GetItemList(j).Get(0), info2, objectMap);
								info.parameters.push_back(make_pair(val->GetItemList(j).GetName(), temp));
							}
						}
						for (int j = 0; j < val->GetUserTypeListSize(); ++j) {
							if (val->GetUserTypeListSize() > 0) {
								string temp = ToBool4(global, info2.parameters, val->GetUserTypeList(j)->ToString(), info2, objectMap);
								info.parameters.push_back(make_pair(val->GetUserTypeList(j)->GetName(), temp));
							}
						}
						eventStack.top().userType_idx.top()++;
					}
					else {
						if (val->GetItemListSize() > 0) {
							for (int j = 0; j < val->GetItemListSize(); ++j) {
								string temp = ToBool4(global, info2.parameters, val->GetItemList(j).Get(0), info, objectMap);
								for (int k = 0; k < info.parameters.size(); ++k)
								{
									if (info.parameters[k].first == val->GetItemList(j).GetName())
									{
										info.parameters[k].second = temp;
									}
								}
							}
						}
						if (val->GetUserTypeListSize() > 0) {
							for (int j = 0; j < val->GetUserTypeListSize(); ++j) {
								string temp = ToBool4(global, info2.parameters, val->GetUserTypeList(j)->ToString(), info, objectMap);
								for (int k = 0; k < info.parameters.size(); ++k)
								{
									if (info.parameters[k].first == val->GetUserTypeList(j)->GetName())
									{
										info.parameters[k].second = temp;
									}
								}
							}
						}

						eventStack.top().userType_idx.top()++;

						// remove remove_now_event_stack_a?
						if (eventStack.top().option == "REMOVE_NOW_EVENT_STACK_A" || "REMOVE_IF_CALL_ONESELF_EVENT" == eventStack.top().option) //
						{
							eventStack.pop();
						}
					}

					if (false == eventStack.empty() && eventStack.top().option == "REMOVE_IF_CALL_ANY_EVENT")
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
					if (waits.size() >= 4) {
						for (int i = 0; i < waits.size(); ++i) {
							waits[i]->join();
							delete waits[i]; // chk ?
						}
						waits.resize(0);
					}

					if (false == val->GetItem("option").empty() && val->GetItem("option")[0].Get(0) == "USE_THREAD") {
						std::shared_ptr<wiz::load_data::EventInfo> pInfo( new wiz::load_data::EventInfo(info) );
						thread* A = new thread(excute_module, &global, &events, pInfo);
						
						waits.push_back(A);
					}
					else {
						eventStack.push(info);
					}

					break;
				}

				else if ("$assign" == val->GetName()) /// -> assign2?
				{
					pair<string, string> dir = Find2(&global, val->GetItemList(0).Get(0));
					string data = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);

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
					pair<string, string> dir = Find2(&global, ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap));
					string data = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);

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
					pair<string, string> dir = Find2(&global, ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap));
					string data = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);

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
				else if ("$assign_global" == val->GetName()) // Ï£ºÏùò!! dir=> dir/name ( dir= { name = val } } , @Î•??ûÏóê Î∂ôÏó¨???úÎã§. 
				{
					pair<string, string> dir = Find2(&global, ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap));
					string data = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);

					string condition;
					if (val->GetUserTypeListSize() >= 3) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top(), objectMap);
					}
					wiz::load_data::LoadData::SetData(global, dir.first, dir.second, data, condition);

					// chk local?

					eventStack.top().userType_idx.top()++;
					break;
				}
				/// cf) insert3? - any position?
				else if ("$push_back" == val->GetName() || "$insert" == val->GetName() || "$insert2" == val->GetName())
				{
					string value = val->GetUserTypeList(1)->ToString();
					string dir;
					if (val->GetUserTypeList(0)->GetItemListSize() > 0) {
						dir = val->GetUserTypeList(0)->GetItemList(0).Get(0);
						dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top(), objectMap);
					}
					else ///val->Ge
					{
						dir = string(val->GetUserTypeList(0)->ToString());
						dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top(), objectMap);
					}

					value = ToBool4(global, eventStack.top().parameters, value, eventStack.top(), objectMap);

					string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 3) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top(), objectMap);
					}
					wiz::load_data::LoadData::AddData(global, dir, value, condition);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$insert_by_idx" == val->GetName())
				{
					string value = val->GetUserTypeList(2)->ToString();
					long long idx = atoll(ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap).c_str());
					string dir;
					if (val->GetUserTypeList(0)->GetItemListSize() > 0) {
						dir = val->GetUserTypeList(0)->GetItemList(0).Get(0);
						dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top(), objectMap);
					}
					else ///val->Ge
					{
						dir = string(val->GetUserTypeList(0)->ToString());
						dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top(), objectMap);
					}

					value = ToBool4(global, eventStack.top().parameters, value, eventStack.top(), objectMap);

					string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 4) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(3)->ToString(), eventStack.top(), objectMap);
					}
					wiz::load_data::LoadData::Insert(global, dir, idx, value, condition);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$make" == val->GetName()) // To Do? - make2? or remake? 
													// cf) make empty ut??
				{
					string dir;
					bool is2 = false;
					if (val->GetItemListSize() > 0) {
						dir = val->GetItemList(0).Get(0);
						dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top(), objectMap);
					}
					else // 
					{
						dir = string(val->ToString());
						dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top(), objectMap);
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
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);
					}
					else if (false == is2 && val->GetUserTypeListSize() >= 1) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					}

					wiz::load_data::LoadData::AddUserType(global, dir, name, "", condition);


					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$findIndex" == val->GetName()) // For list : { 1 2  3 4 5 }
				{
					string dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					string value = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);

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

					dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top(), objectMap);

					string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 2) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);
					}
					wiz::load_data::LoadData::Remove(global, dir, condition);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$remove2" == val->GetName()) // remove /dir/name 
													   // if name is empty, then chk!!
				{
					string dir = string(val->GetItemList(0).Get(0).c_str()); // item -> userType
					dir = ToBool4(global, eventStack.top().parameters, dir, eventStack.top(), objectMap);
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
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);
					}
					wiz::load_data::LoadData::Remove(global, dir, name, condition);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$remove3" == val->GetName()) /// remove itemlist by idx.
				{
					string dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					string value = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);

					long long idx = atoll(value.c_str());  // long long -> int?

					string condition = "TRUE";

					if (val->GetUserTypeListSize() >= 3) {
						condition = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top(), objectMap);
					}

					wiz::load_data::LoadData::Remove(global, dir, idx, condition);
					// remove -> UserType::Find(&global, dir).second[0]->RemoveItemList(idx); /// change ?

					eventStack.top().userType_idx.top()++;
					break;
				}

				else if ("$setElement" == val->GetName())
				{
					string dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					string idx = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);
					string value = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(2)->ToString(), eventStack.top(), objectMap);

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

					value1 = ToBool4(global, eventStack.top().parameters, value1, eventStack.top(), objectMap);
					value2 = ToBool4(global, eventStack.top().parameters, value2, eventStack.top(), objectMap);
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
							string temp = ToBool4(global, eventStack.top().parameters, listName, eventStack.top(), objectMap);
							if (temp.empty()) {
								cout << "EMPTY";
							}
							else {
								cout << temp;
							}
						}
						else if (wiz::String::startsWith(listName, "/") && listName.size() > 1)
						{
							string temp = ToBool4(global, eventStack.top().parameters, listName, eventStack.top(), objectMap);
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
					// ?
					else if (val->GetUserTypeListSize() == 1
						&& val->GetUserTypeList(0)->GetItemListSize() == 0
						&& val->GetUserTypeList(0)->GetUserTypeListSize() == 1)
					{
						string temp = val->GetUserTypeList(0)->ToString();

						string name = ToBool4(global, eventStack.top().parameters,
							temp, eventStack.top(), objectMap);
						cout << name;
					}
					else
					{
						string start = val->GetUserTypeList(1)->ToString();
						string last = val->GetUserTypeList(2)->ToString();

						start = ToBool4(global, eventStack.top().parameters, start, eventStack.top(), objectMap);
						last = ToBool4(global, eventStack.top().parameters, last, eventStack.top(), objectMap);

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
					string dir = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
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
						string fileName = ToBool4(global, eventStack.top().parameters, val->GetItemList(i).Get(0), eventStack.top(), objectMap);
						fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);

						if (wiz::load_data::LoadData::LoadDataFromFile(fileName, ut)) {
							{
								int item_count = 0;
								int userType_count = 0;

								for (int i = 0; i < ut.GetIListSize(); ++i) {
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
					string fileName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					string dirName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);
					wiz::load_data::UserType* utTemp = global.GetUserTypeItem(dirName)[0];

					

					if (wiz::load_data::LoadData::LoadDataFromFile(fileName, ut)) {
						{
							int item_count = 0;
							int userType_count = 0;

							for (int i = 0; i < ut.GetIListSize(); ++i) {
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
						string temp = ToBool4(global, eventStack.top().parameters, val->ToString(), eventStack.top(), objectMap);
						/// if temp just one
						eventStack[eventStack.size() - 2].return_value = temp;
					}

					if (eventStack.size() == 1)
					{
						string temp = ToBool4(global, eventStack.top().parameters, val->ToString(), eventStack.top(), objectMap);

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
				// make sort stable.
				else if ("$sort" == val->GetName()) {
					vector<wiz::load_data::SortInfo> siVec;
					wiz::load_data::UserType* utTemp =
						wiz::load_data::UserType::Find(&global,
							ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap)).second[0];

					vector<wiz::load_data::Type*> temp;


					int item_count = 0, ut_count = 0;
					for (int i = 0; i < utTemp->GetIListSize(); ++i) {
						if (utTemp->IsItemList(i)) {
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
							ut.AddItem(siVec[i].data, static_cast<wiz::load_data::ItemType<string>*>(temp[siVec[i].idx])->Get(0));
						}
						else {
							ut.AddUserTypeItem(*(static_cast<wiz::load_data::UserType*>(temp[siVec[i].idx])));
						}
					}

					utTemp->Remove();
					
					//cf) chk? *utTemp = ut;
					wiz::load_data::LoadData::AddData(*(utTemp), "", ut.ToString(), "TRUE");


					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$sort2" == val->GetName()) { // colName -> just one! ? 
					/// condition = has just one? in one usertype!
					vector<wiz::load_data::SortInfo> siVec;
					wiz::load_data::UserType* utTemp =
						wiz::load_data::UserType::Find(&global,
							ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap)).second[0];
					const string colName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);

					vector<wiz::load_data::Type*> temp;


					int item_count = 0, ut_count = 0;
					for (int i = 0; i < utTemp->GetIListSize(); ++i) {
						if (utTemp->IsItemList(i)) {
							//
							item_count++;
						}
						else {
							temp.push_back(utTemp->GetUserTypeList(ut_count));
							if (utTemp->GetUserTypeList(ut_count)->GetItem(colName).empty())
							{
								siVec.emplace_back("", 2, ut_count);
							}
							else {
								siVec.emplace_back(utTemp->GetUserTypeList(ut_count)->GetItem(colName)[0].Get(0), 2, ut_count);
							}
							ut_count++;
						}
					}

					std::sort(siVec.begin(), siVec.end());


					wiz::load_data::UserType ut;
					for (int i = 0; i < temp.size(); ++i)
					{
						if (siVec[i].iElement == 1) {
							//
						}
						else {
							ut.AddUserTypeItem(*(static_cast<wiz::load_data::UserType*>(temp[siVec[i].idx])));
						}
					}

					utTemp->RemoveUserTypeList();

					//cf) chk? *utTemp = ut;
					wiz::load_data::LoadData::AddData(*(utTemp), "", ut.ToString(), "TRUE");


					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$sort2_dsc" == val->GetName()) { // colName -> just one! ? 
													   /// condition = has just one? in one usertype!
					vector<wiz::load_data::SortInfo2> siVec;
					wiz::load_data::UserType* utTemp =
						wiz::load_data::UserType::Find(&global,
							ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap)).second[0];
					const string colName = ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(1)->ToString(), eventStack.top(), objectMap);

					vector<wiz::load_data::Type*> temp;


					int item_count = 0, ut_count = 0;
					for (int i = 0; i < utTemp->GetIListSize(); ++i) {
						if (utTemp->IsItemList(i)) {
							//
							item_count++;
						}
						else {
							temp.push_back(utTemp->GetUserTypeList(ut_count));
							if (utTemp->GetUserTypeList(ut_count)->GetItem(colName).empty())
							{
								siVec.emplace_back("", 2, ut_count);
							}
							else {
								siVec.emplace_back(utTemp->GetUserTypeList(ut_count)->GetItem(colName)[0].Get(0), 2, ut_count);
							}
							ut_count++;
						}
					}

					std::sort(siVec.begin(), siVec.end());


					wiz::load_data::UserType ut;
					for (int i = 0; i < temp.size(); ++i)
					{
						if (siVec[i].iElement == 1) {
							//
						}
						else {
							ut.AddUserTypeItem(*(static_cast<wiz::load_data::UserType*>(temp[siVec[i].idx])));
						}
					}

					utTemp->RemoveUserTypeList();

					//cf) chk? *utTemp = ut;
					wiz::load_data::LoadData::AddData(*(utTemp), "", ut.ToString(), "TRUE");


					eventStack.top().userType_idx.top()++;
					break;
				}
				// removal?
				else if ("$stable_sort" == val->GetName()) {
					// todo
					// todo
					vector<wiz::load_data::SortInfo> siVec;
					wiz::load_data::UserType* utTemp =
						wiz::load_data::UserType::Find(&global,
							ToBool4(global, eventStack.top().parameters, val->GetUserTypeList(0)->ToString(), eventStack.top(), objectMap)).second[0];

					vector<wiz::load_data::Type*> temp;


					int item_count = 0, ut_count = 0;
					for (int i = 0; i < utTemp->GetIListSize(); ++i) {
						if (utTemp->IsItemList(i)) {
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
							ut.AddItem(siVec[i].data, static_cast<wiz::load_data::ItemType<string>*>(temp[siVec[i].idx])->Get(0));
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
					temp = ToBool4(global, eventStack.top().parameters, temp, eventStack.top(), objectMap);

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
				else { //
					cout << "it does not work. : " << val->GetName() << endl;

					eventStack.top().userType_idx.top()++;
					break;
				}
			}
		}
	}

	for (int i = 0; i < waits.size(); ++i) {
		waits[i]->join();
		delete waits[i];
	}
	waits.resize(0);

	return module_value;
}


int main(int argc, char* argv[])
{
	srand(time(nullptr)); // 

	string fileName;


	if (argc == 1) {
		cout << "FileName: ";
		getline(cin, fileName);
		//fileName = "test 6.txt";
	}
	else
	{
		fileName = string(argv[1]);
	}
	wiz::load_data::UserType global;
	try {
		
		wiz::load_data::LoadData::LoadDataFromFile(fileName, global);

		cout << "fileName is " << fileName << endl;
		cout << "excute result is " << excute_module(&global) << endl;
		//_getch(); // pause..
	}
	catch (const char* str) {
		cout << str << endl;
	}
	catch (const string& str) {
		cout << str << endl;
	}
	catch (const wiz::Error& e) {
		cout << e << endl;
	}
	//catch (...) {
	//	cout << "Error.." << endl;
	//}

   	return 0;
}

