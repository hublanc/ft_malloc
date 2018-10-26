/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 16:50:16 by hublanc           #+#    #+#             */
/*   Updated: 2018/10/26 16:27:33 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

void free_large(t_area_to_free area_to_free) {
	g_allocator[area_to_free.memory_type].size_allocated -= area_to_free.area->size;
    if (!area_to_free.prev)
        g_allocator[area_to_free.memory_type].area = area_to_free.area->next;
    else
        area_to_free.prev->next = area_to_free.area->next;
    munmap(area_to_free.area, area_to_free.area->size);
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
    size_t pad;

    pad = 0;
    if (block_to_free.block)
    {
		g_allocator[area_to_free.memory_type].size_allocated -= block_to_free.block->size;
        if (area_to_free.memory_type == TINY)
        {
            pad = round_up(sizeof(t_block_metadata) + block_to_free.block->size, TINY_ALLOC_RESOLUTION);
        }
        else if (area_to_free.memory_type == SMALL)
        {
            pad = round_up(sizeof(t_block_metadata) + block_to_free.block->size, SMALL_ALLOC_RESOLUTION);
        }
        block_to_free.block->is_free = 1;
        block_to_free.block->size = (pad - sizeof(t_block_metadata));
        defrag(block_to_free);
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
            && (LARGE == area_to_free.memory_type))
            free_large(area_to_free);
        else if (area_to_free.area
			&& (area_to_free.memory_type < LARGE
			&& area_to_free.memory_type >= TINY))
        {
            block_to_free = find_block_to_free(ptr, area_to_free.area);
            free_small_tiny(block_to_free, area_to_free);
        }
    }
}

void free(void *ptr)
{
	pthread_mutex_lock(&g_mutex);
    ft_free(ptr);
	pthread_mutex_unlock(&g_mutex);
}
