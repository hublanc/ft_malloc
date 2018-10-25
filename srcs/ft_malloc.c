/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 16:36:35 by hublanc           #+#    #+#             */
/*   Updated: 2018/10/25 19:20:42 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

t_allocator g_allocator[3] = {{0, NULL}, {0, NULL}, {0, NULL}};
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

void	*allocate_memory(e_memory_type memory_type, size_t size)
{
	t_block_metadata	*block;

 	block = NULL;
	if (LARGE == memory_type)
	{
		block = first_block_new_area(memory_type, size,
									&(g_allocator[memory_type].area));
		if (block)
			block->is_free = 0;
	}
	else if (TINY == memory_type || SMALL == memory_type)
	{
 		block = carve_memory_block(memory_type, size,
								&(g_allocator[memory_type].area));
	}
	if (block)
		g_allocator[memory_type].size_allocated += size;
	return (block ? block + 1 : NULL);
}

void	*ft_malloc(size_t size){
	void	*memory_allocated;

	if (size == 0)
	{
		size = TINY_MIN_ALLOC_SIZE;
		memory_allocated = allocate_memory(TINY, size);
	}
	else if (size <= TINY_MAX_ALLOC_SIZE)
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

void *malloc(size_t size) {
	void *new;

	//pthread_mutex_lock(&g_mutex);
	new = ft_malloc(size);
	//pthread_mutex_unlock(&g_mutex);
	return (new);
}
