#include "./includes/ft_malloc.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	char *str;
	int i = 993;
	printf("%lu\n", sizeof(t_area));
	printf("%lu\n", sizeof(t_block_metadata));
	sleep(1);
	while (i < 1000000)
	{
		str = (char*)ft_malloc(i);
		show_alloc_mem();
		ft_free(str);
		show_alloc_mem();
		sleep(2);
		//strcpy(str, "a");
		//puts(str);
		printf("%d\n", i);
		i += 100;
	}
	show_alloc_mem();
	return 0;
}
