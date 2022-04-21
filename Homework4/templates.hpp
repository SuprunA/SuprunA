#pragma once

#include <iostream>
#include <random>
#include <iterator>

template<template<typename, typename> class TContainer, typename TItem, typename TIterator, typename T>
  
void generator(TContainer<TItem, TIterator> &c, const T &rangeStart, const T &rangeEnd)
{
    std::random_device rd;
    std::mt19937 mersenne(rd());
    std::uniform_real_distribution<double> urd(rangeStart, rangeEnd);
    generate(c.begin(), c.end(), [&]()
    {
        return urd(mersenne);
    });
}

template<template<typename, typename> class TContainer, typename TItem, typename TIterator>
void print(const TContainer<TItem, TIterator> &c)
{
    copy(c.begin(), c.end(), std::ostream_iterator<TItem> { std::cout, " " });
    std::cout << std::endl;
}

template<template<typename, typename> class TContainer, typename TItem, typename TIterator>
void insert_sorted(TContainer<TItem, TIterator> &c, const TItem &i)
{
    typename TContainer<TItem, TIterator>::iterator it = find_if(c.begin(), c.end(), [&](const TItem &current)
    {
        return current >= i;
    });
    c.insert(it, i);
}
