/** @file */
#ifndef __DEQUE_H
#define __DEQUE_H

#include "ElementNotExist.h"
#include "IndexOutOfBound.h"

/**
 * An deque is a linear collection that supports element insertion and removal at both ends.
 * The name deque is short for "double ended queue" and is usually pronounced "deck".
 * Remember: all functions but "contains" and "clear" should be finished in O(1) time.
 *
 * You need to implement both iterators in proper sequential order and ones in reverse sequential order. 
 */
template <class T>
class Deque
{
	T *data;
	int left, right, maxSize;
	
	void doubleSpace()
	{
		T *tmp = new T[maxSize];
		for (int i = left; i <= right; ++i) tmp[i] = data[i];
		delete [] data;
		maxSize <<= 2;
		data = new T[maxSize];
		int mid0 = (left + right) / 2, mid1 = maxSize / 2;
		int left1 = mid1 - (mid0 - left), right1 = right - mid0 + mid1;
		for (int i = left1; i <= right1; ++i) data[i] = tmp[i - left1 + left];
		left = left1, right = right1;
		delete [] tmp;
	}
	
public:
    class Iterator
    {
		Deque<T> *deq;
		int status, position, deleted;
    public:
	
		Iterator(Deque<T> *deq, int status = 1):status(status), deq(deq), deleted(-1)
		{
			if (status == 1) position = deq -> left - 1;
			else position = deq -> right + 1;
		};
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() 
		{
			if (deq -> left == -1) return false;
			if ((position + status >= deq -> left) && (position + status <= deq -> right)) return true;
			return false;
		}

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next() 
		{
			if (!hasNext()) throw ElementNotExist("Deque:next:ElementNotExist");
			position += status;
			deleted = 0;
			return deq -> data[position];
		}

        /**
         * TODO Removes from the underlying collection the last element
         * returned by the iterator
         * The behavior of an iterator is unspecified if the underlying
         * collection is modified while the iteration is in progress in
         * any way other than by calling this method.
         * @throw ElementNotExist
         */
        void remove() 
        {
        	if ((position < deq -> left) || (position > deq -> right) || (deleted == -1)) throw ElementNotExist("Deque:remove:ElementNotExist");
        	deleted = -1;
        	if (deq -> left == deq -> right) deq -> left = deq -> right = -1;
        	for (int i = position; i < deq -> right; ++i) deq -> data[i] = deq -> data[i + 1];
        	if (status == 1) --position;
        	--deq -> right;
        }
    };

    /**
     * TODO Constructs an empty deque.
     */
    Deque(int maxSize = 10):left(-1), right(-1), maxSize(maxSize) 
	{
		data = new T[maxSize];
	}

    /**
     * TODO Destructor
     */
    ~Deque() 
	{
		delete [] data;
	}

    /**
     * TODO Assignment operator
     */
    Deque& operator=(const Deque& x) 
	{
		if (this == &x) return *this;
		delete [] data;
		maxSize = x.maxSize, left = x.left, right = x.right;
		data = new T[maxSize];
		if (left != -1) for (int i = left; i <= right; ++i) data[i] = x.data[i];
		return *this;
	}

    /**
     * TODO Copy-constructor
     */
    Deque(const Deque& x) 
	{
		maxSize = x.maxSize, left = x.left, right = x.right;
		data = new T[maxSize];
		if (left != -1) for (int i = left; i <= right; ++i) data[i] = x.data[i];
	}
	
	/**
	 * TODO Inserts the specified element at the front of this deque. 
	 */
	void addFirst(const T& e) 
	{
		if (left == -1)
		{
			left = right = maxSize / 2;
			data[left] = e;
			return;
		}
		if (left == 0) doubleSpace();
		data[--left] = e;
	}

	/**
	 * TODO Inserts the specified element at the end of this deque.
	 */
	void addLast(const T& e) 
	{
		if (left == -1)
		{
			left = right = maxSize / 2;
			data[right] = e;
			return;
		}
		if (right == maxSize - 1) doubleSpace();
		data[++right] = e;
	}

	/**
	 * TODO Returns true if this deque contains the specified element.
	 */
	bool contains(const T& e) const 
	{
		if (left == -1) return false;
		for (int i = left; i <= right; ++i) if (data[i] == e) return true;
		return false;
	}

	/**
	 * TODO Removes all of the elements from this deque.
	 */
	 void clear() 
	 {
		left = right = -1;
	 }

	 /**
	  * TODO Returns true if this deque contains no elements.
	  */
	bool isEmpty() const 
	{
		return left == -1;
	}

	/**
	 * TODO Retrieves, but does not remove, the first element of this deque.
	 * @throw ElementNotExist
	 */
	 const T& getFirst() 
	 {
		if (isEmpty()) throw ElementNotExist("Deque:getFirst:ElementNotExist");
		return data[left];
	 }

	 /**
	  * TODO Retrieves, but does not remove, the last element of this deque.
	  * @throw ElementNotExist
	  */
	 const T& getLast() 
	 {
		if (isEmpty()) throw ElementNotExist("Deque:getLast:ElementNotExist");
		return data[right];
	 }

	 /**
	  * TODO Removes the first element of this deque.
	  * @throw ElementNotExist
	  */
	void removeFirst() 
	{
		if (isEmpty()) throw ElementNotExist("Deque:removeFirst:ElementNotExist");
		if (left == right) left = right = -1;
		else ++left;
	}

	/**
	 * TODO Removes the last element of this deque.
	 * @throw ElementNotExist
	 */
	void removeLast() 
	{
		if (isEmpty()) throw ElementNotExist("Deque:removeLast:ElementNotExist");
		if (left == right) left = right = -1;
		else --right;
	}

	/**
	 * TODO Returns a const reference to the element at the specified position in this deque.
	 * The index is zero-based, with range [0, size).
	 * @throw IndexOutOfBound
	 */
	const T& get(int index) const 
	{
		if ((index < 0) || (index >= (right - left + 1))) throw IndexOutOfBound("Deque:get:IndexOutOfBound");
		return data[left + index];
	}
	
	/**
	 * TODO Replaces the element at the specified position in this deque with the specified element.
	 * The index is zero-based, with range [0, size).
	 * @throw IndexOutOfBound
	 */
	void set(int index, const T& e) 
	{
		if ((index < 0) || (index >= (right - left + 1))) throw IndexOutOfBound("Deque:set:IndexOutOfBound");
		data[left + index] = e;
	}

	/**
	 * TODO Returns the number of elements in this deque.
	 */
	 int size() const 
	 {
	 	if (left == -1) return 0;
		return right - left + 1;
	 }

	 /**
	  * TODO Returns an iterator over the elements in this deque in proper sequence.
	  */
	 Iterator iterator() 
	 {
		return Iterator(this, 1);
	 }

	 /**
	  * TODO Returns an iterator over the elements in this deque in reverse sequential order.
	  */
	 Iterator descendingIterator() 
	 {
		return Iterator(this, -1);
	 }
};

#endif
