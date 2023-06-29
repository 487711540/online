#include<iostream>
 int main(){while(1);std::cout<<"hello!!!!!!!"<<std::endl;}
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
     bool ret=Solution().isPalindrome(-10);
   if(!ret)
   {
    std::cout<<"通过测试用例1，测试值121"<<std::endl;
   }
   else{
    std::cout<<"没有通过测试用例，测试值121"<<std::endl;
   }
    
}



int main()
{
Test1();
Test2();
return 0;
}
