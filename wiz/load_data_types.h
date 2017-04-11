
#ifndef LOAD_DATA_TYPES_H
#define LOAD_DATA_TYPES_H

#include <string>
#include <vector>
#include <list>
#include <regex>
using namespace std;

#include <wiz/newArrays.h>
#include <wiz/load_data_utility.h>
#include <wiz/queues.h>
#include <wiz/cpp_string.h>

namespace wiz {
	namespace load_data {

		class Type {
		private:
			string name;

			void chk() {
				if ("_" == name || (String::startsWith(name, "$ut") && name.size() > 3)|| (String::startsWith(name, "$it")&&name.size() > 3)) {
					cout << "name is " << name << endl;
					cout << "in funciton chk() in Type" << endl;
					_getch();
					throw "ERROR for name in Type";
				}
			}
		public:
			explicit Type(const string& name = "", const bool valid = true) : name(name) { chk();  }
			explicit Type(string&& name, const bool valid = true) : name(move(name)) { chk(); }
			Type(const Type& type)
				: name(type.name)
			{
				chk();
			}
			virtual ~Type() { }
			bool IsFail() const { // change body
				return "" == name;
			}
			const string& GetName()const {
				return name;
			}
			void SetName(const string& name)
			{
				this->name = name;

				chk();
			}
			void SetName(string&& name)
			{
				this->name = name;

				chk();
			}
			bool operator==(const Type& t) const {
				return name == t.name;
			}
			bool operator<(const Type& t) const {
				return name < t.name;
			}
			Type& operator=(const Type& type)
			{
				name = type.name;
				return *this;
			}
			void operator=(Type&& type)
			{
				name = std::move(type.name);
				type.name.clear();
			}
		};

		template < class T >
		class ItemType : public Type {
		public:
			typedef T item_type; //
		private:
			//std::vector<T> arr;
			T data;
			bool inited;
		public:
			ItemType(const ItemType<T>& ta) : Type(ta), data(ta.data), inited(ta.inited)
			{

			}
			ItemType(ItemType<T>&& ta) : Type(move(ta))
			{
				data = move(ta.data);
				inited = ta.inited;
			}
		public:
			explicit ItemType()
				: Type("", true), inited(false) { }
			explicit ItemType(const string& name, const string& value, const bool valid=true)
				:Type(name, valid), data(value), inited(true)
			{

			}
			explicit ItemType(string&& name, string&& value, const bool valid = true) 
				:Type(move(name), valid), data(move(value)), inited(true)
			{

			}
			virtual ~ItemType() { }
		public:
			void Remove(const int idx = 0)
			{
				data = T();
			}
			bool Push(const T& val) { /// do not change..!!
				if (inited) { throw "ItemType already inited"; }
				data = val;
				inited = true;

				return true;
			}
			bool Push(T&& val) {
				if (inited) { throw "ItemType already inited"; }
				data = val;
				inited = true;

				return true;
			}
			T& Get(const int index) {
				if (!inited) { throw "ItemType, not inited";  }
				return data;
			}
			const T& Get(const int index) const {
				if (!inited) { throw "ItemType, not inited"; }
				return data;
			}
			void Set(const int index, const T& val) {
				if (!inited) { throw "ItemType, not inited"; } // removal?
				data = val;
			}
			void Set(const int index, const T&& val) {
				if (!inited) { throw "ItemType, not inited"; } // removal?
				data = val;
			}
			int size()const {
				return inited? 1 : 0;
			}
			bool empty()const { return !inited; }
			string ToString()const
			{
				if (Type::GetName().empty()) {
					return Get(0);
				}
				return Type::GetName() + " = " + Get(0);
			}
		public:
			ItemType<T>& operator=(const ItemType<T>& ta)
			{
				Type::operator=(ta);
				ItemType<T> temp = ta;

				data = std::move(temp.data);
				inited = temp.inited;
				return *this;
			}
			ItemType<T>& operator=(ItemType<T>&& ta)
			{
				Type::operator=(ta);
				if (data == ta.data) { return *this; }

				data = std::move(ta.data);
				inited = ta.inited;
				return *this;
			}
		};

		class UserType : public Type {
		public:
			void PushComment(const string& comment)
			{
				commentList.push_back(comment);
			}
			void PushComment(string&& comment)
			{
				commentList.push_back(move(comment));
			}
			int GetCommentListSize()const { return commentList.size(); }
			const string& GetCommentList(const int idx) const { return commentList[idx]; }
			string& GetCommentList(const int idx) {
				return commentList[idx];
			}
		public:
			int GetIListSize()const { return ilist.size(); }
			int GetItemListSize()const { return itemList.size(); }
			int GetUserTypeListSize()const { return userTypeList.size(); }
			ItemType<string>& GetItemList(const int idx) { return itemList[idx]; }
			const ItemType<string>& GetItemList(const int idx) const { return itemList[idx]; }
			UserType* GetUserTypeList(const int idx) { return userTypeList[idx]; }
			const UserType* GetUserTypeList(const int idx) const { return const_cast<const UserType*&>(userTypeList[idx]); }

