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
	builtins/printf.c builtins/test.c builtins/times.c \
	toylib/args.c toylib/dirtree.c toylib/getmountlist.c \
	toylib/help.c toylib/interestingtimes.c toylib/lib.c \
	toylib/llist.c toylib/net.c toylib/password.c \
	toylib/portability.c toylib/xwrap.c
#	libedit/chared.c libedit/chartype.c libedit/common.c \
#	libedit/el.c libedit/eln.c libedit/emacs.c libedit/fcns.c \
#	libedit/fgetln.c libedit/filecomplete.c libedit/hist.c \
#	libedit/history.c libedit/keymacro.c libedit/map.c \
#	libedit/parse.c libedit/prompt.c libedit/read.c \
#	libedit/refresh.c libedit/search.c \
#	libedit/sig.c libedit/strlcat.c libedit/strlcpy.c \
#	libedit/terminal.c libedit/tokenizer.c libedit/tty.c \
#	libedit/unvis.c libedit/vi.c libedit/vis.c \
#	libedit/wcsdup.c
	
COBJS = $(CSRCS:%.c=${ARCH}/%.o)

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)
CFLAGS += -I. -Ishell -Ilibedit -Icommands -DSHELL -DHAVE_CONFIG_H -include config.h

all:    ${ARCH} ${ARCH}/shell $(PGM)

$(PGM): $(OBJS)
	$(make-exe)

${ARCH}/shell:
	mkdir -p ${ARCH}/shell ${ARCH}/builtins ${ARCH}/libedit ${ARCH}/toylib
