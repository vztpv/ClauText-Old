#ifndef NEWARRAYS_H_INCLUDED
#define NEWARRAYS_H_INCLUDED
///////////////////////////

/// #define ARRAYS_DEBUG

///////////////////////////
#include <wiz/global.h>
#include <wiz/wizardError.h>

namespace wiz{
template <class T>
class Array{
private:
    T* p; // array
    int N; // array 사이즈
private:
    static T sBegin; /// static -> s, /// to not static?
    static T sEnd;

private:
    void resetP( const T* p )
    {
        if( NULL != this->p && NULL != p )
        {
            for( int i=0;i< N;i++ ){
                this->p[i] = p[i];
            }
        }
    }
///for c++11
public:
    class Iter //
    {
    private:
        T* _pt;//
        /// following constant values..
        T* p;
        int N;
        T* pBegin;
        T* pEnd;
    public:
        explicit Iter( T* _pt, T* p, int N, T* pBegin, T* pEnd ) : _pt(_pt),
        p(p), N(N), pBegin(pBegin), pEnd(pEnd)
        {
            if( NULL == _pt ) { throw wiz::Error( "NULL == _pt in Iter" ); }
        }
        //
        void left() {
            // p == NULL -> err.
            if( p == _pt ) { _pt = pBegin; }
            else if( _pt == pBegin ) { throw wiz::Error( " _pt == pBegin in left " ); }
            else if( _pt == pEnd ) { _pt = p+N-1; }
            else {
                _pt--;
            }
        }
        void right() {
            // p == NULL -> err.
            if( p+N-1 == _pt ) { _pt = pEnd; }
            else if( _pt == pEnd ) { throw wiz::Error( " _pt == pEnd in right " ); }
            else if( _pt == pBegin ) { _pt = p; }
            else {
                _pt++;
            }
        }
        T data()const { return *_pt; }
        bool isNULL()const { return NULL == _pt; }
        bool operator!=( const Iter& x ) const
        {
            return x._pt != this->_pt;
        }
        void operator++() { right(); }
        void operator--() { left(); }
        /// TO DO
        /// T* operator->()
        /// const T* operator->()const
        T* operator->() { return _pt; }
        const T* operator->()const { return _pt; }
        T& operator*() { return *_pt; }
        const T& operator*()const { return *_pt; }
    };
    class ConstIter //
    {
    private:
        T* _pt;//
        /// following constant values..
        T* p;
        int N;
        T* pBegin;
        T* pEnd;
    public:
        explicit ConstIter( T* _pt, T* p, int N, T* pBegin, T* pEnd ) : _pt(_pt),
        p(p), N(N), pBegin(pBegin), pEnd(pEnd)
        {
            if( NULL == _pt ) { throw wiz::Error( "NULL == _pt in ConstIter" ); }
        }
        //
        void left() {
            // p == NULL -> err.
            if( p == _pt ) { _pt = pBegin; }
            else if( _pt == pBegin ) { throw wiz::Error( " _pt == pBegin in left " ); }
            else if( _pt == pEnd ) { _pt = p+N-1; }
            else {
                _pt--;
            }
        }
        void right() {
            // p == NULL -> err.
            if( p+N-1 == _pt ) { _pt = pEnd; }
            else if( _pt == pEnd ) { throw wiz::Error( " _pt == pEnd in right " ); }
            else if( _pt == pBegin ) { _pt = p; }
            else {
                _pt++;
            }
        }
        T data()const { return *_pt; }
        bool isNULL()const { return NULL == _pt; }
        bool operator!=( const ConstIter& x ) const
        {
            return x._pt != this->_pt;
        }
        void operator++() { right(); }
        void operator--() { left(); }

        const T* operator->()const { return _pt; }
        const T& operator*()const { return *_pt; }
    };
    Iter begin()  { if( empty() ) { return end(); }
        return Iter( p, p, N, &sBegin, &sEnd );
    }
    Iter end()  { return Iter( &sEnd, p, N, &sBegin, &sEnd ); }
    Iter rbegin()  { if( empty() ){ return rend(); }
        return Iter( p+N-1, p, N, &sBegin, &sEnd );
    }

