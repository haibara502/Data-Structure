/** @file */
#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H

#include "IndexOutOfBound.h"
#include "ElementNotExist.h"

/**
 * A linked list.
 *
 * The iterator iterates in the order of the elements being loaded into this list.
 */
template <class T>
class LinkedList
{
    class Node
    {
    public:
        T data;
        Node *pre, *next;

        Node(Node *_pre, T _data, Node *_next) :pre(_pre), data(_data), next(_next){}

    } *head, *tail;
    int currentSize;

    void deleteElement(Node *now)
    {
		if ((now == head) && (head == tail))
		{
			if (now -> pre != NULL) now -> pre -> next = NULL;
			delete now;
			head = tail = NULL;
			return;
		}
        if (now == head) head = head -> next;
        if (now == tail) tail = tail -> pre;
        if (now -> pre != NULL) now -> pre -> next = now -> next;
        if (now -> next != NULL) now -> next -> pre = now -> pre;
        delete now;
    }

public:
    class Iterator
    {
        Node *now, *tmp;
        LinkedList<T> *link;
		int status;
		T *T_T;
    public:

        Iterator(LinkedList<T> *_link):link(_link), status(-1)
        {
            now = new Node(NULL, *(T_T = new T), link -> head);
            tmp = now;
			if (link -> head != NULL) link -> head -> pre = now;
        }
        ~Iterator()
        {
        	delete tmp;
        	delete T_T;
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
            if (!hasNext()) throw ElementNotExist("LinkedList:next:ElementNotExist");
            now = now -> next;
			status = 0;
            return now -> data;
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
            if ((now == NULL) || (status == -1)) throw ElementNotExist("LInkedList:remove:ElementNotExist");
            Node *tmp = now; now = now -> pre;
            link -> deleteElement(tmp);
			-- link -> currentSize;
			status = -1;
        }
    };

    /**
     * TODO Constructs an empty linked list
     */
    LinkedList():head(NULL), tail(NULL), currentSize(0){}

    /**
     * TODO Copy constructor
     */
    LinkedList(const LinkedList<T> &c)
    {
		head = tail = NULL;
    	/*
        for (Node *tmp = head; tmp != NULL; )
        {
            Node *temp = tmp;
            tmp = tmp -> next;
            deleteElement(temp);
        }
        */
        for (Node *tmp = c.head; tmp != NULL; tmp = tmp -> next)
            addLast(tmp -> data);
        currentSize = c.currentSize;
    }

    /**
     * TODO Assignment operator
     */
    LinkedList<T>& operator=(const LinkedList<T> &c)
    {
		if (this == &c) return *this;
        for (Node *tmp = head; tmp != NULL; )
        {
            Node *temp = tmp;
            tmp = tmp -> next;
            deleteElement(temp);
        }
        currentSize = 0;
        head = tail = NULL;
        for (Node *tmp = c.head; tmp != NULL; tmp = tmp -> next)
			addLast(tmp -> data);
        currentSize = c.currentSize;
        return *this;
    }

    /**
     * TODO Desturctor
     */
    ~LinkedList()
    {
        for (Node *tmp = head; tmp != NULL; )
        {
            Node *temp = tmp;
            tmp = tmp -> next;
            deleteElement(temp);
        }
    }

    /**
     * TODO Appends the specified element to the end of this list.
     * Always returns true.
     */
    bool add(const T& e)
    {
        addLast(e);
		return true;
    }

    /**
     * TODO Inserts the specified element to the beginning of this list.
     */
    void addFirst(const T& elem)
    {
        if (head == NULL)
        {
            head = tail = new Node(NULL, elem, NULL);
            ++currentSize;
            return;
        }
        Node *tmp = new Node(NULL, elem, head);
        head -> pre = tmp;
        head = tmp;
        ++currentSize;
    }

