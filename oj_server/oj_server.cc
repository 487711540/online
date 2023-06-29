#include<iostream>
#include"../common/httplib.h"
#include"oj_control.hpp"
#include"oj_model.hpp"

using namespace httplib;
using namespace ns_control;

int main(){
    Server svr;
    Control ctrl;
//获取所有题目列表
    svr.Get("/all_questions",[&ctrl](const Request &req,Response &resp){
        std::string html;
        ctrl.AllQuestions(&html);
resp.set_content(html,"text/html;charset=utf-8");
    });
    //用户根据题目编号获取题目内容
svr.Get(R"(/question/(\d+))",[&ctrl](const Request &req,Response &resp){
    std::string number=req.matches[1];
    std::string html;
    ctrl.Question(number,&html);
    resp.set_content(html,"text/html;charset=utf-8");
});
    //用户提交代码使用判题功能
 svr.Post(R"(/judge/(\d+))", [&ctrl](const Request &req, Response &resp){
      std::cout<<"接收到请求"<<std::endl;
        std::string number = req.matches[1];
        std::string result_json;
        ctrl.Judge(number, req.body, &result_json);
        resp.set_content(result_json, "application/json;charset=utf-8");
        // resp.set_content("指定题目的判题: " + number, "text/plain; charset=utf-8");
    });
    svr.set_base_dir("./wwwroot");
    svr.listen("0.0.0.0",8080);
    return 0;
}