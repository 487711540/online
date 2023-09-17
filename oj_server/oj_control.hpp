#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <mutex>
#include <cassert>
#include "oj_model.hpp"
#include "../common/util.hpp"
#include "../common/log.hpp"
#include "../common/httplib.h"
#include "oj_view.hpp"
#include <jsoncpp/json/json.h>
namespace ns_control
{
    using namespace std;
    using namespace ns_log;
    using namespace ns_util;
    using namespace ns_model;
    using namespace ns_view;
    using namespace httplib;
    // 负载均衡模块
    class Machine
    {
    public:
        std::string ip;  // 编译服务的ip
        int port;        // 编译服务的端口
        uint64_t load;   // 编译服务的负载
        std::mutex *mtx; // mutex禁止拷贝
    public:
        Machine() : ip(""), port(0), load(0), mtx(nullptr)
        {
        }
        ~Machine()
        {
        }

    public:
        void IncLoad()
        {
            if (mtx)
                mtx->lock();
            ++load;
            if (mtx)
                mtx->unlock();
        }
        void Descrease()
        {
            if (mtx)
                mtx->lock();
            --load;
            if (mtx)
                mtx->unlock();
        }
        void RestLoad()
        {
            if(mtx)mtx->lock();
            load=0;
            if(mtx)mtx->unlock();
        }
        uint64_t Load()
        {
            uint64_t _load = 0;
            if (mtx)
                mtx->lock();
            _load = load;
            if (mtx)
                mtx->unlock();
            return _load;
        }
    };
    const std::string service_machine = "./conf/service_machine.conf";
    class LoadBlance
    {
    private:
        // 可以提供编译服务的所有主机
        std::vector<Machine> machines;
        // 所有在线主机的id
        std::vector<int> online;
        // 所有离线主机的id
        std::vector<int> offline;
        // 保证选择主机的数据安全
        std::mutex mtx;

    public:
        LoadBlance()
        {
            assert(LoadConf(service_machine));
            LOG(INFO) << "加载" << service_machine << "成功"
                      << "\n";
        }
        ~LoadBlance()
        {
        }
        bool LoadConf(const std::string &machine_conf)
        {
            std::ifstream in(machine_conf);
            if (!in.is_open())
            {
                LOG(FATAL) << "加载：" << machine_conf << "失败"
                           << "\n";
                return false;
            }
            std::string line;
            while (std::getline(in, line))
            {
                std::vector<std::string> tokens;
                StringUtil::SplitString(line, &tokens, ":");
                if (tokens.size() != 2)
                {
                    LOG(WARNING) << "切分" << line << "失败"
                                 << "\n";
                    continue;
                }
                Machine m;
                m.ip = tokens[0];
                m.port = atoi(tokens[1].c_str());
                m.load = 0;
                m.mtx = new std::mutex();
                online.push_back(machines.size());
                machines.push_back(m);
            }
            in.close();
            return true;
        }
        // id:输出型参数
        // m：输出型参数
        bool SmartChoice(int *id, Machine **m)
        {
            // 使用我们选择好的主机
            mtx.lock();

            int online_num = online.size();
            if (online_num == 0)
            {
                mtx.unlock();

                LOG(FATAL) << "所有后端编译主机均已离线"
                           << endl;
                return false;
            }
            *id = online[0];
            *m = &machines[online[0]];
            // 通过遍历的方式找到负载最小的机器
            uint64_t min_load = machines[online[0]].Load();
            for (int i = 1; i < online_num; i++)
            {
                uint64_t curr_load = machines[online[i]].Load();
                if (min_load > curr_load)
                    
                {
                    min_load = curr_load;
                    *id = online[i];
                    *m = &machines[online[i]];
                }
            }
            mtx.unlock();
            return true;
        }
        void OfflineMachine(int const which)
        {
            mtx.lock();
            for (auto iter = online.begin(); iter != online.end(); iter++)
            {
                if (*iter == which)
                {
                    machines[which].RestLoad();
                    // 已经找到要离线的主机
                    online.erase(iter);
                    offline.push_back(which);
                    break;
                }
            }
            mtx.unlock();
        }
        void OnlineMachine()
        {
            // 统一上线
             mtx.lock();
            online.insert(online.end(), offline.begin(), offline.end());
            offline.erase(offline.begin(), offline.end());
            mtx.unlock();

            LOG(INFO) << "所有的主机有上线啦!" << "\n";
        }
        // test
        void ShowMachine()
        {
            mtx.lock();
            std::cout << "当前在线主机列表：";
            for (auto &id : online)
            {
                std::cout << id << " ";
            }

            std::cout << std::endl;
            mtx.unlock();
        }
    };
    // 核心控制器
    class Control
    {
    private:
        Model model_;             // 提供后台数据
        View view_;               // 提供html渲染功能
        LoadBlance load_balance_; // 核心负载均衡器
    public:
        Control()
        {
        }
        ~Control()
        {
        }

