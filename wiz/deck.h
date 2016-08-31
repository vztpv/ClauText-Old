#ifndef DECK_H_INCLUDED
#define DECK_H_INCLUDED

#include <wiz/wizardError.h>

namespace wiz{

template <class T>
class Deck
{
private:
    class Delement
    {
    public:
        Delement* left; //
        Delement* right;
        T data;
    public:
        //
        Delement(){ left = NULL; right = NULL; } // 초기화 설정.
        Delement( T copy ){  // 기본 대입. // 얕은 복사.
            data = copy; // why this?
            left = NULL; right =  NULL;
        }
    };
public:
    class Diter // d iterator
    {
    private:
        Delement* _de;/// mutual???
    public:
        explicit Diter( Delement* _de ) : _de(_de)
        {
            if( _de == NULL )
            { throw wiz::Error( "_de is NULL" ); }
        }
        //
        void left() { if( NULL == _de ) { throw wiz::Error("_de is NULL"); } _de = _de->left; }
        void right() { if( NULL == _de ) { throw wiz::Error("_de is NULL"); } _de = _de->right; }
        T data()const { return _de->data; }
        bool isNULL()const { return NULL == _de; }
        bool operator!=( const Diter& x ) const
        {
            return x._de != this->_de;
        }
        void operator++() { right(); }
        void operator--() { left(); }
        T& operator*() { return _de->data; }
        const T& operator*()const { return _de->data; }

		T* operator->() { return &(_de->data); }
		const T* operator->()const { return &(_de->data); }
    };
    class ConstDiter // d iterator
    {
    private:
        Delement* _de;/// mutable???
    public:
        explicit ConstDiter( Delement* _de ) : _de(_de)
        {
            if( _de == NULL )
            { throw wiz::Error( "_de is NULL" ); }
        }
        //
        void left()  { if( NULL == _de ) { throw wiz::Error("_de is NULL"); } _de = _de->left; }
        void right() { if( NULL == _de ) { throw wiz::Error("_de is NULL"); } _de = _de->right; }
        T data()const { return _de->data; }
        bool isNULL()const { return NULL == _de; }
        bool operator!=( const ConstDiter& x ) const
        {
            return x._de != this->_de;
        }
        void operator++()  { right(); }
        void operator--()  { left(); }
        ///T& operator*() { return _de->data; }
        const T& operator*()const { return _de->data; }
		const T* operator->()const { return &(_de->data); }
    };
public: /// ToDo
    Deck( const Deck<T>& deck )
    {
        init();

        for( auto x = deck.cbegin(); x != deck.cend(); ++x )
        {
            push_back( *x ); ///
        }
    }
    Deck<T>& operator=( const Deck<T>& deck )
    {
        Deck<T> temp = deck;

        for( auto x = temp.cbegin(); x != temp.cend(); ++x )
        {
            push_back( *x );
        }
        return *this;
    }
private:
    Delement* Head; //
    Delement* Rear; //
	int N;
private:
    void init() /// first...
    {
		N = 0;
        Head = new Delement(); // Head,Rear 고정된 Delement를 가진다.
        Rear = new Delement();  // 그리고 두개가 서로 가르키게 한다.
        Head->left = NULL;
        Head->right = Rear;
        Rear->left = Head;
        Rear->right = NULL;
    }
public:
    explicit Deck(){
        init();
    }
    /// c++11
    Deck( initializer_list<T> args ) // : Deck()
    {
        init();

        for( T _x : args )
        {
            push_back( _x );
        }
    }
    ///
    virtual ~Deck(){
        while( !isEmpty() ){
             pop_back();
        }
        delete Head; delete Rear;
    }
    bool isEmpty()const{
       return ( Rear->left == Head ); // Rear의 left가 head를 가르키면 Empty.
    }
    bool empty() const{ return isEmpty(); }
    //
    void push_back( const T& p ){
		N++;
        /////////////////////////////////////////////////////
        // New한다.
        Delement* temp = new Delement( p ); //
        //
        //d double linked list
        temp->right = Rear; // 오른 쪽은 Right.!
        temp->left  = Rear->left; // 끝부분이 가르키고 있는 값을 left에 넣는다.
        temp->left->right = temp; // 그리고 temp의 왼쪽 element의 right에 temp를 연결한다.
        Rear->left = temp;
        //////////////////////////////////////////////////////
    }//
    T pop_front(){
		N--;
        ////////////////////////////
        if( isEmpty() ) // 예외 검사.
           { throw wiz::Error( "for pop, empty deck.." ); }//return NULL;

        Delement* temp = Head->right->right; //
        // Rear 처리
        T returnTemp = Head->right->data;
        delete Head->right;
        temp->left = Head;
        Head->right = temp;

        return returnTemp;
        //
    }

    void push_front( const T& p ) {
		N++;
        Delement* temp = new Delement( p );

        temp->right = Head->right;
        temp->left = Head;
        temp->right->left = temp;
        Head->right = temp;
    }
    T pop_back() {
        if( isEmpty() ) { throw wiz::Error( "for pop, empty deck.." ); }
        Delement* temp = Rear->left->left;

        T returnTemp = Rear->left->data;
        delete Rear->left;
        temp->right = Rear;
        Rear->left = temp;
		
		N--;
        return returnTemp;
    }
	
private:
    void remove( Delement* z )
    {
        z->left->right = z->right;
        z->right->left = z->left;
		delete z;
    }
public:
    void remove( const T& data ) /// if data exist then remove just one data!, no delete memory.
    {
        for( Diter iter=begin(); iter != end(); ++iter )
        {
            if( (*iter) == data )
            {
                remove( &iter );
                break;
            }
        }
    }
	
    //
    Diter begin() { return Diter(Head->right); }
    Diter rbegin() { return Diter(Rear->left); }

    Diter rend() { return Diter(Head); }
    Diter end() { return Diter(Rear); }

    /// chk..
    ConstDiter cbegin() const{ return ConstDiter(Head->right); }
    ConstDiter crbegin() const{ return ConstDiter(Rear->left); }

    ConstDiter crend() const{ return ConstDiter(Head); }
    ConstDiter cend() const{ return ConstDiter(Rear); }

	int size()const { return N; }
public:
    void deck_union( Deck<T>& deck )
    {
        if( this == &deck ) { return; } /// throw error??
        while( !deck.empty() )
            push_back( deck.pop_front() );
    }
};


}

#endif // DECK_H_INCLUDED
