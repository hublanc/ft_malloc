/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 16:36:35 by hublanc           #+#    #+#             */
/*   Updated: 2018/09/05 17:56:33 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	create_new_area(e_memory_type memory_type, s_area area)
{
}

void	allocate_memory(e_memory_type memory_type, size_t size)
{
	s_area area = g_area_list[memory_type];
	if (!area.addr)
	{
		create_new_area(memory_type);
	}
}

void	*malloc(size_t size){
	if (size <= 0)
	{
		return (NULL);
	}
	if (size <= TINY_MAX_ALLOC_SIZE)
	{
		allocate_memory(TINY, size);
	}
	else if (size <= SMALL_MAX_ALLOC_SIZE)
	{
		allocate_memory(SMALL, size);
	}
	else
	{
		allocate_memory(LARGE, size);
	}
	return ();
}
