#pragma once

#include <iostream>
#include <iterator>
#include <string>
#include <unordered_set>
#include <queue>

std::string getSentence(std::string &s)
{
    std::string punctuationMarks { ".?!" };                              
    std::string temp; 
 
    for (const char &c : s)                                               
    {
        temp.push_back(c);  
      
        if (punctuationMarks.find(c) != std::string::npos)                
        {
            s.erase(0, temp.size());                                      
            if (temp[0] == ' ') { temp.erase(0, 1); }                     
            return temp;                                                 
        }
    }

    return {};                                                           
}

void exercise_2()
{
    std::unordered_set<std::string, std::hash<std::string>> sentences;
    std::string currentString, tempString;    
  
    while (std::getline(std::cin, currentString))
    {
        if (!currentString.empty())                                       
        {
            tempString += currentString + ' ';                           
                                                                         
            while (true)
            {
                std::string sentence(std::move(getSentence(tempString))); 
                if (sentence.empty())
                {
                    break;                                                
                }
                sentences.insert(std::move(sentence));                    
            }
        }
    }

    std::priority_queue<std::pair<size_t, std::string>> q;
    for (const auto &sentence : sentences)
    {
        q.push({ sentence.size(), sentence });
    }
    while (!q.empty())
    {
        std::cout << q.top().first << ": " << q.top().second << '\n';
        q.pop();
    }
}
