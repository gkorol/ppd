CC = mpicc
CFLAGS =
LDFLAGS =

src = $(wildcard *.c)
obj = $(src:.c=.o)

all: clean ring

ring: $(obj)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(obj) ring

run:
	mpirun -np 8 ./ring
