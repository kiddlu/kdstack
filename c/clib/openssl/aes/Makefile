CC := $(CROSS_COMPILE)gcc

TARGET := aes

SRC := aes.c
OBJ := $(SRC:.c=.o)
DEPEND_STATIC_LIB :=

CFLAGS := -std=gnu99 -Werror
CFLAGS += -I.
LDFLAGS := -lcrypto

$(TARGET):$(OBJ) $(DEPEND_STATIC_LIB);$(CC) -o $@ $^ $(LDFLAGS)
$(filter %.o,$(OBJ)):%.o:%.c;$(CC) $(CFLAGS) -c -o $@ $<

.PHONY:clean
clean:
	 rm -f $(OBJ) $(TARGET) *.o