    Iter rend()  { return Iter( &sBegin, p, N, &sBegin, &sEnd ); }
    /// Check....
    ConstIter crend() const { return ConstIter( &sBegin, p, N, &sBegin, &sEnd ); }
    ConstIter cbegin() const { if( empty() ) { return cend(); }
        return ConstIter( p, p, N, &sBegin, &sEnd );
    }
    ConstIter cend() const { return ConstIter( &sEnd, p, N, &sBegin, &sEnd ); }
    ConstIter crbegin() const { if( empty() ){ return crend(); }
        return ConstIter( p+N-1, p, N, &sBegin, &sEnd );
    }


    explicit Array( initializer_list<T> args )
    {
        N = args.size();

        #ifdef ARRAYS_DEBUG
        wiz::checkUnderError( 1, N, 0 );
        #endif

        if( 0 == N ) { p = NULL; return; }
        p = new T[N];

        int count = 0;
        for( auto iter = args.begin(); iter != args.end(); ++iter )
        {
            p[count] = *iter;
            count++;
        }

    }

    Array<T>& operator=( Array<T>&& arr )
    {
        if( p == arr.p ) { return *this; }

        if( p ) { delete[] p; }
        p = arr.p; arr.p = NULL;
        N = arr.N; arr.N = 0;

        return *this;
    }
    Array( Array<T>&& arr )
    {
        p = arr.p; arr.p = NULL;
        N = arr.N; arr.N = 0;
    }

//
public:
    explicit Array( const int _N, const T reset_val=T() ) : N(_N)
    {
        #ifdef ARRAYS_DEBUG
        wiz::checkUnderError( 2, N, 1 );
        #endif
        p = new T[_N];

        reset( reset_val );
    }

    Array( const Array<T>& _arr )
    : p(NULL), N(0)
    {
        *this = _arr;
    }
    explicit Array() : p(NULL), N(0)
    { }

    virtual ~Array()
    {
        if( p )
            delete[] p;
    }

    void reset( const T& resetValue )
    {
        for( int i=0;i< N;i++ )//
            p[i] = resetValue;
    }
    bool operator==( const Array<T>& arr ) const /// 주의할 것..
    {
        return this->p == arr.p;
    }
    bool operator!=( const Array<T>& arr ) const
    {
        return this->p != arr.p;
    }
    /// cf) isSameValues?
    template<class IS_SAME_VALUE=EE<T>> /// need more thinking..
    bool isSameValue( const Array<T>& arr ) const
    {
        if( arr.size() != size() ) { return false; }
        for( int i=0; i < arr.size(); i++ )
        {
            if( !( IS_SAME_VALUE()( arr[i], p[i] ) ) ) { return false; }
        }
        return true;
    }
    // 크기
    int size()const{ return N; }
    int length()const{ return N; }
    //
    T& operator[]( const int index )
    {
        #ifdef ARRAYS_DEBUG
        wiz::checkIndexError( index, N );
        #endif
		return p[index];
    }
    const T& operator[]( const int index )const
    {
        #ifdef ARRAYS_DEBUG
        wiz::checkIndexError( index, N );
        #endif
		return p[index];
    }
    //
    Array<T>& operator=( const Array<T>& _p )
    {
        if( _p.p == p ) { return *this; }

        if( NULL != p && size() != _p.size() )
        {
            delete[] p;
            p = NULL;
        }

		if( NULL == p && NULL != _p.p )
        {
            p = new T[_p.N];
        }

        //reset
        N = _p.N;
        resetP( _p.p );

        return *this;
    }