			bool IsItemList(const int idx) const
			{
				return ilist[idx] == 1;
			}
			bool IsUserTypeList(const int idx) const
			{
				return ilist[idx] == 2;
			}

			void AddItemList(const ItemType<string>& strTa)
			{
				for (int i = 0; i < strTa.size(); ++i) {
					this->AddItem(strTa.GetName(), strTa.Get(i));
				}
			}
		public:
			UserType* GetParent() { return parent; }
			const UserType* GetParent()const { return parent; }

			void LinkUserType(UserType* ut)
			{
				userTypeList.push_back(ut);
				ilist.push_back(2);
				ut->parent = this;
			}
		private:
			UserType* parent;
			std::vector<string> commentList;
			std::vector<int> ilist;
			std::vector< ItemType<string> > itemList;
			std::vector< UserType* > userTypeList;
			bool noRemove = false;
			bool reservedA = false;
			//	bool userTypeList_sortFlagA; // A : sorted < , B ; sorted > , false : not sorted!
			//	bool userTypeList_sortFlagB;
		public:
			explicit UserType(string&& name, bool noRemove = false) : Type(move(name)), parent(nullptr), noRemove(noRemove) { }
			explicit UserType(const string& name = "", bool noRemove = false) : Type(name), parent(nullptr), noRemove(noRemove) { } //, userTypeList_sortFlagA(true), userTypeList_sortFlagB(true) { }
			UserType(const UserType& ut) : Type(ut.GetName()) {
				Reset(ut);  // Initial
			}
			UserType(UserType&& ut) : Type(move(ut.GetName())) {
				Reset2(std::move(ut));
			}
			virtual ~UserType() {
				if (false == noRemove) {
					_Remove();
				}
			}
			UserType& operator=(const UserType& ut) {
				if (this == &ut) { return *this; }
				Type::operator=(ut);

				RemoveUserTypeList();
				Reset(ut);
				return *this;
			}
			UserType& operator=(UserType&& ut) {
				if (this == &ut) { return *this; }

				Type::operator=(std::move(ut));
				RemoveUserTypeList();
				Reset2(std::move(ut));
				return *this;
			}
		private:
			void Reset(const UserType& ut) { /// UT ��ü�� �����Ѵ�.
											 //	userTypeList_sortFlagA = ut.userTypeList_sortFlagA;
											 //userTypeList_sortFlagB = ut.userTypeList_sortFlagB;

				ilist = ut.ilist;
				itemList = ut.itemList;
				parent = ut.parent;
				commentList = ut.commentList;

				for (int i = 0; i < ut.userTypeList.size(); i++) {
					userTypeList.push_back(new UserType(*ut.userTypeList[i]));
				}
			}
			void Reset2(UserType&& ut) {
				//std::swap( userTypeList_sortFlagA, ut.userTypeList_sortFlagA );
				//std::swap( userTypeList_sortFlagB, ut.userTypeList_sortFlagB );

				parent = ut.parent;
				ut.parent = nullptr; /// chk..
				ilist = std::move(ut.ilist);
				itemList = std::move(ut.itemList);
				commentList = std::move(ut.commentList);

				for (int i = 0; i < ut.userTypeList.size(); ++i) {
					userTypeList.push_back(move(ut.userTypeList[i]));
					ut.userTypeList[i] = nullptr;
				}
				ut.userTypeList.clear();
			}

			void _Remove()
			{
				ilist = vector<int>();
				itemList = vector< ItemType<string> >();
				RemoveUserTypeList();

				commentList.clear();
			}

