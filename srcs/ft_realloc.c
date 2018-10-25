/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 17:45:13 by hublanc           #+#    #+#             */
/*   Updated: 2018/10/25 20:40:55 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

int		enough_room(void *ptr, size_t size, t_area_to_free *area_s,
				t_block_to_free *block_s)
{
	*area_s = find_area_where_block_to_free(ptr);
	*block_s = find_block_to_free(ptr, area_s->area);
	if (area_s->area && (LARGE == area_s->memory_type))
		return (NOT_ENOUGH_ROOM);
	else if (area_s->area)
	{
		if (block_s->block)
		{
			if (block_s->block->size >= size)
				return (ENOUGH_ROOM);
			else if ((size <= TINY_MAX_ALLOC_SIZE
					&& area_s->memory_type == TINY)
					|| (size <= SMALL_MAX_ALLOC_SIZE
					&& area_s->memory_type == SMALL))
			{
				//defrag(*block_s);
				return (block_s->block->size
						>= size ? ENOUGH_ROOM : NOT_ENOUGH_ROOM);
			}
			else
				return (NOT_ENOUGH_ROOM);
		}
	}
	return (ERROR_ROOM);
}

void	*do_realloc(void *ptr, size_t size)
{
	void			*new;
	int				ret;
	t_area_to_free	area_s;
	t_block_to_free	block_s;

	new = NULL;
	ret = enough_room(ptr, size, &area_s, &block_s);
	if (ENOUGH_ROOM == ret && block_s.block)
	{
		new = block_s.block;
		//block_s.block->size = size;
	}
	else if (NOT_ENOUGH_ROOM == ret)
	{
		new = ft_malloc(size);
		ft_memcpy(new, ptr, block_s.block->size);
		ft_free(ptr);
	}
	return (new);
}

void	*ft_realloc(void *ptr, size_t size)
{
	void			*new;

	new = NULL;
	if (NULL == ptr)
		new = ft_malloc(size);
	else if ((0 == size) && (NULL != ptr))
	{
		new = ft_malloc(TINY_ALLOC_RESOLUTION);
		ft_free(ptr);
	}
	else
		new = do_realloc(ptr, size);
	return (new);
}

void	*realloc(void *ptr, size_t size)
{
	void *new;

	//pthread_mutex_lock(&g_mutex);
	new = ft_realloc(ptr, size);
	//pthread_mutex_unlock(&g_mutex);
	return (new);
}
