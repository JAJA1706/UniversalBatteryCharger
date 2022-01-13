#pragma once

class InfiniteIterator
{
private:
    const unsigned int tableSize;
    unsigned int position;
public:
    InfiniteIterator(const unsigned int _tableSize);
    unsigned int at() const;
    unsigned int operator+(const unsigned int offset) const;
    unsigned int operator-(const unsigned int offset) const;
    InfiniteIterator& operator++();
    InfiniteIterator& operator--();
    InfiniteIterator& operator=(const unsigned int number);
};
