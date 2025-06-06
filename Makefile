cc = gcc
cflags = \
	-g \
	-fsanitize=address

src = $(shell ls src/*.c)
obj = $(src:.c=.o)

tests=$(shell ls tests/*.c)
tests_bin=$(tests:.c=.bin)

all: libalgds.a
	-rm -rf build/
	-@mkdir -p build/include/algds/
	-@mkdir -p build/lib
	mv libalgds.a build/lib/
	cp src/*.h build/include/algds

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
	-rm $(shell find tests/ -name '*.bin')
	-rm $(shell find . -name '*.o' -or -name '*.a' -or -name '*.d')
	-rm -rf build

DEPS := $(shell find . -name *.d)
ifneq ($(DEPS),)
include $(DEPS)
endif
