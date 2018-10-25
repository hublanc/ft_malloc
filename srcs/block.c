/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 19:07:32 by hublanc           #+#    #+#             */
/*   Updated: 2018/10/25 19:17:29 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

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

static void set_block_value(e_memory_type memory_type, size_t size, t_block_metadata *block)
{
	size_t	tmp;
	size_t	pad;
	t_block_metadata *next;

	tmp = block->size;
	block->size = size;
	block->is_free = 0;
	next = NULL;
	pad = 0;
	if (TINY == memory_type)
		pad = round_up(sizeof(t_block_metadata) + size, TINY_ALLOC_RESOLUTION);
	else if (SMALL == memory_type)
		pad = round_up(sizeof(t_block_metadata) + size, SMALL_ALLOC_RESOLUTION);
	if (tmp > pad + sizeof(t_block_metadata))
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

t_block_metadata	*first_block_new_area(e_memory_type memory_type, size_t size, t_area **area)
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
