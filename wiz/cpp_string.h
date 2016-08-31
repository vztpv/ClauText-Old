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
        static int find( const string str, const string dilim, const int start_index )
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
        static bool Comp( const char* cstr1, const char* cstr2, const int n ) /// isSameData?
        {
            for( int i=0; i < n; i++ )
            {
                if( cstr1[i] != cstr2[i] ) { return false; }
            }
            return true;
        }
        static bool Comp( const string& str1, const string& str2, const int n ) /// isSameData?
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

/*
            /// chk fromIndex...
            if( str1.empty() ) { return -1; }
            const char* pStr = str.c_str();
            string result;

            for( int i=fromIndex; i <= str.size() - str1.size(); i++ ) {
                const int n = strlen( pStr+i );
                if( n >= str1.size()
                   && Comp( pStr+i, str1.c_str(), str1.size() ) )
                {
                    return i;
                }
            }

            return -1;
            **/
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
            return str.substr( start, last-start+1 );
            /*
            if( str.empty() ) { return string(); }
            char* buf = new char[str.size()+1];

            int count = 0;
            for(int i=start; i <= last; i++ ) /// cf) i <=end - why??
            {
                buf[count++] = str[i];
            }
            buf[count] = '\0';
            string temp = buf;
            delete[] buf;

            return temp;
            */
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
            if( last.empty() ) { return true; } /// chk... return false; ??
            return Comp(  str.c_str() + ( str.size() - last.size() ), last.c_str(), last.size() );
        }
    };
    class StringTokenizer
    {
    private:
        vector<string> _m_str;
        int count;
		bool exist;
    private:
        void Init( const string& str, const vector<string>& separator )
        {
            if( separator.empty() || str.empty() ) { return; }
            std::pair<bool, size_t> idx; int k = 0;
            int i=0; int select=-1;
            string tempStr;

            //_m_str.reserve( str.size() );

            while(true) {
                int counter_minus1 = 0;
                k = str.size(); ///
				idx.first = false; idx.second = 0; select = 0; ///
                for( int j = 0; j < separator.size(); j++ ) {
                    idx = String::indexOf( str, separator[j], i );

                    if( false == idx.first ) { counter_minus1++; }
                    if( false == idx.first && counter_minus1 == separator.size() ) {
                        tempStr = String::substring( str, i );
                        if( !tempStr.empty() ) { _m_str.push_back( tempStr ); }
                       // vector<string>( _m_str ).swap( _m_str );
                        return;
                    }
					if (idx.first) { exist = true; if (k > idx.second) { select = j; k = idx.second; } } /// idx가 같다면 가장 처음에 나온것이 선택된다. -> 길이가 긴 것을 우선시 해야한다?
                }
                tempStr = String::substring( str, i, k-1 );
                if( !tempStr.empty() ) { _m_str.push_back( tempStr ); }
                i = k + separator[select].size();
            }
        }
    public:
        explicit StringTokenizer() : count(0), exist(false) { }
        explicit StringTokenizer( const string& str, const string& separator )
        : count( 0 ), exist(false)
        {
            vector<string> vec; vec.push_back( separator );
            Init( str, vec );
        }
        explicit StringTokenizer( const string& str, const vector<string>& separator  )
        : count( 0 ), exist(false)
        {
            Init( str, separator );
        }
        explicit StringTokenizer( const string& str )
        : count( 0 ), exist(false)
        {
            vector<string> vec;
            vec.push_back( " " );
            vec.push_back( "\t" );
            vec.push_back( "\r" );
            vec.push_back( "\n" );
            Init( str, vec );
        }
        int countTokens()const
        {
            return _m_str.size();
        }
        string nextToken()
        {
            if( hasMoreTokens() )
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
    /*
    class StringTokenizer
    {
    private:
        vector<string> _m_str;
        int count;
    public:
        explicit StringTokenizer() :count(0) { }
        explicit StringTokenizer( const string& str, const string& separator  )
        : count( 0 )
        {
            if( separator.empty() || str.empty() ) { return; }
            int idx = 0;
            int i=0;
            string tempStr;

            while(true)
            {
                idx = String::indexOf( str, separator , i );
                if( -1 == idx ) {
                    tempStr = String::substring( str, i );
                    if( !tempStr.empty() ) { _m_str.push_back( tempStr ); }
                    break;
                }
                tempStr = String::substring( str, i, idx-1 );
                if( !tempStr.empty() ) { _m_str.push_back( tempStr ); }
                i = idx + separator.size();
            }
        }
        int countTokens()const
        {
            return _m_str.size();
        }
        string nextToken()
        {
            return _m_str[count++];
        }
        bool hasMoreTokens()const
        {
            return count < _m_str.size();
        }

    };
}*/
}
#endif // CPP_STRING_H_INCLUDED
