GCC=gcc
CFLAGS= -Wall
LDFLAGS=-static -nostdlib -nostartfiles -lgcc
#WIDTH=6

# Change this to add tests
PROGRAMS = combinations

default: dispatch $(PROGRAMS)

dispatch: dispatch.c
	$(GCC) $(CFLAGS) -c $^

combinations.o: combinations.S
	$(GCC) $(CFLAGS) -D__ASSEMBLY__=1 -c $< -o $@

combinations.o: combinations.c mmio.h
	$(GCC) $(CFLAGS) -c $< -o $@

combinations.S: combinations.c mmio.h
	$(GCC) $(CFLAGS) -S -c $< -o $@

combinations: combinations.o dispatch.o
	$(GCC) -Wall  $^ -o $@.riscv

clean:
	rm  *.riscv *.o *~
