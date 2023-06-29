#include "compile_run.hpp"
#include "../common/httplib.h"
using namespace ns_compile_and_run;
using namespace httplib;
void Usage (std::string proc)
{
  std::cerr<<"Usage"<<"\n\t"<<proc<<"port"<<std::endl;
}
int main(int argc,char *argv[])
{
  if(argc!=2){
    Usage(argv[0]);
    return 1;
  }
  Server svr;
svr.Get("/hello",[](const Request &req, Response &resp){

    resp.set_content("hello httplib,你好 httplib!", "content-type: text/plain;");
    });
  //std::cout<<"hello"<<std::endl;
 
  svr.Post("/compile_and_run", [](const Request &req, Response &resp){
        // 用户请求的服务正文是我们想要的json string
        std::string in_json = req.body;
        std::string out_json;
        if(!in_json.empty()){
            CompileAndRun::Start(in_json, &out_json);
            resp.set_content(out_json, "application/json;charset=utf-8");
        }
    });
 svr.listen("0.0.0.0",atoi(argv[1]));
  std::string in_json;

  Json::Value in_value;
  in_value["code"]=R"(#include<iostream>
   int main(){
     std::cout<<"can you see me?"<< std::endl ;
     while(1);
    return 0;
    })";
  in_value["input"]="";
  in_value["cpu_limit"]=1;
  in_value["mem_limit"]=10240*3;
  Json::FastWriter writer;
  in_json=writer.write(in_value);

  std::string out_json;
  CompileAndRun::Start(in_json,&out_json);
  std::cout<<in_json<<std::endl;

   std::cout<<out_json<<std::endl;
  return 0;
}