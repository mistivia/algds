mode ?= debug

cc = gcc
ifeq ($(mode), debug)
	cflags = -g
else
	cflags = -O2
endif

src = $(shell ls *.c)
obj = $(src:.c=.o)

tests=$(shell ls tests/*.c)
tests_bin=$(tests:.c=.bin)

all: libalgds.a

libalgds.a: $(obj)
	ar cr $@ $^

install: libalgds.a
	sudo mkdir -p /usr/local/include/algds
	sudo cp *.h /usr/local/include/algds
	sudo cp libalgds.a /usr/local/lib

test: $(tests_bin)
	@echo
	@echo "Run tests:"
	@scripts/runall.sh $^

$(obj):%.o:%.c
	$(cc) -c $(cflags) $< -MD -MF $@.d -o $@

$(tests_bin):%.bin:%.c libalgds.a
	$(cc) $(cflags) -I./ $< libalgds.a -MD -MF $@.d -o $@

clean:
	-rm -rf build/
	-rm $(shell find tests/ -name '*.bin')
	-rm $(shell find . -name '*.o' -or -name '*.a' -or -name '*.d')

DEPS := $(shell find . -name '*.d')
ifneq ($(DEPS),)
include $(DEPS)
endif
