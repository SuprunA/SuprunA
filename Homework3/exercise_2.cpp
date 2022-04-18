#include <iostream>
#include <iomanip>
#include <random>
#include <math.h>
#include "exercise_2.hpp"

using namespace std;

Matrix::Matrix(const int size) : matrixSize(size), matrixCapacity(size * size)
{
    array = new double[matrixCapacity];
}

/*
 * Перегрузка операторов итератора
 */
Matrix::Iterator::Iterator(double *pointer) : current(pointer)
{
}

double& Matrix::Iterator::operator[](size_t i)
{
    return *(current + i);
}

Matrix::Iterator& Matrix::Iterator::operator ++()
{
    ++current;
    return *this;
}

bool Matrix::Iterator::operator !=(const Iterator &iterator)
{
    return current != iterator.current;
}

double& Matrix::Iterator::operator *()
{
    return *current;
}

Matrix::Iterator Matrix::operator[](size_t i)
{
    return Iterator(array + i * matrixSize);
}

Matrix::Iterator Matrix::begin()
{
    return Iterator(array);
}

Matrix::Iterator Matrix::end()
{
    return Iterator(array + matrixCapacity);
}

/*
 * Расчет определителя
 */
Matrix Matrix::getModifiedMatrix(Matrix &arr, const size_t item)
{
    Matrix newArray(arr.size() - 1);
    size_t currentRow = 0;

    for (size_t i = 0; i < arr.size(); ++i)
    {
        if (item == i)
        {
            currentRow = i;
            continue;
        }

        for (size_t j = 0; j < newArray.size(); ++j)
        {
            newArray[currentRow][j] = arr[i][j + 1];
        }

        ++currentRow;
    }

    return newArray;
}

double Matrix::calculateDeterminant(Matrix &arr)
{
    double determinant = 0;

    if (arr.size() == 1)
    {
        determinant = arr[0][0];
    }
    else if (arr.size() == 2)
    {
        determinant = arr[0][0] * arr[1][1] - arr[1][0] * arr[0][1];
    }
    else
    {
        int sign = 1;

        for (size_t i = 0; i < arr.size(); ++i)
        {
            Matrix newArray = getModifiedMatrix(arr, i);
            determinant += sign * arr[i][0] * calculateDeterminant(newArray);
            sign = -sign;
        }
    }

    return determinant;
}

void Matrix::fillRandom()
{
    random_device rd;
    mt19937 mersenne(rd());                                   // x32-битное число
    uniform_real_distribution<double> urd(-10.0, 10.0);       // диапазон вещественных чисел от -10 до 10
    for (size_t i = 0; i < matrixCapacity; ++i)
    {
        *(array + i) = round(urd(mersenne) * 10) / 10;
    }

    matrixDeterminant = calculateDeterminant((*this));
}

void Matrix::print() const
{
    cout << '[';

    for (size_t i = 0; i < matrixCapacity;)
    {
        cout << setw(5) << *(array + i++);

        if (!(i % matrixSize))
        {
            cout << " ]" << endl;

            if (i < matrixCapacity)
            {
                cout << '[';
            }
        }
        else
        {
            cout << ',';
        }
    }
}

size_t Matrix::size() const
{
    return matrixSize;
}

double Matrix::determinant() const
{
    return matrixDeterminant;
}

Matrix::~Matrix()
{
    delete[] array;
}

void exercise_2()
{
    for (size_t i = 1; i < 8; ++i)
    {
        cout << "Матрица " << i << 'x' << i << ':' << endl;
        Matrix m(i);
        m.fillRandom();
        m.print();
        cout << "Определитель матрицы: " << setprecision(10) << m.determinant() << "\n\n";    // манипулятор для вывода без экспоненты
    }
}
