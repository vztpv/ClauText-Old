#ifndef WIZARDERROR_H_INCLUDED
#define WIZARDERROR_H_INCLUDED

#include <wiz/global.h>

namespace wiz{
class Error{
private:
    string error;
public:
    explicit Error( const string err="" ){ error = err; }
	explicit Error( const string fileName, const int line, const string err="" )
	{
		error = "fileName: " + fileName + " \nline: " + toStr( line ) + " \nerr: " + err;
	}
    virtual ~Error(){ }

    virtual string toString()const { return error; }

    friend ostream& operator << ( ostream& stream, const Error& e )
    {
    	stream << e.toString();
    	return stream;
    }
};
class RangeOverError : public Error{
public:
    explicit RangeOverError():Error(){ }
    explicit RangeOverError( const string err ):Error( err ){ }

    virtual ~RangeOverError(){ }

    virtual string toString()const{
        return "range over error\n" + Error::toString();
    }
};
class IndexError : public Error{
public:
    explicit IndexError():Error(){ }
    explicit IndexError( const string err ):Error( err ){ }

    virtual ~IndexError(){ }

    virtual string toString()const{
        return "index error\n" + Error::toString();
    }
};
/**
* To Do?
*/
class AssertFail : public Error{
public:
    explicit AssertFail():Error(){ }
    explicit AssertFail( const string err ):Error( err ){ }

    virtual ~AssertFail(){ }

    virtual string toString()const{
        return "AssertFail\n" + Error::toString();
    }
};
/**
* To Do, string FileName, int line ??
*/
inline void checkIndexError( const int index, const int N ){ // N is array size..
    if( index < 0 || index >= N ){
        string temp = "index is ";
        temp = temp + toStr( index );
        temp = temp + string( ", N is ");
        temp = temp + toStr( N );
        throw IndexError( temp );
    }
}
inline void checkUnderError( int variableNo, const int x, const int min ){
    if( x < min ){
        string temp = toStr( variableNo ) + string( " is " ) +
                    toStr( x ) + string( " < " ) + toStr( min );
        throw RangeOverError( temp );
    }
}
inline void checkOverError( int variableNo, const int x, const int max ){
    if( x > max ){
        string temp = toStr( variableNo ) +
                string( " is " ) + toStr( x ) + string( " > " ) + toStr( max );
        throw RangeOverError( temp );
    }
}

/**
* To Do
* assert -> debug == 1 일대만, using #if, #endif
* and throw..!!
*/
// cf) using function call
template <typename T>
void assertEquals( string message, T expected, T actual );
template <typename T>
 void assertEquals( T expected, T actual );
template <typename T>
 void assertNotEquals( T expected, T actual );
 void assertTrue( string message, bool expected );
 void assertTrue( string message, int expected );
 void assertTrue( bool expected );
 void assertTrue( int expected );
 void assertFalse( string message, bool expected );
 void assertFalse( string message, int expected );
 void assertFalse( bool expected );
 void assertFalse( int expected );

/// of course,  below T are Pointer Type !!!
template <typename T> // pointer 만..
 void assertNULL( string message, T expected  );
template <typename T> // pointer 만..
 void assertNULL( T expected  );
template <typename T>
 void assertNotNULL( string message, T expected );
template <typename T>
 void assertNotNULL( T expected );

/** To Do
*  need make wizardError.cpp
*/
template <typename T>
inline void assertEquals( T expected, T actual ){
    assertEquals( " is not equals ", expected, actual );
}
template <typename T>
inline void assertEquals( string message, T expected, T actual ){
    if( expected == actual ){
    }
    else{
        throw AssertFail( message ); /// string?
    }
}
template <typename T>
inline void assertNotEquals( string message, T expected, T actual ){
    if( expected != actual ){
    }
    else{
        throw AssertFail( message );
    }
}
template <typename T>
inline void assertNotEquals( T expected, T actual )
{
    assertNotEquals( "is Equals", expected, actual );
}
inline void assertTrue( string message, bool expected ){
    if( expected ){
    }
    else{
        throw AssertFail( message );
    }
}
inline void assertTrue( string message, int expected ){
    if( expected ){
    }
    else{
        throw AssertFail( message );
    }
}
inline void assertTrue( bool expected ){
    assertTrue( "is False", expected );
}
inline void assertTrue( int expected ){
    assertTrue( "is False", expected );
}
inline void assertFalse( string message, bool expected )
{
    if( !expected ){
    }
    else{
        throw AssertFail( message );
    }
}
inline void assertFalse( string message, int expected )
{
    if( !expected ){
    }
    else{
        throw AssertFail( message );
    }
}
inline void assertFalse( bool expected ){
    assertFalse( "is true", expected );
}
inline void assertFalse( int expected ){
    assertFalse( "is false", expected );
}

/// of course,  below T are Pointer Type !!!
template <typename T> // pointer 만..
inline void assertNULL( string message, T expected  ){
    if( expected == NULL ){
    }
    else{
        throw AssertFail( message );
    }

}
template <typename T> // pointer 만..
inline void assertNULL( T expected  ){
    assertNULL( "is not NULL", expected );
}

template <typename T>
inline void assertNotNULL( string message, T expected ){
    if( expected != NULL ){
        //cout << "is not NULL" << endl;
    }
    else{
        throw AssertFail( message );
    }
}

template <typename T>
inline void assertNotNULL( T expected ){
    assertNotNULL( "is NULL", expected );
}

};

#endif // WIZARDERROR_H_INCLUDED
