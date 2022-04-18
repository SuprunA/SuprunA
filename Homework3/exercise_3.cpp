#include <iostream>
#include <iomanip>
#include "exercise_3.hpp"
#include "exercise_2.hpp"

using namespace std;

void exercise_3()
{
    Matrix m(4);
    m.fillRandom();

    for (const double &i : m)
    {
        cout << setw(5) << i;
    }

    cout << endl;
}
