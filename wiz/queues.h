#ifndef QUEUES_H_INCLUDED
#define QUEUES_H_INCLUDED

#include <wiz/global.h>
#include <wiz/wizardError.h>
#include <wiz/newarrays.h>

/// #define QUEUES_DEBUG

namespace wiz{
class QueueEmptyError : public wiz::Error
{
public:
    QueueEmptyError() : wiz::Error( "queueEmptyError" )
    {

    }
};
class QueueFullError : public wiz::Error
{
public:
    QueueFullError() : wiz::Error( "queueFullError" )
    {

    }
};

// check array Queue, array stack?
template <typename T>
class Queue //: public wizObject
{
private:
    class Element
    {
    public:
        Element* next;
        T data;
    public:
        explicit Element( const T d = T() )
        {
            data = d;
            next =  NULL;
        }
    };
private:
    Element* Head; //
    Element* Rear; //
public:
    explicit Queue() : Head( NULL ), Rear( NULL ) { Head = new Element(); Rear = Head; }

    virtual ~Queue(){
        clear();
        delete Head; Head = Rear = NULL;
    }
    void clear()
    {
        while( !isEmpty() ){
           deleteq();
        }
     }
    bool isEmpty() const
    {
        return NULL == Head->next; /// size == 0, empty..
    }
    bool empty() const { return isEmpty(); }
    //
    void addq( const T& p ) /// push
    {
        Element* temp = new Element(p); //
        //
        Rear->next = temp;//
        temp->next = NULL;// auto look def of Element
        Rear = Rear->next; //
    }//
    void push( const T& p ) { addq( p ); }
    T deleteq(){ /// pop
        if( isEmpty() )
        {
            throw QueueEmptyError();
        }
        //
        Element* temp = Head->next;//
        Head->next = temp->next;
        //Rear 처리
        if( NULL == Head->next )
            Rear = Head;
        T returnTemp = temp->data;
        delete temp;
        return returnTemp;
    }
    T pop() { return deleteq(); }
    Queue& operator<<( const T& p ){
        addq( p ); //
        return *this;
    }
private:
    void copy( const Queue<T>& q )
    {
        //this->clear();

        Element* qTemp = q.Head->next;

        while( qTemp != NULL )
        {
            addq( qTemp->data );
            qTemp = qTemp->next;
        }
    }
public:
    //
    Queue( const Queue<T>& q ) : Head( NULL ), Rear( NULL )
    {// clear();
        Head = new Element(); Rear = Head;
        copy( q );
    }
    Queue& operator=( const Queue<T>& q )
    {
        if( Head == q.Head ) { return *this; }

        clear();

        // head, rear reser..!!
        copy( q );

        return *this;
    }
    Queue( Queue<T>&& q )
    {
        Head->next = q.Head->next;
        Rear = q.Rear;

        /// do-empty..
        q.Head->next = NULL;
        q.Rear = q.Head;
    }
    Queue<T>& operator=( Queue<T>&& q )
    {
        if( Head == q.Head ) { return *this; }
        this->clear();
        Head->next = q.Head->next;
        Rear = q.Rear;

        q.Head->next = NULL;
        q.Rear = q.Head;
        return *this;
    }
};

// Queue using Array
template <class T>
class ArrayQueue
{
public:
    ArrayQueue( const ArrayQueue<T>& aq )
    {
        this->que = aq.que;
        this->start = aq.start;
        this->num = aq.num;
    }
    ArrayQueue( ArrayQueue<T>&& aq )
    {
        que = std::move( aq.que );
        start = aq.start;
        num = aq.num;

        aq.que = Array<T>( 1 );
        aq.start = 0;
        aq.num = 0;
    }
    ArrayQueue<T>& operator=( ArrayQueue<T>&& aq )
    {
        if( que == aq.que ) { return *this; }
        this->que.DoEmpty();
        que = std::move( aq.que );
        start = aq.start;
        num = aq.num;

        aq.que = Array<T>( 1 );
        aq.start = 0;
        aq.num = 0;
        return *this;
    }
    ArrayQueue<T>& operator=( const ArrayQueue<T>& aq )
    {
        if( que == aq.que ) { return *this; }
        que.DoEmpty();
        this->que = aq.que;
        this->start = aq.start;
        this->num = aq.num;
        return *this;
    }
private:
    Array<T> que;
    int start;
    int num;
public:
    explicit ArrayQueue( const int max=1 ) : start(0), num(0)
    {
        #ifdef QUEUES_DEBUG
        // max > 0
        wiz::checkUnderError( 0, max, 1 );
        #endif
        que = Array<T>( max );
    }
    virtual ~ArrayQueue()
    {
    }
public:
	const T& operator[](const int idx) const
	{
		return que[(start + idx) % que.size()];
	}
	T& operator[](const int idx)
	{
		return que[(start + idx) % que.size()];
	}
public:
    void push( const T& val )
    {
        if( isFull() )
        {
            // expand array queue.
            ArrayQueue temp( que.size() * 2 );
            //
            while( !this->isEmpty() ) {
                temp.push( this->pop() );
            }
            *this = std::move( temp );
        }
        que[(start+num) % (que.size())] = val;
        num++;
    }

    T pop()
    {
        if( isEmpty() ) { throw QueueEmptyError(); }

        T temp = que[start];

        que[start] = T();

        start = ( start + 1 ) % (que.size());
        num--;

        return temp;
    }

    bool isFull()const
    {
        return num >= ( que.size() );
    }

    bool isEmpty()const
    {
        return 0 == num;
    }
    int size()const{ return num; }
    bool empty()const { return isEmpty(); }
    int getNumMax()const { return que.size(); }
    int getNum()const { return num; }
    Array<T> toArray()const
    {
        Array<T> temp;

        if( num > 0 )
        {
            temp = Array<T>( num );
            int count=0;

            for( int i=0; i < num; i++ )
            {
                temp[i] = que[ ( start + i ) % (que.size()) ];
            }
        }

        return temp;
    }

    ArrayQueue<T>& operator<<( const T& data )
    {
        push( data );

        return *this;
    }
};

}
#endif // QUEUES_H_INCLUDED
