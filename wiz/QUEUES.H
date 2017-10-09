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
            next =  nullptr;
        }
    };
private:
    Element* Head; //
    Element* Rear; //
public:
    explicit Queue() : Head( nullptr ), Rear( nullptr ) { Head = new Element(); Rear = Head; }

    virtual ~Queue(){
        clear();
        delete Head; Head = Rear = nullptr;
    }
    void clear()
    {
        while( !isEmpty() ){
           deleteq();
        }
     }
    bool isEmpty() const
    {
        return nullptr == Head->next; /// size == 0, empty..
    }
    bool empty() const { return isEmpty(); }
    //
    void addq( const T& p ) /// push
    {
        Element* temp = new Element(p); //
        //
        Rear->next = temp;//
        temp->next = nullptr;// auto look def of Element
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
        if( nullptr == Head->next )
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

        while( qTemp != nullptr )
        {
            addq( qTemp->data );
            qTemp = qTemp->next;
        }
    }
public:
    //
    Queue( const Queue<T>& q ) : Head( nullptr ), Rear( nullptr )
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
        q.Head->next = nullptr;
        q.Rear = q.Head;
    }
    Queue<T>& operator=( Queue<T>&& q )
    {
        if( Head == q.Head ) { return *this; }
        this->clear();
        Head->next = q.Head->next;
        Rear = q.Rear;

		q.Head->next = nullptr;
		q.Rear = q.Head;
		return *this;
	}
};

