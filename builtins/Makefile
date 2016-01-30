SRCS	= printf.c test.c times.c
TARGET	= gosh_builtins.a
CFLAGS	= -Wall -I.. -I../shell -include ../config.h

OBJDIR  ?= ../obj
GOBJDIR	= $(OBJDIR)/gosh/builtins
OBJS	= $(SRCS:%.c=$(GOBJDIR)/%.o)

all:	$(GOBJDIR)/$(TARGET)

$(GOBJDIR)/$(TARGET): $(GOBJDIR) $(OBJS)
	$(AR) r $(GOBJDIR)/$(TARGET) $(OBJS)

$(GOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(GOBJDIR):
	mkdir -p $(GOBJDIR)

clean:
	rm -f $(GOBJDIR)/$(TARGET) $(OBJS) *~

depend:
	gcc -MM $(SRCS) | sed 's|[^.]\{1,\}\.o:|$(GOBJDIR)/&|' >$(GOBJDIR)/deps.mk

-include $(GOBJDIR)/deps.mk
