#include "./includes/ft_malloc.h"
#include <stdio.h>


int main(int argc, const char *argv[])
{
	int i = 0;
	i = 45328;
	char *str;
	while (i < 1000000)
	{
		str = (char*)ft_malloc(i);
//		ft_free(str);
		i++;
	}
	show_alloc_mem();
	return 0;
}
