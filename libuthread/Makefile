# Target library
lib	:= libuthread.a
objs	:= context.o preempt.o queue.o uthread.o

CC	:= gcc
CFLAGS	:= -Wall -Wextra -Werror -MMD
CFLAGS	+= -g

AR	:= ar
AFLAGS	:= rcs

ifneq ($(V),1)
	Q = @
endif

all: $(lib)

# Dep tracking
deps	:= $(patsubst %.o,%.d,$(ojbs))
-include $(deps)

## TODO
libuthread.a: $(objs)
	@echo "AR $@"
	$(Q)$(AR) $(AFLAGS) $@ $^

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo "CLEAN"
	$(Q)rm -f $(lib) $(objs) $(deps) *.d
