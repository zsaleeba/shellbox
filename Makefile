BINDIR  ?= bin
OBJDIR  ?= ../obj
GOBJDIR = $(OBJDIR)/gosh
TARGET	= gosh

all:
	(cd shell && $(MAKE) )
#	(cd builtins && $(MAKE) )
	$(MAKE) $(BINDIR)/$(TARGET)

clean:
	(cd shell && $(MAKE) $@ )
#	(cd builtins && $(MAKE) $@ )
	rm -f $(BINDIR)/$(TARGET)
	
$(BINDIR)/$(TARGET): $(BINDIR)
	$(CC) -o $(BINDIR)/$(TARGET) $(GOBJDIR)/shell/gosh_shell.a $(GOBJDIR)/builtins/gosh_builtins.a

$(BINDIR):
	mkdir -p $(BINDIR)