    bool empty()const
    {
    	return NULL == p;
    }
    bool isValid()const
    {
        return !empty();
    }
    void DoEmpty()
    {
        if( p ) delete[] p;
        p = NULL;
        N = 0;
    }
public:
	void expand()
	{
		if( NULL != p )
		{
		    // 2012.5.8
			const int newN = 2 * N;
            T* temp = new T[2*N]; // new Array!

            // data copy!
		    for( int i=0; i < N; i++ )
			{
				temp[i] = std::move( p[i] ); // cf)  간단한것은 p[i]가 더 빠르다???
			}
			for( int i=N; i < newN; i++ )
			{
                temp[i] = T();
			}

            ///
            delete[] p;
            p = temp; N = newN;
		}
	}

public: /// Done-swap..
    void swap( Array<T>& arr )
    {
        wiz::Swap( this->p, arr.p );
        wiz::Swap( this->N, arr.N );
    }
};// end class array
template <class T>
T Array<T>::sBegin = T();
template <class T>
T Array<T>::sEnd = T();
// not use debug() and use below!
template <class T>
inline ostream& operator<<( ostream& stream, const Array<T>& arr )
{
    if( arr.empty() ) { return stream; }

    for( int index=0; index < arr.length()-1; index++ )
        stream << arr[index] << " ";
    stream << arr[arr.length()-1]; // chk 2012.7.30

    return stream;
}
template <class T>
inline istream& operator>>( istream& stream, Array<T>& arr )
{
    for( int i=0; i < arr.size(); ++i )
    {
        stream >> arr[i];
    }

    return stream;
}
template <class T>
class SecondArray{ /// To Do Add bool isSameValue?
private:
    Array<T>** p;
    int row_N; // array 사이즈
    int column_N;

private:
    void Delete() /// to public??
    {
		if( NULL != p )
        {
			for( int i=0;i< row_N;i++ )
			{
				if( p[i] ){ delete p[i]; p[i] = NULL; }
			}
			delete[] p;//
            p = NULL;
        }
        row_N = 0; column_N = 0;
    }

    void reset( const int row_N, const int column_N )
    {
        #ifdef ARRAYS_DEBUG
        checkUnderError( 4, row_N, 0 );
        checkUnderError( 5, column_N, 0 );
        #endif

        this->row_N = row_N;
        this->column_N = column_N;
    }

public:
    explicit SecondArray()
	:  p( NULL ), row_N( 0 ), column_N( 0 )
	{
        //
	}
    explicit SecondArray( const int _row_N, const int _column_N,  const T _resetValue=T() )
    : row_N(_row_N), column_N(_column_N)
    {
        #ifdef ARRAYS_DEBUG
        wiz::checkUnderError( 6, row_N, 1 );
        wiz::checkUnderError( 7, column_N, 1 );
        #endif

        p = new Array<T>*[row_N];
        for( int i=0;i< row_N;i++ )
        {
            p[i] = new Array<T>( column_N, _resetValue );
        }
    }

    virtual ~SecondArray()
    {
        Delete();
    }
    // 최대 크기를 받는다.
    SecondArray( const SecondArray<T>& dap )
    : p( NULL ), row_N( 0 ), column_N( 0 )
    {
        *this = dap;
    }

    SecondArray<T>& operator=( const SecondArray<T>& dap ) /// need more thinking...
    {
        if( dap.p == p ) { return *this; }

        /// NULL != p && NULL == dap.p -> ( NULL!=p && other size ) is true.
        if( NULL != p && ( row_N != dap.row_N || column_N != dap.column_N ) )
        /// NULL != p && NULL == dap.p
        /// NULL != p && NULL != dap.p && ( different row_size or different col_size )
        {
            for( int i=0; i < this->row_N; ++i )
            {
                delete p[i];
            }
            delete[] p;
            p = NULL;
        }

    	if( NULL == p && NULL != dap.p )
        {
            p = new Array<T>*[dap.row_N];
            for( int i=0;i< dap.row_N;i++ )
            {
                p[i] = new Array<T>( *(dap.p[i]) );
            }
        }
        else /// else if( NULL != p && NULL != dap.p && same row_size && same col_size )
             /// else if( NULL == p && NULL == dap.p ) -> dap.row_N == 0.
        {
            for( int i=0; i < dap.row_N; i++ )
            {
                (*p[i]) = (*dap.p[i]);
            }
        }

        reset( dap.row_N, dap.column_N );
        //
        return *this;
    }

    /// DONE 2014.3.7 - 이동생성자, 이동대입연산자 추가.
    SecondArray<T>& operator=( SecondArray<T>&& arr )
    {
        if( p == arr.p ) { return *this; }
        if( p ) { Delete(); }
        p = arr.p;
        row_N = arr.row_N;
        column_N = arr.column_N;

        arr.p = NULL;
        arr.row_N = 0;
        arr.column_N = 0;
        return *this;
    }
    SecondArray( SecondArray<T>&& arr )
    {
        p = arr.p; arr.p = NULL;
        row_N = arr.row_N; arr.row_N = 0;
        column_N = arr.column_N; arr.column_N = 0;
    }

