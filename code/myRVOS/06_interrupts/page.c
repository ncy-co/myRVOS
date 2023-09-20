/* 
 * Author: bye
 * Date: 2023-09-13 08:57:56
 * LastEditors: bye
 * LastEditTime: 2023-09-20 16:18:56
 * FilePath: /study/RVOS/code/myRVOS/05_traps/page.c
 * Description: page 管理
 */

#include "os.h"

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

#define PAGE_SIZE 4096

static uint32_t _alloc_start = 0;
static uint32_t _alloc_end = 0;
static uint32_t _num_pages = 0;

typedef struct Page {
      // 只使用后两位bit
      uint8_t flags;
}Page, *PagePtr;

/*
 * 内存中的位置：静态存储区（静态存储区在整个程序运行期间都存在）
 * 初始化：未经初始化的全局静态变量会被程序自动初始化为0（自动对象的值是任意的，除非他被显示初始化）
 * 作用域：全局静态变量在声明他的文件之外是不可见的。准确地讲从定义之处开始到文件结尾。
 * 好处：除了声明它的文件之外，其他文件不可访问，可以在不同文件中重复命名
 */

static inline void _clear(PagePtr page) {
      page->flags = 0;
}

static inline int _is_free(PagePtr page) {
      return !(page->flags & (uint8_t)1);
}

// static inline void _set_flags(PagePtr page, uint8_t flags) {
//       page->flags |= flags;
// }

static inline int _is_last(PagePtr page) {
      if (page->flags & ((uint8_t)1 << 1)) return 0;
      else return 1;
}


/* 
 * description: 进行边界对其
 * param {uint32_t} address
 * return {uint32_t} 返回下一个页的起始地址
 */
static inline uint32_t _align_page(uint32_t address) {
      return ((address + (1 << 12)) & (~((1 << 12) - 1)));
}

void page_init() {
      // 预留前8页来存储页的数据结构,初始化，即初始页管理数据结构
      _num_pages = (HEAP_SIZE / PAGE_SIZE) - 8;
      printf("HEAR_START = %p, HEAP_SIZE = %p, NUM_PAGE = %d\n", HEAP_START, HEAP_START, _num_pages);
      PagePtr page = (PagePtr)HEAP_START;
      for (int i = 0; i < _num_pages; i++ ) {
            _clear(page + i);
      }
      // 前8页用来存储管理数据结构
      _alloc_start = _align_page(HEAP_START + 8 * PAGE_SIZE);
      // _alloc_end 指向 最后一个可分配页的下一个页
      _alloc_end = _alloc_start + (PAGE_SIZE * _num_pages);

      printf("TEXT:     %p - %p\n", TEXT_START, TEXT_END);
      printf("RODATA:   %p - %p\n", RODATA_START, RODATA_END);
      printf("DATA:     %p - %p\n", DATA_START, DATA_END);
      printf("BSS:      %p - %p\n", BSS_START, BSS_END);
      printf("HEAP:     %p - %p\n", _alloc_start, _alloc_end);
}

void *page_alloc(int num_pages) {
      PagePtr start_ptr = (PagePtr)_alloc_start;
      int j;
      for (int i = 0; i < _num_pages; i++ ) {
            if ((start_ptr + i)->flags & 1) continue;
            for (j = 1; j < num_pages; j++ ) {
                  if ((start_ptr + i + j)->flags & 1) break;
            }
            if (j < num_pages) i = i + j + 1;
            else {      
                  for (int k = 0; k < num_pages; k++ ) {
                        (start_ptr + i + k)->flags |= (uint8_t)1;
                        if (k != num_pages - 1) {
                              (start_ptr + i + k)->flags |= (uint8_t)2;
                        } 
                  }

                  return start_ptr + i;
            }
      }
      return NULL;
}

void page_free(void *p) {
      if (!p || (uint32_t)p >= _alloc_end) return;

      PagePtr cur_ptr = (PagePtr)p;
      if (_is_free(cur_ptr)) return;
      while (cur_ptr->flags & ((uint8_t)2)) {
            _clear(cur_ptr);
            cur_ptr++;
      }
      _clear(cur_ptr);
}

void page_test()
{
	void *p = page_alloc(2);
	printf("p = 0x%x\n", p);
	//page_free(p);

	void *p2 = page_alloc(7);
	printf("p2 = 0x%x\n", p2);
	page_free(p2);

	void *p3 = page_alloc(4);
	printf("p3 = 0x%x\n", p3);
}


