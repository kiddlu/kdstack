SRC = malloc-max.c malloc.c
OBJ = $(SRC:.c=.obj)
CFLAGS = /c
LDFLAGS = /SUBSYSTEM:CONSOLE /LARGEADDRESSAWARE

all:
	cl $(CFLAGS) $(SRC)
	link /OUT:malloc-max.exe $(LDFLAGS) $(OBJ)

clean:
	rm $(OBJ)
