//
// Created by duwei on 2021/3/26.
//

#include "mem_pool.h"

#define align(d, a)     (((d) + (a - 1)) & ~(a - 1))

#define align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

u_int memory::MemPool::_align = 10;

memory::MemPool::MemPool(size_t size)
{
    pool_t * p = nullptr;
    void * tmp = new char[size];
    p = (pool_t*)tmp;
    _pool = p;
    _pool->s_d.s_last = (u_char*)p + sizeof(pool_t);
    _pool->s_d.s_end = (u_char*)p + size;
    _pool->s_d.s_next = nullptr;
    _pool->s_d.s_failed = 0;
    _pool->s_max = PAGE_SIZE-1;
    _pool->s_current = _pool;
    _pool->s_large = nullptr;
}

memory::MemPool::~MemPool()
{
    delete _pool;
}

void * memory::MemPool::allocate(size_t size)
{
    if(_pool->s_d.s_last + size < _pool->s_d.s_end)
    {
        void* old = _pool->s_d.s_last;
        _pool->s_d.s_last += size;
        return old;
    }
    return nullptr;
}

void memory::MemPool::deallocate(void * ptr, size_t size)
{
    if((u_char*)ptr + size == _pool->s_d.s_last)
    {
        _pool->s_d.s_last -= size;
    }
}

void * memory::MemPool::allocate_small(size_t size)
{
    pool_t *p = _pool;
    do {
        u_char* m = _pool->s_d.s_last;
        if(_align)
        {
            m = align_ptr(m, _align);
            if(p->s_d.s_end - m >= size)
            {
                _pool->s_d.s_last += size;
                return m;
            }
        }
        p = p->s_d.s_next;
    }while (p);

    return palloc_block(size);
}

void* memory::MemPool::palloc_block(size_t size)
{
    size_t new_size = (size_t)(_pool->s_d.s_end - (u_char*)_pool);// 新pool的总大小
    u_char *tmp = (u_char*)new char[new_size]; // 申请跟父结点一样的大小的结点
    pool_t *new_pool = (pool_t *)tmp; //
    new_pool->s_d.s_end = (u_char*)tmp + new_size; // 指定末尾
    new_pool->s_d.s_next = nullptr; // 下一个结点置空
    new_pool->s_d.s_failed = 0; // 失败次数初始值0
    tmp += sizeof(pool_s);  // tmp本来指向新创建的结点的首地址，现在将前sizeof(pool_s)字节空出来给管理结构使用(pool_t)
    tmp = align_ptr(tmp,_align); // 对tmp进行对齐一下，将地址对齐到8的整数倍上
    _pool->s_d.s_last = tmp + size; // tmp往后移动size个，分配申请的内存

    pool_t *pt = nullptr;
    for (pt = _pool->s_current; pt->s_d.s_next;pt = pt->s_d.s_next) {
        ;
    }
    pt->s_d.s_next = new_pool;
    return tmp;
}