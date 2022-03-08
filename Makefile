#
# Created by tblaudez on 3/7/22.
#

TARGET := Cluster

CC := gcc
CFLAGS := -I include/ -I MLX42/include
LDFLAGS := -LMLX42/ -lmlx42 -ldl -lglfw -lGL -lX11 -lpthread -lXrandr -lXi


SOURCES := $(shell find src/ -type f -name '*.c')
OBJECTS := $(SOURCES:.c=.o)
HEADERS :=  $(shell find src/ -type f -name '*.h')

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@rm -vf $(OBJECTS)

fclean: clean
	@rm -vf $(TARGET)

re: fclean all

.PHONY: all clean fclean re