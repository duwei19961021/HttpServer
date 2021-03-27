#include "src/core/mem_pool.h"
#include <time.h>
using  namespace  std;
int main() {
    clock_t start, finish;
    double  duration;

    memory::MemPool p(4095);
    char* str1 = (char*)p.allocate_small(4000);
    strcpy(str1,"黄冠希");
    char* str2 = (char*)p.allocate_small(4000);
    strcpy(str2,"傻逼");
    char* str3 = (char*)p.allocate_small(4000);
    strcpy(str3,"傻逼");
    char* str4 = (char*)p.allocate_small(4000);
    strcpy(str4,"傻逼");
    std::cout << str1 << str2 << str3 << str4<< endl;





//    int count = 10;

//    start = clock();
//    while (count > 0)
//    {
//        char* str = (char *)p.allocate(sizeof(char) * 3000);
//        std::cout << str << std::endl;
//        p.deallocate(str,40);
//        count--;
//    }

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    std::cout << duration << std::endl;
    return 0;
}
