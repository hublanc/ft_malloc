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

void seek_in_area(void *ptr, int index, t_area_to_free *area_to_free) {
    t_area              *area;
    t_area              *prev;
    t_block_metadata    *begin;
    t_block_metadata    *end;

    area = g_allocator[index].area;
    begin = NULL;
    end = NULL;
    prev = NULL;
    while (area)
    {
        begin = ((t_block_metadata*)(area + 1) + 1);
        end = ((t_block_metadata*)(area + 1) + area->size);
        if (((LARGE == index) && (ptr == (void*)begin)) || ((index < LARGE)
            && ((ptr >= (void*)begin) && (ptr <= (void*)end))))
            break;
        prev = area;
        area = area->next;
    }
    if (area)
    {
        area_to_free->prev = prev;
        area_to_free->area = area;
    }
}

t_area_to_free find_area_where_block_to_free(void *ptr)
{
    t_area_to_free      area_to_free;
    int                 index;

    area_to_free.prev = NULL;
    area_to_free.area = NULL;
    index = TINY;
    while ((index <= LARGE) && !(area_to_free.area))
    {
        seek_in_area(ptr, index, &area_to_free);
        ++index;
    }
    return (area_to_free);
}

t_block_to_free find_block_to_free(void *ptr, t_area *area)
{
    t_block_to_free     block_to_free;
    t_block_metadata    *block;
    t_block_metadata    *prev;

    block_to_free.prev = NULL;
    block_to_free.block = NULL;
    block = NULL;
    prev = NULL;
    if (area)
    {
        block = (t_block_metadata*)(area + 1);
        while (block && (ptr != (void*)(block + 1)))
        {
            prev = block;
            block = block->next;
        }
        block_to_free.prev = prev;
        block_to_free.block = block;
    }
    return (block_to_free);
}

void free_large(t_area_to_free area_to_free) {
    if (!area_to_free.prev)
        area_to_free.area = area_to_free.area->next;
    else
        area_to_free.prev->next = area_to_free.area->area;
    munmap(area_to_free.area, area_to_free.area->size);
}

void defrag(t_block_to_free block_to_free)
{
    if (block_to_free.prev && block_to_free.prev->is_free)
    {
        block_to_free.block->size += block_to_free.prev->size;
        block_to_free.block = block_to_free.prev;
        block_to_free.prev = NULL;
    }
    if (block_to_free.block->next && block_to_free.block->next->is_free)
    {
        block_to_free.block->size += block_to_free.block->next->size;
        block_to_free.block->next = block_to_free.block->next->next;
    }
}

void free_empty_area(t_block_to_free block_to_free, t_area_to_free area_to_free)
{
    if ((!block_to_free.prev && !block_to_free.block->next)
        && (area_to_free.prev))
    {
        area_to_free.prev->next = area_to_free.area->next;
        munmap(area_to_free.area, area_to_free.area->size);
    }
}

void free_small_tiny(t_block_to_free block_to_free, t_area_to_free area_to_free)
{
    if (block_to_free.block)
    {
        if (block_to_free.block->size <= TINY_MAX_ALLOC_SIZE)
            pad = round_up(sizeof(t_block_metadata) + block_to_free.block->size, TINY_ALLOC_RESOLUTION);
        else if (block->size <= SMALL_MAX_ALLOC_SIZE)
            pad = round_up(sizeof(t_block_metadata) + block_to_free.block->size, SMALL_ALLOC_RESOLUTION);
        block_to_free.block->is_free = 1;
        block_to_free.block->size += (pad - block_to_free.block->size - sizeof(block_metadata));
        defrag(block_to_free, memory_type);
        free_empty_area(block_to_free, area_to_free);
    }
}

void ft_free(void *ptr)
{
    t_area_to_free      area_to_free;
    t_block_to_free     block_to_free;

    if (NULL != ptr)
    {
        area_to_free = find_area_where_block_to_free(ptr);
        if (area_to_free.area
            && (area_to_free.area->size > getpagesize() * SMALL_REGION_SIZE))
            free_large(area_to_free);
        else
        {
            block_to_free = find_block_to_free(ptr, area_to_free.area);
            free_small_tiny(block_to_free, area_to_free);
        }
    }
}

void free(void *ptr)
{
    ft_free(ptr);
}