		private:
			/// val : 1 or 2
			int _GetIndex(const vector<int>& ilist, const int val, const int start = 0) {
				for (int i = start; i < ilist.size(); ++i) {
					if (ilist[i] == val) { return i; }
				}
				return -1;
			}
		// test? - need more thinking!
			int _GetItemIndexFromIlistIndex(const vector<int>& ilist, const int ilist_idx) {
				if (ilist.size() == ilist_idx) { return ilist.size(); }
				int idx = _GetIndex(ilist, 1, 0);
				int item_idx = -1;

				while (idx != -1) {
					item_idx++;
					if (ilist_idx == idx) { return item_idx; }
					idx = _GetIndex(ilist, 1, idx + 1);
				}

				return -1;
			}
			int _GetUserTypeIndexFromIlistIndex(const vector<int>& ilist, const int ilist_idx) {
				if (ilist.size() == ilist_idx) { return ilist.size(); }
				int idx = _GetIndex(ilist, 2, 0);
				int usertype_idx = -1;

				while (idx != -1) {
					usertype_idx++;
					if (ilist_idx == idx) { return usertype_idx; }
					idx = _GetIndex(ilist, 2, idx + 1);
				}

				return -1;
			}
			/// type : 1 or 2
			int _GetIlistIndex(const vector<int>& ilist, const int index, const int type) {
				int count = -1;

				for (int i = 0; i < ilist.size(); ++i) {
					if (ilist[i] == type) {
						count++;
						if (index == count) {
							return i;
						}
					}
				}
				return -1;
			}
		public:
			void RemoveItemList(const int idx)
			{
				// left shift start idx, to end, at itemList. and resize!
				for (int i = idx + 1; i < GetItemListSize(); ++i) {
					itemList[i - 1] = std::move(itemList[i]);
				}
				itemList.resize(itemList.size() - 1);
				//  ilist left shift and resize - count itemType!
				int count = 0;
				for (int i = 0; i < ilist.size(); ++i) {
					if (ilist[i] == 1) { count++; }
					if (count == idx + 1) {
						// i���� left shift!and resize!
						for (int k = i + 1; k < ilist.size(); ++k) {
							ilist[k - 1] = std::move(ilist[k]);
						}
						ilist.resize(ilist.size() - 1);
						break;
					}
				}
			}
			void RemoveUserTypeList(const int idx, const bool chk = true)
			{
				if (chk) {
					delete userTypeList[idx];
				}
				// left shift start idx, to end, at itemList. and resize!
				for (int i = idx + 1; i < GetUserTypeListSize(); ++i) {
					userTypeList[i - 1] = std::move(userTypeList[i]);
				}
				userTypeList.resize(userTypeList.size() - 1);
				//  ilist left shift and resize - count itemType!
				int count = 0;
				for (int i = 0; i < ilist.size(); ++i) {
					if (ilist[i] == 2) { count++; }
					if (count == idx + 1) {
						// i���� left shift!and resize!
						for (int k = i + 1; k < ilist.size(); ++k) {
							ilist[k - 1] = std::move(ilist[k]);
						}
						ilist.resize(ilist.size() - 1);
						break;
					}
				}
			}
			void RemoveItemList(const string& varName)
			{
				int k = _GetIndex(ilist, 1, 0); 
				vector<ItemType<string>> tempDic;
				for (int i = 0; i < itemList.size(); ++i) {
					if (varName != itemList[i].GetName()) {
						tempDic.push_back(itemList[i]);
						k = _GetIndex(ilist, 1, k + 1);
					}
					else {
						// remove item, ilist left shift 1.
						for (int j = k + 1; j < ilist.size(); ++j) {
							ilist[j - 1] = ilist[j];
						}
						ilist.resize(ilist.size() - 1);
						k = _GetIndex(ilist, 1, k);
					}
				}
				itemList = (tempDic);
			}
			void RemoveItemList() /// ALL
			{
				itemList = vector<ItemType<string>>();
				//
				vector<int> temp;
				for (int i = 0; i < ilist.size(); ++i) {
					if (ilist[i] == 2)
					{
						temp.push_back(2);
					}
				}
				ilist = move(temp);
			}
			void RemoveEmptyItem() // fixed..
			{
				int k = _GetIndex(ilist, 1, 0);
				vector<ItemType<string>> tempDic;
				for (int i = 0; i < itemList.size(); ++i) {
					if (itemList[i].size() > 0) {
						tempDic.push_back(itemList[i]);
						k = _GetIndex(ilist, 1, k + 1);
					}
					else {
						// remove item, ilist left shift 1.
						for (int j = k + 1; j < ilist.size(); ++j) {
							ilist[j - 1] = ilist[j];
						}
						ilist.resize(ilist.size() - 1);
						k = _GetIndex(ilist, 1, k);
					}
				}
				itemList = move(tempDic);
			}
			void Remove()
			{
				/// parent->removeUserType(name); - ToDo - X
				ilist = vector<int>();
				itemList = vector< ItemType<string> >();

				RemoveUserTypeList();

				commentList.clear();
			}
			void RemoveUserTypeList() { /// chk memory leak test!!
				for (int i = 0; i < userTypeList.size(); i++) {
					if (nullptr != userTypeList[i]) {
						delete userTypeList[i]; //
						userTypeList[i] = nullptr;
					}
				}
				// DO Empty..
				userTypeList = vector< UserType* >();

				vector<int> temp;
				for (int i = 0; i < ilist.size(); ++i) {
					if (ilist[i] == 1)
					{
						temp.push_back(1);
					}
				}
				ilist = move(temp);
			}
			void RemoveUserTypeList(const string& varName, const bool chk = true)
			{
				int k = _GetIndex(ilist, 2, 0);
				vector<UserType*> tempDic;
				for (int i = 0; i < userTypeList.size(); ++i) {
					if (varName != userTypeList[i]->GetName()) {
						tempDic.push_back(userTypeList[i]);
						k = _GetIndex(ilist, 2, k + 1);
					}
					else {
						if (chk) {
							delete userTypeList[i];
						}
						// remove usertypeitem, ilist left shift 1.
						for (int j = k + 1; j < ilist.size(); ++j) {
							ilist[j - 1] = ilist[j];
						}
						ilist.resize(ilist.size() - 1);
						k = _GetIndex(ilist, 2, k);
					}
				}
				userTypeList = move(tempDic);
			}
		//			
			void RemoveList(const int idx) // ilist_idx!
			{
				// chk whether item or usertype.
				// find item_idx or usertype_idx.
				// remove item or remove usertype.
				if (ilist[idx] == 1) {
					int item_idx = -1;

					for (int i = 0; i < ilist.size() && i <= idx; ++i) {
						if (ilist[i] == 1) { item_idx++; }
					}

					RemoveItemList(item_idx);
				}
				else {
					int usertype_idx = -1;

					for (int i = 0; i < ilist.size() && i <= idx; ++i) {
						if (ilist[i] == 2) { usertype_idx++; }
					}

					RemoveUserTypeList(usertype_idx);
				}
			}
		public:
			bool empty()const { return ilist.empty(); }
			
