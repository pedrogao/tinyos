#include "os.h"

/*
 * Following global vars are defined in mem.S
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

/*
 * _alloc_start points to the actual start address of heap pool
 * _alloc_end points to the actual end address of heap pool
 * _num_pages holds the actual max number of pages we can allocate.
 */
static uint32_t _alloc_start = 0;
static uint32_t _alloc_end = 0;
static uint32_t _num_pages = 0;

#define PAGE_SIZE 4096 // size of page
#define PAGE_ORDER 12  // 2^12 = 4096

#define PAGE_TAKEN (uint8_t)(1 << 0) // 页是否被使用
#define PAGE_LAST (uint8_t)(1 << 1)  // 是否为最后一页

/*
 * Page Descriptor 
 * flags:
 * - bit 0: flag if this page is taken(allocated)
 * - bit 1: flag if this page is the last page of the memory block allocated
 */
struct Page
{
    uint8_t flags; // page flags
};

/* clear page flags */
static inline void _clear(struct Page *page)
{
    page->flags = 0;
}

/* check page is free */
static inline int _is_free(struct Page *page)
{
    if (page->flags & PAGE_TAKEN) // check the taken bit
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/* set page flags, attention: attch flags not reset */
static inline void _set_flag(struct Page *page, uint8_t flags)
{
    page->flags |= flags;
}

/* check page is the last page */
static inline int _is_last(struct Page *page)
{
    if (page->flags & PAGE_LAST)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 * align the address to the border of page(4K)
 */
static inline uint32_t _align_page(uint32_t address)
{
    uint32_t order = (1 << PAGE_ORDER) - 1; // 4095
    return (address + order) & (-order);    // 4k 对齐
}

void page_init()
{
    /* 
	 * We reserved 8 Page (8 x 4096) to hold the Page structures.
	 * It should be enough to manage at most 128 MB (8 x 4096 x 4096) 
     * 预留 8 页数据存储页元数据
	 */
    _num_pages = (HEAP_SIZE / PAGE_SIZE) - 8;
    printf("HEAP_START = %x, HEAP_SIZE = %x, num of pages = %d\n",
           HEAP_START, HEAP_SIZE, _num_pages);
    struct Page *page = (struct Page *)HEAP_START;
    for (int i = 0; i < _num_pages; i++)
    {
        _clear(page);
        page++;
    }
    // 从 HEAP_START + 8 * PAGE_SIZE 开始，前面的保存元数据
    _alloc_start = _align_page(HEAP_START + 8 * PAGE_SIZE);
    _alloc_end = _alloc_start + (PAGE_SIZE * _num_pages);

    printf("TEXT:   0x%x -> 0x%x\n", TEXT_START, TEXT_END);
    printf("RODATA: 0x%x -> 0x%x\n", RODATA_START, RODATA_END);
    printf("DATA:   0x%x -> 0x%x\n", DATA_START, DATA_END);
    printf("BSS:    0x%x -> 0x%x\n", BSS_START, BSS_END);
    printf("HEAP:   0x%x -> 0x%x\n", _alloc_start, _alloc_end);
}

/*
 * Allocate a memory block which is composed of contiguous physical pages
 * - npages: the number of PAGE_SIZE pages to allocate
 */
void *page_alloc(int npages)
{
    // search the page descriptor bitmaps.
    int found = 0;
    struct Page *page_i = (struct Page *)(HEAP_START);
    for (int i = 0; i < (_num_pages - npages); i++)
    {
        if (_is_free(page_i)) // page is free
        {
            found = 1;
            // check following (npages - 1) pages are also unallocated.
            struct Page *page_j = page_i;
            for (int j = i; j < (i + npages); j++)
            {
                if (!_is_free(page_j))
                {
                    found = 0;
                    break;
                }
                page_j++;
            }

            // if npages is found
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
                // 注意 Page 是元数据，真正分配的还是 _alloc_start 后面的数据
                return (void *)(_alloc_start + i * PAGE_SIZE);
            }
        }
        page_i++;
    }
    return NULL;
}

/*
 * Free the memory block
 * - p: start address of the memory block
 */
void page_free(void *p)
{
    /*
	 * Assert (TBD) if p is invalid
     * p 非法，或者 p 已经超过了 _alloc_end 地址，无效地址
	 */
    if (!p || (uint32_t)p >= _alloc_end)
    {
        return;
    }
    // 获取第一页元数据
    struct Page *page = (struct Page *)HEAP_START;
    page += ((uint32_t)p - _alloc_start) / PAGE_SIZE;
    // 设置页元数据
    while (!_is_free(page))
    {
        if (_is_last(page)) // 最后一页
        {
            _clear(page);
            break;
        }
        else
        {
            _clear(page);
            page++;
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

    void *p3 = page_alloc(4);
    printf("p3 = 0x%x\n", p3);
    page_free(p3);
}