    public:
        // 跟据题目数据构建网页
          void RecoveryMachine()
        {
            load_balance_.OnlineMachine();
        }
        bool AllQuestions(string *html)
        {
            vector<struct Question> all;
            if (model_.GetAllQuetions(&all))
            {
                sort(all.begin(),all.end(),[](const struct Question &q1,const struct Question &q2){
                    return atoi(q1.number.c_str())<atoi(q2.number.c_str());
                });
                view_.AllExpandHtml(all, html);
            }
            else
            {
                *html = "获取题目失败，形成题目列表失败";
                return false;
            }
            return true;
        }
        bool Question(const string &number, string *html)
        {
            bool ret = true;
            struct Question q;
            // cout<<"hello"<<endl;
            if (model_.GetOneQuestion(number, &q))
            {
                // 获取指定题目信息成功，将所有的题目数据构建成网页
                view_.OneExpandHtml(q, html);
                cout << "hello" << endl;
            }

            else
            {
                *html = "指定题目: " + number + " 不存在!";
                ret = false;
            }
            return ret;
        }

        // code:#include
        // input:""
        void Judge(const std::string &number, const std::string in_json, std::string *out_json)
        {

            // 跟据题号直接拿到对应的题目细节
            struct Question q;
            model_.GetOneQuestion(number, &q);
            // in_json进行反序列化，得到题目id，得到用户提交的源代码，input
            Json::Reader reader;
            Json::Value in_value;
            reader.parse(in_json, in_value);
            std::string code = in_value["code"].asString();
            // 拼接用户代码加测试用例代码形成新的代码
            Json::Value compile_value;
            compile_value["input"] = in_value["value"].asString();
            compile_value["code"] = code + q.tail;
            compile_value["cpu_limit"] = q.cpu_limit;
            compile_value["mem_limit"] = q.mem_limit;
            Json::FastWriter writer;
            std::string compile_string = writer.write(compile_value);
            // 选择负载最低的主机
            // 一直选择我，直到主机可用，否则就是全部挂掉
            std::cout << "正常" << std::endl;
            while (true)
            {
                int id = 0;
                Machine *m = nullptr;
                if (!load_balance_.SmartChoice(&id, &m))
                {
                    std::cout<<"选择失败"<<endl;
                    break;
                }
              
                // 发起http请求得到结果
                Client cli(m->ip, m->port);
                m->IncLoad();
                LOG(INFO) << "选择主机成功，主机id" << id << "详情" << m->ip << ":" << m->port <<"当前主机的负载是"<<m->load<< "port" << endl;
                if (auto res = cli.Post("/compile_and_run", compile_string, "application/json;charset=utf-8"))
                {
                    if (res->status == 200)
                    {
                        *out_json = res->body;
                        m->Descrease();
                        LOG(INFO) << "请求编译和运行服务"
                                  << "\n";
                        break;
                    }
                    m->Descrease();
                }
                else
                {
                    LOG(ERROR) << "当前求情的主机已经离线，主机id" << id << "详情" << m->ip << ":" << m->port << "port" << endl;

                    load_balance_.OfflineMachine(id);
                    load_balance_.ShowMachine(); // 仅用于调试
                }
            }
        }
    };
}