//
// Created by duwei on 2021/3/26.
//
#ifndef HTTPSERVER_MEM_POOL_H
#define HTTPSERVER_MEM_POOL_H

#include "../config/config.h"

typedef struct pool_s pool_t;
typedef struct pool_data_s pool_data_t;
typedef struct ngx_pool_large_s ngx_pool_large_t;

struct pool_data_s
{
    u_char *s_last;
    u_char *s_end;
    pool_t *s_next;
    size_t s_failed;
};

struct pool_s
{
    pool_data_t s_d;
    size_t s_max;
    ngx_pool_large_t *s_large;
    pool_t *s_current;
};

struct pool_large_s
{
    ngx_pool_large_t *s_next;
    void *s_data;
};

namespace memory
{
    class MemPool
    {
    public:
        MemPool(size_t size = 1024);
        ~MemPool();
        void* allocate(size_t size); // 分配内存
        void deallocate(void * ptr, size_t size); // 释放内存
        void* allocate_small(size_t size);
        void* palloc_block(size_t size);

        pool_s *_pool;
        static u_int _align;
    };
}




#endif //HTTPSERVER_MEM_POOL_H
