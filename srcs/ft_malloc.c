/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 16:36:35 by hublanc           #+#    #+#             */
/*   Updated: 2018/09/14 12:16:00 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"
#include <stdio.h>

t_allocator g_allocator[3] = {{0, NULL}, {0, NULL}, {0, NULL}};

void	init_block_metadata_value(t_area *area)
{
	t_block_metadata	*block_metadata;

	if (area != NULL)
	{
		block_metadata = (t_block_metadata*)(area + 1);
		if (NULL != block_metadata)
		{
			block_metadata->size = area->size;
			block_metadata->magic = 0;
			block_metadata->is_free = 1;
			block_metadata->next = NULL;
		}
	}
}

void	init_area_value(size_t size, t_area *area)
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
		mmap_size = size;
	new_area = (t_area*)mmap(NULL, mmap_size,
						PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	init_area_value(mmap_size, new_area);
	init_block_metadata_value(new_area);
	printf("NEW AREA ADDRESS: %p\n",(void*)&new_area);
	return (new_area);
}

t_block_metadata *find_free_block(size_t size, t_area *area)
{
	t_block_metadata	*current;

	current = NULL;
	if (NULL != area)
	{
		current = (t_block_metadata*)(area + 1);
		while (current && !(current->is_free && (current->size >= size)))
		{
			current = current->next;
		}
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

t_block_metadata	*first_block_new_area(e_memory_type memory_type, size_t size, t_area *area)
{
	t_area				*area_allocated;
	t_block_metadata	*block_metadata;

	area_allocated = find_free_area(area);
	area_allocated = create_new_area(memory_type, size);
	block_metadata = (t_block_metadata*)(area_allocated + 1);
	return (block_metadata);
}

t_block_metadata	*carve_memory_block(e_memory_type memory_type, size_t size, t_area *area)
{
	t_block_metadata	*block;
	size_t				tmp;

	block = find_free_block(size, area);
	if (NULL == block)
	{
		block = first_block_new_area(memory_type, size, area);
	}
	tmp = block->size;
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
	block->next->size = tmp - size;
	block->next->is_free = 1;
	block->next->next = NULL;
	return (block);
}

void	*allocate_memory(e_memory_type memory_type, size_t size)
{
	t_allocator			allocator;
	t_block_metadata	*block;

 	block = NULL;
	allocator = g_allocator[memory_type];
	if (LARGE == memory_type)
	{
		block = first_block_new_area(memory_type, size, allocator.area);
	}
	else if (TINY == memory_type || SMALL == memory_type)
	{
 		block = carve_memory_block(memory_type, size, allocator.area);
	}
	return (block ? block + 1 : NULL);
}

void	*ft_malloc(size_t size){
	void	*memory_allocated;

	if (size <= 0)
	{
		memory_allocated = NULL;
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

#include <stdio.h>

int main(int argc, const char *argv[])
{
	(void)argc;
	(void)argv;
	char *str = (char*)ft_malloc(sizeof(char) * 8);
	char *str1 = (char*)ft_malloc(sizeof(char) * 120000);
	char *str2 = (char*)ft_malloc(sizeof(char) * 210000);
	if (!str)
	{
		puts("error ft malloc returned NULL");
	}
	else
	{
		puts("HURRAY ft malloc returned something");
		strcpy(str, "salut\0");
		printf("Str after strcpy: %s \n", str);
		printf("%p\n",(void*)&str);
	}
	if (!str1)
	{
		puts("error ft malloc returned NULL");
	}
	else
	{
		puts("HURRAY ft malloc returned something");
		strcpy(str1, "salut\0");
		printf("Str after strcpy: %s \n", str1);
		printf("%p\n",(void*)&str1);
	}
	if (!str2)
	{
		puts("error ft malloc returned NULL");
	}
	else
	{
		puts("HURRAY ft malloc returned something");
		strcpy(str2, "salut\0");
		printf("Str after strcpy: %s \n", str2);
		printf("%p\n",(void*)&str2);
	}
	return 0;
}