			// chk
			void InsertItemByIlist(const int ilist_idx, const string& name, const string& item ) {
				ilist.push_back(1);
				for (int i = ilist.size()-1; i > ilist_idx; --i) {
					ilist[i] = ilist[i - 1];
				}
				ilist[ilist_idx] = 1;


				int itemIndex = _GetItemIndexFromIlistIndex(ilist, ilist_idx);

				itemList.emplace_back("", "");

				if (itemIndex != -1) {
					for (int i = itemList.size() - 1; i > itemIndex; --i) {
						itemList[i] = move(itemList[i - 1]);
					}
					itemList[itemIndex] = ItemType<string>(name, item);
				}
				else {
					itemList[0] = ItemType<string>(name, item); // chk!!
				}
				
			}
			void InsertItemByIlist(const int ilist_idx, string&& name, string&& item) {
				ilist.push_back(1);

				
				for (int i = ilist.size() - 1; i > ilist_idx; --i) {
					ilist[i] = ilist[i - 1];
				}
				ilist[ilist_idx] = 1;

				int itemIndex = _GetItemIndexFromIlistIndex(ilist, ilist_idx);

				itemList.emplace_back("", "");
				if (itemIndex != -1) {
					for (int i = itemList.size() - 1; i > itemIndex; --i) {
						itemList[i] = move(itemList[i - 1]);
					}
					itemList[itemIndex] = ItemType<string>(move(name), move(item));
				}
				else {
					itemList[0] = ItemType<string>(move(name), move(item));
				}

				
			}
			// chk
			void InsertUserTypeByIlist(const int ilist_idx, UserType&& item) {
				ilist.push_back(2);
				UserType* temp = new UserType(std::move(item));

				temp->parent = this;

				for (int i = ilist.size() - 1; i > ilist_idx; --i) {
					ilist[i] = ilist[i - 1];
				}
				ilist[ilist_idx] = 2;

				int userTypeIndex = _GetUserTypeIndexFromIlistIndex(ilist, ilist_idx);
				userTypeList.push_back(nullptr);
				if (userTypeIndex != -1) {
					for (int i = userTypeList.size() - 1; i > userTypeIndex; --i) {
						userTypeList[i] = move(userTypeList[i - 1]);
					}
					userTypeList[userTypeIndex] = temp;
				}
				else {
					userTypeList[0] = temp;
				}
				
			}
			void InsertUserTypeByIlist(const int ilist_idx, const UserType& item) {
				ilist.push_back(2);
				UserType* temp = new UserType(item);

				temp->parent = this;

				
				for (int i = ilist.size() - 1; i > ilist_idx; --i) {
					ilist[i] = ilist[i - 1];
				}
				ilist[ilist_idx] = 2;

				int userTypeIndex = _GetUserTypeIndexFromIlistIndex(ilist, ilist_idx);
				userTypeList.push_back(nullptr);
				if (userTypeIndex != -1) {
					for (int i = userTypeList.size() - 1; i > userTypeIndex; --i) {
						userTypeList[i] = move(userTypeList[i - 1]);
					}
					userTypeList[userTypeIndex] = temp;
				}
				else {
					userTypeList[0] = temp;
				}
			}
			
