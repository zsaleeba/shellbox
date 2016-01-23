SRCS	= alias.c arith_yacc.c arith_yylex.c cd.c builtins.c error.c \
	  eval.c exec.c expand.c histedit.c init.c input.c jobs.c \
	  mail.c main.c memalloc.c miscbltin.c mystring.c nodes.c \
	  options.c output.c parser.c redir.c show.c signames.c \
	  system.c trap.c var.c
TARGET	= gosh
CFLAGS	= -Wall

BINDIR  ?= ../bin
OBJDIR  ?= ../obj
GOBJDIR= $(OBJDIR)/gosh
OBJS	= $(SRCS:%c=$(GOBJDIR)/%o)

all:	$(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(BINDIR) $(GOBJDIR) $(OBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/$(TARGET) $(OBJS)

$(GOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BINDIR):
	mkdir -p $(BINDIR)

$(GOBJDIR):
	mkdir -p $(GOBJDIR)

regenerate: $(GOBJDIR)/mkinit $(GOBJDIR)/mknodes $(GOBJDIR)/mksignames $(GOBJDIR)/mksyntax
	sh mktokens
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
	gcc -MM $(SRCS) | sed 's|[^.]\{1,\}\.o:|$(GOBJDIR)/&|' >.deps.mk

-include dep.mk
