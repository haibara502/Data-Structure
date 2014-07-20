/** @file */
#ifndef __TREEMAP_H
#define __TREEMAP_H

#include "ElementNotExist.h"
#include <ctime>
#include <cstdlib>

/**
 * TreeMap is the balanced-tree implementation of map. The iterators must
 * iterate through the map in the natural order (operator<) of the key.
 */
template<class K, class V>
class TreeMap
{
public:
	class Entry;
private:
    class Node
    {
	public:
        Entry data;
        int priority;
        Node *left, *right, *pre, *next;
		Node(K key, V value, int priority, Node *left, Node *right, Node *pre, Node *next):data(Entry(key, value)), priority(priority), left(left), right(right), pre(pre), next(next){};
    } *root, *head, *tail;
    int currentSize;
    K *T_T1, *T_T3;
    V *T_T2, *T_T4;

public:
    class Entry
    {
        K key;
        V value;
    public:
        Entry(K k, V v)
        {
            key = k;
            value = v;
        }

        const K& getKey() const
        {
            return key;
        }

        const V& getValue() const
        {
            return value;
        }
		void setValue(V value1)
		{	
			value = value1;
		}
    };

    class Iterator
    {
		TreeMap<K, V> *tre;
		Node *now;
    public:
	
		Iterator(TreeMap<K, V> *tree):tre(tree)
		{
			now = tre -> head;
		}
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() 
		{
			return now -> next != tre -> tail;
		}

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const Entry &next() 
		{
			if (!hasNext()) throw ElementNotExist("TreeMap::next::ElementNotExist");
			now = now -> next;
			return now -> data;
		}
    };
	
private:
	void deleteTree(Node *&root)
	{
		if (root == NULL) return;
		if (root -> left != NULL) deleteTree(root -> left);
		if (root -> right != NULL) deleteTree(root -> right);
		delete root;
		root = NULL;
	}
	
	void insertNode(Node *&root, K key, V value, int status, Node *father)
	{
		if (root == NULL)
		{
			++currentSize;
			if (!status) root = new Node(key, value, rand(), NULL, NULL, father -> pre, father);
			else root = new Node(key, value, rand(), NULL, NULL, father, father -> next);
			root -> pre -> next = root;
			root -> next -> pre = root;
			return;
		}
		if (key < root -> data.getKey()) 
		{
			insertNode(root -> left, key, value, 0, root);
			if (root -> left -> priority < root -> priority) R(root);
		}
		else if (key > root -> data.getKey())
		{
			insertNode(root -> right, key, value, 1, root);
			if (root -> right -> priority < root -> priority) L(root);
		}
		else root -> data.setValue(value);
	}
	
	void R(Node *&root)
	{
		Node *p = root -> left;
		root -> left = p -> right;
		p -> right = root;
		root = p;
	}
	
	void L(Node *&root)
	{
		Node *p = root -> right;
		root -> right = p -> left;
		p -> left = root;
		root = p;
	}
	
	void removeNode(Node *&root, K key)
	{
		if (key < root -> data.getKey()) removeNode(root -> left, key);
		else if (key > root -> data.getKey()) removeNode(root -> right, key);
		else if (root -> left != NULL)
		{
			if (root -> right != NULL)
			{
				if (root -> left -> priority < root -> right -> priority)
				{
					R(root);
					removeNode(root -> right, key);
				}
				else
				{
					L(root);
					removeNode(root -> left, key);
				}
			}
			else
			{
				Node *tmp = root;
				root -> pre -> next = root -> next;
				root -> next -> pre = root -> pre;
				root = root -> left;
				delete tmp;
			}
		}
		else
		{
			if (root -> right != NULL)
			{
				Node *tmp = root;
				root -> pre -> next = root -> next;
				root -> next -> pre = root -> pre;
				root = root -> right;
				delete tmp;
			}
			else
			{
				root -> pre -> next = root -> next;
				root -> next -> pre = root -> pre;
				delete root;
				root = NULL;
			}
		}
	}
	
