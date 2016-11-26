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
        Delement(){ left = NULL; right = NULL; } // ì´ˆê¸°???¤ì •.
        Delement( T copy ){  // ê¸°ë³¸ ?€?? // ?•ì? ë³µì‚¬.
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
        Head = new Delement(); // Head,Rear ê³ ì •??Delementë¥?ê°€ì§„ë‹¤.
        Rear = new Delement();  // ê·¸ë¦¬ê³??ê°œê°€ ?œë¡œ ê°€ë¥´í‚¤ê²??œë‹¤.
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
       return ( Rear->left == Head ); // Rear??leftê°€ headë¥?ê°€ë¥´í‚¤ë©?Empty.
    }
    bool empty() const{ return isEmpty(); }
    //
    void push_back( const T& p ){
		N++;
        /////////////////////////////////////////////////////
        // New?œë‹¤.
        Delement* temp = new Delement( p ); //
        //
        //d double linked list
        temp->right = Rear; // ?¤ë¥¸ ìª½ì? Right.!
        temp->left  = Rear->left; // ?ë?ë¶„ì´ ê°€ë¥´í‚¤ê³??ˆëŠ” ê°’ì„ left???£ëŠ”??
        temp->left->right = temp; // ê·¸ë¦¬ê³?temp???¼ìª½ element??right??tempë¥??°ê²°?œë‹¤.
        Rear->left = temp;
        //////////////////////////////////////////////////////
    }//
    T pop_front(){
		N--;
        ////////////////////////////
        if( isEmpty() ) // ?ˆì™¸ ê²€??
           { throw wiz::Error( "for pop, empty deck.." ); }//return NULL;

        Delement* temp = Head->right->right; //
        // Rear ì²˜ë¦¬
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