			/*
			// chk
			void InsertItem(const int item_idx, const string& name, const string& item) {
				int ilist_idx = _GetIlistIndex(ilist, item_idx, 1);

				ilist.push_back(0);
				for (int i = ilist_idx + 1; i < ilist.size(); ++i) {
					ilist[i] = ilist[i - 1];
				}
				ilist[ilist_idx] = 1;

				itemList.emplace_back("", "");
				for (int i = item_idx + 1; i < itemList.size(); ++i) {
					itemList[i] = move(itemList[i - 1]);
				}
				itemList[item_idx] = ItemType<string>(name, item);
			}
			void InsertItem(const int item_idx, string&& name, string&& item) {
				int ilist_idx = _GetIlistIndex(ilist, item_idx, 1);

				ilist.push_back(0);
				for (int i = ilist_idx + 1; i < ilist.size(); ++i) {
					ilist[i] = ilist[i - 1];
				}
				ilist[ilist_idx] = 1;

				itemList.emplace_back("", "");
				for (int i = item_idx + 1; i < itemList.size(); ++i) {
					itemList[i] = move(itemList[i - 1]);
				}
				itemList[item_idx] = ItemType<string>(move(name), move(item));
			}
			// chk
			void InsertUserType(const int ut_idx, UserType&& item) {
				int ilist_idx = _GetIlistIndex(ilist, ut_idx, 2);
				UserType* temp = new UserType(std::move(item));

				temp->parent = this;

				ilist.push_back(0);
				for (int i = ilist_idx + 1; i < ilist.size(); ++i) {
					ilist[i] = ilist[i - 1];
				}
				ilist[ilist_idx] = 2;

				userTypeList.push_back(nullptr);
				for (int i = ut_idx + 1; i < userTypeList.size(); ++i) {
					userTypeList[i] = userTypeList[i - 1];
				}
				userTypeList[ut_idx] = temp;
			}
			void InsertUserType(const int ut_idx, const UserType& item) {
				int ilist_idx = _GetIlistIndex(ilist, ut_idx, 2);
				UserType* temp = new UserType(item);

				temp->parent = this;

				ilist.push_back(0);
				for (int i = ilist_idx + 1; i < ilist.size(); ++i) {
					ilist[i] = ilist[i - 1];
				}
				ilist[ilist_idx] = 2;

				userTypeList.push_back(nullptr);
				for (int i = ut_idx + 1; i < userTypeList.size(); ++i) {
					userTypeList[i] = userTypeList[i - 1];
				}
				userTypeList[ut_idx] = temp;
			}
			*/
			//
			void AddItem(string&& name, string&& item) {
				itemList.emplace_back(move(name), move(item));
				ilist.push_back(1);
			}
			void AddItem(const string& name, const string& item) {
				itemList.emplace_back(name, item);
				ilist.push_back(1);
			}
			void AddUserTypeItem(UserType&& item) {
				UserType* temp = new UserType(std::move(item));
				temp->parent = this;
				//temp->SetName("");

				ilist.push_back(2);

				userTypeList.push_back(temp);
			}
			void AddUserTypeItem(const UserType& item) {
				UserType* temp = new UserType(item);
				temp->parent = this;

				ilist.push_back(2);

				userTypeList.push_back(temp);
			}
			void AddItemAtFront(string&& name, string&& item) {
				itemList.emplace(itemList.begin(), name, item);

				ilist.insert(ilist.begin(), 1);
			}
			void AddItemAtFront(const string& name, const string& item) {
				itemList.emplace(itemList.begin(), name, item);

				ilist.insert(ilist.begin(), 1);
			}
			void AddUserTypeItemAtFront(const UserType& item) {
				UserType* temp = new UserType(item);
				temp->parent = this;

				ilist.insert(ilist.begin(), 2);

				userTypeList.insert(userTypeList.begin(), temp);
			}
			void AddUserTypeItemAtFront(UserType&& item) {
				UserType* temp = new UserType(item);
				temp->parent = this;

				ilist.insert(ilist.begin(), 2);

				userTypeList.insert(userTypeList.begin(), temp);
			}


