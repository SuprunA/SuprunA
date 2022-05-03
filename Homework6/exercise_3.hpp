#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <random>

std::mutex m;

void generate_things(std::vector<int> &v)
{
    std::random_device rd;
    std::mt19937 mersenne(rd());
    std::uniform_int_distribution<int> urd(0, 1000);

    std::lock_guard lg(m);

    std::generate(v.begin(), v.end(), [&]()
    {
        return urd(mersenne);
    });
}

void pop_thing(std::vector<int> &v)
{
    std::lock_guard lg(m);

    std::cout << *std::max_element(v.begin(), v.end()) << std::endl;
}

void exercise_3()
{
    std::vector<int> v(100);
    size_t count = 100;

    while (count--)
    {
        std::thread owner([&]()
        {
            generate_things(v);
        });
        std::thread thief([&]()
        {
            pop_thing(v);
        });

        owner.join();
        thief.join();
    }
}
