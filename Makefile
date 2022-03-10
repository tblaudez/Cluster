#
# Created by tblaudez on 3/7/22.
#

TARGET := Cluster

CC := gcc
CFLAGS := -I include/ -I MLX42/include
LDFLAGS := -LMLX42/ -lmlx42 -ldl -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -lbsd -lm

# from : https://stackoverflow.com/questions/714100/os-detecting-makefile/14777895#14777895
ifeq '$(findstring ;,$(PATH))' ';'
    detected_OS := Windows
else
    detected_OS := $(shell uname 2>/dev/null || echo Unknown)
    detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
    detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
    detected_OS := $(patsubst MINGW%,MSYS,$(detected_OS))
endif

# Mac OS X
ifeq ($(detected_OS),Darwin)
    LDFLAGS := -LMLX42/ -lmlx42 -ldl -lglfw -lm
endif

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