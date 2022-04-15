#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Timer.hpp"

using namespace std;

template<typename T>
void Swap(T *a, T *b)
{
    T temp = *a;
    *a = *b;
    *b = temp;
}

template<typename T>
void SortPointers(vector<T*> &v)
{
    sort(v.begin(), v.end(), [](const auto &v1, const auto &v2)
    {
        return *v1 < *v2;
    });
}

void exercise_1()
{
    int a(5);
    int b(6);

    Swap(&a, &b);

    cout << a << ' ' << b << endl;
}

void exercise_2()
{
    int length = 5;
    vector<int*> v;

    for (int i = 0; i < length; ++i)
    {
        int *a = new int;
        *a = rand() % 10;
        v.push_back(a);
    }

    for (int i = 0; i < length; ++i)
    {
        cout << *v[i] << ' ';
    }

    cout << endl;

    SortPointers(v);

    for (int i = 0; i < length; ++i)
    {
        cout << *v[i] << ' ';
    }

    cout << endl;
}

namespace counting
{
    const string_view vowels
    { "AEIOUaeiou" };

    void way1(const string_view &s)
    {
        Timer timer("Counting vowels [way 1]");
        size_t count = count_if(s.begin(), s.end(), [&](const auto &c)
        {
            return vowels.find(c) != string::npos;
        });
        cout << count << endl;
        timer.print();
    }

    void way2(const string_view &s)
    {
        Timer timer("Counting vowels [way 2]");
        size_t count = count_if(s.begin(), s.end(), [&](const auto &c)
        {
            for (size_t i = 0; i < vowels.size(); ++i)
            {
                if (vowels[i] == c)
                return true;
            }
            return false;
        });
        cout << count << endl;
        timer.print();
    }

    void way3(const string_view &s)
    {
        size_t count = 0;
        Timer timer("Counting vowels [way 3]");
        for (size_t i = 0; i < s.size(); ++i)
        {
            if (vowels.find(s[i]) != string::npos)
            {
                ++count;
            }
        }
        cout << count << endl;
        timer.print();
    }

    void way4(const string_view &s)
    {
        size_t count = 0;
        Timer timer("Counting vowels [way 4]");
        for (size_t i = 0; i < s.size(); ++i)
        {
            for (size_t j = 0; j < vowels.size(); ++j)
            {
                if (vowels[j] == s[i])
                    ++count;
            }
        }
        cout << count << endl;
        timer.print();
    }
}

void exercise_3()
{
    ifstream file("War_and_peace.txt");
    file.seekg(0, ios::end);
    size_t size = file.tellg();
    file.seekg(0);
    string s(size, ' ');
    file.read(&s[0], size);

    counting::way1(s);               // Counting vowels [way 1]:    59.6968 ms
    counting::way2(s);               // Counting vowels [way 2]:    177.219 ms
    counting::way3(s);               // Counting vowels [way 3]:    51.9079 ms
    counting::way4(s);               // Counting vowels [way 4]:    234.762 ms
}

int main()
{
//    exercise_1();
//    exercise_2();
    exercise_3();

    return 0;