			vector<ItemType<string>> GetItem(const string& name) const {
				vector<ItemType<string>> temp;
				if (String::startsWith(name, "$.") && name.size() >= 5) {
					string str = name.substr(3, name.size() - 4);
					regex rgx(str);

					for (int i = 0; i < itemList.size(); ++i) {
						if (regex_match(itemList[i].GetName(), rgx)) {
							temp.push_back(itemList[i]);
						}
					}
				}
				else {
					for (int i = 0; i < itemList.size(); ++i) {
						if (itemList[i].GetName() == name) {
							temp.push_back(itemList[i]);
						}
					}
				}
				return temp;
			}
			// regex to SetItem?
			bool SetItem(const string& name, const string& value) {
				int index = -1;

				for (int i = 0; i < itemList.size(); ++i) {
					if (itemList[i].GetName() == name)
					{
						itemList[i].Set(0, value);
						index = i;
					}
				}

				return -1 != index;
			}
			/// add set Data
			bool SetItem(const int var_idx, const string& value) {
				itemList[var_idx].Set(0, value);
				return true;
			}
			
			vector<UserType*> GetUserTypeItem(const string& name) const { /// chk...
				vector<UserType*> temp;

				for (int i = 0; i < userTypeList.size(); ++i) {
					if (userTypeList[i]->GetName() == name) {
						temp.push_back(userTypeList[i]);
					}
				}

				return temp;
			}

