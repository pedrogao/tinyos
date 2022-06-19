#include "os.h"

/*
 * 全局内存元数据变量
 */
extern uint32_t TEXT_START;
extern uint32_t TEXT_END;
extern uint32_t DATA_START;
extern uint32_t DATA_END;
extern uint32_t RODATA_START;
extern uint32_t RODATA_END;
extern uint32_t BSS_START;
extern uint32_t BSS_END;
extern uint32_t HEAP_START;
extern uint32_t HEAP_SIZE;

static uint32_t _alloc_start = 0;
static uint32_t _alloc_end = 0;
static uint32_t _num_pages = 0;

#define PAGE_SIZE 4096
#define PAGE_ORDER 12 // for align

#define PAGE_TAKEN (uint8_t)(1 << 0)
#define PAGE_LAST (uint8_t)(1 << 1)

/*
 * Page Descriptor
 * flags:
 * - bit 0: flag if this page is taken(allocated)
 * - bit 1: flag if this page is the last page of the memory block allocated
 */
struct Page
{
    uint8_t flags;
};

static inline void _clear(struct Page *page)
{
    page->flags = 0;
}

static inline int _is_free(struct Page *page)
{
    if (page->flags & PAGE_TAKEN)
    {
        return 0;
    }
    return 1;
}

static inline int _is_last(struct Page *page)
{
    if (page->flags & PAGE_LAST)
    {
        return 0;
    }
    return 1;
}

static inline void _set_flag(struct Page *page, uint8_t flags)
{
    page->flags |= flags;
}

static inline uint32_t _align_page(uint32_t address)
{
    uint32_t order = (1 << PAGE_ORDER) - 1;
    return (address + order) & (~order);
}

void page_init()
{
    // 8页用来存储 Page 元数据
    _num_pages = (HEAP_SIZE / PAGE_SIZE) - 8;
    printf("HEAP_START = %x, HEAP_SIZE = %x, num of pages = %d\n", HEAP_START, HEAP_SIZE, _num_pages);
    // 初始化元数据
    struct Page *page = (struct Page *)HEAP_START;
    for (int i = 0; i < _num_pages; i++)
    {
        _clear(page);
        page++;
    }
    // 剩下的分配
    _alloc_start = _align_page(HEAP_START + 8 * PAGE_SIZE);
    _alloc_end = _alloc_start + (PAGE_SIZE * _num_pages);

    printf("TEXT:   0x%x -> 0x%x\n", TEXT_START, TEXT_END);
    printf("RODATA: 0x%x -> 0x%x\n", RODATA_START, RODATA_END);
    printf("DATA:   0x%x -> 0x%x\n", DATA_START, DATA_END);
    printf("BSS:    0x%x -> 0x%x\n", BSS_START, BSS_END);
    printf("HEAP:   0x%x -> 0x%x\n", _alloc_start, _alloc_end);
}

//
// 最基础、也是最直观的内存页分配算法
//

void *page_alloc(int npages)
{
    int found = 0;
    struct Page *page_i = (struct Page *)HEAP_START;
    for (int i = 0; i < (_num_pages - npages); i++)
    {
        if (_is_free(page_i))
        {
            found = 1;
            struct Page *page_j = page_i;
            // 同时找到 n 个空闲的页
            for (int j = i; j < (i + npages); j++)
            {
                if (!_is_free(page_j))
                {
                    found = 0;
                    break;
                }
                page_j++;
            }
            if (found)
            {
                struct Page *page_k = page_i;
                for (int k = i; k < (i + npages); k++)
                {
                    _set_flag(page_k, PAGE_TAKEN);
                    page_k++;
                }
                page_k--;
                _set_flag(page_k, PAGE_LAST);
                return (void *)(_alloc_start + i * PAGE_SIZE);
            }
        }
        page_i++;
    }
    return NULL;
}

void page_free(void *p)
{
    if (!p || (uint32_t)p >= _alloc_end)
    {
        return;
    }
    struct Page *page = (struct Page *)HEAP_START;
    page += ((uint32_t)p - _alloc_start) / PAGE_SIZE;
    while (!_is_free(page))
    {
        if (_is_last(page))
        {
            _clear(page);
            break;
        }
        else
        {
            _clear(page);
            page++;
            ;
        }
    }
}

void page_test()
{
    void *p = page_alloc(2);
    printf("p = 0x%x\n", p);
    page_free(p);

    void *p2 = page_alloc(7);
    printf("p2 = 0x%x\n", p2);
    page_free(p2);
}