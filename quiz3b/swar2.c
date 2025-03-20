#include <stdio.h>
#include <stdint.h>
#include <string.h>

/**
 * memchr - Find a character in an area of memory.
 * @s: The memory area
 * @c: The byte to search for
 * @n: The size of the area.
 *
 * returns the address of the first occurrence of @c, or %NULL
 * if @c is not found
 */
void *memchr(const void *s, int c, size_t n)
{
    const unsigned char *p = s;
    while (n-- != 0) {
        if ((unsigned char)c == *p++) {
            return (void *)(p - 1);
        }
    }
    return NULL;
}

int main()
{   
    const char str[] = "https://wiki.csie.ncku.edu.tw";
    const char ch = '.';
    
    char *ret = memchr(str, ch, strlen(str));
    printf("String after |%c| is - |%s|\n", ch, ret);
    return 0;
}
