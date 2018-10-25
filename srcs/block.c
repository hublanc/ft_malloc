/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 19:07:32 by hublanc           #+#    #+#             */
/*   Updated: 2018/10/25 22:17:25 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

#include <stdio.h>
void defrag(t_block_to_free block_to_free)
{
	size_t				tmp;
	t_block_metadata	*next;

	tmp = 0;
	next = NULL;
    if (block_to_free.prev && block_to_free.prev->is_free)
    {
		//ft_putendl("AGGLO PREV");
		tmp = block_to_free.prev->size +
			block_to_free.block->size + sizeof(t_block_metadata);
		next = block_to_free.block->next;
        //block_to_free.block->size += block_to_free.prev->size;
        block_to_free.block = block_to_free.prev;
		block_to_free.block->size = tmp;
		block_to_free.block->next = next;
        block_to_free.prev = NULL;
    }
    if (block_to_free.block->next && block_to_free.block->next->is_free)
    {
		//ft_putendl("AGGLO NXT");
        block_to_free.block->size += block_to_free.block->next->size
									+ sizeof(t_block_metadata);
        block_to_free.block->next = block_to_free.block->next->next;
    }
}

static void set_block_value(e_memory_type memory_type, size_t size,
							t_block_metadata *block)
{
	size_t	tmp;
	size_t	pad;
	size_t	min_size;
	t_block_metadata *next;

	tmp = block->size;
	block->size = size;
	block->is_free = 0;
	next = NULL;
	pad = 0;
	min_size = 0;
	if (TINY == memory_type)
	{
		pad = round_up(sizeof(t_block_metadata) + size, TINY_ALLOC_RESOLUTION);
		min_size = TINY_MIN_ALLOC_SIZE;
	}
	else if (SMALL == memory_type)
	{
		pad = round_up(sizeof(t_block_metadata) + size, SMALL_ALLOC_RESOLUTION);
		min_size = TINY_MAX_ALLOC_SIZE + 1;
	}
	if (tmp > pad + sizeof(t_block_metadata) + min_size)
	{
		if (!block->next)
		{
			block->next = (t_block_metadata*)((char*)block + pad);
			block->next->size = tmp - pad;
			block->next->is_free = 1;
			block->next->next = NULL;
		}
		else
		{
			next = block->next;
			block->next = (t_block_metadata*)((char*)block + pad);
			block->next->size = tmp - pad;
			block->next->is_free = 1;
			block->next->next = next;
		}
	}
	else
	{
		block->next = NULL;
	}
}

t_block_metadata	*first_block_new_area(e_memory_type memory_type,
										size_t size, t_area **area)
{
	t_block_metadata	*block_metadata;
	t_area 				*last;
	t_area				*new;

	block_metadata = NULL;
	new = NULL;
	last = NULL;
	if (area)
	{
		last = *area;
		new = create_new_area(memory_type, size);
		if (new)
		{
			if (!*area)
				*area = new;
			else
			{
				while (last->next)
					last = last->next;
				last->next = new;
			}
			block_metadata = (t_block_metadata*)(new + 1);
		}
	}
	return (block_metadata);
}

t_block_metadata	*carve_memory_block(e_memory_type memory_type, size_t size, t_area **area)
{
	t_block_metadata	*block;

	block = find_free_block(size, memory_type, *area);
	if (!block)
		block = first_block_new_area(memory_type, size, area);
	if (block)
	{
		set_block_value(memory_type, size, block);
	}
	return (block);
}
