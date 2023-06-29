#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "../common/util.hpp"
#include "../common/log.hpp"
// 该模块只负责代码编译
namespace ns_compiler
{
    using namespace ns_util;
    using namespace ns_log;
    class Compiler
    {
    public:
        Compiler()
        {
        }
        ~Compiler()
        {
        }
        // 判断是否编译成功
        static bool Compile(const std::string &file_name)
        {
            pid_t pid = fork();
            if (pid < 0)
            {
                LOG(ERROR)<<"内部错误，创建子进程失败"<<"\n";
                return false;
            }
            else if (pid == 0)
            {
                umask(0);//确保接下来创建的文件能够拥有尽可能大的权限
                int _stderr=open(PathUtil::CompilerError(file_name).c_str(),O_CREAT|O_WRONLY,0644);
                if(_stderr<0){
                    LOG(WARNING)<<"没有成功形成stderr文件"<<"\n";
                    exit(1);
                }
                dup2(_stderr,2);
                // 调用编译器编译代码
                execlp("g++", "g++","-o", PathUtil::Exe(file_name).c_str(), PathUtil::Src(file_name).c_str(), "-std=c++11","-D","COMPILER_ONLINE", nullptr);
                LOG(ERROR)<<"g++启动失败"<<"\n";
                exit(2);//运行失败进程终止
            }
            else
            {
                waitpid(pid,nullptr,0);
                //编译是否成功
                if(FileUtil::IsFileExists(PathUtil::Exe(file_name))){
                    LOG(INFO)<<PathUtil::Src(file_name)<<"编译成功"<<"\n";
                return true;
                }
                LOG(ERROR)<<"编译失败没有形成可执行程序"<<"\n";
                return false;
            }
        }
    };
}
