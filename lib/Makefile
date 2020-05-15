GCC=gcc
CFLAGS= -Wall
LDFLAGS=-static -nostdlib -nostartfiles -lgcc
#WIDTH=6

# Change this to add tests
PROGRAMS = combinations bloom strcpy

default: dispatch $(PROGRAMS)

dispatch: dispatch.c
	$(GCC) $(CFLAGS) -c $^

combinations.o: combinations.S
	$(GCC) $(CFLAGS) -D__ASSEMBLY__=1 -c $< -o $@

combinations.S: combinations.c mmio.h
	$(GCC) $(CFLAGS) -S -c $< -o $@

combinations: combinations.o dispatch.o
	$(GCC) -Wall  $^ -o $@.riscv

####
bloom.o: bloom.S
	$(GCC) $(CFLAGS) -D__ASSEMBLY__=1 -c $< -o $@

bloom.S: bloom.c mmio.h
	$(GCC) $(CFLAGS) -S -c $< -o $@

bloom: bloom.o dispatch.o
	$(GCC) -Wall  $^ -o $@.riscv

####
strcpy.o: strcpy.S
	$(GCC) $(CFLAGS) -D__ASSEMBLY__=1 -c $< -o $@

strcpy.S: strcpy.c mmio.h
	$(GCC) $(CFLAGS) -S -c $< -o $@

strcpy: strcpy.o dispatch.o
	$(GCC) -Wall  $^ -o $@.riscv

clean:
	rm  *.riscv *.o *~
