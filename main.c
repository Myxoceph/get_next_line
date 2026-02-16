#include <stdio.h>
#include "get_next_line.h"
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd = open("test.txt", O_RDONLY);
	char *line;

	if (fd == -1)
	{
		perror("Error opening file");
		return 1;
	}

	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return 0;
}
