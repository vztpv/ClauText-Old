#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

// C++11 Standard.
#include <string>
#include <sstream>
#include <utility>
#include <type_traits> /// for is_class, ...
using namespace std;

///
/// GLOBAL_DEBUG
///

namespace wiz{
    #define COL_BASED
    #define ROW_BASED

	#define WIZ_IN const
	#define IN
	#define OUT
	#define INOUT

	#define AND &&
	#define OR ||
	#define NOT !
	#define EQUAL ==

	/*
    template <class T>
    inline T max( const T& t1, const T& t2 )
    {
        if( t2 < t1 ) { return t1; }
        return t2;
    }

    template <class T>
    inline T min( const T& t1, const T& t2 )
    {
        if( t1 < t2 ) { return t1; }
        return t2;
    }
	*/
    ///

    template <class T>
    class STD_SWAP
    {
    public:
        void operator()( T& t1, T& t2 )
        {
            std::swap( t1, t2 ); /// if c++11, maybe move...or make Move
        }
    };
    template <class T>
    class NORMAL_SWAP
    {
    public:
        void operator()( T& t1, T& t2 )
        {
            T temp = t1;
            t1 = t2;
            t2 = temp;
        }
    };
    template <class T>
    class HAS_SWAP_METHOD
    {
    public:
        void operator()( T& t1, T& t2 )
        {
            t1.swap( t2 );
        }
    };
    template <class T>
    class WIZ_SWAP
    {
    public:
        void operator()( T& t1, T& t2 )
        {
            std::swap( t1, t2 );
        }
    };
	template < class T, class SWAP=STD_SWAP<T> >
	inline void Swap( T& t1, T& t2 )
	{
	    SWAP()( t1, t2 );
	}

    /// TO DO
    /// MOVE, NO_MOVE
    /// ( T& t1, T& t2 )
    /// t1 = t2; // NO_MOVE
    /// t1 = move( t2 ); // MOVE

