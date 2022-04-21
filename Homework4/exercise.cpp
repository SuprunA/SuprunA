#include <vector>
#include <list>
#include <math.h>
#include <algorithm>
#include "exercises.hpp"
#include "templates.hpp"
using namespace std;

void exercise_1()
{
    const unsigned int size = 10;
    int newIntValue = -3;
    double newDoubleValue = 2.2;

    vector<int> v(size);
    generator(v, -10, 10);
    sort(v.begin(), v.end());
    print(v);                             // -9 -9 -4 3 3 4 6 8 9 10
    insert_sorted(v, newIntValue);
    print(v);                             // -9 -9 -4 -3 3 3 4 6 8 9 10
    cout << endl;

    list<double> l(size);
    generator(l, -10.0, 10.0);
    l.sort();
    print(l);                             // -9.31992 -6.76971 -5.98545 -2.1297 -1.9722 -0.292701 0.715274 3.43089 4.35249 9.45377
    insert_sorted(l, newDoubleValue);
    print(l);                             // -9.31992 -6.76971 -5.98545 -2.1297 -1.9722 -0.292701 0.715274 2.2 3.43089 4.35249 9.45377
    cout << endl;
}

void exercise_2()
{
    vector<double> v(100);

    double error = 0;
    generator(v, -1000.0, 1000.0);

    print(v);                                                          // 1-ый вектор - печать исходного вектора
    copy(v.begin(), v.end(), ostream_iterator<int> { cout, " " });     // 2-ой вектор - печать целочисленного вектора
    cout << endl;

    for_each(v.begin(), v.end(), [&](const double &i)
    {
        error += pow(i - static_cast<int>(i), 2);
    });

    cout << endl << "Ошибка между цифровым и аналоговым сигналом равна " << error << endl;
}
