#include "headers/iterator.h"

Iterator::Iterator(const unsigned int _tableSize) : tableSize(_tableSize), position(0)
{}

unsigned int Iterator::operator+(const unsigned int offset) const
{
    return (position + offset) % tableSize;
}

unsigned int Iterator::operator-(const unsigned int offset) const
{
    int newPos = position - offset % tableSize;
    if (newPos < 0)
        return tableSize + newPos;
    else
        return newPos;
}

Iterator& Iterator::operator++()
{
    ++position;
    if(position == tableSize)
        position = 0;
}

Iterator& Iterator::operator--()
{
    if(position == 0)
        position = tableSize;
        
    --position;
}

Iterator& Iterator::operator=(const unsigned int number)
{
    position = number % tableSize;
}

unsigned int Iterator::at() const
{
    return position;
}