/** @file */

#ifndef __HASHMAP_H
#define __HASHMAP_H

#include "ElementNotExist.h"

/**
 * HashMap is a map implemented by hashing. Also, the 'capacity' here means the
 * number of buckets in your internal implemention, not the current number of the
 * elements.
 *
 * Template argument H are used to specify the hash function.
 * H should be a class with a static function named ``hashCode'',
 * which takes a parameter of type K and returns a value of type int.
 * For example, the following class
 * @code
 *      class Hashint {
 *      public:
 *          static int hashCode(int obj) {
 *              return obj;
 *          }
 *      };
 * @endcode
 * specifies an hash function for integers. Then we can define:
 * @code
 *      HashMap<int, int, Hashint> hash;
 * @endcode
 *
 * Hash function passed to this class should observe the following rule: if two keys
 * are equal (which means key1 == key2), then the hash code of them should be the
 * same. However, it is not generally required that the hash function should work in
 * the other direction: if the hash code of two keys are equal, the two keys could be
 * different.
 *
 * Note that the correctness of HashMap should not rely on the choice of hash function.
 * This is to say that, even the given hash function always returns the same hash code
 * for all keys (thus causing a serious collision), methods of HashMap should still
 * function correctly, though the performance will be poor in this case.
 *
 * The order of iteration could be arbitary in HashMap. But it should be guaranteed
 * that each (key, value) pair be iterated exactly once.
 */
template <class K, class V, class H>
class HashMap
{
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

        const K & getKey() const
        {
            return key;
        }

        const V & getValue() const
        {
            return value;
        }
        void setValue(V value1)
        {
        	value = value1;
        }
    };
 
private:
	class Node
	{
	public:
		K k;
		V v;
		Entry data;
		Node *next;
		Node(Node *nex = NULL, K _k = *(new K), V _v = *(new V)):next(nex), k(_k), v(_v), data(Entry(_k, _v)){}
	};
	
	static const int Mod = 1000003;
	int currentSize;
	Node **table;
	
public:
    class Iterator
    {
    	int pos;
    	HashMap *hash;
    	Node *now;
    public:
    	Iterator(HashMap *_hash):pos(-1), hash(_hash), now(NULL){};
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() 
        {
        	if (hash -> table == NULL) return false;
        	if ((now != NULL) and (now -> next != NULL)) return true;
        	for (int i = pos + 1; i < Mod; ++i) if (hash -> table[i] != NULL) return true;
        	return false;
        }

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const Entry &next() 
        {
        	if (!hasNext()) throw ElementNotExist("HashMap:next:ElementNotExist");
        	if ((now != NULL) && (now -> next != NULL)) now = now -> next;
        	else
        	{
        		++pos;
        		for (; (pos < Mod) && (hash -> table[pos] == NULL);) ++pos;
        		now = hash -> table[pos];
        	}
        	return now -> data;
        }
    };

    /**
     * TODO Constructs an empty hash map.
     */
    HashMap():currentSize(0)
    {
    	table = new Node*[Mod];
    	for (int i = 0; i < Mod; ++i) table[i] = NULL; 
    }

    /**
     * TODO Destructor
     */
    ~HashMap() 
    {
    	if (table != NULL)
    		for (int i = 0; i < Mod; ++i) if (table[i] != NULL)
    			for (Node *tmp = table[i]; tmp != NULL; )
    			{
    				Node *temp = tmp;
    				tmp = tmp -> next;
    				delete temp;
    			} 
    	if (table != NULL) delete [] table;
    }

    /**
     * TODO Assignment operator
     */
    HashMap &operator=(const HashMap &x) 
    {
    	if (this == &x) return *this;
    	clear();
    	for (int i = 0; i < Mod; ++i) 
    		for (Node *tmp = x.table[i]; tmp != NULL; tmp = tmp -> next)
    			put(tmp -> k, tmp -> v); 
    	return *this;
    }

    /**
     * TODO Copy-constructor
     */
    HashMap(const HashMap &x) 
    {
    	table = new Node*[Mod];
    	currentSize = 0;
    	for (int i = 0; i < Mod; ++i)
    		for (Node *tmp = x.table[i]; tmp != NULL; tmp = tmp -> next)
    			put(tmp -> k, tmp -> v); 
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
    	if (table != NULL)
    		for (int i = 0; i < Mod; ++i)
    		{
    			for (Node *tmp = table[i]; tmp != NULL;)
    			{
    				Node *temp = tmp;
    				tmp = tmp -> next;
    				delete temp;
    			}
    			table[i] = NULL;
    		}
    	currentSize = 0;
    }

    /**
     * TODO Returns true if this map contains a mapping for the specified key.
     */
    bool containsKey(const K &key) const 
    {
    	int pos = H::hashCode(key) % Mod;
    	if (pos < 0) pos += Mod;
    	for (Node *tmp = table[pos]; tmp != NULL; tmp = tmp -> next)
    		if (tmp -> k == key) return true;
    	return false;
    }

    /**
     * TODO Returns true if this map maps one or more keys to the specified value.
     */
    bool containsValue(const V &value) const 
    {
    	if (currentSize == 0) return false;
    	for (int i = 0; i < Mod; ++i)
    		for (Node *tmp = table[i]; tmp != NULL; tmp = tmp -> next)
    			if (tmp -> v == value) return true;
    	return false;
    }

    /**
     * TODO Returns a const reference to the value to which the specified key is mapped.
     * If the key is not present in this map, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V &get(const K &key) const 
    {
    	if (!containsKey(key)) throw ElementNotExist("HashMap:get:ElementNotExist");
    	int pos = H::hashCode(key) % Mod;
    	if (pos < 0) pos += Mod;
    	for (Node *tmp = table[pos]; tmp != NULL; tmp = tmp -> next)
    		if (tmp -> k == key) return tmp -> v;
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
    	int pos = H::hashCode(key) % Mod;
    	if (pos < 0) pos += Mod;
    	for (Node *tmp = table[pos]; tmp != NULL; tmp = tmp -> next)
    		if (tmp -> k == key)
    		{
    			tmp -> v = value;
    			tmp -> data.setValue(value);
    			return;
    		}
    	++currentSize;
    	Node *now = new Node(table[pos], key, value);
    	table[pos] = now;
    }

    /**
     * TODO Removes the mapping for the specified key from this map if present.
     * If there is no mapping for the specified key, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void remove(const K &key) 
    {
    	if (!containsKey(key)) throw ElementNotExist("HashMap:remove:ElementNotExist");
    	--currentSize;
    	int pos = H::hashCode(key) % Mod;
    	if (pos < 0) pos += Mod;
    	if (table[pos] -> k == key)
    	{
    		Node *tmp = table[pos];
    		table[pos] = table[pos] -> next;
    		delete tmp;
    		return;
    	}
    	for (Node *temp = table[pos], *tmp = temp -> next; tmp != NULL; tmp = tmp -> next, temp = temp -> next)
    		if (tmp -> k == key)
    		{
    			temp -> next = tmp -> next;
    			delete tmp;
    			return;
    		}
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
