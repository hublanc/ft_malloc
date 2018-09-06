/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 16:36:35 by hublanc           #+#    #+#             */
/*   Updated: 2018/09/06 18:28:14 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

void	init_block_list_value(size_t size, t_block_metadata *blocks)
{
	blocks->size	= size;
	blocks->magic	= 0;
	blocks->is_free	= 1;
	blocks->next	= NULL;
	return ;
}

void	init_area_value(e_memory_type memory_type, size_t size, t_area * area)
{
	if (TINY == memory_type)
	{
		area->size = getpagesize() * TINY_REGION_SIZE;
	}
	else if (SMALL == memory_type)
	{
		area->size = getpagesize() * SMALL_REGION_SIZE;
	}
	else if (LARGE == memory_type)
	{
		area->size = size;
	}
	init_block_list_value(area->size, area->block_list);
	area->next = NULL;
	return ;
}

void	create_new_area(e_memory_type memory_type, size_t size, t_area *area)
{
	if (TINY == memory_type)
	{
		area = (t_area*)mmap(NULL, getpagesize() * TINY_REGION_SIZE,
							PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		init_area_value(memory_type, size, area);
	}
	else if (SMALL == memory_type)
	{
		area = (t_area*)mmap(NULL, getpagesize() * SMALL_REGION_SIZE,
							PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		init_area_value(memory_type, size, area);
	}
	else if (LARGE == memory_type)
	{
		area = (t_area*)mmap(NULL, size,
							PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		init_area_value(memory_type, size, area);
	}
	return ;
}

t_block_metadata *find_free_block(size_t size, t_area *area)
{
	t_block_metadata	*current;

	current = area->block_list;
	while (current && !(current->is_free && (current->size >= size)))
	{
		current = current->next;
	}
	return (current);
}

t_area	*find_free_area(t_area *current)
{
	while (NULL != current)
	{
		current = current->next;
	}
	return (current);
}

void	*carve_memory_bock(e_memory_type memory_type, size_t size, t_area *area)
{
	t_block_metadata	*block;
	size_t				tmp;

	tmp = block->size;
	block = find_free_block(size, area);
	block->size = size;
	block->is_free = 0;
	if (TINY == memory_type)
	{
		block->next = (t_block_metadata*)((char*)block +
				round_up(sizeof(t_block_metadata) + size, TINY_ALLOC_RESOLUTION)); 
	}
	if (SMALL == memory_type)
	{
		block->next = (t_block_metadata*)((char*)block +
				round_up(sizeof(t_block_metadata) + size, SMALL_ALLOC_RESOLUTION)); 
	}
	block->next->size		= tmp - size;
	block->next->is_free	= 1;
	block->next->next		= NULL;
	return (block + 1);
}

void	*allocate_memory_large(size_t size)
{
	t_area		*area_allocated;

	area_allocated = find_free_area(g_allocator[LARGE].area);
	create_new_area(LARGE, size, area_allocated);
	return (area_allocated + 1);
}

void	*allocate_memory(e_memory_type memory_type, size_t size)
{
	t_allocator allocator;
	void		*memory_allocated;

	allocator = g_allocator[memory_type];
	if (LARGE == memory_type)
	{
		memory_allocated = allocate_memory_large(size);
	}
	else if (TINY == memory_type || SMALL == memory_type)
	{
		if (NULL == allocator.area)
			create_new_area(memory_type, size, allocator.area);
		memory_allocated = carve_memory_block(memory_type, size, allocator.area);
	}
	return (memory_allocated);
}

void	*malloc(size_t size){
	void	*memory_allocated;

	if (size <= 0)
	{
		return (NULL);
	}
	if (size <= TINY_MAX_ALLOC_SIZE)
	{
		memory_allocated = allocate_memory(TINY, size);
	}
	else if (size <= SMALL_MAX_ALLOC_SIZE)
	{
		memory_allocated = allocate_memory(SMALL, size);
	}
	else
	{
		memory_allocated = allocate_memory(LARGE, size);
	}
	return (memory_allocated);
}
