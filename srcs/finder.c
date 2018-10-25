/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finder.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 18:48:49 by hublanc           #+#    #+#             */
/*   Updated: 2018/10/25 18:59:06 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

static void seek_in_area(void *ptr, int index, t_area_to_free *area_to_free)
{
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
        begin = ((t_block_metadata*)(area));
        end = ((t_block_metadata*)((char*)area + area->size));
        if ((ptr >= (void*)begin) && (ptr <= (void*)end))
            break;
        prev = area;
        area = area->next;
    }
    if (area)
    {
        area_to_free->prev = prev;
        area_to_free->area = area;
        area_to_free->memory_type = index;
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

static int check_size_free_block(size_t size, e_memory_type memory_type,
								t_block_metadata *block)
{
	int is_valid;
	size_t min_type;

	is_valid = 0;
	min_type = 0;
	if (TINY == memory_type)
		min_type = TINY_MIN_ALLOC_SIZE;
	else if (SMALL == memory_type)
		min_type = TINY_MAX_ALLOC_SIZE + 1;
	if ((block->size >= size + sizeof(t_block_metadata) + min_type)
		|| block->size == size)
		is_valid = 1;
	return (is_valid);
}

t_block_metadata *find_free_block(size_t size, e_memory_type memory_type, t_area *area)
{
	t_block_metadata	*current;
	int					is_found;

	current = NULL;
	is_found = 0;
 	while ((NULL != area) && (!is_found))
	{
		current = (t_block_metadata*)(area + 1);
		while (current && !is_found)
		{
			if (current->is_free
				&& check_size_free_block(size, memory_type, current))
				is_found = 1;
			else
				current = current->next;
		}
		area = area->next;
	}
	return (current);
}
