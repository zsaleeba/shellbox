SRCS	= alias.c arith_yacc.c arith_yylex.c cd.c builtins.c error.c \
	  eval.c exec.c expand.c histedit.c init.c input.c jobs.c \
	  mail.c main.c memalloc.c miscbltin.c mystring.c nodes.c \
	  options.c output.c parser.c redir.c show.c signames.c \
	  syntax.c system.c trap.c var.c
TARGET	= gosh_shell.a
CFLAGS	= -Wall -I.. -include ../config.h

OBJDIR  ?= ../obj
GOBJDIR	= $(OBJDIR)/gosh/shell
OBJS	= $(SRCS:%.c=$(GOBJDIR)/%.o)

all:	$(GOBJDIR)/$(TARGET)

$(GOBJDIR)/$(TARGET): $(GOBJDIR) $(OBJS)
	$(AR) r $(GOBJDIR)/$(TARGET) $(OBJS)

$(GOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(GOBJDIR):
	mkdir -p $(GOBJDIR)

regenerate: $(GOBJDIR)/mkinit $(GOBJDIR)/mknodes $(GOBJDIR)/mksignames $(GOBJDIR)/mksyntax
	sh mktokens
	$(CC) $(CFLAGS) -E -x c -o builtins.def builtins.def.in
	sh mkbuiltins builtins.def
	$(GOBJDIR)/mkinit input.c output.c trap.c var.c
	$(GOBJDIR)/mknodes nodetypes nodes.c.pat
	$(GOBJDIR)/mksignames
	$(GOBJDIR)/mksyntax

$(GOBJDIR)/mkinit: mkinit.c
	$(CC) -o $@ $<

$(GOBJDIR)/mknodes: mknodes.c
	$(CC) -o $@ $<

$(GOBJDIR)/mksignames: mksignames.c
	$(CC) -o $@ $<

$(GOBJDIR)/mksyntax: mksyntax.c
	$(CC) -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS) *~ $(GOBJDIR)/mkinit $(GOBJDIR)/mknodes $(GOBJDIR)/mksignames $(GOBJDIR)/mksyntax

depend:
	gcc -MM $(SRCS) | sed 's|[^.]\{1,\}\.o:|$(GOBJDIR)/&|' >$(GOBJDIR)/deps.mk

-include $(GOBJDIR)/deps.mk
