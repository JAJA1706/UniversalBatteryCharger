#pragma once

class Iterator
{
private:
    const unsigned int tableSize;
    unsigned int position;
public:
    Iterator(const unsigned int _tableSize);
    unsigned int at() const;
    unsigned int operator+(const unsigned int offset) const;
    unsigned int operator-(const unsigned int offset) const;
    Iterator& operator++();
    Iterator& operator--();
    Iterator& operator=(const unsigned int number);
};
