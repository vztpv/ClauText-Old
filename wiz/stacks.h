#ifndef STACKS_H_INCLUDED
#define STACKS_H_INCLUDED

#include <wiz/global.h>
#include <wiz/wizardError.h>
#include <wiz/newarrays.h>

/// #define STACKS_DEBUG

namespace wiz {

class StackEmptyError : public wiz::Error
{
public:
    StackEmptyError() : wiz::Error( "sEmptyError" )
    {

    }
};
class StackFullError : public wiz::Error
{
public:
    StackFullError() : wiz::Error( "sFullError" )
    {

    }
};

template <class T>
class Stack
{
    class Element
    {
    public:
        Element* next;
        T data;
    public:
        //
        explicit Element( const T& d=T() )
        {  // 기본 대입. // 얕은 복사.
            data = d; // why this?
            next = NULL;
        }
    };
private:
    void copy( const Stack<T>& s )
    {
        /// this->clear();
        Element* pE = s.Rear;
        Stack<T> temp;
        while( NULL != pE )
        {
            temp.push( pE->data );
            pE = pE->next;
        }
        pE = temp.Rear;
        while( NULL != pE )
        {
            this->push( pE->data );
            pE = pE->next;
        }
    }
public:
    Stack( const Stack<T>& s )
    {
        num = 0;
        Rear = NULL;
        copy( s );
    }
    Stack<T>& operator=( const Stack<T>& s )
    {
        if( this->Rear == s.Rear ) { return *this; }
        clear();
        copy( s );
        return *this;
    }
    Stack( Stack<T>&& s )
    {
        Rear = s.Rear;
        num = s.num;

        s.Rear = NULL;
        s.num = 0;
    }
    Stack<T>& operator=( Stack<T>&& s )
    {
        if( Rear == s.Rear ) { return *this; }
        clear();
        Rear = s.Rear;
        num = s.num;

        s.Rear = NULL;
        s.num = 0;
        return *this;
    }
private:
    Element* Rear;// 꼬리에 데이터를 넣는다.
    int num;  // for toArray
public:
    explicit Stack() : Rear( NULL ), num( 0 ) { }
    ~Stack(){
        clear();
    }
    void clear() {
        while( !isEmpty() ){
            pop();
        }
    }
    bool isEmpty()const{ // 비었나? //
        return NULL == Rear; // num <= 0
    }
    bool empty() const{ return isEmpty(); }
    //
    void push( const T& p ){ // 넣기
        Element* temp = new Element(p); //
        //Rear
        temp->next = Rear; // Rear가 가르키던 값을 가르키게 한다.
        Rear = temp; //새로 들어오는 것을 기준으로 한다
        //
        num++;
    }
    //
    T pop(){ // 빼기... cf  Rear가 고정, 데이터가 이동하는 것 처럼 행동.
        // 예외 테스트..
        if( isEmpty() )
        {
            throw StackEmptyError();
        }
        //
        Element* temp = Rear; // 현재 가르키는 것.
        Rear = Rear->next; // Rear에는 ..
        T returnTemp = (temp->data);
        delete temp;
        //
        num--;
        //
        return returnTemp; //return이 없어도 컴파일이 되다니;;; - in CodeBlocks.
    }
    T getTop()const
    {
        if( isEmpty() )
        {
            throw StackEmptyError();
        }
        return (Rear->data);
    }
    void setTop( const T& val )
    {
        if( isEmpty() )
        {
            throw StackEmptyError();
        }
        Rear->data = val;
    }
    T& top()
    {
        if( isEmpty() )
        {
            throw StackEmptyError();
        }
        else
        {
            return Rear->data;
        }
    }
    const T& top() const
    {
        if( isEmpty() )
        {
            throw StackEmptyError();
        }
        else
        {
            return Rear->data;
        }
    }

    // 스택에 넣는다.
    Stack<T>& operator<<( const T& p ){
        push(p);
        return *this;
    }

    Array<T> toArray()const
    {
        if( isEmpty() ) { return Array<T>(); }
        Array<T> arrayTemp( num );
        Element* eTemp = Rear; // ElementTemp
        for( int i=0; i < arrayTemp.size(); i++ )
        {
            arrayTemp[i] = eTemp->data;
            eTemp = eTemp->next;
        }

        return arrayTemp;
    }

    int getNum()const { return num; }
};

template <class T>
class ArrayStack
{
public:
    ArrayStack( const ArrayStack<T>& s )
    {
        p = s.p;
        count = s.count;
    }
    ArrayStack<T>& operator=( const ArrayStack<T>& s )
    {
        if( p == s.p ) { return *this; }
        p.DoEmpty();
        p = s.p;
        count = s.count;
        return *this;
    }
    ArrayStack<T>& operator=( ArrayStack<T>&& s )
    {
        if( p == s.p ) { return *this; }
        p = std::move( s.p );
        count = s.count;

        s.p = Array<T>( 1 );
        s.count = 0;
        return *this;
    }
    ArrayStack( ArrayStack<T>&& s )
    {
        p = std::move( s.p );
        count = s.count;

        s.p = Array<T>( 1 );
        s.count = 0;
    }
private:
    Array<T> p;
    int count;
public:
    explicit ArrayStack( const int size = 1 )
    : count(0)
    {
        #ifdef STACKS_DEBUG
        wiz::checkUnderError( 0, size, 1 ); // must size >= 1
        #endif
        p = Array<T>(size);
    }
    virtual ~ArrayStack() {  } // added!! 2012.06.04
public:
    // push
    void push( const T& data )
    {
        if( count >= p.size() )
        {
            // changed 2012.06.04
            p.expand();
        }

        p[count] = data;
        count++;
    }

    // pop
    T pop()
    {
        if( isEmpty() )
        {
            throw StackEmptyError();
        }
        else
        {
            T temp = p[count-1];
            p[count-1] = T();
            count--;
            return temp;
        }
    }
    void clear()
    {
        for( int i=0; i < count; i++ )
        {
            p[i] = T();
        }
        count = 0;
    }
	const T& Top()const { return p[count = 1]; }
	T& Top() { return p[count - 1]; }
    const T getTop()const
    {
        if( isEmpty() )
        {
            throw StackEmptyError();
        }
        else
        {
            return p[count-1];
        }
    }
    void setTop( const T& val )
    {
        if( isEmpty() )
        {
            throw StackEmptyError();
        }
        else
        {
            p[count-1] = val;
        }
    }
    T& top()
    {
        if( isEmpty() )
        {
            throw StackEmptyError();
        }
        else
        {
            return p[count-1];
        }
    }
    const T& top() const
    {
        if( isEmpty() )
        {
            throw StackEmptyError();
        }
        else
        {
            return p[count-1];
        }
    }

    ArrayStack<T>& operator<<( const T& var )
    {
        push( var );

        return *this;
    }

    bool isEmpty() const { return count <= 0; }
    bool empty() const { return count <= 0; }
    Array<T> toArray()const {
        if( count <= 0 ) { return Array<T>(); }
        Array<T> temp( count );

        for( int i=0; i < temp.size(); i++ )
        {
            temp[i] = p[i];
        }

        return temp;
    }
    // int getMaxNum()const
    int size()const { return count; }
    int getNum()const
    {
        return count;
    }
    int getNumMax()const
    {
        return p.size();
    }
    T& operator[] ( const int idx )
    {
        /// #define?  assert( idx is index 0 ~ count-1 )
        return p[idx];
    }
	const T& operator[](const int idx) const
	{
		return p[idx];
	}
};

}
#endif // STACKS_H_INCLUDED
