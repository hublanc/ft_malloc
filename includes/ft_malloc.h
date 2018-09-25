/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 13:57:56 by hublanc           #+#    #+#             */
/*   Updated: 2018/09/14 10:42:36 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	FT_MALLOC_H
# define FT_MALLOC_H

# include <unistd.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include "../libft/includes/libft.h"

# define TINY_MAX_ALLOC_SIZE 992
# define TINY_REGION_SIZE 512
# define TINY_ALLOC_RESOLUTION 16
# define SMALL_MAX_ALLOC_SIZE 129024
# define SMALL_REGION_SIZE 4096
# define SMALL_ALLOC_RESOLUTION 512

# define round_up(nb, padding) (((nb) + ((padding) - 1)) & ~((padding) - 1))

typedef enum
{
	TINY,
	SMALL,
	LARGE
}					e_memory_type;

typedef struct		s_allocator
{
	size_t			size_allocated;
	struct s_area	*area;
}					t_allocator;

typedef struct		s_area
{
	size_t			size;
	struct s_area	*next;
}					t_area;

typedef struct				s_block_metadata
{
	size_t					size;
	int						is_free;
	int						magic;
	struct s_block_metadata	*next;
}							t_block_metadata;

typedef struct				s_area_to_free
{
	t_area					*prev;
	t_area					*area;
}							t_area_to_free;

typedef struct				s_block_to_free
{
	t_block_metadata		*prev;
	t_block_metadata		*block;
}							t_block_to_free;

typedef struct				s_area_type
{
	e_memory_type			memory_type;
	t_area					*area;
}							t_area_type;

# define METADATA_SIZE sizeof(struct s_block_metadata_data)


/*
** FUNCTIONS
*/

/*
**	MALLOC
**
**	void* malloc(size_t size);
**
**	Input:
**			- size : The size in byte to allocate
**
**	Return:
**			- return a pointer of the new memory area allocated
*/
void*	malloc(size_t size);

//SUB_FUNC
void	*ft_malloc(size_t size);
void	*allocate_memory(e_memory_type memory_type, size_t size);
t_block_metadata	*carve_memory_block(e_memory_type memory_type, size_t size, t_area **area);
t_block_metadata	*first_block_new_area(e_memory_type memory_type, size_t size, t_area **area);
t_area	*find_free_area(t_area *current);
t_block_metadata *find_free_block(size_t size, t_area *area);
t_area	*create_new_area(e_memory_type memory_type, size_t size);
void	init_area_value(size_t size, t_area *area);
void	init_block_metadata_value(t_area *area);




/*
**	FREE
**
**	void malloc(void *ptr);
**
**	Input:
**			- ptr : pointer to free his allaction
*/
t_area_to_free find_area_where_block_to_free(void *ptr);
t_block_to_free find_block_to_free(void *ptr, t_area *area);
void 	ft_free(void *ptr);
void	free(void *ptr);




/*
**	SHOW ALLOCATION MEMORY
**
**	void show_alloc_mem();
**
**	Input:
**			none
**
**	This function print current memory allocation
**	Ex:
**	TINY : 0xA0000
**	0xA0020 - 0xA004A : 42 octets
**	0xA006A - 0xA00BE : 84 octets
**	SMALL : 0xAD000
**	0xAD020 - 0xADEAD : 3725 octets
**	LARGE : 0xB0000
**	0xB0020 - 0xBBEEF : 48847 octets
**	Total : 52698 octets
*/
void show_alloc_mem();

extern t_allocator g_allocator[3];

#endif
