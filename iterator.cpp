#include "headers/iterator.h"

InfiniteIterator::InfiniteIterator(const unsigned int _tableSize) : tableSize(_tableSize), position(0)
{}

unsigned int InfiniteIterator::operator+(const unsigned int offset) const
{
    return (position + offset) % tableSize;
}

unsigned int InfiniteIterator::operator-(const unsigned int offset) const
{
    int newPos = position - offset % tableSize;
    if (newPos < 0)
        return tableSize + newPos;
    else
        return newPos;
}

InfiniteIterator& InfiniteIterator::operator++()
{
    ++position;
    if(position == tableSize)
        position = 0;
}

InfiniteIterator& InfiniteIterator::operator--()
{
    if(position == 0)
        position = tableSize;
        
    --position;
}

InfiniteIterator& InfiniteIterator::operator=(const unsigned int number)
{
    position = number % tableSize;
}

unsigned int InfiniteIterator::at() const
{
    return position;
}