// Queue using Array
template <class T>
class ArrayQueue
{
public:
	ArrayQueue(const ArrayQueue<T>& aq)
		:que(aq.que), start(aq.start), num(aq.num)
	{
		//
	}
	ArrayQueue(ArrayQueue<T>&& aq)
	{
		que = std::move(aq.que);
		start = aq.start;
		num = aq.num;

		aq.que = Array<T>(2);
		aq.start = 0;
		aq.num = 0;
	}
	ArrayQueue<T>& operator=(ArrayQueue<T>&& aq)
	{
		if (que == aq.que) { return *this; }
		this->que.DoEmpty();
		que = std::move(aq.que);
		start = aq.start;
		num = aq.num;

		aq.que = Array<T>(2);
		aq.start = 0;
		aq.num = 0;
		return *this;
	}
	ArrayQueue<T>& operator=(const ArrayQueue<T>& aq)
	{
		if (que == aq.que) { return *this; }
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
	struct Iter
	{
		T* ptr;
		int pos;

		Iter() { }
		Iter(T* ptr, int pos)
			: ptr(ptr), pos(pos)
		{

		}

		bool operator==(const Iter& other) const {
			return pos == other.pos;
		}
		bool operator!=(const Iter& other) const {
			return pos != other.pos;
		}
	};

	Iter begin(int count = 0) { return Iter(&((*this)[count]), count); }

	Iter end() { return Iter(nullptr, size()); }

	Iter erase(Iter ptr) {
		for (int i = ptr.pos; i < size() - 1; ++i) {
			(*this)[i] = std::move((*this)[i + 1]);
		}
		//(*this)[size() - 1] = T();

		num--;

		return ptr;
	}
public:
	explicit ArrayQueue(const int max = 2) : start(0), num(0)
	{
#ifdef QUEUES_DEBUG
		// max > 0
		wiz::checkUnderError(0, max, 1);
#endif
		que = Array<T>(max);
	}
	virtual ~ArrayQueue()
	{
	}
public:
	const T& operator[](const int idx) const
	{
		return que[(start + idx) & (que.size() - 1)];
	}
	T& operator[](const int idx)
	{
		return que[(start + idx) & (que.size() - 1)];
	}
public:
	void push(const ArrayQueue<T>& val) {
		if (val.empty()) { return; }
		int newSize = this->que.size();

		while (newSize - num - 1 < val.size()) {
			newSize = newSize * 2;
		}
		if (newSize != this->que.size()) {
			// expand array queue.
			ArrayQueue<T> temp(newSize);
			temp.start = 0;
			//
			for (int i = 0; i < this->size(); ++i) {
				temp[i] = std::move((*this)[i]);
			}

			const int iend = val.num;
			for (int i = 0; i < iend; ++i) {
				temp[i + this->num] = val[i];
			}

			temp.num = this->num + val.num;
			*this = std::move(temp);
		}
		else {
			for (int i = 0; i < val.size(); ++i) {
				this->push(val.que[i]);
			}
		}
	}
	void push(ArrayQueue<T>&& val) { // chk..
		if (val.empty()) { return; }
		int newSize = this->que.size();

		while (newSize - num - 1 < val.size()) {
			newSize = newSize * 2;
		}
		if (newSize != this->que.size()) {
			// expand array queue.
			ArrayQueue temp(newSize);
			temp.start = 0;
			//
			for (int i = 0; i < this->size(); ++i) {
				temp[i] = std::move((*this)[i]);
			}

			const int iend = val.num;
			for (int i = 0; i < iend; ++i) {
				temp[i + this->num] = std::move(val[i]);
			}

			temp.num = this->num + val.num;
			*this = std::move(temp);
		}
		else {
			for (int i = 0; i < val.size(); ++i) {
				this->push(std::move(val[i]));
			}
		}
	}
	void push(const T& val)
	{
		if (isFull())
		{
			// expand array queue.
			ArrayQueue temp(que.size() * 2);
			//
			for (int i = 0; i < que.size(); ++i) {
				temp[i] = std::move(que[(start + i) & (que.size() - 1)]);
			}
			temp.start = 0;
			temp.num = que.size() - 1;

			*this = std::move(temp);
		}
		que[(start + num) & (que.size() - 1)] = val;
		num++;
	}

	void push(T&& val)
	{
		if (isFull())
		{
			// expand array queue.
			ArrayQueue temp(que.size() * 2);
			//
			for (int i = 0; i < que.size(); ++i) {
				temp[i] = std::move(que[(start + i) & (que.size() - 1)]);
			}
			temp.start = 0;
			temp.num = que.size() - 1;

			*this = std::move(temp);
		}
		que[(start + num) & (que.size() - 1)] = std::move(val);
		num++;
	}

	void pop(T* t = nullptr) {
		if (isEmpty()) { throw QueueEmptyError(); }

		T temp = std::move(que[start]);

		//que[start] = T();

		start = (start + 1) & (que.size() - 1); // % que.size(), 2^n.
		num--;

		if (nullptr != t) {
			*t = std::move(temp);
		}
	}
	void pop_front(T* t = nullptr) {
		pop(t);
	}

	T pop_back()
	{
		if (isEmpty()) { throw QueueEmptyError(); }
		T temp = std::move((*this)[num - 1]);

		//(*this)[num - 1] = T();

		num--;

		return temp;
	}

	bool isFull()const
	{
		return num >= (que.size() - 1);
	}

	bool isEmpty()const
	{
		return 0 == num;
	}
	int size()const { return num; }
	bool empty()const { return isEmpty(); }
	int getNumMax()const { return que.size(); }
	int getNum()const { return num; }
	T& front() { return (*this)[0]; }
	const T& front()const { return (*this)[0]; }
	Array<T> toArray()const
	{
		Array<T> temp;

		if (num > 0)
		{
			temp = Array<T>(num);
			int count = 0;

			for (int i = 0; i < num; i++)
			{
				temp[i] = que[(start + i) & (que.size() - 1)];
			}
		}

		return temp;
	}

	ArrayQueue<T>& operator<<(const T& data)
	{
		push(data);

		return *this;
	}
public:
	// size must 2^n!, init?
	bool reserve(const int size)
	{
		que = Array<T>(size);
		start = 0;
		num = 0;

		return true;
	}
};
}

#endif // QUEUES_H_INCLUDED
