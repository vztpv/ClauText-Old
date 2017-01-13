#ifndef CPP_STRING_H_INCLUDED
#define CPP_STRING_H_INCLUDED


#include <wiz/global.h>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

namespace wiz{
    class String{
    public:
        static int find( const string& str, const string& dilim, const int start_index )
        {
            if( str.empty() || dilim.empty() ) { return -1; }

            int k = 0;

            for( int i = start_index; i < str.size(); i++ ) {
                if( str[i] == dilim[k] ) { k++; if( dilim.size() == k ){ return i - dilim.size() + 1; } }
                else {
                    k = 0;
                    if( str[i] == dilim[k] ) { k++; if( dilim.size() == k ){ return i - dilim.size() + 1; } }
                }
            }

            return -1;
        }
        static bool Comp( const char* cstr1, const char* cstr2, const int n ) /// isSameData
        {
            for( int i=0; i < n; i++ )
            {
                if( cstr1[i] != cstr2[i] ) { return false; }
            }
            return true;
        }
        static bool Comp( const string& str1, const string& str2, const int n ) /// isSameData
        {
            for( int i=0; i < n; i++ )
            {
                if( str1[i] != str2[i] ) { return false; }
            }
            return true;
        }
        static string replace( const string& str, const string& str1, const string& str2 )
        {
            if( str1.empty() ) { return str; }

            // if str1 is empty, then infinite loop.
            const char* pStr = str.c_str();
            string result;

            for( int i=0; i < str.size(); i++ ) {
                if( strlen( pStr+i ) >= str1.size()
                   && Comp( pStr+i, str1.c_str(), str1.size() ) )
                {
                    result = result + str2; //
                    i = i + str1.size() - 1;
                }
                else
                {
                    result.push_back( str[i] );
                }
            }

            return result;
        }
        static string remove( const string& str, const string& str1 )
        {
            return replace( str, str1, string("") );
        }
        //start index
        static int indexOf( const string& str, const char ch )
        {
            for( int i=0; i < str.size(); i++ )
            {
                if( str[i] == ch ) { return i; }
            }
            return -1;
        }
		// chk..
		static std::pair< bool, size_t > indexOf(const string& str, const string& str1, const int fromIndex)
        {
			if (str1.empty()) { return{ false, 0 }; }
			auto idx = str.find(str1, fromIndex);
			if (idx == std::string::npos) { return{ false, 0 }; }
			return{ true, idx };
        }
        static auto indexOf(const string& str, const string& str1 )
        {
            return indexOf( str, str1, 0 );
        }
        /// chk....
        static vector<string> split( const string& str, const string& filter )
        {
            if( filter.empty()) {
                vector<string> result;
                if( !str.empty() ) {
                    result.push_back( str );
                }
                return result;
            }
            /// example...
            /// www.naver.com -> www, naver, com
            /// .www.naver.com. -> www, naver, com!!
            const char* pStr = str.c_str();
            char* pTemp = new char[str.size()+1];
            vector<string> result;

            strcpy( pTemp, str.c_str() );

            for( int i=0; i <= str.size()-filter.size(); i++ ) {
                if( strlen( pStr+i ) >= filter.size()
                   && Comp( pStr+i, filter.c_str(), filter.size() ) )
                { // finded..
                    pTemp[i] = '\0';
                    for( int j=i+1; j < i+filter.size() ; j++ )
                    {
                        pTemp[j] = '\0';
                    }
                    i = i + filter.size() - 1;
                }
            }
            for( int i=0; i < str.size(); i++ )
            {
                if( pTemp[i] != '\0' )
                {
                    result.push_back( string( pTemp+i ) );
                    i = i + strlen( pTemp+i ) - 1;
                }
            }

            // free
            delete[] pTemp;
            return result;
        }
        static string substring( const string& str, const int start, const int last )
        {
			return str.substr(start, last - start + 1);
        }
        static string substring( const string& str, const int start )
        {
            return substring( str, start, str.size()-1 );
        }
        static bool startsWith( const string& str, const string& start )
        {
            if( str.size() < start.size() ) { return false; }
            return Comp( str.c_str(), start.c_str(), start.size() );
        }
        static bool endsWith( const string& str, const string& last )
        {
            if( str.size() < last.size() ) { return false; }
            if( last.empty() ) { return true; } /// chk... return false; 
            return Comp(  str.c_str() + ( str.size() - last.size() ), last.c_str(), last.size() );
        }
    };