	bool findKey(Node *root, K key) const
	{
		if (root == NULL) return false;
		if (key < root -> data.getKey()) return findKey(root -> left, key);
		else if (key > root -> data.getKey()) return findKey(root -> right, key);
		return true;
	}
	
	V getKey(Node *root, K key) const
	{
		if (root -> data.getKey() == key) return root -> data.getValue();
		if (root -> data.getKey() < key)
			return getKey(root -> right, key);
		else return getKey(root -> left, key);
	}
	
public:

    /**
     * TODO Constructs an empty tree map.
     */
    TreeMap():currentSize(0)
    {
		srand(time(0));
        root = NULL;
		head = new Node(*(T_T1 = new K), *(T_T2 = new V), 0, NULL, NULL, NULL, NULL);
		tail = new Node(*(T_T3 = new K), *(T_T4 = new V), 1073741824, NULL, NULL, head, NULL);
		head -> next = tail;
    }

    /**
     * TODO Destructor
     */
    ~TreeMap()
    {
        deleteTree(root);
        if (root != NULL) delete root;
		delete head;
		delete tail;
		delete T_T1, delete T_T2, delete T_T3, delete T_T4;
    }

    /**
     * TODO Assignment operator
     */
    TreeMap &operator=(const TreeMap &x)
    {
        if (this == &x) return *this;
        clear();
        currentSize = 0;
        for (Node *tmp = x.head -> next; tmp != x.tail; tmp = tmp -> next)
            put(tmp -> data.getKey(), tmp -> data.getValue());
        return *this;
    }

    /**
     * TODO Copy-constructor
     */
    TreeMap(const TreeMap &x)
    {
		srand(time(0));
        root = NULL;
		head = new Node(*(T_T1 = new K), *(T_T2 = new V), 0, NULL, NULL, NULL, NULL);
		tail = new Node(*(T_T3 = new K), *(T_T4 = new V), 0, NULL, NULL, head, NULL);
		head -> next = tail;
        currentSize = 0;
        for (Node *tmp = x.head -> next; tmp != x.tail; tmp = tmp -> next)
            put(tmp -> data.getKey(), tmp -> data.getValue());
    }

    /**
     * TODO Returns an iterator over the elements in this map.
     */
    Iterator iterator()
    {
        return Iterator(this);
    }

    /**
     * TODO Removes all of the mappings from this map.
     */
    void clear()
    {
        deleteTree(root);
		tail -> pre = head, head -> next = tail;
        currentSize = 0;
    }

    /**
     * TODO Returns true if this map contains a mapping for the specified key.
     */
    bool containsKey(const K &key) const
    {
        return findKey(root, key);
    }

    /**
     * TODO Returns true if this map maps one or more keys to the specified value.
     */
    bool containsValue(const V &value) const
    {
        for (Node *tmp = head -> next; tmp != tail; tmp = tmp -> next)
            if (tmp -> data.getValue() == value) return true;
        return false;
    }

    /**
     * TODO Returns a const reference to the value to which the specified key is mapped.
     * If the key is not present in this map, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V get(const K &key) const
    {
        if (containsKey(key) == false) throw ElementNotExist("TreeMap::get::ElementNotExist");
        return getKey(root, key);
    }

    /**
     * TODO Returns true if this map contains no key-value mappings.
     */
    bool isEmpty() const
    {
        return currentSize == 0;
    }

    /**
     * TODO Associates the specified value with the specified key in this map.
     */
    void put(const K &key, const V &value)
    {
        if (root == NULL)
        {
            root = new Node(key, value, rand(), NULL, NULL, head, tail);
            head -> next = root, tail -> pre = root;
            currentSize = 1;
        }
		else if (key < root -> data.getKey())
			insertNode(root -> left, key, value, 0, root);
		else if (key > root -> data.getKey())
			insertNode(root -> right, key, value, 1, root);
		else root -> data.setValue(value);
    }

    /**
     * TODO Removes the mapping for the specified key from this map if present.
     * If there is no mapping for the specified key, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void remove(const K &key) 
	{
		if (containsKey(key) == false) throw ElementNotExist("TreeMap::remove::ElementNotExist");
		removeNode(root, key);
		--currentSize;
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
