*This project has been created as part of the 42 curriculum by abakirca.*

# get_next_line

## Description

`get_next_line` is a C function that returns the next line read from a file descriptor each time it is called:

```c
char	*get_next_line(int fd);
```

Calling it in a loop reads a file, a pipe or the standard input one line at a time. The returned line includes the terminating `\n`, except for the last line when the input does not end with one. When there is nothing left to read, or when an error occurs, the function returns `NULL`.

The goal of the project is to learn how **static variables** keep state between function calls, and how to read a stream in chunks of a size fixed at compile time (`BUFFER_SIZE`) without reading more than needed.

The **bonus** part handles several file descriptors at the same time, using a single static variable, without mixing up the reading state of each one.

## Instructions

Compile together with your own `main.c`:

```bash
# mandatory part
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c

# bonus part
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

- `-D BUFFER_SIZE=n` sets how many bytes each `read()` call asks for. It is optional: without it, the default value is `64`.
- Include `get_next_line.h` (or `get_next_line_bonus.h` for the bonus) in your program.
- Every returned line is allocated with `malloc` and must be released with `free` by the caller.

Minimal usage:

```c
int		fd;
char	*line;

fd = open("file.txt", O_RDONLY);
line = get_next_line(fd);
while (line)
{
	printf("%s", line);
	free(line);
	line = get_next_line(fd);
}
close(fd);
```

## Algorithm

### Chosen approach: a growable buffer with indexes

The whole reading state is one structure:

```c
typedef struct s_reader
{
	char	*data;
	size_t	cap;
	size_t	start;
	size_t	end;
	size_t	scanned;
}	t_reader;
```

- `data` is a heap buffer of `cap` bytes.
- `data[start .. end)` are the bytes already read from the file descriptor but not returned yet.
- `data[start .. scanned)` is the part of them that was already searched for `\n`.

Each call to `get_next_line` does the following:

1. **Search** for `\n` from `scanned` to `end`. The search resumes where the previous one stopped, so every byte is inspected only once.
2. If no newline is found, **read** up to `BUFFER_SIZE` bytes directly into `data + end`. Before reading, make sure there is enough room:
   - first slide the pending bytes `data[start .. end)` to the front of the buffer (compaction), reusing the space of lines that were already returned;
   - if that is still not enough, allocate a new buffer of at least **twice** the capacity and copy the pending bytes into it.
3. When a newline is found, **cut** `data[start .. newline]` into a new string and move `start` after it. If the buffer becomes empty, all indexes are reset to `0` so the next read starts at the front for free.
4. When `read()` returns `0` (end of file), the remaining pending bytes form the last line. When `read()` fails or `malloc` fails, the pending data is discarded. In both cases the buffer is freed and the reader goes back to its empty state.

### Why this approach

The common solution keeps a "stash" string and joins every newly read chunk to it with a `strjoin`-like function. Each join copies the whole stash again, so a line of length *n* read in chunks of size *b* costs about *n² / (2b)* byte copies. In addition, the read buffer is usually a local array `char buf[BUFFER_SIZE + 1]`, which overflows the stack with large values such as `BUFFER_SIZE=10000000`.

The growable buffer avoids both problems:

- **Linear time**: data is read in place and the capacity doubles when it grows, so the total amount of copying stays proportional to the input size. On a 1 MB file containing a single line with `BUFFER_SIZE=42`, my previous stash-based version took about 40 s, this one about 0.01 s.
- **No large stack allocation**: the read buffer lives on the heap, so any positive `BUFFER_SIZE` works.
- **Reads as little as possible**: `read()` is only called when no complete line is pending, and a line is returned as soon as its `\n` is found. The file is never read in full beforehand.
- **Simple ownership**: there is exactly one allocation for the state (`data`) plus one per returned line, and a single function (`reader_clear`) to release it.

### Bonus: multiple file descriptors

The bonus keeps a single static variable, `static t_fd_node *head`, the head of a linked list with one node per file descriptor:

```c
typedef struct s_fd_node
{
	int					fd;
	t_reader			reader;
	struct s_fd_node	*next;
}	t_fd_node;
```

- On each call, the node of `fd` is looked up; if it does not exist yet, a new node with an empty reader is added to the front of the list.
- The same algorithm as above runs on that node's reader, so each file descriptor keeps its own pending bytes.
- As soon as a file descriptor reaches end of file or an error, its node is unlinked (using a pointer to the `next` link, so removing the head needs no special case) and freed.

Compared with a static array indexed by fd, the list has no `OPEN_MAX` limit and finished file descriptors do not keep any memory.

## Resources

- `man 2 read`, `man 3 malloc`, `man 3 free`
- [ISO C standard — static storage duration and zero initialization (C11 §6.7.9)](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)
- [Dynamic array — amortized analysis of capacity doubling](https://en.wikipedia.org/wiki/Dynamic_array)
- [42 Norm (norminette)](https://github.com/42School/norminette)

### Use of AI

An AI assistant was used during this project for the following tasks:

- **Design discussion**: reviewing my previous stash-based implementation, pointing out its weaknesses (quadratic copying, stack buffer, repeated newline scans) and comparing alternative algorithms.
- **Documentation**: drafting this README.