	template <class T>
	class ASC {
	public:
		bool operator() (const T& t1, const T& t2) const  { return t1 < t2; }
	};
	template <class T>
	class DSC // DESC
	{
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 > t2; }
	};
	template <class T>
	class EQ { // EE -> EQ!
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 == t2; }
	};
    /// TO DO
    /// ASC_EE, DSC_EE, NOT_EE, EE_SAME_VALUE, NOT_EE_SAME_VALUE , chk red-black tree!!
    template <class T>
	class ASC_EE {
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 <= t2; }
	};
	template <class T>
	class DSC_EE {
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 >= t2; }
	};
	template <class T>
	class NOT_EE {
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 != t2; }
	};
    template <class T>
	class EE_SAME_VALUE {
	public:
		bool operator() (const T& t1, const T& t2) const { return t1.isSameValue( t2 ); }
	};
    template <class T>
	class NOT_EE_SAME_VALUE { /// chk..
	public:
		bool operator() (const T& t1, const T& t2) const { return !t1.isSameValue( t2 ); }
	};


    template <class T> /// T is pointer type..
	class PASC {
	public:
		bool operator() (const T t1, const T t2) const  { return *t1 < *t2; }
	};
    template <class T> /// T is pointer type..
    class PDSC // PDESC
    {
    public:
        bool operator() ( const T t1, const T t2 ) const
        {
            return *t1 > *t2;
        }
    };
    template <class T> /// T is pointer type..
	class PEE {
	public:
		bool operator() (const T t1, const T t2) const { return *t1 == *t2; }
	};

    /// TO DO
    /// PASC_PEE, PDSC_PEE, PNOT_EE, PEE_SAME_VALUE, PNOT_EE_SAME_VALUE
    /// LEFT_HAS_SMALL_VALUE, LEFT_HAS_LARGE_VALUE, PLEFT_HAS_SMALL_VALUE, PLEFT_HAS_LARGE_VALUE
	template <typename T> /// T <- char, int, long, long long...
	string toStr(const T x, const int base); /// chk!!

	template <class T, class COMP = ASC<T>, class COMP2 = ASC<int>, class EE = EQ<T> > /// úÏÑú Î∞îÍæ∏Í∏ - 2015.07.18
	class WrapForInfinity
	{
		enum Op{ MIF = 0, GR = 1, IF = 2 };
		COMP comp;
		COMP2 comp2;
		EE ee;
		Op op; // Option
	private:
		explicit WrapForInfinity(const T& val, const Op& op) : val(val), op(op) { }
	public:
		explicit WrapForInfinity(const T& val = T()) : val(val), op(GR) { }
	public:
		T val;
		bool operator>(const WrapForInfinity<T,COMP,COMP2,EE>& wfi)const
		{
		    return wfi < (*this);
		}
		bool operator<(const WrapForInfinity<T,COMP,COMP2,EE>& wfi)const
		{
			if (GR == this->op&& GR == wfi.op)
			{
				return comp(this->val, wfi.val);
			}
			else
			{
				return comp2( this->op, wfi.op ); /// chk...
			}
		}
		bool operator==(const WrapForInfinity<T,COMP,COMP2,EE>& wfi)const
		{
		    if( wfi.op == IF && this->op == IF ) { return true; }
		    if( wfi.op == MIF && this->op == MIF ) { return true; }
			return (ee(wfi.val, this->val) && wfi.op == this->op);
		}
		bool operator!=( const WrapForInfinity<T,COMP,COMP2,EE>& wfi)const
		{
		    return !( *this == wfi );
		}
		bool operator<=(const WrapForInfinity<T,COMP,COMP2,EE>& wfi)const
		{
			return *this < wfi ||
				*this == wfi;
		}
		bool operator>=(const WrapForInfinity <T,COMP, COMP2, EE>& wfi)const
		{
			return wfi <= *this;
		}
		WrapForInfinity<T> operator+(const WrapForInfinity<T>& other)const
		{
			if (other.op == this->op) {
				return WrapForInfinity<T>(this->val + ohter.val, this->op);
			}
			else if (this->op == MIF && other.op == GR) {
				return WrapForInfinity<T>::GetMinusInfinity();
			}
			else if (this->op == IF && other.op == GR) {
				return WrapForInfinity<T>::GetInfinity();
			}
			else if (this->op == GR && other.op == IF) {
				return WrapForInfinity<T>::GetInfinity();
			}
			else if (this->op == GR && other.op == MIF) {
				return WrapForInfinity<T>::GetMinusInfinity();
			}
			else if (this->op == MIF && other.op == MIF) {
				return WrapForInfinity<T>::GetMinusInfinity();
			}
			else if (this->op == IF && other.op == IF) {
				return WrapForInfinity<T>::GetInfinity();
			}
			else
			{
				throw string(" + Error in wrapforinfiinity");
			}
		}
		WrapForInfinity<T> operator-(const WrapForInfinity<T>& other)const
		{
			if (other.op == this->op && this->op == GR) {
				return WrapForInfinity<T>(this->val - other.val, this->op);
			}
			else if (this->op == MIF && other.op == GR) {
				return WrapForInfinity<T>::GetMinusInfinity();
			}
			else if (this->op == GR && other.op == MIF) {
				return WrapForInfinity<T>::GetInfinity();
			}
			else if (this->op == IF && other.op == GR) {
				return WrapForInfinity<T>::GetInfinity();
			}
			else if (this->op == GR && other.op == IF) {
				return WrapForInfinity<T>::GetMinusInfinity();
			}
			else if (this->op == MIF && other.op == IF) {
				return WrapForInfinity<T>::GetMinusInfinity();
			}
			else if (this->op == IF && other.op == MIF) {
				return WrapForInfinity<T>::GetInfinity();
			}
			else
			{
				throw string(" - Error in wrapforinfiinity");
			}
		}
		string toString()const
		{
			if (this->op == MIF) return "minus infinity";
			else if (this->op == IF) return "plus infinity";
			else return wiz::toStr(this->val);
		}

		friend
        ostream& operator<<(ostream& stream, const WrapForInfinity<T,COMP,COMP2,EE>& wfi)
		{
		    if( wfi.op == MIF ) stream << "minus infinity";
            else if(wfi.op == IF ) stream << "plus infinity";
            else stream << wfi.val;

            return stream;
        }
		static WrapForInfinity<T, COMP, COMP2,EE > GetInfinity()
		{
			return WrapForInfinity<T, COMP, COMP2, EE >(T(), IF);
		}
		static WrapForInfinity<T, COMP, COMP2, EE > GetMinusInfinity()
		{
			return WrapForInfinity<T, COMP, COMP2, EE >(T(), MIF);
		}
	};

    template <typename T> /// x is 10ÏßÑÏàò..
    inline T pos_1(const T x, const int base=10) // 1êÎ¶¨ Í∞Í≥ÑÏÇ∞
	{
		if( x >= 0 ) { return x % base; }// x - ( x / 10 ) * 10; }
		else{ return (x / base) * base - x; }
		// -( x - ( (x/10) * 10 ) )
	}
    template <typename T> /// T <- char, int, long, long long...
    string toStr(const T x, const int base=10 ) /// chk!!
	{
	    if( base < 2 || base > 16 ) { return "base is not valid"; }
		T i = x;

		const int INT_SIZE = sizeof(T) << 3; ///*8
		char* temp = new char[INT_SIZE + 1 + 1]; /// 1 NULL, 1 minus
		string tempString;
		int k;
		bool isMinus = (i < 0);
		temp[INT_SIZE+1] = '\0'; ///Î¨∏ÏûêúÏãú..

		for (k = INT_SIZE; k >= 1; k--){
            T val = pos_1<T>(i, base); /// 0 ~ base-1
            /// number to ['0'~'9'] or ['A'~'F']
            if( val < 10 ) { temp[k] = val + '0'; }
            else { temp[k] = val-10 + 'A'; }

			i /= base;

			if (0 == i){ // ´Ïûê.
				k--;
				break;
			}
		}

		if (isMinus){
			temp[k] = '-';
			tempString = string(temp + k);//
		}
		else{
			tempString = string(temp + k + 1); //
		}
		delete[] temp;

		return tempString;
	}

    /// chk.... need more thinking..., ToDo...
	template <typename T> /// T <- char, int, long, long long...
    string toStr2(const T x, const int str_space, const int base=10 ) /// chk!!
	{
	    if( base < 2 || base > 16 ) { return "base is not valid"; }
		T i = x;
		T k2 = 0;

		const int INT_SIZE = sizeof(T) << 3; ///*8
		char* temp = new char[INT_SIZE + 1 + 1]; /// 1 NULL, 1 minus
		for(int i=0; i < INT_SIZE+2; ++i ) { temp[i] = '0'; }//
		string tempString;
		int k;
		bool isMinus = (i < 0);
		temp[INT_SIZE+1] = '\0'; ///Î¨∏ÏûêúÏãú..

		for (k = INT_SIZE; k >= 1; k--){
            T val = pos_1<T>(i, base); /// 0 ~ base-1
            /// number to ['0'~'9'] or ['A'~'F']
            if( val < 10 ) { temp[k] = val + '0'; }
            else { temp[k] = val-10 + 'A'; }

			i /= base;

			if (0 == i){ // ´Ïûê.
				k--;
				break;
			}
		}

		if (isMinus){
			temp[k] = '-';
			tempString = string(temp + k);//
		}
		else{
            if( INT_SIZE+1 - (k+1) +1 < str_space+1 )
            {
                k2 = str_space+1 - ( INT_SIZE+1 - (k+1) + 1 );
            }
            else
            {
                k2 = 0;
            }
			tempString = string(temp + k + 1 - k2 ); //
		}
		delete[] temp;

		return tempString;
	}

	inline string str(const int x)
	{
		return toStr<int>(x);
	}

	template <typename T> /// ∏Ï∂ú†Îïå åÏïÑÏ≤¥ÌÅ¨úÎã§
	inline string _toString(const T x)
	{
		std::stringstream strs;
		strs << x;
		return strs.str();
	}
	template <>
	inline string _toString(const int x)
	{
		return str(x);
	}
    template <>
	inline string _toString(const bool x)
	{
		if (x) { return "true"; }
		return "false";
	}

    // following remove
    inline bool isWhitespace( const char ch )
    {
        switch( ch )
        {
        case ' ':
        case '\t':
        case '\r':
        case '\n':
            return true;
        }
        return false;
    }

    template <typename T> // T must be unsigned type!
    inline T Unsigned_Maximum()
    {
        return -1;
    }

    template <typename T> // T must be signed type!
    T Signed_Maximum()
    {
        size_t byteSize = sizeof(T);
		T val = 0;

        val = 0x7F;
		for (size_t i = 1; i < byteSize; i++)
		{
			val = val << 8; // left shift 1 byte

			val = val | 0xFF;
		}
		return val;
    }
    // C++11
    template <typename T>
    inline T Maximum()
    {
        if( std::is_signed<T>::value )
        {
            return Signed_Maximum<T>();
        }
        if( std::is_unsigned<T>::value )
        {
            return Unsigned_Maximum<T>();
        }
        throw string( "unsupport type : double, class, and etc..." );
    }
	inline int getFirstIndex(const string& str, const char ch)
	{
		for (int i = 0; i < str.size(); ++i)
		{
			if (ch == str[i]) { return i; }
		}
		return -1;
	}
	class wizObject
	{
	private:
	public:
		wizObject() { }
		wizObject(const wizObject& object) { }

		virtual ~wizObject() { }

		virtual std::string toString()const = 0;
		//virtual void initial( const wizObject& object )=0;
		virtual wizObject& operator=(const wizObject& object) { return *this; }
		virtual wizObject* clone()const = 0;
	};
}
#endif
