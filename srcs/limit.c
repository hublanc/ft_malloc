/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   limit.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 19:02:05 by hublanc           #+#    #+#             */
/*   Updated: 2018/10/25 19:03:25 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

static size_t compromise_size(size_t mmap_size, size_t total_size,
							struct rlimit rlp)
{
	while (total_size > rlp.rlim_cur)
	{
		total_size -= mmap_size / 2;
		mmap_size /= 2;
	}
	return (mmap_size);
}

size_t get_size_allocated()
{
	size_t size;

	size = g_allocator[TINY].size_allocated;
	size += g_allocator[SMALL].size_allocated;
	size += g_allocator[LARGE].size_allocated;
	return (size);
}

int 	check_limit(size_t *mmap_size, e_memory_type memory_type)
{
	struct 	rlimit rlp;
	int		is_valid;
	size_t  total_size;

	is_valid = 0;
	total_size = *mmap_size + get_size_allocated();
	if (!getrlimit(RLIMIT_MEMLOCK, &rlp))
	{
		if (total_size > rlp.rlim_cur)
		{
			if ((TINY == memory_type) || (SMALL == memory_type))
			{
				*mmap_size = compromise_size(*mmap_size, total_size, rlp);
				if (mmap_size)
					is_valid = 1;
			}
		}
		else
			is_valid = 1;
	}
	return (is_valid);
}
