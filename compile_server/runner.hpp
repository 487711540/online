#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include<sys/resource.h>
#include<sys/time.h>
#include "../common/log.hpp"
#include "../common/util.hpp"

namespace ns_runner
{
    using namespace ns_log;
    using namespace ns_util;

    class Runner
    {
    public:
        Runner(){};
        ~Runner(){};

    public:
    //设置进程占用资源大小
    static void SetProcLimit(int _cpu_limit,int _mem_limit)
    {
        //设置cpu时常
        struct rlimit cpu_rlimit;
        cpu_rlimit.rlim_max=RLIM_INFINITY;
        cpu_rlimit.rlim_cur=_cpu_limit;
        setrlimit(RLIMIT_CPU,&cpu_rlimit);

        //设置内存大小
        struct rlimit mem_rlimit;
        mem_rlimit.rlim_max=RLIM_INFINITY;
        mem_rlimit.rlim_cur=_mem_limit*1024;
        setrlimit(RLIMIT_CPU,&cpu_rlimit);
        
    }
        static int Run(const std::string &file_name,int cpu_limit,int mem_limit)
        {
            std::string _execute = PathUtil::Exe(file_name);
            std::string _stdin = PathUtil::Stdin(file_name);
            std::string _stdout = PathUtil::Stdout(file_name);
            std::string _stderr = PathUtil::Stderr(file_name);
            umask(0);
            int _stdin_fd = open(_stdin.c_str(), O_CREAT | O_RDONLY, 0644);
            int _stdout_fd = open(_stdout.c_str(), O_CREAT | O_WRONLY, 0644);
            int _stderr_fd = open(_stderr.c_str(), O_CREAT | O_WRONLY, 0644);

            if (_stdin_fd < 0 || _stdout_fd < 0 || _stderr_fd < 0)
            {
                LOG(ERROR)<<"运行时打开标准文件失败"<<"\n";
                return -1; // 打开文件失败
            }

            pid_t pid = fork();
            if (pid < 0)
            {
                LOG(ERROR)<<"运行时创建子进程失败"<<"\n";

                close(_stdin_fd);
                close(_stdout_fd);
                close(_stderr_fd);
                return -2; // 代表创建子进程失败
            }
            else if (pid == 0)
            {
                dup2(_stdin_fd,0);
                dup2(_stdout_fd,1);
                dup2(_stderr_fd,2);

                SetProcLimit(cpu_limit,mem_limit);
                execl(_execute.c_str(),_execute.c_str(),nullptr);
                exit(1);
            }
            else
            {
                close(_stdin_fd);
                close(_stdout_fd);
                close(_stderr_fd);
                int status = 0;
                waitpid(pid, &status, 0);
                LOG(INFO)<<"运行完毕，info："<<(status &0x7F)<<"\n";//&的优先级比<<低
        return status & 0x7F;//获取信号
            }
        }
    };
}