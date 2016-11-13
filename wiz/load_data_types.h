
#ifndef LOAD_DATA_TYPES_H
#define LOAD_DATA_TYPES_H

#include <string>
#include <vector>
#include <memory>
using namespace std;

#include <wiz/newArrays.h>
//#include <wiz/dictionary.h>
//#include <wiz/binary_search.h>
#include <wiz/load_data_utility.h>

namespace wiz {
	namespace load_data {

		class Type {
		private:
			string name;
		public:
			explicit Type(const string& name = "", const bool valid = true) : name(name) { }
			Type(const Type& type)
				: name(type.name)
			{ }
			virtual ~Type() { }
			bool IsFail() const { // change body
				return "" == name;
			}
			const string& GetName()const {
				return name;
			}
			void SetName(string name)
			{
				this->name = name;
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
		private:
			//std::vector<T> arr;
			T data;
			bool inited;
		public:
			ItemType(const ItemType<T>& ta) : Type(ta)
			{
				data = ta.data;
				inited = ta.inited;
			}
			ItemType(ItemType<T>&& ta) : Type(ta)
			{
				data = move(ta.data);
				inited = ta.inited;
			}
		public:
			explicit ItemType(const string& name = "", const bool valid = true) 
				: Type(name, valid), inited(false) { }
			virtual ~ItemType() { }
		public:
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
				if (!inited) { throw "ItemType, not inited"; }
				data = val;
			}
			int size()const {
				return inited? 1 : 0;
			}
			bool empty()const { return !inited; }
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
			/*string ToString()const
			{
			string temp;

			for (int i = 0; i < size(); ++i) {
			temp = temp + arr[i].ToString();
			if (i != size() - 1) {
			temp = temp + "/";
			}
			}

			return temp;
			} */
		};

