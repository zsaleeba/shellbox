#
#  RTEMS_MAKEFILE_PATH is typically set in an environment variable
#

PGM=${ARCH}/gosh.exe

# optional managers required
MANAGERS=all

# C source names
CSRCS = shell/alias.c shell/arith_yacc.c shell/arith_yylex.c \
	shell/cd.c shell/builtins.c shell/error.c shell/eval.c \
	shell/exec.c shell/expand.c shell/histedit.c shell/init.c \
	shell/input.c shell/jobs.c shell/mail.c shell/main.c \
	shell/memalloc.c shell/miscbltin.c shell/mystring.c \
	shell/nodes.c shell/options.c shell/output.c \
	shell/parser.c shell/redir.c shell/show.c \
	shell/signames.c shell/syntax.c shell/system.c \
	shell/trap.c shell/var.c \
	builtins/printf.c builtins/test.c builtins/times.c
	
COBJS = $(CSRCS:%.c=${ARCH}/%.o)

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)
CFLAGS += -I. -Ishell -DSHELL -include config.h

all:    ${ARCH} $(PGM)

$(PGM): $(OBJS)
	$(make-exe)
