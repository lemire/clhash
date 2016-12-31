# minimalist makefile
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h
ifeq ($(DEBUG),1)
CFLAGS = -fPIC  -std=c99 -ggdb -msse4.2 -mpclmul -march=native -funroll-loops -Wstrict-overflow -Wstrict-aliasing -Wall -Wextra -pedantic -Wshadow -fsanitize=undefined  -fno-omit-frame-pointer -fsanitize=address
else
CFLAGS = -fPIC -std=c99 -O3 -msse4.2 -mpclmul -march=native -funroll-loops -Wstrict-overflow -Wstrict-aliasing -Wall -Wextra -pedantic -Wshadow
endif # debug

HEADERS=include/clhash.h

OBJECTS= clhash.o

all: $(OBJECTS) unit benchmark example

unit : ./tests/unit.c  $(HEADERS) $(OBJECTS)
	$(CC) $(CFLAGS) -o unit ./tests/unit.c -Iinclude  $(OBJECTS)

example : ./example.c $(HEADERS) $(OBJECTS)
	$(CC) $(CFLAGS) -o example example.c -Iinclude  $(OBJECTS)

benchmark :./benchmarks/benchmark.c $(HEADERS) $(OBJECTS)
	$(CC) $(CFLAGS) -o benchmark ./benchmarks/benchmark.c -Iinclude  $(OBJECTS)


clhash.o: ./src/clhash.c $(HEADERS)
	$(CC) $(CFLAGS) -c ./src/clhash.c -Iinclude

clean:
	rm -f $(OBJECTS) unit example benchmark