		class UserType : public Type {
		public:
			const vector<int>& GetIList() const { return ilist; }
			vector<int>& GetIList() { return ilist; }
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
			//todo - bool IsItemList? IsUserTypeList? by ilist_idx
			//		Type* GetList( const int idx )
			Type* GetList(const int idx)
			{
				if (ilist[idx] == 1) {
					int item_idx = -1;

					for (int i = 0; i < ilist.size() && i <= idx; ++i) {
						if (ilist[idx] == 1) { item_idx++; }
					}

					return (Type*)(&itemList[item_idx]);
				}
				else {
					int usertype_idx = -1;

					for (int i = 0; i < ilist.size() && i <= idx; ++i) {
						if (ilist[idx] == 2) { usertype_idx++; }
					}
					return (Type*)(userTypeList[usertype_idx]);
				}
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
			}
		private:
			UserType* parent;
			std::vector<int> ilist;
			std::vector< ItemType<string> > itemList;
			std::vector< UserType* > userTypeList;
			bool noRemove = false;
			//	bool userTypeList_sortFlagA; // A : sorted < , B ; sorted > , false : not sorted!
			//	bool userTypeList_sortFlagB;
		private:
		public:
			explicit UserType(const string& name = "", bool noRemove = false) : Type(name), parent(NULL), noRemove(noRemove) { } //, userTypeList_sortFlagA(true), userTypeList_sortFlagB(true) { }
			UserType(const UserType& ut) : Type(ut.GetName()) {
				Reset(ut);  // Initial
			}
			UserType(UserType&& ut) : Type(ut.GetName()) {
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
			void Reset(const UserType& ut) { /// UT 전체를 복사한다.
											 //	userTypeList_sortFlagA = ut.userTypeList_sortFlagA;
											 //userTypeList_sortFlagB = ut.userTypeList_sortFlagB;

				ilist = ut.ilist;
				itemList = ut.itemList;
				parent = ut.parent;

				for (int i = 0; i < ut.userTypeList.size(); i++) {
					userTypeList.push_back(new UserType(*ut.userTypeList[i]));
				}
			}
			void Reset2(UserType&& ut) {
				//std::swap( userTypeList_sortFlagA, ut.userTypeList_sortFlagA );
				//std::swap( userTypeList_sortFlagB, ut.userTypeList_sortFlagB );

				parent = ut.parent;
				ut.parent = NULL; /// chk..
				ilist = std::move(ut.ilist);
				itemList = std::move(ut.itemList);

				for (int i = 0; i < ut.userTypeList.size(); ++i) {
					userTypeList.push_back(move(ut.userTypeList[i]));
					ut.userTypeList[i] = NULL;
				}
				ut.userTypeList.clear();
			}

			void _Remove()
			{
				ilist = vector<int>();
				itemList = vector< ItemType<string> >();
				RemoveUserTypeList();
			}

		private:
			int _GetIndex(const vector<int>& ilist, const int val, const int start = 0)
			{
				for (int i = start; i < ilist.size(); ++i) {
					if (ilist[i] == val) { return i; }
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
						// i부터 left shift!and resize!
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
				for (int i = idx + 1; i < GetItemListSize(); ++i) {
					userTypeList[i - 1] = std::move(userTypeList[i]);
				}
				userTypeList.resize(userTypeList.size() - 1);
				//  ilist left shift and resize - count itemType!
				int count = 0;
				for (int i = 0; i < ilist.size(); ++i) {
					if (ilist[i] == 2) { count++; }
					if (count == idx + 1) {
						// i부터 left shift!and resize!
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
					}
					else {
						// remove item, ilist left shift 1.
						for (int j = k + 1; j < ilist.size(); ++j) {
							ilist[j - 1] = ilist[j];
						}
						ilist.resize(ilist.size() - 1);
					}
					k = _GetIndex(ilist, 1, k + 1);
				}
				itemList = move(tempDic);
			}
			void Remove()
			{
				/// parent->removeUserType(name); - ToDo - X
				ilist = vector<int>();
				itemList = vector< ItemType<string> >();

				RemoveUserTypeList();
			}
			void RemoveUserTypeList() { /// chk memory leak test!!
				for (int i = 0; i < userTypeList.size(); i++) {
					if (NULL != userTypeList[i]) {
						delete userTypeList[i]; //
						userTypeList[i] = NULL;
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
			// todo - 
			void RemoveList(const int idx) // ilist_idx!
			{
				// chk whether item or usertype.
				// find item_idx or usertype_idx.
				// remove item or remove usertype.
				if (ilist[idx] == 1) {
					int item_idx = -1;

					for (int i = 0; i < ilist.size() && i <= idx; ++i) {
						if (ilist[idx] == 1) { item_idx++; }
					}

					RemoveItemList(item_idx);
				}
				else {
					int usertype_idx = -1;

					for (int i = 0; i < ilist.size() && i <= idx; ++i) {
						if (ilist[idx] == 2) { usertype_idx++; }
					}

					RemoveUserTypeList(usertype_idx);
				}
			}
		public:
			bool empty()const { return ilist.empty(); }
			void AddItem(string&& name, string&& item) {
				/*int index = -1;
				if (!itemList.Search(ItemType<string>(name), &index))
				{
				ilist.push_back(1);

				itemList.PushBack(ItemType<string>(name));//
				itemList.Search(ItemType<string>(name), &index);
				}
				itemList[index].Push(item);
				*/
				ItemType<string> temp(std::move(name));
				temp.Push(std::move(item));
				itemList.push_back(std::move(temp));
				ilist.push_back(1);
			}
			void AddItem(const string& name, const string& item) {
				/*int index = -1;
				if (!itemList.Search(ItemType<string>(name), &index))
				{
				ilist.push_back(1);

				itemList.PushBack(ItemType<string>(name));//
				itemList.Search(ItemType<string>(name), &index);
				}
				itemList[index].Push(item);
				*/
				ItemType<string> temp(name);
				temp.Push(item);
				itemList.push_back(std::move(temp));
				ilist.push_back(1);
			}
			void AddUserTypeItem(UserType&& item) {
				/*if (this->userTypeList_sortFlagA) {
				string str_compare = Compare(userTypeList.back().GetName(), item.GetName());

				if (str_compare == "<" || str_compare == "==")
				{
				this->userTypeList_sortFlagA = true;
				}
				else {
				this->userTypeList_sortFlagA = false;
				}
				}
				if (this->userTypeList_sortFlagB) {
				string str_compare = Compare(userTypeList.back().GetName(), item.GetName());

				if (str_compare == ">" || str_compare == "==")
				{
				this->userTypeList_sortFlagB = true;
				}
				else {
				this->userTypeList_sortFlagB = false;
				}
				}*/

				/*
				int index = -1;
				if (!userTypeList.Search(ItemType<UserType*>(item.GetName()), &index))
				{
				ilist.push_back(2);

				userTypeList.PushBack(ItemType<UserType*>(item.GetName()));//
				userTypeList.Search(ItemType<UserType*>(item.GetName()), &index);
				}
				*/
				UserType* temp = new UserType(std::move(item));
				temp->parent = this;
				//temp->SetName("");

				ilist.push_back(2);

				userTypeList.push_back(temp);
			}
			void AddUserTypeItem(const UserType& item) {
				/*if (this->userTypeList_sortFlagA) {
				string str_compare = Compare(userTypeList.back().GetName(), item.GetName());

				if (str_compare == "<" || str_compare == "==")
				{
				this->userTypeList_sortFlagA = true;
				}
				else {
				this->userTypeList_sortFlagA = false;
				}
				}
				if (this->userTypeList_sortFlagB) {
				string str_compare = Compare(userTypeList.back().GetName(), item.GetName());

				if (str_compare == ">" || str_compare == "==")
				{
				this->userTypeList_sortFlagB = true;
				}
				else {
				this->userTypeList_sortFlagB = false;
				}
				}*/

				/*
				int index = -1;
				if (!userTypeList.Search(ItemType<UserType*>(item.GetName()), &index))
				{
				ilist.push_back(2);

				userTypeList.PushBack(ItemType<UserType*>(item.GetName()));//
				userTypeList.Search(ItemType<UserType*>(item.GetName()), &index);
				}
				*/
				UserType* temp = new UserType(item);
				temp->parent = this;
				//temp->SetName("");

				ilist.push_back(2);

				userTypeList.push_back(std::move(temp));
			}

			void AddItemAtFront(const string& name, const string& item) {
				ItemType<string> temp(name);
				temp.Push(item);

				itemList.insert(itemList.begin(), temp);

				ilist.insert(ilist.begin(), 1);
			}
			void AddUserTypeItemAtFront(const UserType& item) {
				UserType* temp = new UserType(item);
				temp->parent = this;

				ilist.insert(ilist.begin(), 2);

				userTypeList.insert(userTypeList.begin(), temp);
			}


			vector<ItemType<string>> GetItem(const string& name) const {
				vector<ItemType<string>> temp;

				for (int i = 0; i < itemList.size(); ++i) {
					if (itemList[i].GetName() == name) {
						temp.push_back(itemList[i]);
					}
				}
				return temp;
			}
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

				for (int i = 0; i < userTypeList.size(); ++i)
				{
					if (name == userTypeList[i]->GetName()) {
						idx = i;
					}
				}
				if (idx > -1) {
					ref = userTypeList[idx];
				}
				return idx > -1;
			}
		private:
			/// save1 - like EU4 savefiles.
			void Save1(ostream& stream, const UserType* ut) const {
				int itemListCount = 0;
				int userTypeListCount = 0;

				for (int i = 0; i < ut->ilist.size(); ++i) {
					//std::cout << "ItemList" << endl;
					if (ut->ilist[i] == 1) {
						for (int j = 0; j < ut->itemList[itemListCount].size(); j++) {
							if (ut->itemList[itemListCount].GetName() != "") {
								stream << ut->itemList[itemListCount].GetName() << "=";
							}
							stream << ut->itemList[itemListCount].Get(j);
							if (j != ut->itemList[itemListCount].size() - 1) {
								stream << "\n";
							}
						}
						stream << "\n";
						itemListCount++;
					}
					else if (ut->ilist[i] == 2) {
						// std::cout << "UserTypeList" << endl;
						if (ut->userTypeList[userTypeListCount]->GetName() != "") {
							stream << ut->userTypeList[userTypeListCount]->GetName() << "=";
						}
						stream << "{\n";
						Save1(stream, ut->userTypeList[userTypeListCount]);
						stream << "\n";
						stream << " }" << "\n";

						userTypeListCount++;
					}
				}
			}
			/// savw2 - for more seed loading data!
			void Save2(ostream& stream, const UserType* ut) const {
				int itemListCount = 0;
				int userTypeListCount = 0;

				for (int i = 0; i < ut->ilist.size(); ++i) {
					//std::cout << "ItemList" << endl;
					if (ut->ilist[i] == 1) {
						for (int j = 0; j < ut->itemList[itemListCount].size(); j++) {
							if (ut->itemList[itemListCount].GetName() != "")
								stream << ut->itemList[itemListCount].GetName() << " = ";
							stream << ut->itemList[itemListCount].Get(j);
							if (j != ut->itemList[itemListCount].size() - 1)
								stream << " ";
						}
						stream << "\n";
						itemListCount++;
					}
					else if (ut->ilist[i] == 2) {
						// std::cout << "UserTypeList" << endl;
						if (ut->userTypeList[userTypeListCount]->GetName() != "")
						{
							stream << ut->userTypeList[userTypeListCount]->GetName() << " = ";
						}
						stream << " {\n";
						Save2(stream, ut->userTypeList[userTypeListCount]);
						stream << "\n";
						stream << " }" << "\n";
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
							if (itemList[itemListCount].GetName() != "")
								temp += itemList[itemListCount].GetName() + " = ";
							temp += itemList[itemListCount].Get(j);
							if (j != itemList[itemListCount].size() - 1)
								temp += " ";
						}
						temp += " ";
						itemListCount++;
					}
					else if (ilist[i] == 2) {
						// std::cout << "UserTypeList" << endl;
						if (userTypeList[userTypeListCount]->GetName() != "") {
							temp = temp + userTypeList[userTypeListCount]->GetName() + " = ";
						}
						temp += " { ";
						temp += (userTypeList[userTypeListCount]->ToString()) + " ";
						temp += (string(" } ") + " ");

						userTypeListCount++;
					}
				}
				return temp;
			}

			friend ostream& operator<<(ostream& stream, const UserType& ut)
			{
				int itemListCount = 0;
				int userTypeListCount = 0;

				for (int i = 0; i < ut.ilist.size(); ++i) {
					//std::cout << "ItemList" << endl;
					if (ut.ilist[i] == 1) {
						for (int j = 0; j < ut.itemList[itemListCount].size(); j++) {
							if (ut.itemList[itemListCount].GetName() != "")
								stream << ut.itemList[itemListCount].GetName() << "=";
							stream << ut.itemList[itemListCount].Get(j);
							if (j != ut.itemList[itemListCount].size() - 1)
								stream << " ";
						}
						stream << "\n";
						itemListCount++;
					}
					else if (ut.ilist[i] == 2) {
						// std::cout << "UserTypeList" << endl;
						if (ut.userTypeList[userTypeListCount]->GetName() != "") {
							stream << ut.userTypeList[userTypeListCount]->GetName() << "=";
						}
						stream << "{\n";
						stream << *ut.userTypeList[userTypeListCount] << "\n";
						stream << " }" << "\n";

						userTypeListCount++;
					}
				}

				return stream;
			}
		private:
			class DoThread // need to rename!
			{
			private:
				vector<UserType*>* utVec;
				ArrayQueue<string>* aq;
				string target_ch;
				string result_ch;
				int utVecStart;
				int utVecEnd;
			public:
				DoThread(vector<UserType*>* utVec, const string& target_ch, const string& result_ch, int utVecStart, int utVecEnd)
					: utVec(utVec), target_ch(target_ch), result_ch(result_ch), utVecStart(utVecStart), utVecEnd(utVecEnd)
				{
				}
				void operator() () {
					for (int i = utVecStart; i <= utVecEnd; ++i)
					{
						ReplaceAll((*utVec)[i], target_ch, result_ch);
					}
				}
			};
		public:
			static void ReplaceAll(UserType* temp, const string& target_ch, const string& result_ch) {
				const int itemListSize = temp->GetItemListSize();
				const int userTypeListSize = temp->GetUserTypeListSize();

				for (int i = 0; i < itemListSize; ++i) {
					ItemType<std::string>& itemList = temp->GetItemList(i);

					//string name = itemList.GetName();
					//name = Utility::ChangeStr(name, target_ch, result_ch);
					//itemList.SetName(name);

					for (int j = 0; j < itemList.size(); ++j) {
						itemList.Get(j) = Utility::ChangeStr(itemList.Get(j), target_ch, result_ch);
					}
				}

				if (userTypeListSize > 100) { ///  chk 20, ... ?
					const int count = userTypeListSize;
					DoThread dtA(&(temp->userTypeList), target_ch, result_ch, 0, count / 4 - 1),
						dtB(&(temp->userTypeList), target_ch, result_ch, count / 4, (count / 4) * 2 - 1),
						dtC(&(temp->userTypeList), target_ch, result_ch, (count / 4) * 2, (count / 4) * 3 - 1),
						dtD(&(temp->userTypeList), target_ch, result_ch, (count / 4) * 3, count - 1);
					std::thread _threadA(dtA), _threadB(dtB), _threadC(dtC), _threadD(dtD);

					_threadA.join();
					_threadB.join();
					_threadC.join();
					_threadD.join();
				}
				else {
					for (int i = 0; i < userTypeListSize; ++i) {
						ReplaceAll(temp->GetUserTypeList(i), target_ch, result_ch);
					}
				}
			}
		public:
			/// move to UserType
			static bool ChkData(const UserType* utTemp)
			{
				bool chk = true;
				const int itemListSize = utTemp->GetItemListSize();
				for (int i = 0; i < itemListSize; ++i) {
					if (utTemp->GetItemList(i).GetName() != ""
						&& utTemp->GetItemList(i).size() > 1) {
						cout << utTemp->GetItemList(i).GetName() << endl;
						return false;
					}
				}
				const int UserTypeSize = utTemp->GetUserTypeListSize();
				for (int i = 0; i < UserTypeSize; ++i) {
					chk = chk && ChkData(utTemp->GetUserTypeList(i));
				}

				return chk;
			}
			// find userType! not itemList!,// this has bug
			static std::pair<bool, vector< UserType*> > Find(UserType* global, const string& _position) /// option, option_offset
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

				StringTokenizer tokenizer(position, "/");
				vector<string> strVec;
				Deck<pair< UserType*, int >> utDeck;
				pair<UserType*, int> utTemp;
				utTemp.first = global;
				utTemp.second = 0;
				ItemType<UserType*> utTemp2;

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
						if (utTemp.first == NULL) {
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
					utTemp = utDeck.pop_front();

					if (utTemp.second < strVec.size() && strVec[utTemp.second] == "$")
					{
						for (int j = utTemp.first->GetUserTypeListSize() - 1; j >= 0; --j) {
							UserType* x = utTemp.first->GetUserTypeList(j);
							utDeck.push_front(make_pair(x, utTemp.second + 1));
						}
					}
					else if (utTemp.second < strVec.size() &&
						wiz::String::startsWith(strVec[utTemp.second], "$ut")
						)
					{
						long long idx = std::stoll(wiz::String::substring(strVec[utTemp.second], 3));

						if (idx < 0 || idx >= utTemp.first->GetUserTypeListSize()) {
							throw string("ERROR NOT VALID IDX");
						}

						utDeck.push_front(make_pair(utTemp.first->GetUserTypeList(idx), utTemp.second + 1));
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
			static std::pair<bool, vector< pair< UserType*, pair<string, string> >>> Find2(UserType* global, const string& _position) /// option, option_offset
			{
				string position = _position;
				vector< UserType* > temp;
				vector<  pair< UserType*, pair<string, string> > > strVec2;
				string str; ///

				if (!position.empty() && position[0] == '@') { position.erase(position.begin()); }
				if (position.empty()) { temp.push_back(global); return{ true,  vector< pair< UserType*, pair<string, string> >>() }; }
				if (position == ".") { temp.push_back(global); return{ true,  vector< pair< UserType*, pair<string, string> >>() }; }
				if (position == "/./") { temp.push_back(global); return{ true, vector< pair< UserType*, pair<string, string> >>() }; } // chk..
				if (position == "/.") { temp.push_back(global); return{ true,  vector< pair< UserType*, pair<string, string> >>() }; }
				if (String::startsWith(position, "/."))
				{
					position = String::substring(position, 3);
				}

				StringTokenizer tokenizer(position, "/");
				vector<string> strVec;
				Deck<pair< UserType*, pair<int, pair<string, string>> >> utDeck;
				pair<UserType*, pair<int, pair<string, string>>> utTemp;
				utTemp.first = global;
				utTemp.second.first = 0;
				utTemp.second.second.first = "/";
				utTemp.second.second.second = "/";

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
						if (utTemp.first == NULL) { // || strVec.empty()
							return{ false, vector< pair< UserType*, pair<string, string> >>() };
						}
						utTemp.first = utTemp.first->GetParent();
						strVec.pop_back();
					}
					std::reverse(strVec.begin(), strVec.end());
				}

				utDeck.push_front(utTemp);

				bool exist = false;
				while (false == utDeck.empty()) {
					utTemp = utDeck.pop_front();

					if (utTemp.second.first < strVec.size() && strVec[utTemp.second.first] == "$")
					{
						for (int j = utTemp.first->GetUserTypeListSize() - 1; j >= 0; --j) {
							UserType* x = utTemp.first->GetUserTypeList(j);
							utDeck.push_front(make_pair(x, make_pair(utTemp.second.first + 1, 
								make_pair(utTemp.second.second.first + x->GetName(), wiz::toStr(j)))));
						}
					}
					else if (utTemp.second.first < strVec.size() &&
						wiz::String::startsWith(strVec[utTemp.second.first], "$ut")
						)
					{
						long long idx = std::stoll(wiz::String::substring(strVec[utTemp.second.first], 3));

						if (idx < 0 || idx >= utTemp.first->GetUserTypeListSize()) {
							throw string("ERROR NOT VALID IDX");
						}

						utDeck.push_front(make_pair(utTemp.first->GetUserTypeList(idx), make_pair(utTemp.second.first + 1, 
								make_pair(utTemp.second.second.first + utTemp.first->GetUserTypeList(idx)->GetName(), wiz::toStr(idx)))));
					}
					else if (utTemp.second.first < strVec.size() &&
						(utTemp.first->GetUserTypeItem(strVec[utTemp.second.first]).empty() == false))
					{
						auto  x = utTemp.first->GetUserTypeItem(strVec[utTemp.second.first]);
						for (int j = x.size() - 1; j >= 0; --j) {
							utDeck.push_front(make_pair(x[j], make_pair(utTemp.second.first + 1, 
								make_pair(utTemp.second.second.first + x[j]->GetName(), wiz::toStr(j)))));
						}
					}

					if (utTemp.second.first == strVec.size()) {
						exist = true;

						pair< UserType*, pair<string, string> > temp;
						temp.first = utTemp.first;
						temp.second.first = utTemp.second.second.first;
						temp.second.second = utTemp.second.second.second;

						strVec2.push_back(temp);
					}
				}
				
				if (false == exist) { return{ false, vector< pair< UserType*, pair<string, string> >>() }; }
				return{ true, strVec2 };
			}
		};
	}
}

#endif