/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 16:50:16 by hublanc           #+#    #+#             */
/*   Updated: 2018/09/17 17:02:55 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

t_area *find_area_where_block_to_free(void *ptr)
{
    t_area              *area_where_block_to_free;
    t_block_metadata    *begin;
    t_block_metadata    *end;
    int     index;

    area_where_block_to_free = NULL;
    begin = NULL;
    end = NULL;
    index = TINY;
    while (index <= LARGE)
    {
        area_where_block_to_free = g_allocator[index].area;
        while (area_where_block_to_free)
        {
            begin = ((t_block_metadata*)(area_where_block_to_free + 1) + 1);
            end = ((t_block_metadata*)(area_where_block_to_free + 1) + area_where_block_to_free->size);
            if (((LARGE == index) && (ptr == (void*)begin)) || ((index < LARGE)
                && ((ptr >= (void*)begin) && (ptr <= (void*)end))))
                return (area_where_block_to_free);
            area_where_block_to_free = area_where_block_to_free->next;
        }
        ++index;
    }
    return (area_where_block_to_free);
}

t_block_metadata *find_block_to_free(void *ptr, t_area *area)
{
    t_block_metadata    *block;

    block = NULL;
    if (area)
    {
        block = (t_block_metadata*)(area + 1);
        while (block && (ptr != (void*)(block + 1)))
            block = block->next;
    }
    return (block);
}

#include <stdio.h>

void free_large(t_area *area) {
    t_area  **list;
    t_area  *prev;

    list = &(g_allocator[LARGE].area);
    prev = NULL;
    if (*list == area)
    {
        *list = (*list)->next;
        munmap(area, area->size);
    }
    else
    {
        while (list && (list != area))
        {
            prev = *list;
            *list = (*list)->next;
        }
        if (*list)
        {
            prev->next = (*list)->next;
            munmap(area, area->size);
        }
    }
}

t_block_metadata    *get_next(t_block_metadata *block)
{
    t_area              **area;
    t_block_metadata    *list;
    t_block_metadata    *prev;

    area = &(g_allocator[memory_type].area);
    list = (t_block_metadata*)((*area) + 1);
    prev = NULL;
    while (list && (list->next != block))
    {
        prev = list;
        list = list->next;
    }

    return (prev);
}

t_block_metadata    *get_prev(t_block_metadata *block)
{

}

void defrag(t_block_metadata *block, e_memory_type memory_type) {
    t_block_metadata    *next;
    t_area              **area;
    t_block_metadata    *prev;

    area = &(g_allocator[memory_type].area);
    list = (t_block_metadata*)((*area) + 1);
    next = get_next(block);
    prev = get_prev(block);
    if (block)
    {
        while (list && (list != block)) {
            prev = list;
            list = list->next;
        }
        if (list)
        {
            prev->size += list->size;
        }
    }
}

void free_small_tiny(t_block_metadata *block, t_area *area) {
    e_memory_type memory_type;
    if (block)
    {
        printf("BLOCK TO FREE ISNT NULL\n");
        if (block->size <= TINY_MAX_ALLOC_SIZE)
        {
            memory_type = TINY;
            pad = round_up(sizeof(t_block_metadata) + block->size, TINY_ALLOC_RESOLUTION);
        }
        else if (block->size <= SMALL_MAX_ALLOC_SIZE)
        {
            memory_type = SMALL;
            pad = round_up(sizeof(t_block_metadata) + block->size, SMALL_ALLOC_RESOLUTION);
        }
        block->is_free = 1;
        block->size += (pad - block->size - sizeof(block_metadata));
        defrag(block, memory_type);
        int r = round_up(sizeof(t_block_metadata) + block->size, TINY_ALLOC_RESOLUTION);
        int r1 = round_up(block->size, TINY_ALLOC_RESOLUTION);
        printf("SIZE: %zu ; ARRONDI %d ; ARRONDI W/SIZE: %d\n", block->size, r, r1);
        printf("FREE: %d ; SIZE: %zu ; NEXT: %p\n", block->is_free, block->size, block->next);
    }
    else
        printf("BLOCK IS NULL\n");
}

void ft_free(void *ptr)
{
    t_area              *area;
    t_block_metadata    *block;

    block = NULL;
    area = NULL;
    if (NULL != ptr)
    {
        area = find_area_where_block_to_free(ptr);
        if (area && (area->size > getpagesize() * SMALL_REGION_SIZE))
            free_large(area);
        else
        {
            block = find_block_to_free(ptr, area);
            free_small_tiny(block, area);
        }
    }
}

void f