	class StringTokenizer
	{
	private:
		vector<string> _m_str;
		int count;
		bool exist;
		const vector<string> whitespaces = { " ", "\t", "\r", "\n" };
	private:
		void Init(string str, const vector<string>& separator) // assumtion : separators are sorted by length?, long -> short
		{
			if (separator.empty() || str.empty()) { return; } // if str.empty() == false then _m_str.push_back(str); // ?
			//vector<int> arr(str.size(), 0);

			int left = 0;
			int right = 0;
			bool chkEnd = false;

			this->exist = false;

			for (int i = 0; i < str.size(); ++i) {
				right = i;
				int _select = -1;
				bool pass = false;
				
				for (int j = 0; j < separator.size(); ++j) {
					for (int k = 0; k < separator[j].size(); ++k) {
						if (str[i + k] == separator[j][k]) {
							pass = true;
						}
						else {
							pass = false;
							break;
						}
					}
					if (pass) { _select = j; break; }
				}

				if (pass) {
					this->exist = true;

					if (right-1 - left + 1 > 0) {
						if (right <= str.size() - 1) {
							str[right] = '\0';
						}
						_m_str.emplace_back(str.c_str() + left);
					}
					i = i + separator[_select].size() - 1;
					left = i + 1;
					right = left;
				}
				else if (!pass && i == str.size() - 1) {
					if (right - left + 1 > 0) {
						_m_str.emplace_back(str.c_str() + left);
					}
				}
			}
		}
		/*
		void Init(string&& str, const vector<string>& separator) // assumtion : separators are sorted by length?, long -> short
		{
			if (separator.empty() || str.empty()) { return; } // if str.empty() == false then _m_str.push_back(str); // ?
															  //vector<int> arr(str.size(), 0);

			int left = 0;
			int right = 0;
			bool chkEnd = false;

			this->exist = false;

			for (int i = 0; i < str.size(); ++i) {
				right = i;
				int _select = -1;
				bool pass = false;

				for (int j = 0; j < separator.size(); ++j) {
					for (int k = 0; k < separator[j].size(); ++k) {
						if (str[i] == separator[j][k]) {
							pass = true;
						}
						else {
							pass = false;
							break;
						}
					}
					if (pass) { _select = j; break; }
				}

				if (pass) {
					this->exist = true;

					string temp = wiz::String::substring(str, left, right - 1);
					if (!temp.empty()) {
						_m_str.push_back(std::move(temp));
					}
					i = i + separator[_select].size() - 1;
					left = i + 1;
					right = left;
				}
				else if (!pass && i == str.size() - 1) {
					string temp = wiz::String::substring(str, left, right);
					if (!temp.empty()) {
						_m_str.push_back(std::move(temp));
					}
				}
			}
		}
		*/
		/*
		void Init(const string& str, const vector<string>& separator)
		{
			if (separator.empty() || str.empty()) { return; }
			std::pair<bool, size_t> idx; int k = 0;
			int i = 0; int select = -1;
			string tempStr;

			//_m_str.reserve( str.size() );
			vector<int> arr(separator.size(), 0);

			while (true) {
				int counter_minus1 = 0;
				k = str.size(); /// 긴문장은 뒤로??
				idx.first = false; idx.second = 0; select = 0; ///
				for (int j = 0; j < separator.size(); j++) {
					if (-1 == arr[j]) {
						counter_minus1++;
						if (counter_minus1 == separator.size()) {
							tempStr = String::substring(str, i);
							if (!tempStr.empty()) { _m_str.push_back(std::move(tempStr)); }
							return;
						}
						continue;
					}

					idx = String::indexOf(str, separator[j], i);

					if (false == idx.first) { arr[j] = -1; }

					if (false == idx.first) { counter_minus1++; }
					if (false == idx.first && counter_minus1 == separator.size()) {
						tempStr = String::substring(str, i);
						if (!tempStr.empty()) { _m_str.push_back(std::move(tempStr)); }
						return;
					}
					if (idx.first) { exist = true; if (k >= idx.second) { select = j; k = idx.second; } }
				}
				tempStr = String::substring(str, i, k - 1);
				if (!tempStr.empty()) { _m_str.push_back(std::move(tempStr)); }
				i = k + separator[select].size();
			}
		}
		*/
	public:
		explicit StringTokenizer() : count(0), exist(false) { }
		explicit StringTokenizer(const string& str, const string& separator)
			: count(0), exist(false)
		{
			vector<string> vec; vec.push_back(separator);
			Init(str, vec);
		}
		explicit StringTokenizer(const string& str, const vector<string>& separator)
			: count(0), exist(false)
		{
			Init(str, separator);
		}
		explicit StringTokenizer(const string& str)
			: count(0), exist(false)
		{
			Init(str, whitespaces);
		}
		explicit StringTokenizer(string&& str)
			: count(0), exist(false)
		{
			Init(std::move(str), whitespaces);
		}
		int countTokens()const
		{
			return _m_str.size();
		}
		string nextToken()
		{
			if (hasMoreTokens())
				return _m_str[count++];
			else
				return "";
		}
		bool hasMoreTokens()const
		{
			return count < _m_str.size();
		}

		bool isFindExist()const
		{
			return exist;
		}

	};

}
#endif // CPP_STRING_H_INCLUDED
