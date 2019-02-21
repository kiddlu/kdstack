#include <stdio.h>
#include <windows.h>

int main(int argc, void *argv[])
{
	printf("%d\n", GetConsoleOutputCP());
#if 0
	if(argc == 2) {
		SetConsoleOutputCP(atoi(argv[1]));
	}
#endif
}