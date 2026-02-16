#include <stdio.h>
#include "get_next_line.h"
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd = open("test.txt", O_RDONLY);
	int fd2 = open("test2.txt", O_RDONLY);
	char *line;

	if (fd == -1)
	{
		perror("Error opening file");
		return 1;
	}
	if (fd2 == -1)
	{
		perror("Error opening file");
		return 1;
	}

	line = get_next_line(fd);
	printf("First line from test.txt: %s", line);
	free(line);

	line = get_next_line(fd2);
	printf("First line from test2.txt: %s", line);
	free(line);

	printf("Rest of test.txt:\n");
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}

	printf("Rest of test2.txt:\n");
	while ((line = get_next_line(fd2)) != NULL)
	{
		printf("%s", line);
		free(line);
	}

	close(fd2);
	close(fd);
	return 0;
}
