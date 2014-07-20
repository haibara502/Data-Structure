/** @file */
#ifndef __PRIORITYQUEUE_H
#define __PRIORITYQUEUE_H

#include "ArrayList.h"
#include "ElementNotExist.h"
#include <iostream>
using namespace std;

/**
 * This is a priority queue based on a priority priority queue. The
 * elements of the priority queue are ordered according to their
 * natual ordering (operator<), or by a Comparator provided as the
 * second template parameter.
 * The head of this queue is the least element with respect to the
 * specified ordering (different from C++ STL).
 * The iterator does not return the elements in any particular order.
 * But it is required that the iterator will eventually return every
 * element in this queue (even if removals are performed).
 */

/*----------------------------------------------------------------------*/
/**
 * Default Comparator with respect to natural order (operator<).
 */
 
template <class V>
class Less
{
public:
    bool operator()(const V& a, const V& b) { return a < b; }
};

/**
 * To use this priority queue, users need to either use the
 * default Comparator or provide their own Comparator of this
 * kind.
 * The Comparator should be a class with a public function
 * public: bool operator()(const V&, const V&);
 * overriding the "()" operator.
 * The following code may help you understand how to use a
 * Comparator and, especially, why we override operator().
 */

// #include <iostream>
// using namespace std;
//
// template <class T, class C = Less<T> >
// class Example
// {
// private:
//     C cmp;
// public:
//     bool compare(const T& a, const T& b)
//     {
//         return cmp(a, b);
//     }
// };
//
// int main()
// {
//     Example<int, Less<int> > example; // Less<int> can be omitted.
//     cout << example.compare(1, 2) << endl;
//     cout << example.compare(2, 1) << endl;
// }

/*----------------------------------------------------------------------*/

template <class T, class C = Less<T> >
class PriorityQueue
{
    C cmp;


    class Node
    {
    public:
        T data;
        Node *pre, *next;
		int position;
        Node(Node *pre, T data, Node *next, int position):pre(pre), data(data), next(next), position(position){};
    };

private:
    Node **heap, *head, *tail;
    int currentSize, maxSize;

    void down(int v)
    {
        for (; (v << 1) <= currentSize;)
        {
            int t = v << 1;
            if ((t < currentSize) && (cmp(heap[t + 1] -> data, heap[t] -> data))) ++t;
            if (cmp(heap[v] -> data, heap[t] -> data)) return;
            swap(heap[v], heap[t]);
            heap[v] -> position = v, heap[t] -> position = t;
            v = t;
        }
    }

    void up(int v)
    {
        for (; v > 1; v >>= 1)
        {
            int t = v >> 1;
            if (cmp(heap[t] -> data, heap[v] -> data)) return;
            swap(heap[t], heap[v]);
			heap[t] -> position = t, heap[v] -> position = v;
        }
    }

    void doubleSpace()
    {
        T *tmp = new T[currentSize + 1];
		int i = 1;
        for (Node *temp = head; temp != NULL; temp = temp -> next, ++i) tmp[i] = temp -> data;
        for (int i = 1; i <= currentSize; ++i) if (heap[i] != NULL) delete heap[i];
		delete [] heap;
        maxSize <<= 1;
        heap = new Node*[maxSize];
        head = tail = NULL;
        for (int i = 1; i <= currentSize; ++i)
        {
			heap[i] = new Node(tail, tmp[i], NULL, i);
			if (tail != NULL) tail -> next = heap[i];
			tail = heap[i];
        }
        if (currentSize) head = heap[1];
        delete [] tmp;
        for (int i = currentSize / 2; i; --i) down(i);
    }

public:
    class Iterator
    {
        PriorityQueue *pri;
        Node *now, *tmp;
        T *T_T;
        int status;
    public:
		~Iterator()
		{
			delete tmp; tmp = NULL;
			delete T_T;
		}
        Iterator(PriorityQueue<T, C> *pri):pri(pri), status(-1)
        {
            now = new Node(NULL, *(T_T = new T), pri -> head, -1);
            tmp = now;
            if (pri -> head != NULL) pri -> head -> pre = now;
        }
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext()
        {
            return now -> next != NULL;
        }

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next()
        {
            if (hasNext())
            {
            	status = 0;
                now = now -> next;
                return now -> data;
            }
            else throw ElementNotExist("PriorityQueue:next:ElementNotexist");
        }

		/**
		 * TODO Removes from the underlying collection the last element
		 * returned by the iterator.
		 * The behavior of an iterator is unspecified if the underlying
		 * collection is modified while the iteration is in progress in
		 * any way other than by calling this method.
		 * @throw ElementNotExist
		 */
		void remove() 
		{
			if ((now == NULL) || (now -> next == pri -> head) || (status == -1)) throw ElementNotExist("PriorityQueue:remove:ElementNotExist");
			int pos = now -> position;
			if (pos != pri -> currentSize)
			{
				pri -> heap[pos] = pri -> heap[pri -> currentSize]; pri -> heap[pri -> currentSize--] = NULL;
				pri -> heap[pos] -> position = pos;
				pri -> up(pos), pri -> down(pos);
			}
			else --pri -> currentSize;
			if (now -> pre != NULL) now -> pre -> next = now -> next;
			if (now -> next != NULL) now -> next -> pre = now -> pre;
			if ((now == pri -> head) && (pri -> head == pri -> tail))
				pri -> head = pri -> tail = NULL;
			else 
			{
				if (now == pri -> head) pri -> head = pri -> head -> next;
				if (now == pri -> tail) pri -> tail = pri -> tail -> pre;
			}
			Node *tmp = now;
			now = now -> pre;
			delete tmp; tmp = NULL;
			status = -1;
		}
    };

