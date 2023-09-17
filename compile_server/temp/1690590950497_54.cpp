#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>

using namespace std;
class Solution{
    public:
    bool isPalindrome(int x)
    {
        while(1)
        return true;
    }
};
#ifndef COMPILER_ONLINE
#include"header.cpp"
#endif

bool Test1()
{
   bool ret = Solution().isPalindrome(121);
   if(!ret){
       std::cout << "没有通过测试用例1，测试值121" << std::endl;
   }
   return ret;
}

bool Test2()
{
    bool ret = Solution().isPalindrome(-10);
    if(ret){
        std::cout << "没有通过测试用例2，测试值-10" << std::endl;
    }
    return !ret;
}

bool Test3()
{
    bool ret = Solution().isPalindrome(12321);
    if(!ret){
        std::cout << "没有通过测试用例3，测试值12321" << std::endl;
    }
    return ret;
}

bool Test4()
{
    bool ret = Solution().isPalindrome(12345);
    if(ret){
        std::cout << "没有通过测试用例4，测试值12345" << std::endl;
    }
    return !ret;
}

bool Test5()
{
    bool ret = Solution().isPalindrome(0);
    if(!ret){
        std::cout << "没有通过测试用例5，测试值0" << std::endl;
    }
    return ret;
}

bool Test6()
{
    bool ret = Solution().isPalindrome(1234321);
    if(!ret){
        std::cout << "没有通过测试用例6，测试值1234321" << std::endl;
    }
    return ret;
}

void RunAllTests()
{
    if(Test1() && Test2() && Test3() && Test4() && Test5() && Test6()){
        std::cout << "全部通过测试" << std::endl;
    }
}

int main()
{
    RunAllTests();
    return 0;
}
