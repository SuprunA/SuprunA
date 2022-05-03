#pragma once

#include <gtest/gtest.h>
#include "lesson_1.hpp"

class TestBook: public testing::Test
{
protected:
    PhoneBook *book;

    void SetUp() override
    {
        std::stringstream src_stream;
        src_stream << "Ivanov;Daniil;Maksimovich;7;366;7508887;;\n"
                << "Aleksandrov;Georgii;;493;7637;6114861;;\n"
                << "Aleksandrov;Andrey;Mikhailovich;7;247;1377660;5542;\n"
                << "Markelov;Dmitrii;Vadimovich;19;7576;5734416;2;\n";

        book = new PhoneBook(src_stream);
    }

    void TearDown() override
    {
        delete book;
    }
};

TEST_F(TestBook, GetPhoneNumber_notFound)
{
    std::string lastname = "Egorov";

    auto answer = book->GetPhoneNumber(lastname);
    EXPECT_EQ(std::get<0>(answer), "not found");
}

TEST_F(TestBook, GetPhoneNumber_foundOne)
{
    std::string lastname = "Ivanov";

    auto answer = book->GetPhoneNumber(lastname);
    EXPECT_EQ(std::get<0>(answer), "");
}

TEST_F(TestBook, GetPhoneNumber_foundMore)
{
    std::string lastname = "Aleksandrov";

    auto answer = book->GetPhoneNumber(lastname);
    EXPECT_EQ(std::get<0>(answer), "found more than 1");
}

TEST_F(TestBook, GetPhoneNumber_empty)
{
    std::string lastname = "";

    auto answer = book->GetPhoneNumber(lastname);
    EXPECT_EQ(std::get<0>(answer), "not found");
}

TEST_F(TestBook, ChangePhoneNumber)
{
    std::string lastname = "Ivanov";

    ASSERT_EQ(std::get<0>(book->GetPhoneNumber(lastname)).empty(), true);

    PhoneNumber answer = std::get<1>(book->GetPhoneNumber(lastname))[0];
    EXPECT_EQ(answer.number, "7508887");

    book->ChangePhoneNumber(Person { "Ivanov", "Daniil", "Maksimovich" }, PhoneNumber { 7, 123, "15344458", std::nullopt });

    answer = std::get<1>(book->GetPhoneNumber(lastname))[0];
    EXPECT_EQ(answer.country_code, 7);
    EXPECT_EQ(answer.city_code, 123);
    EXPECT_EQ(answer.number, "15344458");
    EXPECT_EQ(answer.additional_number, std::nullopt);
}

TEST_F(TestBook, sortByName)
{
    std::stringstream src_stream;
    src_stream << "    Aleksandrov      Andrey     Mikhailovich   +7(247)1377660 5542\n"
            << "    Aleksandrov     Georgii                    +493(7637)6114861\n"
            << "         Ivanov      Daniil      Maksimovich   +7(366)7508887\n"
            << "       Markelov     Dmitrii       Vadimovich   +19(7576)5734416 2\n";

    book->SortByName();

    std::stringstream dst_stream;
    dst_stream << *book;

    while (!dst_stream.eof())
    {
        std::string str_src;
        std::string str_dst;

        src_stream >> str_src ;
        dst_stream >> str_dst;

        EXPECT_EQ(str_dst, str_src);
    }
}

TEST_F(TestBook, sortByPhone)
{
    std::stringstream src_stream;
    src_stream << "    Aleksandrov      Andrey     Mikhailovich   +7(247)1377660 5542\n"
            << "         Ivanov      Daniil      Maksimovich   +7(366)7508887\n"
            << "       Markelov     Dmitrii       Vadimovich   +19(7576)5734416 2\n"
            << "    Aleksandrov     Georgii                    +493(7637)6114861\n";

    book->SortByPhone();

    std::stringstream dst_stream;
    dst_stream << *book;

    while (!dst_stream.eof())
    {
        std::string str_src;
        std::string str_dst;

        src_stream >> str_src ;
        dst_stream >> str_dst;

        EXPECT_EQ(str_dst, str_src);
    }
}
