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
 unsigned i=x;
    unsigned m=0;
    while(i>0)
    {
        m=m*10+i%10;
        i/=10;
    }
    return m==x;

        return true;
    }
};
#ifndef COMPILER_ONLINE
#include"header.cpp"
#endif


void Test1()
{

   bool ret=Solution().isPalindrome(121);
   if(ret)
   {
    std::cout<<"通过测试用例1，测试值121"<<std::endl;
   }
   else{
    std::cout<<"没有通过测试用例，测试值121"<<std::endl;
   }
}

void Test2()
{
    // 通过定义临时对象，来完成方法的调用
    bool ret = Solution().isPalindrome(-10);
    if(!ret){
        std::cout << "通过用例2, 测试-10通过 ... OK!" << std::endl;
    }
    else{
        std::cout << "没有通过用例2, 测试的值是: -10"  << std::endl;
    }
}

void Test3()
{
    bool ret = Solution().isPalindrome(12321);
    if(ret){
        std::cout << "通过用例3, 测试12321通过 ... OK!" << std::endl;
    }
    else{
        std::cout << "没有通过用例3, 测试的值是: 12321"  << std::endl;
    }
}

void Test4()
{
    bool ret = Solution().isPalindrome(12345);
    if(!ret){
        std::cout << "通过用例4, 测试12345通过 ... OK!" << std::endl;
    }
    else{
        std::cout << "没有通过用例4, 测试的值是: 12345"  << std::endl;
    }
}

void Test5()
{
    bool ret = Solution().isPalindrome(0);
    if(ret){
        std::cout << "通过用例5, 测试0通过 ... OK!" << std::endl;
    }
    else{
        std::cout << "没有通过用例5, 测试的值是: 0"  << std::endl;
    }
}

void Test6()
{
    bool ret = Solution().isPalindrome(1234321);
    if(ret){
        std::cout << "通过用例6, 测试1234321通过 ... OK!" << std::endl;
    }
    else{
        std::cout << "没有通过用例6, 测试的值是: 1234321"  << std::endl;
    }
}
int main()
{
Test1();
Test2();
Test3();
Test4();
Test5();
Test6();


return 0;
}