			// deep copy.
			vector<UserType*> GetCopyUserTypeItem(const string& name) const { /// chk...
				vector<UserType*> temp;

				for (int i = 0; i < userTypeList.size(); ++i) {
					if (userTypeList[i]->GetName() == name) {
						temp.push_back(new UserType(*userTypeList[i]));
					}
				}

				return temp;
			}
		public:
			bool GetUserTypeItemRef(const int idx, UserType*& ref)
			{
				ref = userTypeList[idx];
				return true;
			}
			bool GetLastUserTypeItemRef(const string& name, UserType*& ref) {
				int idx = -1;

				for (int i = userTypeList.size()-1; i >= 0; --i)
				{
					if (name == userTypeList[i]->GetName()) {
						idx = i;
						break;
					}
				}
				if (idx > -1) {
					ref = userTypeList[idx];
				}
				return idx > -1;
			}
		private:
			/// save1 - like EU4 savefiles.
			void Save1(ostream& stream, const UserType* ut, const int depth = 0) const {
				int itemListCount = 0;
				int userTypeListCount = 0;

				for (int i = 0; i < ut->commentList.size(); ++i) {
					for (int k = 0; k < depth; ++k) {
						stream << "\t";
					}
					stream << (ut->commentList[i]);

					if (i < ut->commentList.size() - 1 || false == ut->ilist.empty()) {
						stream << "\n";
					}
				}

				for (int i = 0; i < ut->ilist.size(); ++i) {
					//std::cout << "ItemList" << endl;
					if (ut->ilist[i] == 1) {
						for (int j = 0; j < ut->itemList[itemListCount].size(); j++) {
							string temp;
							for (int k = 0; k < depth; ++k) {
								temp += "\t";
							}
							if (ut->itemList[itemListCount].GetName() != "") {	
								temp += ut->itemList[itemListCount].GetName();
								temp += "=";
							}
							temp += ut->itemList[itemListCount].Get(j);
							if (j != ut->itemList[itemListCount].size() - 1) {
								temp += "\n";
							}
							stream << temp;
						}
						if (i != ut->ilist.size() - 1) {
							stream << "\n";
						}
						itemListCount++;
					}
					else if (ut->ilist[i] == 2) {
						// std::cout << "UserTypeList" << endl;
						for (int k = 0; k < depth; ++k) {
							stream << "\t";
						}

						if (ut->userTypeList[userTypeListCount]->GetName() != "") {
							stream << ut->userTypeList[userTypeListCount]->GetName() << "=";
						}

						stream << "{\n";
						
						Save1(stream, ut->userTypeList[userTypeListCount], depth + 1);
						stream << "\n";
						
						for (int k = 0; k < depth; ++k) {
							stream << "\t";
						}
						stream << "}";
						if (i != ut->ilist.size() - 1) {
							stream << "\n";
						}

						userTypeListCount++;
					}
				}
			}
			/// savw2 - for more seed loading data!
			void Save2(ostream& stream, const UserType* ut, const int depth = 0) const {
				int itemListCount = 0;
				int userTypeListCount = 0;

				for (int i = 0; i < ut->commentList.size(); ++i) {
					for (int k = 0; k < depth; ++k) {
						stream << "\t";
					}
					stream << (ut->commentList[i]);

					if (i < ut->commentList.size() - 1 || false == ut->ilist.empty()) {
						stream << "\n";
					}

				}
				for (int i = 0; i < ut->ilist.size(); ++i) {
					//std::cout << "ItemList" << endl;
					if (ut->ilist[i] == 1) {
						for (int j = 0; j < ut->itemList[itemListCount].size(); j++) {
							for (int k = 0; k < depth; ++k) {
								stream << "\t";
							}
							if (ut->itemList[itemListCount].GetName() != "")
								stream << ut->itemList[itemListCount].GetName() << " = ";
							stream << ut->itemList[itemListCount].Get(j);
							if (j != ut->itemList[itemListCount].size() - 1)
								stream << " ";
						}
						if (i != ut->ilist.size() - 1) {
							stream << "\n";
						}
						itemListCount++;
					}
					else if (ut->ilist[i] == 2) {
						// std::cout << "UserTypeList" << endl;
						if (ut->userTypeList[userTypeListCount]->GetName() != "")
						{
							stream << ut->userTypeList[userTypeListCount]->GetName() << " = ";
						}
						stream << "{\n";
						
						Save2(stream, ut->userTypeList[userTypeListCount], depth + 1);
						stream << "\n";
						
						for (int k = 0; k < depth; ++k) {
							stream << "\t";
						}
						stream << "}";
						if (i != ut->ilist.size() - 1) {
							stream << "\n";
						}
						userTypeListCount++;
					}
				}
			}
		public:
			void Save1(ostream& stream) const {
				Save1(stream, this);
			}
			void Save2(ostream& stream) const {
				Save2(stream, this);
			}
			string ItemListToString()const
			{
				string temp;
				int itemListCount = 0;

				for (int i = 0; i < itemList.size(); ++i) {
					for (int j = 0; j < itemList[itemListCount].size(); j++) {
						if (itemList[itemListCount].GetName() != "")
							temp = temp + itemList[itemListCount].GetName() + " = ";
						temp = temp + itemList[itemListCount].Get(j);
						if (j != itemList[itemListCount].size() - 1) {
							temp = temp + "/";
						}
					}
					if (i != itemList.size() - 1)
					{
						temp = temp + "/";
					}
					itemListCount++;
				}
				return temp;
			}
			string ItemListNamesToString()const
			{
				string temp;
				int itemListCount = 0;

				for (int i = 0; i < itemList.size(); ++i) {
					for (int j = 0; j < itemList[itemListCount].size(); j++) {
						if (itemList[itemListCount].GetName() != "")
							temp = temp + itemList[itemListCount].GetName();
						else
							temp = temp + " ";

						if (j != itemList[itemListCount].size() - 1) {
							temp = temp + "/";
						}
					}
					if (i != itemList.size() - 1)
					{
						temp = temp + "/";
					}
					itemListCount++;
				}
				return temp;
			}
			std::vector<string> userTypeListNamesToStringArray()const
			{
				std::vector<string> temp;
				int userTypeListCount = 0;

				for (int i = 0; i < userTypeList.size(); ++i) {
					if (userTypeList[userTypeListCount]->GetName() != "") {
						temp.push_back(userTypeList[userTypeListCount]->GetName());
					}
					else {
						temp.push_back(" "); // chk!! cf) wiz::load_data::Utility::Find function...
					}
					userTypeListCount++;
				}
				return temp;
			}
			string UserTypeListNamesToString()const
			{
				string temp;
				int userTypeListCount = 0;

				for (int i = 0; i < userTypeList.size(); ++i) {
					if (userTypeList[userTypeListCount]->GetName() != "") {
						temp = temp + userTypeList[userTypeListCount]->GetName();
					}
					else {
						temp = temp + " "; // chk!! cf) wiz::load_data::Utility::Find function...
					}

					if (i != itemList.size() - 1)
					{
						temp = temp + "/";
					}
					userTypeListCount++;
				}
				return temp;
			}
			string ToString()const
			{
				string temp;
				int itemListCount = 0;
				int userTypeListCount = 0;

				for (int i = 0; i < ilist.size(); ++i) {
					//std::cout << "ItemList" << endl;
					if (ilist[i] == 1) {
						for (int j = 0; j < itemList[itemListCount].size(); j++) {
							if (itemList[itemListCount].GetName() != "") {
								temp.append(itemList[itemListCount].GetName());
								temp.append(" = ");
							}
							temp.append(itemList[itemListCount].Get(j));
							if (j != itemList[itemListCount].size() - 1)
							{
								temp.append(" ");
							}
						}
						if (i != ilist.size() - 1) {
							temp.append(" ");
						}
						itemListCount++;
					}
					else if (ilist[i] == 2) {
						// std::cout << "UserTypeList" << endl;
						if (userTypeList[userTypeListCount]->GetName() != "") {
							temp.append(userTypeList[userTypeListCount]->GetName());
							temp.append(" = ");
						}
						temp.append( " { ");
						temp.append(userTypeList[userTypeListCount]->ToString());
						temp.append(" ");
						temp.append(" }");
						if (i != ilist.size() - 1) {
							temp.append(" ");
						}

						userTypeListCount++;
					}
				}
				return temp;
			}
		public:
			// find userType! not itemList!,// this has bug
			static std::pair<bool, vector< UserType*> > Find(UserType* global, const string& _position, StringBuilder* builder) /// option, option_offset
			{
				string position = _position;
				vector< UserType* > temp;

				if (!position.empty() && position[0] == '@') { position.erase(position.begin()); }
				if (position.empty()) { temp.push_back(global); return{ true, temp }; }
				if (position == ".") { temp.push_back(global); return{ true, temp }; }
				if (position == "/./") { temp.push_back(global); return{ true, temp }; } // chk..
				if (position == "/.") { temp.push_back(global); return{ true, temp }; }
				if (String::startsWith(position, "/."))
				{
					position = String::substring(position, 3);
				}

				StringTokenizer tokenizer(position, "/", builder, 1);
				vector<string> strVec;
				list<pair< UserType*, int >> utDeck;
				pair<UserType*, int> utTemp;
				utTemp.first = global;
				utTemp.second = 0;

				for (int i = 0; i < tokenizer.countTokens(); ++i) {
					string strTemp = tokenizer.nextToken();
					if (strTemp == "root" && i == 0) {
					}
					else {
						strVec.push_back(strTemp);
					}

					if ((strVec.size() >= 1) && (" " == strVec[strVec.size() - 1])) /// chk!!
					{
						strVec[strVec.size() - 1] = "";
					}
					else if ((strVec.size() >= 1) && ("_" == strVec[strVec.size() - 1]))
					{
						strVec[strVec.size() - 1] = "";
					}
				}

				// maybe, has bug!
				{
					int count = 0;

					for (int i = 0; i < strVec.size(); ++i) {
						if (strVec[i] == "..") {
							count++;
						}
						else {
							break;
						}
					}

					std::reverse(strVec.begin(), strVec.end());

					for (int i = 0; i < count; ++i) {
						if (utTemp.first == nullptr) {
							return{ false, vector< UserType* >() };
						}
						utTemp.first = utTemp.first->GetParent();
						strVec.pop_back();
					}
					std::reverse(strVec.begin(), strVec.end());
				}

				utDeck.push_front(utTemp);

				bool exist = false;
				while (false == utDeck.empty()) {
					utTemp = utDeck.front();
					utDeck.pop_front();
					
					if (utTemp.second < strVec.size() &&
						wiz::String::startsWith(strVec[utTemp.second], "$ut")
						)
					{
						int idx = std::stoi(wiz::String::substring(strVec[utTemp.second], 3));

						if (idx < 0 || idx >= utTemp.first->GetUserTypeListSize()) {
							throw string("ERROR NOT VALID IDX");
						}

						utDeck.push_front(make_pair(utTemp.first->GetUserTypeList(idx), utTemp.second + 1));
					}
					else if (utTemp.second < strVec.size() && strVec[utTemp.second] == "$")
					{
						for (int j = utTemp.first->GetUserTypeListSize() - 1; j >= 0; --j) {
							UserType* x = utTemp.first->GetUserTypeList(j);
							utDeck.push_front(make_pair(x, utTemp.second + 1));
						}
					}
					else if (utTemp.second < strVec.size() && String::startsWith(strVec[utTemp.second], "$.")) /// $."abc"
					{
						string rex_str = strVec[utTemp.second].substr(3, strVec[utTemp.second].size() - 4);
						std::regex rgx(rex_str);

						for (int j = utTemp.first->GetUserTypeListSize() - 1; j >= 0; --j) {
							if (std::regex_match(utTemp.first->GetUserTypeList(j)->GetName(), rgx)) {
								UserType* x = utTemp.first->GetUserTypeList(j);
								utDeck.push_front(make_pair(x, utTemp.second + 1));
							}
						}
					}
					else if (utTemp.second < strVec.size() &&
						(utTemp.first->GetUserTypeItem(strVec[utTemp.second]).empty() == false))
					{
						auto  x = utTemp.first->GetUserTypeItem(strVec[utTemp.second]);
						for (int j = x.size() - 1; j >= 0; --j) {
							utDeck.push_front(make_pair(x[j], utTemp.second + 1));
						}
					}

					if (utTemp.second == strVec.size()) {
						exist = true;
						temp.push_back(utTemp.first);
					}
				}
				if (false == exist) { return{ false, vector<UserType*>() }; }
				return{ true, temp };
			}	
		};
	}
}

#endif