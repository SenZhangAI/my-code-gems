#include "stddef.h"
#include <iostream>


//=============================================================
// 核心代码
// st为某struct//class，m为该struct/class中的成员变量
template<typename T>
T* _nullptr() { T* t = NULL; return t; }

#define OFFSET_OF(st, m) \
((size_t) ( (char* )&(_nullptr<st>() -> m) - (char* )0 ))
//=============================================================



//=============================================================
// 测试
struct Test {
    int m1;
    double m2;
    char m3;
    bool m4;
    char* m5;
};

int main(int argc, char const* argv[])
{
//为了方便测试写的宏
#define GET_OFFSET_OF_TEST(x) OFFSET_OF(Test, m##x)
#define OUTPUT(x) std::cout << GET_OFFSET_OF_TEST(x) << std::endl

#define _STR(m_x) #m_x
#define STR(m_x) _STR(m_x)
    size_t out1 = GET_OFFSET_OF_TEST(1);

    //测试宏展开内容
    std::cout << "宏展开：" << STR(GET_OFFSET_OF_TEST(1)) << std::endl;
    //输出
    std::cout << "\n输出：" << std::endl;
    OUTPUT(1);
    OUTPUT(2);
    OUTPUT(3);
    OUTPUT(4);
    OUTPUT(5);
#undef OUTPUT
#undef GET_OFFSET_OF_TEST
    return 0;
}
