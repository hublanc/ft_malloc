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

t_allocator g_allocator[3] = {{0, NULL}, {0, NULL}, {0, NULL}};

void	init_block_metadata_value(t_area *area)
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

void	init_area_value(size_t size, t_area *area)
{
	if (NULL != area)
	{
		area->size = size;
		area->next = NULL;
	}
}

size_t get_size_allocated()
{
	size_t size;

	size = g_allocator[TINY].size_allocated;
	size += g_allocator[SMALL].size_allocated;
	size += g_allocator[LARGE].size_allocated;
	return (size);
}

size_t compromise_size(size_t mmap_size, size_t total_size, struct rlimit rlp)
{
	while (total_size > rlp.rlim_cur)
	{
		total_size -= mmap_size / 2;
		mmap_size /= 2;
	}
	return (mmap_size);
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

int check_size_free_block(size_t size, e_memory_type memory_type,t_block_metadata *block)
{
	int is_valid;
	size_t min_type;

	is_valid = 0;
	if (TINY == memory_type)
		min_type = TINY_MIN_ALLOC_SIZE;
	else if (SMALL == memory_type)
		min_type = TINY_MAX_ALLOC_SIZE + 1;
	if ((block->size >= size + sizeof(t_block_metadata) + min_type)
		|| block->size == size)
		is_valid = 1;
	return (is_valid);
}

t_block_metadata *find_free_block(size_t size, e_memory_type memory_type, t_area *area)
{
	t_block_metadata	*current;
	int					is_found;

	current = NULL;
	is_found = 0;
 	while ((NULL != area) && (!is_found))
	{
		current = (t_block_metadata*)(area + 1);
		while (current && !is_found)
		{
			//if (current->is_free && (current->size >= size))
			if (current->is_free
				&& check_size_free_block(size, memory_type, current))
				is_found = 1;
			else
				current = current->next;
		}
		area = area->next;
	}
	return (current);
}

t_block_metadata	*first_block_new_area(e_memory_type memory_type, size_t size, t_area **area)
{
	t_block_metadata	*block_metadata;
	t_area 				*last;
	t_area				*new;

	block_metadata = NULL;
	new = NULL;
	last = NULL;
	if (area)
	{
		last = *area;
		new = create_new_area(memory_type, size);
		if (new)
		{
			if (!*area)
				*area = new;
			else
			{
				while (last->next)
					last = last->next;
				last->next = new;
			}
			block_metadata = (t_block_metadata*)(new + 1);
		}
	}
	return (block_metadata);
}

void set_block_value(e_memory_type memory_type, size_t size, t_block_metadata *block)
{
	size_t	tmp;
	size_t	pad;
	t_block_metadata *next;

	tmp = block->size;
	block->size = size;
	block->is_free = 0;
	next = NULL;
	printf("TMP SIZE: %zu : BLOCK SIZE: %zu	\n", tmp, block->size);
	if (TINY == memory_type)
		pad = round_up(sizeof(t_block_metadata) + size, TINY_ALLOC_RESOLUTION);
	else if (SMALL == memory_type)
		pad = round_up(sizeof(t_block_metadata) + size, SMALL_ALLOC_RESOLUTION);
	if (tmp > pad + sizeof(t_block_metadata))
	{
		if (!block->next)
		{
			printf("TRY TO MALLOC BEFORE NULL BLOCK\n");
			block->next = (t_block_metadata*)((char*)block + pad);
			block->next->size = tmp - pad;
			block->next->is_free = 1;
			block->next->next = NULL;
		}
		else
		{
			printf("TRY TO MALLOC BETWEEN ALLOCATED BLOCK\n");
			next = block->next;
			block->next = (t_block_metadata*)((char*)block + pad);
			block->next->size = tmp - pad;
			block->next->is_free = 1;
			block->next->next = next;
			if (block->next->next == block->next)
				printf("NEXT DIDN$T WORK\n");
		}
	}
	else
	{
		printf("TRY TO MALLOC AT THE END OF THE AREA\n");
		block->next = NULL;
	}
}

t_block_metadata	*carve_memory_block(e_memory_type memory_type, size_t size, t_area **area)
{
	t_block_metadata	*block;

	block = find_free_block(size, memory_type, *area);
	if (!block)
		block = first_block_new_area(memory_type, size, area);
	if (block)
	{
		set_block_value(memory_type, size, block);
	}
	return (block);
}

void	*allocate_memory(e_memory_type memory_type, size_t size)
{
	t_block_metadata	*block;

 	block = NULL;
	if (LARGE == memory_type)
	{
		block = first_block_new_area(memory_type, size, &(g_allocator[memory_type].area));
		if (block)
			block->is_free = 0;
	}
	else if (TINY == memory_type || SMALL == memory_type)
	{
 		block = carve_memory_block(memory_type, size, &(g_allocator[memory_type].area));
	}
	if (block)
		g_allocator[memory_type].size_allocated += size;
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

void *malloc(size_t size) {
	return (ft_malloc(size));
}
/*

#include <stdio.h>

void basic_test()
{
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
}

int main(int argc, const char *argv[])
{
	(void)argc;
	(void)argv;
	basic_test();

	return 0;
}
*/
