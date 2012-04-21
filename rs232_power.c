/* Public Domain -- Russ Dill, April 2012 */
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>

void print_usage(char *argv0)
{
	fprintf(stderr, "Usage: %s <port> [cycle|on|off]\n", basename(argv0));
	exit(1);
}

int main(int argc, char *argv[])
{
	int fd;
	int bits = TIOCM_DTR;
	struct termios termios;

	if (argc < 2)
		print_usage(argv[0]);

	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror(argv[1]);
		return 1;
	}

	ioctl(fd, TCGETS, &termios);
	termios.c_cflag &= ~HUPCL;
	ioctl(fd, TCSETS, &termios);

	if (argc == 2) {
		ioctl(fd, TIOCMGET, &bits);
		puts(bits & TIOCM_DTR ? "ON" : "OFF");
	} else if (!strcasecmp(argv[2], "cycle")) {
		ioctl(fd, TIOCMBIC, &bits);
		sleep(1);
		ioctl(fd, TIOCMBIS, &bits);
	} else if (!strcasecmp(argv[2], "on"))
		ioctl(fd, TIOCMBIS, &bits);
	else if (!strcasecmp(argv[2], "off"))
		ioctl(fd, TIOCMBIC, &bits);
	else
		print_usage(argv[0]);

	return 0;
 }

