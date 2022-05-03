#include "lesson_1.hpp"

#include <sstream>
#include <algorithm>
#include <iomanip>

/****************************
 *          Person          *
 ****************************/

std::optional<std::string> getOptStr(std::string &s)
{
    if (s == "")
    {
        return std::nullopt;
    }

    return s;
}

std::ostream& operator<<(std::ostream &out, const Person &p)
{
    out << std::setw(15) << p.firstname << std::setw(12) << p.lastname;

    if (p.patronymic.has_value())
    {
        out << std::setw(17) << p.patronymic.value();
    }
    else
    {
        out << std::setw(18);
    }

    return out;
}

bool operator<(const Person &p1, const Person &p2)
{
    return tie(p1.firstname, p1.lastname, p1.patronymic) < tie(p2.firstname, p2.lastname, p2.patronymic);
}

bool operator==(const Person &p1, const Person &p2)
{
    return tie(p1.firstname, p1.lastname, p1.patronymic) == tie(p2.firstname, p2.lastname, p2.patronymic);
}

/*********************************
 *          PhoneNumber          *
 *********************************/

bool operator<(const PhoneNumber &p1, const PhoneNumber &p2)
{
    return tie(p1.country_code, p1.city_code, p1.number, p1.additional_number) < tie(p2.country_code, p2.city_code, p2.number, p2.additional_number);
}

std::optional<int> getOptInt(std::string &s)
{
    if (s == "")
    {
        return std::nullopt;
    }

    return std::stoi(s);
}

std::ostream& operator<<(std::ostream &out, const PhoneNumber &p)
{
    out << std::setw(3) << '+' << p.country_code << '(' << p.city_code << ')' << p.number;

    if (p.additional_number.has_value())
    {
        out << ' ' << p.additional_number.value();
    }

    return out;
}

/*******************************
 *          PhoneBook          *
 *******************************/

PhoneBook::PhoneBook(std::istream &file)
{
    if (!file)
    {
        std::cout << "Не удаётся открыть файл!" << std::endl;
        exit(1);
    }

    for (std::string line; std::getline(file, line);)
    {
        std::stringstream str(line);
        std::vector<std::string> rowData;

        for (std::string s; getline(str, s, ';');)
        {
            rowData.push_back(s);
        }

        std::pair<Person, PhoneNumber> entry;

        for (size_t i = 0; i < rowData.size(); ++i)
        {
            switch (i)
            {
                case 0:
                    entry.first.firstname = rowData[i];
                    break;
                case 1:
                    entry.first.lastname = rowData[i];
                    break;
                case 2:
                    entry.first.patronymic = getOptStr(rowData[i]);
                    break;
                case 3:
                    entry.second.country_code = stoi(rowData[i]);
                    break;
                case 4:
                    entry.second.city_code = stoi(rowData[i]);
                    break;
                case 5:
                    entry.second.number = rowData[i];
                    break;
                case 6:
                    entry.second.additional_number = getOptInt(rowData[i]);
                    break;
            }
        }

        m_data.push_back(entry);
    }
}

std::ostream& operator<<(std::ostream &out, const PhoneBook &pb)
{
    for (const auto& [first, second] : pb.m_data)
    {
        out << first << ' ' << second << std::endl;
    }

    return out;
}

void PhoneBook::SortByName()
{
    sort(m_data.begin(), m_data.end(), [](const std::pair<Person, PhoneNumber> &lhs, const std::pair<Person, PhoneNumber> &rhs)
    {
        return lhs.first < rhs.first;
    });
}

void PhoneBook::SortByPhone()
{
    sort(m_data.begin(), m_data.end(), [](const std::pair<Person, PhoneNumber> &lhs, const std::pair<Person, PhoneNumber> &rhs)
    {
        return lhs.second < rhs.second;
    });
}

std::pair<std::string, std::vector<PhoneNumber>> PhoneBook::GetPhoneNumber(const std::string &firstname)
{
    std::vector<PhoneNumber> phoneNumbers;
    int count = 0;

    for_each(m_data.begin(), m_data.end(), [&](const auto &entry)
    {
        if (entry.first.firstname == firstname)
        {
            phoneNumbers.push_back(entry.second);
            ++count;
        }
    });

    switch (count)
    {
        case 0:
            return { "not found", phoneNumbers };
        case 1:
            return { "", phoneNumbers };
        default:
            return { "found more than 1", phoneNumbers };
    }
}

void PhoneBook::ChangePhoneNumber(const Person &p, const PhoneNumber &pn)
{
    auto entry = find_if(m_data.begin(), m_data.end(), [&](const auto &entry)
    {
        return entry.first == p;
    });

    if (entry != m_data.end())
    {
        entry->second = pn;
    }
}
