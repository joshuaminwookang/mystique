GCC=gcc

CFLAGS=-Wall
LDFLAGS=-static -nostdlib -nostartfiles -lgcc
#WIDTH=6


# Change this to add tests
PROGRAMS = fixedWeightCombinations generalCombinations timeTests memoryTest hwTimeTest powerTest

default: $(addsuffix .riscv,$(PROGRAMS))

dumps: $(addsuffix .dump,$(PROGRAMS))

%.o: %.S
	$(GCC) $(CFLAGS) -D__ASSEMBLY__=1 -c $< -o $@

%.o: %.c mmio.h
	$(GCC) $(CFLAGS) -DWIDTH=$(WIDTH) -DFUNCT=$(FUNCT) -DWARE=$(WARE) -c $< -o $@

%.S: %.c mmio.h
	$(GCC) $(CFLAGS) -S -c $< -o $@

%.riscv: %.o 
	$(GCC) -Wall $< -o $@

clean:
	rm -f *.riscv *.o *.dump *~
