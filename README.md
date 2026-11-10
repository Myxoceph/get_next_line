# 📖 Get Next Line

[![42 School](https://img.shields.io/badge/42-School-000000?style=flat&logo=42&logoColor=white)](https://42.fr)
[![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Grade](https://img.shields.io/badge/Grade-125%2F100-brightgreen)](https://github.com/abakirca)

> *"Reading is to the mind what exercise is to the body."* - Joseph Addison

A C function that returns a file descriptor's content one line at a time, written for the 42 School curriculum. This version is a full rewrite: instead of the classic "join the buffer into a stash" approach, it uses a **growable buffer with read/scan indexes**, so every byte is read, scanned and copied a constant number of times.

## ✨ What Makes This Version Different

- ⚡ **Linear time**: no `strjoin` per `read()`, so very long lines no longer cost O(n²)
- 🧱 **No stack buffer**: data is read straight into a heap buffer, so `BUFFER_SIZE=10000000` works
- 🔍 **Each byte scanned once**: the newline search resumes where it stopped last time
- ♻️ **Buffer reuse**: consumed lines are dropped by moving an index, unread bytes are compacted only when space is needed
- 📂 **Bonus: any number of file descriptors** with a single static variable (a linked list of per-fd readers), freed as soon as an fd hits EOF or an error

## 📋 Function Prototype

```c
char *get_next_line(int fd);
```

- `fd`: file descriptor to read from (regular file, pipe, `0` for stdin, ...)
- **Returns** the next line including its `\n` (the last line may have none), or `NULL` at end of file or on error (invalid fd, `read()` failure, allocation failure)

## 🛠️ Usage Example

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("example.txt", O_RDONLY);
	if (fd == -1)
		return (1);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```

For the bonus, include `get_next_line_bonus.h` instead and call `get_next_line()` with different file descriptors in any order.

## ⚙️ Compilation

```bash
# mandatory
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c

# bonus (multiple file descriptors)
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

`-D BUFFER_SIZE` is optional; the default is `64`. Any positive value works, from `1` to `10000000`.

## 📁 Project Structure

```
get_next_line/
├── get_next_line.h              # t_reader struct and prototypes
├── get_next_line.c              # newline scan, read step, get_next_line
├── get_next_line_utils.c        # buffer growth, compaction, line extraction
├── get_next_line_bonus.h        # adds t_fd_node (per-fd list)
├── get_next_line_bonus.c        # per-fd lookup/removal + get_next_line
└── get_next_line_utils_bonus.c  # same helpers for the bonus build
```

## 🧩 How It Works

The state is a single struct:

```c
typedef struct s_reader
{
	char	*data;     /* heap buffer                           */
	size_t	cap;       /* allocated size                        */
	size_t	start;     /* first byte not returned yet           */
	size_t	end;       /* end of valid data                     */
	size_t	scanned;   /* bytes before this were checked for \n */
}	t_reader;
```

```
data:  [ returned lines ][ pending bytes ............ ][ free space ]
       0                 start          scanned       end           cap
```

1. **Scan** `data[scanned .. end)` for `\n`. If found, the line is `data[start .. \n]`.
2. Otherwise **read** up to `BUFFER_SIZE` bytes directly into `data + end`. Before reading, make room: first slide the pending bytes to the front (compaction), and only if that is not enough, reallocate with at least double the capacity.
3. **Cut** the line into a fresh string and advance `start`. When the buffer becomes empty, all indexes go back to `0`.
4. At **EOF**, the remaining pending bytes are the last line. On EOF or error the buffer is freed.

**Bonus:** a single `static t_fd_node *head` holds one `t_reader` per file descriptor. A node is created the first time an fd is seen and unlinked (pointer-to-pointer, no special case for the head) as soon as that fd reaches EOF or fails, so finished fds hold no memory and there is no `OPEN_MAX` limit.

## 📈 Benchmarks

Measured on an Apple M5 (macOS 26), output redirected to `/dev/null`:

| Input | BUFFER_SIZE=1 | 42 | 1024 | 8192 |
|-------|---------------|----|------|------|
| 10 MB, ~170k short lines | 10.44 s | 0.67 s | 0.44 s | 0.49 s |
| 10 MB, one single line   | 10.06 s | 0.36 s | 0.19 s | 0.16 s |

`BUFFER_SIZE=1` is dominated by the ten million `read()` system calls, not by copying.

Compared with my previous stash/`strjoin` implementation, on a **1 MB file with a single line** (`BUFFER_SIZE=42`):

| Implementation | Time |
|----------------|------|
| Old (`strjoin` each read, O(n²)) | 40.59 s |
| New (growable buffer, O(n))      | 0.01 s |

## 🧪 Testing

Verified with `BUFFER_SIZE` = 1, 2, 3, 7, 42, 9999 and 10000000:

- ✅ Output identical byte-for-byte to the input file (files ending with and without `\n`, empty file, only newlines, single character, 5000 random lines, 200k-character lines)
- ✅ Standard input and a slow pipe delivering partial lines
- ✅ Invalid / closed file descriptors and `BUFFER_SIZE=0` return `NULL`
- ✅ Bonus: 7 files read round-robin, with invalid fds interleaved, each output matches its source
- ✅ 0 leaks (`leaks --atExit`), `norminette` clean, `-Wall -Wextra -Werror`

## 🎖️ 42 Standards

- Norm v4.1 compliant (≤ 25 lines and ≤ 5 functions per file, no `for`/ternary, ...)
- Only `read`, `malloc` and `free` are used; no libft, no `lseek`, no globals
- Bonus uses exactly one static variable

## 📜 License

This project is part of the 42 School curriculum. The code is available for educational purposes.

---

## 📞 Contact & Collaboration

**Ahmet Bakırcan**  
📧 abakirca@student.42kocaeli.com.tr  
🎓 42 Kocaeli Student  

*Always open to discussing system programming, C development, and collaborative opportunities!*

<div align="center">

[![42 Kocaeli](https://img.shields.io/badge/42-Kocaeli-000000?style=flat&logo=42&logoColor=white)](https://42kocaeli.com.tr/)

</div>
