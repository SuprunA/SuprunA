#pragma once

#include <iostream>
#include <numeric>
#include <fstream>
#include "src-gen/exercise_2.pb.h"

void exercise_1_2()
{
    exercise_2::FullName fn;
    fn.set_name("Andrey");
    fn.set_surname("Suprun");
    fn.set_patronymic("Andreevich");

    exercise_2::Student s;
    *s.mutable_name() = fn;
    s.add_grades(5);
    s.add_grades(2);
    s.add_grades(4);
    s.add_grades(3);
    s.add_grades(4);
    s.add_grades(3);
    s.add_grades(4);
    s.add_grades(3);
    s.set_avg_score(std::accumulate(s.grades().begin(), s.grades().end(), 0) / s.grades().size());

    exercise_2::StudentsGroup sg;
    *sg.add_students() = s;

    std::ofstream out("student.bin", std::ios_base::binary);
    sg.SerializeToOstream(&out);
    out.close();

    exercise_2::StudentsGroup new_sg;
    std::ifstream in("student.bin", std::ios_base::binary);
    if (new_sg.ParseFromIstream(&in))
    {
        std::cout << new_sg.students(0).name().surname() << std::endl;
        std::cout << new_sg.students(0).avg_score() << std::endl;
    }
    else
    {
        std::cout << "Error!" << std::endl;
    }
    in.close();
}
