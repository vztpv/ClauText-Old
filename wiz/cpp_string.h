#ifndef CPP_STRING_H_INCLUDED
#define CPP_STRING_H_INCLUDED

#define _CRT_SECURE_NO_WARNINGS

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
		static const vector<string> whitespaces;
		int option;
	private:
		void Init(const string& str, const vector<string>& separator, StringBuilder* builder) // assumtion : separators are sorted by length?, long -> short
		{
			const int str_size = str.size();
			if (separator.empty() || str.empty()) { return; } // if str.empty() == false then _m_str.push_back(str); // ?
															  //
			builder->Clear();
			builder->Append(str.c_str(), str.size());

			_m_str.reserve(str.size() / 2);
			int left = 0;
			int right = 0;
			int state = 0;
			this->exist = false;

			for (int i = 0; i < str.size(); ++i) {
				right = i;
				int _select = -1;
				bool pass = false;


				if (1 == option && 0 == state && '\'' == str[i]) {
					state = 2;
					continue;
				}
				else if (1 == option && 2 == state && '\'' == str[i]) {
					state = 0;
					if (i == str.size() - 1) {
						if (right - left + 1 > 0) {
							_m_str.emplace_back(builder->Divide(right - left + 1), right - left + 1);
							builder->LeftShift(right - left + 2);
						}
					}
					continue;
				}
				else if (1 == option && 2 == state) {
					continue;
				}
				else if (1 == option && 0 == state && '\"' == str[i]) {
					if (i == 0) {
						state = 1;
						continue;
					}
					else if (i > 0 && '\\' == str[i - 1]) {
						throw "ERROR for Init on StringTokenizer"; // error!
					}
					else if (i > 0) {
						state = 1;
						continue;
					}
				}
				else if (1 == option && 1 == state && '\"' == str[i]) {
					if ('\\' == str[i - 1]) {
						continue;
					}
					else {
						state = 0;
						if (i == str.size() - 1) {
							if (right - left + 1 > 0) {
								_m_str.emplace_back(builder->Divide(right - left + 1), right - left + 1);
								builder->LeftShift(right - left + 2);
							}
						}
						continue;
					}
				}
				else if (1 == option && 1 == state) {
					continue;
				}

				const int sep_size = separator.size();
				for (int j = 0; j < sep_size; ++j) {
					const int  sep_j_size = separator[j].size();
					for (int k = 0; k < sep_j_size && i + k < str_size; ++k) {
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

					if (right - left >= 0) {
						const char* temp = builder->Divide(right - left);
						if (right - left > 0) {
							_m_str.emplace_back(temp, right - left);
							builder->LeftShift(right - left + 1);
						}
						else {
							builder->LeftShift(1);
						}
					}

					i = i + separator[_select].size() - 1;

					left = i + 1;
					right = left;
				}
				else if (!pass && i == str.size() - 1) {
					if (right - left + 1 > 0) {
						_m_str.emplace_back(builder->Divide(right - left + 1), right - left + 1);
						builder->LeftShift(right - left + 2);
					}
					else {
						//_getch();
					}
				}
			}
			//cout << "str is " << str <<  " state  is " << state << endl;
		}
	public:
		explicit StringTokenizer() : count(0), exist(false), option(0) { }
		explicit StringTokenizer(const string& str, const string& separator, StringBuilder* builder, int option = 0)
			: count(0), exist(false), option(option)
		{
			vector<string> vec; vec.push_back(separator);
			Init(str, vec, builder);
		}
		explicit StringTokenizer(const string& str, const vector<string>& separator, StringBuilder* builder, int option = 0)
			: count(0), exist(false), option(option)
		{
			Init(str, separator, builder);
		}
		explicit StringTokenizer(const string& str, StringBuilder* builder, int option = 0)
			: count(0), exist(false), option(option)
		{
			Init(str, whitespaces, builder);
		}
		explicit StringTokenizer(string&& str, StringBuilder* builder, int option = 0)
			: count(0), exist(false), option(option)
		{
			Init(std::move(str), whitespaces, builder);
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
				throw "error in nextToken!";
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
	const vector<string> StringTokenizer::whitespaces = { " ", "\t", "\r", "\n" };
/*
	class StringTokenizer
	{
	private:
		vector<string> _m_str;
		int count;
		bool exist;
		static const vector<string> whitespaces;
		int option;
	private:
		void Init(string str, const vector<string>& separator) // assumtion : separators are sorted by length?, long -> short
		{
			if (separator.empty() || str.empty()) { return; } // if str.empty() == false then _m_str.push_back(str); // ?

			int left = 0;
			int right = 0;
			int state = 0;
			this->exist = false;

			for (int i = 0; i < str.size(); ++i) {
				right = i;
				int _select = -1;
				bool pass = false;
				
				if (1 == option && 0 == state && '\"' == str[i]) {
					if (i == 0) {
						state = 1;
						continue;
					}
					else if (i > 0 && '\\' == str[i - 1]) {
						throw "ERROR for Init on StringTokenizer"; // error!
					}
					else if (i > 0) {
						state = 1;
						continue;
					}
				}
				else if (1 == option && 1 == state && '\"' == str[i]) {
					if ('\\' == str[i - 1]) {
						continue;
					}
					else {
						state = 0;
					}
				}
				else if (1 == option && 1 == state) {
					continue;
				}


				for (int j = 0; j < separator.size(); ++j) {
					for (int k = 0; k < separator[j].size() && i + k < str.size(); ++k) {
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
						_m_str.push_back(String::substring(str, left, right-1));
					}
					i = i + separator[_select].size() - 1;
					left = i + 1;
					right = left;
				}
				else if (!pass && i == str.size() - 1) {
					if (right - left + 1 > 0) {
						_m_str.push_back(String::substring(str, left, right));
					}
				}
			}
			//cout << "str is " << str <<  " state  is " << state << endl;
		}
		
	public:
		explicit StringTokenizer() : count(0), exist(false), option(0) { }
		explicit StringTokenizer(const string& str, const string& separator, int option=0)
			: count(0), exist(false), option(option)
		{
			vector<string> vec; vec.push_back(separator);
			Init(str, vec);
		}
		explicit StringTokenizer(const string& str, const vector<string>& separator, int option = 0)
			: count(0), exist(false), option(option)
		{
			Init(str, separator);
		}
		explicit StringTokenizer(const string& str, int option = 0)
			: count(0), exist(false), option(option)
		{
			Init(str, whitespaces);
		}
		explicit StringTokenizer(string&& str, int option = 0)
			: count(0), exist(false), option(option)
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
				throw "error in nextToken!";
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
	const vector<string> StringTokenizer::whitespaces = { " ", "\t", "\r", "\n" };
	*/
}
#endif // CPP_STRING_H_INCLUDED
