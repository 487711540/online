#pragma once

#include<iostream>
#include<string>
#include<ctemplate/template.h>
#include"oj_model.hpp"

namespace ns_view
{
    using namespace ns_model;
    const std::string template_path="./template_html/";
    class View
    {
        public:
        View(){};
        ~View(){};
        public:
        void AllExpandHtml(const vector<Question> &questions,std::string*html)
        {
            //形成路径
            std::string src_html=template_path+"all_questions.html";
            //形成数据字典
            ctemplate::TemplateDictionary root("all_questions");
            for(const auto& q: questions)
            {
                ctemplate::TemplateDictionary *sub=root.AddSectionDictionary("question_list");
                sub->SetValue("number",q.number);
                sub->SetValue("title",q.title);

                sub->SetValue("star",q.star);

            }
            //获取渲染HTML
            ctemplate::Template*tpl =ctemplate::Template::GetTemplate(src_html,ctemplate::DO_NOT_STRIP);
            //4开始完成渲染功能
            tpl->Expand(html,&root);
        }
        void OneExpandHtml(const Question &q,std::string *html){

             // 1. 形成路径
            std::string src_html = template_path + "one_question.html";

            // 2. 形成数字典
            ctemplate::TemplateDictionary root("one_question");
            root.SetValue("number", q.number);
            root.SetValue("title", q.title);
            root.SetValue("star", q.star);
            root.SetValue("desc", q.desc);
            root.SetValue("pre_code", q.header);

            //3. 获取被渲染的html
            ctemplate::Template *tpl = ctemplate::Template::GetTemplate(src_html, ctemplate::DO_NOT_STRIP);
           
            //4. 开始完成渲染功能
            tpl->Expand(html, &root);
        }
    };
}