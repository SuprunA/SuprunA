#pragma once

#include <iostream>
#include <iterator>
#include <string>
#include <unordered_set>
#include <vector>
#include <list>
#include <deque>

template<typename IT>
void printWords(IT it, IT end)
{
    std::unordered_set<std::string> countWords(it, end);
    std::copy(countWords.begin(), countWords.end(), std::ostream_iterator<std::string>(std::cout, "; "));
}

void exercise_1()
{
    std::vector<std::string> v { "Один", "Два", "Три", "Пять", "Один", "Пять", "Шесть", "Два", "Два", "Один", "Семь", "Девять" };
    std::cout << "Вывод вектора:" << std::endl;
    printWords(v.begin(), v.end());

    std::list<std::string> l(v.begin(), v.end());
    std::cout << "\nВывод листа:" << std::endl;
    printWords(l.begin(), l.end());

    std::deque<std::string> d(v.begin(), v.end());
    std::cout << "\nВывод деки:" << std::endl;
    printWords(d.begin(), d.end());
}
