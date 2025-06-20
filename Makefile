mode ?= debug

cc = gcc
ifeq ($(mode), debug)
	cflags = \
		-g \
		-fsanitize=address
else
	cflags = -flto -O2
endif

src = $(shell ls src/*.c)
obj = $(src:.c=.o)

tests=$(shell ls tests/*.c)
tests_bin=$(tests:.c=.bin)

all: libalgds.a
	mkdir -p build/lib
	mkdir -p build/include/algds
	cp src/*.h build/include/algds
	cp libalgds.a build/lib/

libalgds.a: $(obj)
	ar cr $@ $^

test: $(tests_bin)
	@echo
	@echo "Run tests:"
	@scripts/runall.sh $^

$(obj):%.o:%.c
	$(cc) -c $(cflags) $< -MD -MF $@.d -o $@

$(tests_bin):%.bin:%.c libalgds.a
	$(cc) $(cflags) -Isrc/ $< libalgds.a -MD -MF $@.d -o $@

clean:
	-rm -rf build/
	-rm $(shell find tests/ -name '*.bin')
	-rm $(shell find . -name '*.o' -or -name '*.a' -or -name '*.d')

DEPS := $(shell find . -name *.d)
ifneq ($(DEPS),)
include $(DEPS)
endif
