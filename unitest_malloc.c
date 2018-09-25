#include <criterion/criterion.h>
#include "./includes/ft_malloc.h"
#include <stdio.h>

Test(createArea, simple_TINY, .disabled = true)
{
    t_area              *area;
    t_block_metadata    *block;
    size_t              index;
    size_t              limit;

    index = 0;
    limit = TINY_REGION_SIZE * getpagesize();
    while (index < limit)
    {
        area = create_new_area(TINY, index);
        block = (t_block_metadata*)(area + 1);
        //printf("New TINY area created with a size of %zu\n", index);
        cr_assert(area);
        cr_assert(area->size == limit);
        cr_assert(area->next == NULL);
        cr_assert(block);
        cr_assert(block->size = area->size);
        cr_assert(block->is_free == 1);
        cr_assert(block->next == NULL);
        index += 100;
    }
}

Test(createArea, simple_SMALL, .disabled = true)
{
    t_area              *area;
    t_block_metadata    *block;
    size_t              index;
    size_t              limit;

    index = 993;
    limit = SMALL_REGION_SIZE * getpagesize();
    while (index < limit)
    {
        area = create_new_area(SMALL, index);
        block = (t_block_metadata*)(area + 1);
        //printf("New SMALL area created with a size of %zu\n", index);
        cr_assert(area);
        cr_assert(area->size == limit);
        cr_assert(area->next == NULL);
        cr_assert(block);
        cr_assert(block->size = area->size);
        cr_assert(block->is_free == 1);
        cr_assert(block->next == NULL);
        index += 500;
    }
}

Test(createArea, simple_LARGE, .disabled = true)
{
    t_area              *area;
    t_block_metadata    *block;
    size_t              index;
    size_t              limit;

    index = 129025;
    limit = (SMALL_REGION_SIZE + TINY_REGION_SIZE) * getpagesize();
    while (index < limit)
    {
        area = create_new_area(LARGE, index);
        block = (t_block_metadata*)(area + 1);
        //printf("New LARGE area created with a size of %zu\n", index);
        cr_assert(area);
        cr_assert(area->size == (index + sizeof(t_block_metadata) + sizeof(t_area)));
        cr_assert(area->next == NULL);
        cr_assert(block);
        cr_assert(block->size == (area->size - sizeof(t_area) - sizeof(t_block_metadata)));
        cr_assert(block->is_free == 1);
        cr_assert(block->next == NULL);
        index += 10000;
    }
}

Test(CarveMemoryBlock, simple, .disabled = true)
{

    t_area              *area_TINY;
    t_area              *area_SMALL;
    t_block_metadata    *block;
    size_t              index;
    size_t              limit;

    area_TINY  = NULL;
    area_SMALL = NULL;
    index = 100;
    limit = TINY_REGION_SIZE * getpagesize();
    //printf("DEBUT LOOP\n");
    while (index < TINY_MAX_ALLOC_SIZE)
    {
        //printf("IN LOOP WITH INDEX %zu\n", index);
        size_t arrondi = round_up(sizeof(t_block_metadata) + index, TINY_ALLOC_RESOLUTION);
        //printf("ARRONDI: %zu\n", arrondi);
        block = carve_memory_block(TINY, index, &area_TINY);
        //printf("BLOCK STAT\n\tSIZE: %zu\n\tFREE: %d\n\tNEXT: %p\n\nBLOCK NEXT STAT\n\tSIZE: %zu\n\tFREE: %d\n\tNEXT: %p\n", block->size, block->is_free, block->next, block->next->size, block->next->is_free, block->next->next);
        //cr_assert(block);
        //cr_assert(block->size == index);
        //cr_assert(block->is_free == 0);
        //cr_assert(block->next == (t_block_metadata*)((char*)block + round_up(sizeof(t_block_metadata) + index, TINY_ALLOC_RESOLUTION)));
        //cr_assert(block->next->is_free == 1);
        //cr_assert(block->next->next == NULL);
        index += 1;
    }
    int i = 1;
    while (area_TINY)
    {
        //printf("NB AREA %d\n", i);
        area_TINY = area_TINY->next;
    }
    //printf("FIN LOOP\n");
    /*
    block = carve_memory_block(TINY, 300, area_TINY);
    cr_assert(block);
    cr_assert(block->size == 300);
    cr_assert(block->is_free == 0);
    cr_assert(block->next == (t_block_metadata*)((char*)block + round_up(sizeof(t_block_metadata) + 300, TINY_ALLOC_RESOLUTION)));
    cr_assert(block->next->is_free == 1);
    cr_assert(block->next->next == NULL);
    block = carve_memory_block(TINY, 400, area_TINY);
    cr_assert(block);
    cr_assert(block->size == 400);
    cr_assert(block->is_free == 0);
    cr_assert(block->next == (t_block_metadata*)((char*)block + round_up(sizeof(t_block_metadata) + 400, TINY_ALLOC_RESOLUTION)));
    cr_assert(block->next->is_free == 1);
    cr_assert(block->next->next == NULL);
    //printf("BLOCK STAT\n\tSIZE: %zu\n\tFREE: %d\n\tNEXT: %p\n\nBLOCK NEXT STAT\n\tSIZE: %zu\n\tFREE: %d\n\tNEXT: %p\n", block->size, block->is_free, block->next, block->next->size, block->next->is_free, block->next->next);
    block = carve_memory_block(SMALL, 10000, area_SMALL);
    cr_assert(block);
    cr_assert(block->size == 10000);
    cr_assert(block->is_free == 0);
    cr_assert(block->next == (t_block_metadata*)((char*)block + round_up(sizeof(t_block_metadata) + 10000, SMALL_ALLOC_RESOLUTION)));
    cr_assert(block->next->is_free == 1);
    cr_assert(block->next->next == NULL);
    */
}

Test(FtMalloc, someTiny, .disabled = true)
{
    char *str = (char*)ft_malloc(8);
    char *str1 = (char*)ft_malloc(8);
    char *str2 = (char*)ft_malloc(8);
    cr_assert(str);
    cr_assert(str1);
    cr_assert(str2);
}

Test(FtFree, someTiny, .disabled = true)
{
    char *str = (char*)ft_malloc(8);
    char *str1 = (char*)ft_malloc(15);
    char *str2 = (char*)ft_malloc(20);
    cr_assert(str);
    cr_assert(str1);
    cr_assert(str2);
    ft_free(str);
    ft_free(str1);
    ft_free(str2);
    cr_assert(str);
    cr_assert(str1);
    cr_assert(str2);
}

Test(FtMalloc, rLimit, .disabled = true)
{
    struct rlimit rlp;

    rlp.rlim_cur = 40000;
    rlp.rlim_max = 50000;
    if (!setrlimit(RLIMIT_MEMLOCK, &rlp))
    {
        char *str = (char*)ft_malloc(8);
        show_alloc_mem();
    }
    else
        printf("Setrlimit returned an error\n");
}

Test(FtFree, badFree, .disabled = false)
{
    ft_free(NULL);
    char *str = (char*)ft_malloc(15);
    cr_assert(str);
    ft_free(str);
    int i = 0;
    while (i < 1000000)
    {
        str = (char*)ft_malloc(i);
        //ft_free(str);
        if (i >= 45327)
            show_alloc_mem();
        i++;
    }
    show_alloc_mem();
}

Test(ShowAllocMem, printTest, .disabled = true)
{
    ft_malloc(8);
    ft_malloc(100);
    ft_malloc(1000);
    ft_malloc(2000);
    ft_malloc(200000);
    ft_malloc(400000);
    show_alloc_mem();
}
