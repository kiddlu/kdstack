//wirte in python
#include <string.h>
#include <errno.h>  /* for strerror */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char ** argv)
{

	int base;
	char *endptr, *str;
	long val;

	if(argc < 2) {
		int i = 0;
		for(i = 0; i < 256; i++) {
			printf("%3d: %s\n", i, strerror(i));
                }
		return 0;
	} else {
		errno = 0;
		str = argv[1];
		//base = (argc > 2) ? atoi(argv[2]) : 10;
		base = 10;
		val = strtol(str, &endptr, base);

		if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
			|| (errno != 0 && val == 0)) {
			perror("strtol");
			exit(EXIT_FAILURE);
		}

		if (endptr == str) {
			fprintf(stderr, "No digits\n");
			exit(EXIT_FAILURE);
		}
		printf("%d: %s\n", (int)val, strerror((int)val));
	}
}
