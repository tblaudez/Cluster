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

MLX := MLX42/libmlx42.a

all: $(TARGET)

$(TARGET): $(MLX) $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(MLX):
	$(MAKE) -C MLX42/

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	$(MAKE) -C MLX42/ clean
	@rm -vf $(OBJECTS)

fclean: clean
	$(MAKE) -C MLX42/ fclean
	@rm -vf $(TARGET)

re: fclean all

.PHONY: all clean fclean re
.SILENT: