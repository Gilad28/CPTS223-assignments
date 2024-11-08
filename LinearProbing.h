#ifndef LINEAR_PROBING_H
#define LINEAR_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <iostream>
#include "Employee.h"
#include "utils.h"

using namespace std;

template <typename HashedObj> 
class ProbingHash
{
  public:
    explicit ProbingHash(int size = 101) : array(nextPrime(size)), currentSize{0}
    { 
        makeEmpty(); 
    }

    bool contains(const HashedObj &x) const 
    {
        return isActive(findPos(x));
    }

    void makeEmpty()
    {
        currentSize = 0;
        for (auto &entry : array) 
        {
            entry.info = EMPTY;
        }
    }

    bool insert(const HashedObj &x) 
    {
        int currentPos = findPos(x);
        if (isActive(currentPos))
        {
            return false;
        }

        array[currentPos].element = x;
        array[currentPos].info = ACTIVE;

        if (++currentSize > array.size() / 2)
        {
            rehash();
        }

        return true;
    }

    bool insert(HashedObj &&x) 
    {
        int currentPos = findPos(x);

        if (isActive(currentPos))
        {
            return false;
        }

        array[currentPos].element = move(x);
        array[currentPos].info = ACTIVE;

        if (++currentSize > array.size() / 2)
        {
            rehash();
        }

        return true;
    }

    bool remove(const HashedObj &x) 
    {
        int currentPos = findPos(x);
        if (!isActive(currentPos))
        {
            return false;
        }

        array[currentPos].info = DELETED;
        return true;
    }

    double readLoadFactor() 
    {
        return loadFactor();
    }

    double readCurrentSize() 
    {
        return currentSize;
    }

    double readArraySize() 
    {
        return array.size();
    }

    enum EntryType { ACTIVE, EMPTY, DELETED };

  private:
    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        HashEntry(const HashedObj &e = HashedObj(), EntryType i = EMPTY)
          : element{e}, info{i} { }
        
        HashEntry(HashedObj &&e, EntryType i = EMPTY)
          : element{move(e)}, info{i} { }
    };
    
    vector<HashEntry> array;
    int currentSize;

    bool isActive(int currentPos) const
    { 
        return array[currentPos].info == ACTIVE; 
    }

    int findPos(const HashedObj &x) const 
    {
        int currentPos = myhash(x);

        while (array[currentPos].info != EMPTY && array[currentPos].element != x) 
        {
            currentPos += 1;
            if (currentPos >= array.size())
            {
                currentPos -= array.size();
            }
        }

        return currentPos;
    }

    void rehash()
    {
        vector<HashEntry> oldArray = array;

        array.resize(nextPrime(2 * oldArray.size()));
        for (auto &entry : array)
        {
            entry.info = EMPTY;
        }

        currentSize = 0;
        for (auto &entry : oldArray)
        {
            if (entry.info == ACTIVE)
            {
                insert(move(entry.element));
            }
        }
    }

    size_t myhash(const HashedObj &x) const
    {
        static hash<HashedObj> hf;
        return hf(x) % array.size();
    }

    double loadFactor() const
    {
        return static_cast<double>(currentSize) / array.size();
    }

};

#endif