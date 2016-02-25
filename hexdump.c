/*
 * Dump memory contents in DEBUG style hex notation
 *
 * compile: $ gcc -o hexdump hexdump.c
 */

#include <stdio.h>

static char *hexmap = "0123456789abcdefgh";

void
hexdump(char * buf, int count, unsigned long addr)
{
	int	i, column;
	char	hexbuf[49], asciibuf[17];
	char	*hptr, *aptr;

	hptr = hexbuf;
	aptr = asciibuf;
	i = column = 0;
	while (i < count) {
		*hptr++ = hexmap[(buf[i] >> 4) & 0xf];
		*hptr++ = hexmap[buf[i] & 0xf];
		*hptr++ = column == 7 ? '-' : ' ';
		*aptr++ = (buf[i] >= ' ' && buf[i] <= '~') ? buf[i] : '.';
		if (++column == 16) {
			*hptr = *aptr = 0;
			printf("%5lx: %-48s  %s\n", addr, hexbuf, asciibuf);
			addr += 16;
			column = 0;
			hptr = hexbuf;
			aptr = asciibuf;
		}
		i++;
	}
	if (column) {
		*hptr = *aptr = 0;
		printf("%5lx: %-48s  %s\n", addr, hexbuf, asciibuf);
	}
}


#include <unistd.h>

void
dumpfile(FILE * fp)
{
	unsigned long	addr = 0;
	char		buf[16];
	int		n;

	while ((n = fread(buf, 1, sizeof(buf), fp)) > 0) {
		hexdump(buf, n, addr);
		addr += n;
	}
}


#include <fcntl.h>

int
main(int ac, char * av[])
{
	FILE *	fp;
	int	i;

	if (ac > 1) {
		for (i = 1; i < ac; i++) {
			fp = fopen(av[i], "r");
			dumpfile(fp);
			fclose(fp);
		}
	} else {
		dumpfile(stdin);
	}

	return 0;
}

/* vi: tabstop=8 noexpandtab softtabstop=8 shiftwidth=8:
 */
