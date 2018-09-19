/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 16:50:16 by hublanc           #+#    #+#             */
/*   Updated: 2018/09/17 17:02:55 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

t_block_metadata *find_area_where_block_to_free(void *ptr)
{
    t_area  *area_where_block_to_free;
    t_block *begin;
    t_block *end;
    int     index;

    area_where_block_to_free = NULL;
    begin = NULL;
    end = NULL;
    index = TINY;
    while (index <= LARGE)
    {
        area_where_block_to_free = g_allocator[index].area;
        while (area_where_block_to_free)
        {
            begin = ((t_block_metadata*)(area_where_block_to_free + 1) + 1);
            end = ((t_block_metadata*)(area_where_block_to_free + 1)
                area_where_block_to_free->size));
            if (((LARGE == index) && (ptr == (void*)begin)) || ((index < LARGE)
                && ((ptr >= (void*)begin) && (ptr <= (void*)end))))
                break ;
            area_where_block_to_free = area_where_block_to_free->next;
        }
        ++index;
    }
    return (area_where_block_to_free);
}

t_block_metadata *find_block_to_free(void *ptr)
{
    t_block_metadata    *block_found;
    t_area              *area_where_block_to_free;

    area_where_block_to_free = find_area_where_block_to_free(ptr);
    return (block_found;)
}

void ft_free(void *ptr) {
    return ;
    t_block_metadata     *block_to_free;
    if (NULL != ptr)
    {
        block_to_free = find_block_to_free(ptr);
    }
}

void free(void *ptr){
    ft_free(ptr);
}
