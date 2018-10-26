/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 11:49:15 by hublanc           #+#    #+#             */
/*   Updated: 2018/10/26 19:05:52 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/ft_malloc.h"

#include <stdio.h>

size_t length_base(uint64_t nb, int base)
{
    int length;

    length = 1;
    while (nb /= base)
        length++;
    return (length);
}

void print_addr(void *addr)
{
    const char      base_str[17] = "0123456789ABCDEF\0";
    char            buffer[100];
    uint64_t        nb;
    size_t			index;
    size_t          length;

    //index = 1;
    nb = (uint64_t)addr;
    length = length_base(nb, 16);
    index = length_base(nb, 16) - 1;
    ft_putstr("0x");
	if (nb == 0)
		buffer[0] = '0';
    while (nb > 0)
    {
        //buffer[length - index] = base_str[nb % 16];
        buffer[index] = base_str[nb % 16];
        nb /= 16;
        index--;
    }
    buffer[length] = '\0';
    ft_putstr(buffer);
}

void print_block(t_block_metadata *block)
{
    print_addr(block + 1);
    ft_putstr(" - ");
    print_addr((char*)(block + 1) + block->size);
    ft_putstr(" : ");
	ft_putnbr(block->size);
    //ft_putnbr(((char*)block + block->size) - (char*)block);
    ft_putstr(" octets");
	if (block->is_free)
		ft_putstr(" [free]");
	ft_putendl("");
    //ft_putstr(" octets\n");
}

void print_area(t_area_type area_type)
{
    t_block_metadata    *block;

    block = (t_block_metadata*)(area_type.area + 1);
    if (TINY == area_type.memory_type)
        ft_putstr("TINY : ");
    if (SMALL == area_type.memory_type)
        ft_putstr("SMALL : ");
    if (LARGE == area_type.memory_type)
        ft_putstr("LARGE : ");
    print_addr(area_type.area);
    ft_putchar('\n');
    while (block)
    {
		//if (!block->is_free)
		print_block(block);
        block = block->next;
    }
}

t_area_type get_smallest_addr(t_area **tiny, t_area **small, t_area **large)
{
    t_area_type smallest;
    t_area      **tmp;

    tmp = tiny;
    smallest.memory_type = TINY;
    if ((*small) && ((*small < *tmp) || (!*tmp)))
    {
        tmp = small;
        smallest.memory_type = SMALL;
    }
    if ((*large) && ((*large < *tmp) || (!*tmp)))
    {
        tmp = large;
        smallest.memory_type = LARGE;
    }
    smallest.area = *tmp;
    if (*tmp)
    {
        *tmp = (*tmp)->next;
    }
    return(smallest);
}

void print_total()
{
    size_t total;

    total = g_allocator[TINY].size_allocated;
    total += g_allocator[SMALL].size_allocated;
    total += g_allocator[LARGE].size_allocated;
    ft_putstr("Total : ");
    ft_putnbr(total);
    ft_putstr(" octets\n");
}

void show_alloc_mem()
{
    t_area      *tiny_area;
    t_area      *small_area;
    t_area      *large_area;
    t_area_type current;

	//pthread_mutex_lock(&g_mutex);
    tiny_area = g_allocator[TINY].area;
    small_area = g_allocator[SMALL].area;
    large_area = g_allocator[LARGE].area;
    current.area = NULL;
    while (tiny_area || small_area || large_area)
    {
        current = get_smallest_addr(&tiny_area, &small_area, &large_area);
        print_area(current);
    }
    print_total();
	//pthread_mutex_unlock(&g_mutex);
}
