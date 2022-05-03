#pragma once

#include <iostream>
#include <thread>
#include <mutex>

static std::mutex mtx_cout;

class pcout
{
private:
    std::lock_guard<std::mutex> lg;
public:
    pcout() : lg(std::lock_guard<std::mutex>(mtx_cout))
    {
    }

    template<typename T>
    pcout& operator<<(const T &data)
    {
        std::cout << data;
        return *this;
    }

    pcout& operator<<(std::ostream& (*fp)(std::ostream&))                // т.к. std::endl является функцией,
    {                                                                    // то для неё делаем перегрузку operator<<
        std::cout << fp;                                                 // которая принимает указатель на функцию
        return *this;                                                    // типа std::ostream& и возвращает наш
    }                                                                    // защищённый поток вывода
};

void doSomething(int number)
{
    pcout() << "start thread " << number << std::endl;
    pcout() << "stop thread " << number << std::endl;
}

void exercise_1()
{
    std::thread th1(doSomething, 1);
    std::thread th2(doSomething, 2);
    std::thread th3(doSomething, 3);
    th1.join();
    th2.join();
    th3.join();
}
