#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include "Timer.hpp"

bool isPrime(size_t num)
{
    size_t limit = num / 2;

    if (num > 2)
    {
        for (size_t i = 2; i <= limit; ++i)
        {
            if (num % i == 0)
            {
                return false;
            }
        }
    }

    return true;
}

void exercise_2()
{

    size_t maxValue = 500000;
    size_t counThreads = 30;
    std::vector<size_t> answers;
    std::mutex m;

    Timer timer("With threads");

    for (size_t i = 0; i < maxValue;)
    {
        std::vector<std::thread> v;

        for (size_t j = i + counThreads; i <= j; ++i)
        {
            v.push_back(std::thread([=, &m, &answers]()
            {
                if (isPrime(i))
                {
                    std::lock_guard lg(m);
                    answers.push_back(i);
                }
            }));
        }

        for (auto &t : v)
        {
            t.join();
        }
    }

//    for (const auto &a : answers)
//    {
//        std::cout << "Число " << a << " простое" << std::endl;
//    }
    timer.print();

    answers.clear();
    answers.shrink_to_fit();

    timer.start("Without threads");

    for (size_t i = 0; i < maxValue; ++i)
    {
        if (isPrime(i))
        {
            answers.push_back(i);
        }
    }

//    for (const auto &a : answers)
//    {
//        std::cout << "Число " << a << " простое" << std::endl;
//    }
    timer.print();
}
