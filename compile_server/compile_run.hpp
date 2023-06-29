#pragma once
#include <signal.h>
#include<unistd.h>
#include "../common/util.hpp"
#include "../common/log.hpp"
#include "compiler.hpp"
#include "runner.hpp"
#include <jsoncpp/json/json.h>

namespace ns_compile_and_run
{
   using namespace ns_log;
   using namespace ns_util;
   using namespace ns_runner;
   using namespace ns_compiler;
   static void RemoveTempFile(const std::string &file_name )
   {
        //清理文件的个数是不确定的，但是有哪些我们是知道的
            std::string _src = PathUtil::Src(file_name);
            if(FileUtil::IsFileExists(_src)) unlink(_src.c_str());

            std::string _compiler_error = PathUtil::CompilerError(file_name);
            if(FileUtil::IsFileExists(_compiler_error)) unlink(_compiler_error.c_str());

            std::string _execute = PathUtil::Exe(file_name);
            if(FileUtil::IsFileExists(_execute)) unlink(_execute.c_str());

            std::string _stdin = PathUtil::Stdin(file_name);
            if(FileUtil::IsFileExists(_stdin)) unlink(_stdin.c_str());

            std::string _stdout = PathUtil::Stdout(file_name);
            if(FileUtil::IsFileExists(_stdout)) unlink(_stdout.c_str());

            std::string _stderr = PathUtil::Stderr(file_name);
            if(FileUtil::IsFileExists(_stderr)) unlink(_stderr.c_str());
   }
   /*******************************************
    输入
    code：用户提交的代码
    input：用户给自己提交的代码对应的输入
    cpu_limit:时间要求
    cpu_limit:空间要求
      选填：
    stdout：程序运行完的结果
    stderr：程序运行完的错误结果

    输出
    status ：状态码
    reason：请求结果

    in_json {"code","#include...","input","cpu_limit","mem_limit"}
    out_json{"status","reason","stdout","stderr",}
    **********************************
   */
   class CompileAndRun
   {
   public:
      // code>0 进程收到了信号导致异常崩溃
      // code<0 非运行报错（代码为空或者编译报错）
      // code=0 过程全部完成
      static std::string CodeToDesc(int code,const std::string &file_name)
      {
         std::string desc;
         switch (code)
         {
         case  0:
            desc = "编译运行时成功";
            break;
         case -1:
            desc = "用户提交的代码为空";
            break;
         case -2:
            desc = "未知错误";
            break;
         case -3:
             FileUtil::ReadFile(PathUtil::CompilerError(file_name),&desc,true);
            break;
         case SIGABRT: // 6
            desc = "内存超过范围";
            break;
         case SIGXCPU: // 24
            desc = "CPU使用超时";
            break;
         case SIGFPE: // 8
            desc = "浮点数溢出";
            break;
         default:
         desc="debug"+std::to_string(code);
            break;
         }
         return desc;
      }
      static std::string SignoToDesc(int signo)
      {
      }
      static void Start(const std::string &in_json, std::string *out_json)
      {
         Json::Value in_value;
         Json::Reader reader;
         reader.parse(in_json, in_value);
         std::string code = in_value["code"].asString();
         std::string input = in_value["input"].asString();
         int cpu_limit = in_value["cpu_limit"].asInt();
         int men_limit = in_value["mem_limit"].asInt();

         int status_code = 0;
         Json::Value out_value;
         int run_result = 0;
         std::string file_name;
         // 差错处理
         if (code.size() == 0)
         {
            status_code = -1; // 代码为空
            // out_value["status"] = -1; // 代码为空
            // out_value["reason"] = "用户提交的代码是空的";
            goto END;
         }
         file_name = FileUtil::UniqueFileName();
         if (!FileUtil::WriteFile(PathUtil::Src(file_name), code)) // 形成临时SRC源文件
         {
            status_code = -2; // 未知错误
            // out_value["status"] = -2;
            // out_value["reason"] = "提交的代码发生了未知错误";
            goto END;
         }
         if (!Compiler::Compile(file_name))
         {

            status_code = -3; // 编译错误
            // out_value["status"] = -3;
            // out_value["reason"] = FileUtil::ReadFile(PathUtil::CompilerError(file_name));
            goto END;
         }
         run_result = Runner::Run(file_name, cpu_limit, men_limit);
         if (run_result < 0)
         {
            status_code = -2; // 编译错误

            // out_value["status"] = -2;
            // out_value["reason"] = "提交的代码发生了未知错误";
            goto END;
         }
         else if (run_result > 0)
         {
            status_code = run_result;
           // out_value["status"] = code;
            //out_value["reason"] = SignoToDesc(run_result); // 将信号转化为报错原因
         }
         else
         {
            // 运行成功
            status_code = 0;
         }
      END:
         out_value["status"] = status_code;
         out_value["reason"] = CodeToDesc(status_code,file_name);
         if (status_code == 0)
         {
            std::string _stdout;
            std::string _stderr;

            FileUtil::ReadFile(PathUtil::Stdout(file_name),&_stdout,true);
            FileUtil::ReadFile(PathUtil::Stderr(file_name),&_stderr,true);
            out_value["stdout"] = _stdout;
            out_value["stderr"] = _stderr;
         }
         Json::StyledWriter writer;
         *out_json = writer.write(out_value);

         //RemoveTempFile(file_name);
      }
   };
}