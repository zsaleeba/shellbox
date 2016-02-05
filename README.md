shellbox is a shell with built-in commands. It was originally designed to be used for embedded systems where you don't have a full OS but you'd like to be able to use a UNIX-like shell anyway. It has the functionality of a full linux/UNIX shell including fast shell scripting and many standard commands, all within a relatively simple structure which can be easily added to your project.

shellbox is designed for modern embedded systems with >1MB of FLASH and RAM plus a usable filesystem and a fairly complete C library implementation. Since many embedded system don't have the ability to exec() programs off the filesystem it doesn't require a working exec() call.

shellbox is based on the dash shell, toybox command line utilities and the libedit command line editor.

Commands available:

| Commands | ... | ... | ... | ... | ... |
| -------- | --- | --- | --- | --- | --- |
| acpi | base64 | basename | blkid | blockdev | bunzip2 | 
| bzcat | cal | cat | catv | chattr | chgrp | 
| chmod | chown | chroot | chvt | cksum | clear | 
| cmp | comm | count | cp | cpio | cut | 
| date | df | dirname | dmesg | dos2unix | du | 
| echo | egrep | eject | env | expand | factor | 
| fallocate | false | fgrep | find | flock | free | 
| freeramdisk | fsfreeze | fstype | fsync | grep | groups | 
| halt | head | help | hexedit | hostid | hostname | 
| hwclock | id | ifconfig | inotifyd | insmod | install | 
| ionice | iorenice | iotop | kill | killall | killall5 | 
| link | ln | login | logname | losetup | ls | 
| lsattr | lsmod | lspci | lsusb | makedevs | md5sum | 
| mix | mkdir | mkfifo | mknod | mkpasswd | mkswap | 
| mktemp | modinfo | mount | mountpoint | mv | nbd-client | 
| nc | netcat | nice | nl | nohup | nproc | 
| nsenter | od | oneit | partprobe | passwd | paste | 
| patch | pgrep | pidof | pivot_root | pkill | pmap | 
| poweroff | printenv | printf | ps | pwd | pwdx | 
| readahead | readlink | realpath | reboot | renice | reset |
| rev | rfkill | rm | rmdir | rmmod | sed | 
| seq | setsid | sha1sum | shred | sleep | sort | 
| split | stat | strings | su | swapoff | swapon | 
| switch_root | sync | sysctl | tac | tail | taskset | 
| tee | time | timeout | top | touch | true | 
| truncate | tty | umount | uname | uniq | unix2dos | 
| unlink | unshare | uptime | usleep | uudecode | uuencode | 
| vconfig | vmstat | w | wc | which | who | 
| whoami | xargs | xxd | yes| | |

Partially implemented:

| Commands | ... | ... | ... | ... | ... |
| -------- | --- | --- | --- | --- | --- |
| addgroup | adduser | arp | arping | bootchartd | brctl | 
| cd | compress | crond | crontab | dd | deallocvt | 
| delgroup | deluser | dhcp | dhcpd | diff | dumpleases | 
| exit | expr | fdisk | fold | fsck | ftpget | 
| ftpput | getty | groupadd | groupdel | gunzip | gzip | 
| host | iconv | init | ip | ipcrm | ipcs | 
| klogd | last | logger | lsof | mdev | mke2fs | 
| modprobe | more | netstat | openvt | ping | route | 
| sulogin | syslogd | tar | tcpsvd | telnet | telnetd |
| test | tftp | tftpd | tr | traceroute | udpsvd | 
| useradd | userdel | vi | watch | xzcat | zcat | 

    