    /**
     * TODO Constructs an empty priority queue.
     */
    PriorityQueue(int _size = 10):currentSize(0), maxSize(_size)
    {
        heap = new Node*[maxSize];
        head = tail = NULL;
    }

    /**
     * TODO Destructor
     */
    ~PriorityQueue()
    {
		if (heap != NULL)
			for (int i = 1; i <= currentSize; ++i) if (heap[i] != NULL) 
			{
				delete heap[i];
				heap[i] = NULL;
			}
        if (heap != NULL) delete [] heap;
        head = tail = NULL;
    }

    /**
     * TODO Assignment operator
     */
    PriorityQueue &operator=(const PriorityQueue &x)
    {
        if (&x == this) return *this;
		if (heap != NULL)
			for (int i = 1; i <= currentSize; ++i) if (heap[i] != NULL) 
			{
				delete heap[i];
				heap[i] = NULL;
			}
        if (heap != NULL) delete [] heap;
        head = tail = NULL;
        currentSize = x.currentSize, maxSize = x.maxSize;
        heap = new Node*[maxSize];
        Node *tmp = x.head;
        for (int i = 1; i <= currentSize; ++i)
        {
			heap[i] = new Node(tail, tmp -> data, NULL, i);
			if (tail != NULL) tail -> next = heap[i];
			tail = heap[i];
			tmp = tmp -> next;
        }
        if (currentSize) head = heap[1];
        for (int i = currentSize / 2; i; --i) down(i);
    }

    /**
     * TODO Copy-constructor
     */
    PriorityQueue(const PriorityQueue &x)
    {
        head = tail = NULL;
        currentSize = x.currentSize, maxSize = x.maxSize;
        heap = new Node*[maxSize];
        Node *tmp = x.head;
        for (int i = 1; i <= currentSize; ++i)
        {
			heap[i] = new Node(tail, tmp -> data, NULL, i);
			if (tail != NULL) tail -> next = heap[i];
			tail = heap[i];
			tmp = tmp -> next;
        }
        if (currentSize) head = heap[1];
        for (int i = currentSize / 2; i; --i) down(i);
    }

	/**
	 * TODO Initializer_list-constructor
	 * Constructs a priority queue over the elements in this Array List.
     * Requires to finish in O(n) time.
	 */
	PriorityQueue(ArrayList<T> &x)
	{
		head = tail = NULL;
        currentSize = x.size(), maxSize = x.size() + 1;
        heap = new Node*[maxSize];
        class ArrayList<T>::Iterator itr = x.iterator();
        for (int i = 1; itr.hasNext(); ++i)
        {
			heap[i] = new Node(tail, itr.next(), NULL, i);
			if (tail != NULL) tail -> next = heap[i];
			tail = heap[i];
        }
        if (currentSize) head = heap[1];
        for (int i = currentSize / 2; i; --i) down(i);
	}

    /**
     * TODO Returns an iterator over the elements in this priority queue.
     */
    Iterator iterator()
    {
        return Iterator(this);
    }

    /**
     * TODO Removes all of the elements from this priority queue.
     */
    void clear()
    {
		for (int i = 1; i <= currentSize; ++i) if (heap[i] != NULL) 
		{
			delete heap[i];
			heap[i] = NULL;
		}
        head = tail = NULL;
        currentSize = 0;
    }

    /**
     * TODO Returns a const reference to the front of the priority queue.
     * If there are no elements, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const T &front() const
    {
        if (currentSize == 0) throw ElementNotExist("PriorityQueue:front:ElementNotExist");
        return heap[1] -> data;
    }

    /**
     * TODO Returns true if this PriorityQueue contains no elements.
     */
    bool empty() const
    {
        return currentSize == 0;
    }

    /**
     * TODO Add an element to the priority queue.
     */
    void push(const T &value)
    {
        if (currentSize >= maxSize - 1) doubleSpace();
		heap[++currentSize] = new Node(tail, value, NULL, currentSize);
		if (tail != NULL) tail -> next = heap[currentSize];
		tail = heap[currentSize];
        if (head == NULL) head = tail;
        up(currentSize);
    }

    /**
     * TODO Removes the top element of this priority queue if present.
     * If there is no element, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void pop()
    {
        if (head == NULL) throw ElementNotExist("PriorityQueue:pop:ElementNotExist");
        Node *tmp = heap[1]; 
        if ((tmp == head) && (tmp == tail)) head = tail = NULL;
        else 
        {
        	if (tmp == head) head = head -> next;
        	if (tmp == tail) tail = tail -> pre;
        }
        if (tmp -> pre != NULL) tmp -> pre -> next = tmp -> next;
        if (tmp -> next != NULL) tmp -> next -> pre = tmp -> pre;
        delete tmp; tmp = NULL;
        if (currentSize != 1)
        {
        	heap[1] = heap[currentSize]; heap[currentSize--] = NULL;
			heap[1] -> position = 1;
        	down(1);
        }
        else currentSize = 0, head = tail = NULL;
    }

    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const
    {
        return currentSize;
    }
};

#endif
