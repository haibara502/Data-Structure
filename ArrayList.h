/** @file */
#ifndef __ARRAYLIST_H
#define __ARRAYLIST_H

#include "IndexOutOfBound.h"
#include "ElementNotExist.h"

/**
 * The ArrayList is just like vector in C++.
 * You should know that "capacity" here doesn't mean how many elements are now in this list, where it means
 * the length of the array of your internal implemention
 *
 * The iterator iterates in the order of the elements being loaded into this list
 */
template <class T>
class ArrayList
{
private:
    T *data;
    int currentSize, maxSize;

public:

    class Iterator
    {
    private:
        int position, status;
        ArrayList *arr;

    public:
        Iterator(ArrayList *ar):arr(ar), position(-1), status(0){};
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() const
        {
            if (position == arr->currentSize - 1) return false;
            return true;
        }

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next()
        {
            if (!hasNext()) throw ElementNotExist("Arraylist:next:ElementNotExist");
			status = 0;
            return arr->data[++position];
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
            if ((position == -1) || (status == -1) || (position >= arr -> currentSize)) throw ElementNotExist("Arraylist:remove:ElimentNotExist");
            arr->removeIndex(position);
            status = -1;
			position --;
        }
    };

private:
    void doubleSpace()
    {
        T *tmp = new T[maxSize];
        for (int i = 0; i < currentSize; ++i) tmp[i] = data[i];
        maxSize <<= 1;
        if (data != NULL) delete [] data;
        data = new T[maxSize];
        for (int i = 0; i < currentSize; ++i) data[i] = tmp[i];
        delete [] tmp;
    }

public:

    /**
     * TODO Constructs an empty array list.
     */
    ArrayList():currentSize(0), maxSize(10)
    {
        data = new T[maxSize];
    }

    /**
     * TODO Destructor
     */
    ~ArrayList()
    {
        if (data != NULL) delete [] data;
    }

    /**
     * TODO Assignment operator
     */
    ArrayList& operator=(const ArrayList& x)
    {
		if (&x == this) return *this;
        currentSize = x.currentSize;
        maxSize = x.maxSize;
        if (data != NULL) delete [] data;
        data = new T[maxSize];
        for (int i = 0; i < currentSize; ++i) data[i] = x.data[i];
        return *this;
    }

    /**
     * TODO Copy-constructor
     */
    ArrayList(const ArrayList& x)
    {
        currentSize = x.currentSize;
        maxSize = x.maxSize;
        // if (data != NULL) delete [] data;
        data = new T[maxSize];
        for (int i = 0; i < currentSize; ++i) data[i] = x.data[i];
    }

    /**
     * TODO Appends the specified element to the end of this list.
     * Always returns true.
     */
    bool add(const T& e)
    {
        if (currentSize == maxSize) doubleSpace();
        data[currentSize++] = e;
        return true;
    }

    /**
     * TODO Inserts the specified element to the specified position in this list.
     * The range of index parameter is [0, size], where index=0 means inserting to the head,
     * and index=size means appending to the end.
     * @throw IndexOutOfBound
     */
    void add(int index, const T& element)
    {
        if ((index < 0) || (index > currentSize)) throw IndexOutOfBound("Arraylist:add:IndexOutOfBound");
        if (currentSize == maxSize) doubleSpace();
        for (int i = currentSize; i > index; --i) data[i] = data[i - 1];
        data[index] = element;
        ++currentSize;
    }

    /**
     * TODO Removes all of the elements from this list.
     */
    void clear()
    {
        currentSize = 0;
    }

    /**
     * TODO Returns true if this list contains the specified element.
     */
    bool contains(const T& e) const
    {
        for (int i = 0; i < currentSize; ++i)
            if (data[i] == e) return true;
        return false;
    }

    /**
     * TODO Returns a const reference to the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    const T& get(int index) 
    {
        if ((index < 0) || (index >= currentSize)) throw IndexOutOfBound("Arraylist:get:IndexOutOfBound");
        return data[index];
    }

    /**
     * TODO Returns true if this list contains no elements.
     */
    bool isEmpty() const
    {
        return currentSize == 0;
    }

    /**
     * TODO Removes the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void removeIndex(int index)
    {
        if ((index < 0) || (index >= currentSize)) throw IndexOutOfBound("Arraylist:removeIndex:IndexOutOfBound");
        for (int i = index; i < currentSize - 1; ++i) data[i] = data[i + 1];
        --currentSize;
    }

    /**
     * TODO Removes the first occurrence of the specified element from this list, if it is present.
     * Returns true if it was present in the list, otherwise false.
     */
    bool remove(const T &e)
    {
        for (int i = 0; i < currentSize; ++i) if (data[i] == e)
        {
            for (int j = i; j < currentSize - 1; ++j) data[j] = data[j + 1];
            --currentSize;
            return true;
        }
        return false;
    }

    /**
     * TODO Replaces the element at the specified position in this list with the specified element.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void set(int index, const T &element)
    {
        if ((index < 0) || (index >= currentSize)) throw IndexOutOfBound("Arraylist:set:IndexOutOfBound");
        data[index] = element;
    }

    /**
     * TODO Returns the number of elements in this list.
     */
    int size() const
    {
        return currentSize;
    }

    /**
     * TODO Returns an iterator over the elements in this list.
     */
    Iterator iterator()
    {
        return Iterator(this);
    }
};

#endif
