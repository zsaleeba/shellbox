#
#  RTEMS_MAKEFILE_PATH is typically set in an environment variable
#

PGM=${ARCH}/shellbox.exe

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
	toylib/portability.c toylib/xwrap.c \
	commands/cmdexec.c commands/posix/basename.c \
	commands/posix/cal.c commands/posix/cat.c \
	commands/posix/chgrp.c commands/posix/chmod.c \
	commands/posix/cksum.c commands/posix/cmp.c \
	commands/posix/comm.c commands/posix/cp.c \
	commands/posix/cpio.c commands/posix/cut.c \
	commands/posix/date.c commands/posix/df.c \
	commands/posix/dirname.c commands/posix/du.c \
	commands/posix/echo.c commands/posix/env.c \
	commands/posix/expand.c commands/posix/false.c \
	commands/posix/find.c commands/posix/grep.c \
	commands/posix/head.c commands/posix/id.c \
	commands/posix/kill.c commands/posix/link.c \
	commands/posix/ln.c commands/posix/ls.c \
	commands/posix/mkdir.c commands/posix/mkfifo.c \
	commands/posix/nice.c commands/posix/nl.c \
	commands/posix/nohup.c commands/posix/od.c \
	commands/posix/paste.c commands/posix/patch.c \
	commands/posix/printf.c commands/posix/ps.c \
	commands/posix/pwd.c commands/posix/renice.c \
	commands/posix/rm.c commands/posix/rmdir.c \
	commands/posix/sed.c commands/posix/sleep.c \
	commands/posix/sort.c commands/posix/split.c \
	commands/posix/strings.c commands/posix/tail.c \
	commands/posix/tee.c commands/posix/time.c \
	commands/posix/touch.c commands/posix/true.c \
	commands/posix/tty.c commands/posix/uname.c \
	commands/posix/uniq.c commands/posix/unlink.c \
	commands/posix/uudecode.c commands/posix/uuencode.c \
	commands/posix/wc.c commands/posix/who.c \
	commands/posix/xargs.c \
	commands/other/acpi.c commands/other/base64.c \
	commands/other/blkid.c commands/other/bzcat.c \
	commands/other/chroot.c commands/other/chvt.c \
	commands/other/clear.c commands/other/count.c \
	commands/other/dos2unix.c \
	commands/other/factor.c commands/other/fallocate.c \
	commands/other/flock.c \
	commands/other/fsync.c commands/other/help.c \
	commands/other/hexedit.c commands/other/hostid.c \
	commands/other/ifconfig.c \
	commands/other/inotifyd.c commands/other/insmod.c \
	commands/other/ionice.c commands/other/login.c \
	commands/other/losetup.c commands/other/lsattr.c \
	commands/other/lsmod.c commands/other/lspci.c \
	commands/other/lsusb.c commands/other/makedevs.c \
	commands/other/mix.c commands/other/mkpasswd.c \
	commands/other/mkswap.c commands/other/modinfo.c \
	commands/other/mountpoint.c commands/other/nbd_client.c \
	commands/other/netcat.c commands/other/nsenter.c \
	commands/other/oneit.c commands/other/partprobe.c \
	commands/other/pivot_root.c commands/other/pmap.c \
	commands/other/printenv.c commands/other/pwdx.c \
	commands/other/readahead.c commands/other/readlink.c \
	commands/other/realpath.c commands/other/reboot.c \
	commands/other/reset.c commands/other/rev.c \
	commands/other/rfkill.c commands/other/rmmod.c \
	commands/other/setsid.c commands/other/shred.c \
	commands/other/stat.c commands/other/swapoff.c \
	commands/other/swapon.c commands/other/switch_root.c \
	commands/other/sysctl.c commands/other/tac.c \
	commands/other/taskset.c commands/other/timeout.c \
	commands/other/truncate.c commands/other/uptime.c \
	commands/other/usleep.c commands/other/vconfig.c \
	commands/other/vmstat.c commands/other/w.c \
	commands/other/which.c commands/other/xxd.c \
	commands/other/yes.c \
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
	mkdir -p ${ARCH}/shell ${ARCH}/builtins ${ARCH}/libedit ${ARCH}/toylib ${ARCH}/commands/posix ${ARCH}/commands/other
