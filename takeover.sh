#!/bin/sh
set -e

TO=/takeover
OLD_INIT=$(readlink /proc/1/exe)

cd "$TO"

if [ ! -e fakeinit ]; then
    ./busybox echo "Please compile fakeinit.c first"
    exit 1
fi


./busybox echo "Type 'OK' to continue"
./busybox echo -n "> "
read a
if [ "$a" != "OK" ] ; then
    exit 1
fi

./busybox echo "Preparing init..."

TTY="$(./busybox tty)"

./busybox cat >tmp/${OLD_INIT##*/} <<EOF
#!${TO}/busybox sh

exec <"${TTY}" >"${TTY}" 2>"${TTY}"
cd "${TO}"

./busybox echo "Init takeover successful"
./busybox echo "preparing to pivot root if you want to..."
./busybox mount --make-rprivate /
./busybox echo "running init..."
exec /takeover/fakeinit
EOF
./busybox chmod +x tmp/${OLD_INIT##*/}

./busybox echo "Type OK to continue"
./busybox echo -n "> "
read a
if [ "$a" != "OK" ] ; then
    exit 1
fi

./busybox echo "About to take over init. This script will now pause for a few seconds."
./busybox echo "If the takeover was successful, you will see output from the new init."
./busybox echo "You may then kill the remnants of this session and any remaining"
./busybox echo "processes from your new SSH session, and umount the old root filesystem."

./busybox mount --bind tmp/${OLD_INIT##*/} ${OLD_INIT}

telinit u

./busybox sleep 5

