/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   area.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 19:12:04 by hublanc           #+#    #+#             */
/*   Updated: 2018/10/26 11:24:35 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

static void	init_block_metadata_value(t_area *area)
{
	t_block_metadata	*block_metadata;

	if (area != NULL)
	{
		block_metadata = (t_block_metadata*)(area + 1);
		if (NULL != block_metadata)
		{
			block_metadata->size = area->size - sizeof(t_area) - sizeof(t_block_metadata);
			block_metadata->is_free = 1;
			block_metadata->next = NULL;
		}
	}
}

static void	init_area_value(size_t size, t_area *area)
{
	if (NULL != area)
	{
		area->size = size;
		area->next = NULL;
	}
}

t_area	*create_new_area(e_memory_type memory_type, size_t size)
{
	t_area	*new_area;
	size_t	mmap_size;

	mmap_size = -1;
	new_area = NULL;
	if (TINY == memory_type)
		mmap_size = getpagesize() * TINY_REGION_SIZE;
	else if (SMALL == memory_type)
		mmap_size = getpagesize() * SMALL_REGION_SIZE;
	else if (LARGE == memory_type)
		mmap_size = size + sizeof(t_block_metadata) + sizeof(t_area);
	if (check_limit(&mmap_size, memory_type))
		new_area = (t_area*)mmap(NULL, mmap_size,
						PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	init_area_value(mmap_size, new_area);
	init_block_metadata_value(new_area);
	return (new_area);
}