    /**
     * TODO Insert the specified element to the end of this list.
     * Equivalent to add.
     */
    void addLast(const T &elem)
    {
        if (head == NULL)
        {
            head = tail = new Node(NULL, elem, NULL);
            ++currentSize;
            return;
        }
        Node *tmp = new Node(tail, elem, NULL);
        tail -> next = tmp;
        tail = tmp;
        ++currentSize;
    }

    /**
     * TODO Inserts the specified element to the specified position in this list.
     * The range of index parameter is [0, size], where index=0 means inserting to the head,
     * and index=size means appending to the end.
     * @throw IndexOutOfBound
     */
    void add(int index, const T& element)
    {
        if ((index < 0) || (index > currentSize)) throw IndexOutOfBound("LinkedLIst:add:IndexOutOfBound");
        if (index == 0)
            addFirst(element);
        else
        {
            Node *tmp = head;
            for (int i = 1; i < index; ++i) tmp = tmp -> next;
            Node *now = new Node(tmp, element, tmp -> next);
            if (tmp -> next != NULL) tmp -> next -> pre = now;
            else tail = now;
            tmp -> next = now;
			++currentSize;
        }
    }

    /**
     * TODO Removes all of the elements from this list.
     */
    void clear()
    {
        for (Node *tmp = head; tmp != NULL;)
        {
            Node *temp = tmp;
            tmp = tmp -> next;
            deleteElement(temp);
        }
        head = tail = NULL;
        currentSize = 0;
    }

    /**
     * TODO Returns true if this list contains the specified element.
     */
    bool contains(const T& e) const
    {
        for (Node *tmp = head; tmp != NULL; tmp = tmp -> next)
            if (tmp -> data == e) return true;
        return false;
    }

    /**
     * TODO Returns a const reference to the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    const T& get(int index) const
    {
        if ((index < 0) || (index >= currentSize)) throw IndexOutOfBound("LinkedList:get:IndexOutOfBound");
        Node *tmp = head;
        for (int i = 0; i < index; ++i) tmp = tmp -> next;
        return tmp -> data;
    }

    /**
     * TODO Returns a const reference to the first element.
     * @throw ElementNotExist
     */
    const T& getFirst() const
    {
        if (head == NULL) throw ElementNotExist("LinkedList:getFirst:ElementNotExist");
        return head -> data;
    }

    /**
     * TODO Returns a const reference to the last element.
     * @throw ElementNotExist
     */
    const T& getLast() const
    {
        if (tail == NULL) throw ElementNotExist("LinkedList:getLast:ElementNotExist");
        return tail -> data;
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
        if ((index < 0) || (index >= currentSize)) throw IndexOutOfBound("LinkedList:removeIndex:IndexOutOfBound");
        Node *tmp = head;
        for (int i = 0; i < index; ++i) tmp = tmp -> next;
        deleteElement(tmp);
        --currentSize;
    }

    /**
     * TODO Removes the first occurrence of the specified element from this list, if it is present.
     * Returns true if it was present in the list, otherwise false.
     */
    bool remove(const T &e)
    {
        for (Node *tmp = head; tmp != NULL; tmp = tmp -> next) if (tmp -> data == e)
        {
            deleteElement(tmp);
            --currentSize;
            return true;
        }
        return false;
    }

    /**
     * TODO Removes the first element from this list.
     * @throw ElementNotExist
     */
    void removeFirst()
    {
        deleteElement(head);
        --currentSize;
    }

    /**
     * TODO Removes the last element from this list.
     * @throw ElementNotExist
     */
    void removeLast()
    {
        deleteElement(tail);
        --currentSize;
    }

    /**
     * TODO Replaces the element at the specified position in this list with the specified element.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void set(int index, const T &element)
    {
        if ((index < 0) || (index >= currentSize)) throw IndexOutOfBound("LinkedList:set:IndexOutOfBound");
        Node *tmp = head;
        for (int i = 0; i < index; ++i) tmp = tmp -> next;
        tmp -> data = element;
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
