#pragma once
#include "../common/util.hpp"
#include "../common/log.hpp"
#include <iostream>
#include <string>
#include <cassert>
#include <unordered_map>
#include <vector>
#include<fstream>
#include<cstdlib>

namespace ns_model
{
    using namespace std;
    using namespace ns_log;
    using namespace ns_util;
   struct Question
    {
       
        std::string number; // 题目编号
        std::string title;
        std::string star;   // 难度
        int cpu_limit;      // 题目时间要求
        int mem_limit;      // 题目空间要求
        std::string desc;   // 题目要求
        std::string header; // 预设代码
        std::string tail;   // 题目的测试用例
    };

    const std::string question_list = "./questions/questions.list";
    const std::string question_path="./questions/";
    class Model
    {
    private:
        // 题号：题目细节
        unordered_map<string, Question> questions;

    public:
        Model()
        {
            assert(LoadQuestionList(question_list));
        };
        bool LoadQuestionList(const std::string &question_list)
        {
            ifstream in(question_list);
            if(!in.is_open())
            {
                LOG(FATAL)<<"加载题库失败，请检查是否存在相应的题库文件"<<"\n";
                return false;
            }
            std::string line;
            while(getline(in,line))
            {
                vector<string> tokens;
                StringUtil::SplitString(line,&tokens," ");
                if(tokens.size()!=5)
                {
                LOG(WARNING)<<"加载部分题目失败，请假查文件格式"<<"\n";

                    continue;
                }
                Question q;
                q.number=tokens[0];
                q.title=tokens[1];
                q.star=tokens[2];
                q.cpu_limit=atoi(tokens[3].c_str());
                q.mem_limit=atoi(tokens[4].c_str());

                string path=question_path;
                 path+=q.number;
                  path += "/";

                if(!(FileUtil::ReadFile(path+"desc.txt",&(q.desc),true)&&FileUtil::ReadFile(path+"header.cpp",&(q.header),true)&&FileUtil::ReadFile(path+"tail.cpp",&(q.tail),true)))
                    return false;
            questions.insert({q.number,q});

            }
                LOG(INFO)<<"加载题库成功"<<"\n";

            in.close();
            return true;
        }
        bool GetAllQuetions(vector<Question> *out)
        {
            if (questions.size() == 0)
            {
                LOG(ERROR)<<"用户获取题库失败"<<endl;

                return false;
            }
            for (const auto &q : questions)
            {
                out->push_back(q.second);
            }
            
            return true;
        }
       bool GetOneQuestion(const std::string &number, Question *q)
        {
            const auto &iter = questions.find(number);
            if (iter == questions.end())  
            {
                  LOG(ERROR)<<"用户获取题库失败，题目编号"<<number<<"\n";
                return false;
            }
            (*q)=iter->second;
            return true;
        }
        ~Model(){

        };
    };
}