    /// chk [i].size() == column_N ??
    T getValue( const int i, const int j )const
    {
        #ifdef ARRAYS_DEBUG
        wiz::checkIndexError( i, row_N );
        wiz::checkIndexError( j, column_N );
        #endif
        return (*p[i])[j]; //
    }

    void setValue( const int i, const int j, const T& set )
    {
        #ifdef ARRAYS_DEBUG
        wiz::checkIndexError( i, row_N );
        wiz::checkIndexError( j, column_N );
        #endif
        (*p[i])[j] = set;
    }

    // get size
    int getRowN()const{ return row_N; }
    int getColumnN()const{ return column_N; }

    void reset( const T& val )
    {
        for( int i=0;i< row_N;i++ ){
            for( int j=0;j< column_N;j++ ){
                (*p[i])[j] = val;
            }
        }
    }

    /// chk [i].size() == column_N ??
    Array<T>& operator[]( const int index )
    {
        #ifdef ARRAYS_DEBUG
        wiz::checkIndexError( index, row_N );
        wiz::assertEquals( p[index]->size(), column_N );
        #endif
        return *(p[index]);
    }

    const Array<T>& operator[]( const int index )const
    {
        #ifdef ARRAYS_DEBUG
        wiz::checkIndexError( index, row_N );
        #endif
        return *(p[index]);
    }

    bool empty() const
    {
        return NULL == p;
    }
	bool isValid()const
    {
    	return NULL != p;
    }
   //Array<T> getColumnArray( const int columnNo );
   //Array<T> getRowArray( const int rowNo );
public:
    virtual bool isSquareSecondArray(){ return false; }
    /// ToDo swap
    void swap( SecondArray<T>& sarr )
    {
        wiz::Swap( this->p, sarr.p );
        wiz::Swap( this->row_N, sarr.row_N );
        wiz::Swap( this->column_N, sarr.column_N );
    }

    /// ToDO row_expand, col_expand, row_col_expand. - *2, *2, *4.
    void row_expand()
    {
        if( isSquareSecondArray() ) { return; } /// throw err.?
        if( NULL != p ) {
            const int new_row_size = 2 * row_N;
            Array<T>** temp = new Array<T>*[new_row_size];

            for( int i=0; i < row_N; i++ ) {
                temp[i] = p[i];
            }
            for( int i=row_N; i < new_row_size; i++ ) {
                temp[i] = new Array<T>( column_N, T() );
            }
            delete[] p;
            p = temp;
            row_N = new_row_size;
        }
    }
};// end double array
template <typename T>
class SquareSecondArray : public SecondArray<T> /// row_expand -> err. 상속???
{
public:
    virtual bool isSquareSecondArray(){ return true; }
public:
    SquareSecondArray() : SecondArray<T>() { }
    virtual ~SquareSecondArray() { }
    SquareSecondArray( const SquareSecondArray& sda ) : SecondArray<T>( sda ) { }
    SquareSecondArray( const int N, const T init=T() ):SecondArray<T>( N, N, init ){
        //
    }

    int getN()const{ return SecondArray<T>::getColumnN(); }

    void Transpose() // only n*n!// cf) return transposed Square Double Array?
    { //
        const int N = SecondArray<T>::getRowN();

        for( int i=0;i < N; i++ ){
            for( int j=i+1;j < N; j++ ){
                // swap [i][j] and [j][i];
                wiz::Swap( (*this)[i][j], (*this)[j][i] );
            }
        }
    }
};
// not use debug() and use below!
template <class T>
inline ostream& operator<<( ostream& stream, const SecondArray<T>& arr )
{
    for( int i=0; i < arr.getRowN(); i++ )
    {
        stream << arr[i];
        stream << "\n";
    }

    return stream;
}

template <class T>
inline istream& operator>>( istream& stream, SecondArray<T>& arr )
{
    for( int i=0; i < arr.getRowN(); ++i )
    {
        stream >> arr[i];
    }

    return stream;
}
}
#endif // NEWARRAYS_H_INCLUDED
