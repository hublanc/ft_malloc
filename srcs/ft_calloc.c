/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 17:54:45 by hublanc           #+#    #+#             */
/*   Updated: 2018/10/26 21:16:14 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

void	*ft_calloc(size_t count, size_t size)
{
	void *new;

	new = ft_malloc(count * size);
	if (new)
	{
		if (count * size == 0)
		{
			ft_bzero(new, TINY_MIN_ALLOC_SIZE);
		}
		else
		{
			ft_bzero(new, count * size);
		}
	}
	return (new);
}

void	*calloc(size_t count, size_t size)
{
	void *new;

	//pthread_mutex_lock(&g_mutex);
	ft_putendl("== CALLOC CALL ==");
	new = ft_calloc(count, size);
	show_alloc_mem();
	//pthread_mutex_unlock(&g_mutex);
	return (new);
}
