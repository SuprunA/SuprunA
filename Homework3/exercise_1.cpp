#include <iostream>
#include "exercise_1.hpp"

using namespace std;

void printList(list<float> &l)
{
    cout << "{ ";

    for (list<float>::const_iterator iter = l.begin(); iter != l.end(); ++iter)
    {
        cout << *iter << (next(iter, 1) == l.end() ? " " : ", ");
    }

    cout << '}' << endl;
}

void pushBackList(list<float> &l)
{
    float sum = 0;

    for (const float &value : l)
    {
        sum += value;
    }

    l.push_back(sum / l.size());
}

void exercise_1()
{
    list<float> l { 54.46, 43.32, 37.89, 19.99 };
    size_t countCycle = 15;

    for (size_t i = 0; i < countCycle; ++i)
    {
        // Печать листа через каждых 5-и добавлений элементов
        if (i % 5 == 0)
        {
            printList(l);
        }

        pushBackList(l);
    }

    printList(l);
}
