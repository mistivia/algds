mode ?= debug

# https://best.openssf.org/Compiler-Hardening-Guides/Compiler-Options-Hardening-Guide-for-C-and-C++.html
common_cflags = -Wall -Wno-sign-conversion -Wno-conversion -Wformat -Wformat=2 \
	-Werror=format-security \
	-D_GLIBCXX_ASSERTIONS \
	-fstrict-flex-arrays=3 \
	-fstack-clash-protection -fstack-protector-strong \
	-Wtrampolines \
	-Wbidi-chars=any \
	-Werror=implicit -Werror=incompatible-pointer-types -Werror=int-conversion \
	-fexceptions

cc = gcc
ifeq ($(mode), debug)
	cflags = $(common_cflags) -g -Werror
else
	cflags = $(common_cflags) -O2 \
		-fno-delete-null-pointer-checks -fno-strict-overflow -fno-strict-aliasing -ftrivial-auto-var-init=zero \
		-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3
endif

src = $(shell ls *.c)
obj = $(src:.c=.o)

tests=$(shell ls tests/*.c)
tests_bin=$(tests:.c=.bin)

libalgds.a: $(obj)
	ar cr $@ $^

all: libalgds.a $(tests_bin)

install: libalgds.a
	sudo mkdir -p /usr/local/include/algds
	sudo cp *.h /usr/local/include/algds
	sudo cp libalgds.a /usr/local/lib
	sudo mkdir -p /usr/local/share/algds
	sudo cp docs/usage.md /usr/local/share/algds/

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
