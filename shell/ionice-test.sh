sync
echo 3 > /proc/sys/vm/drop_caches
TMPDIR=/tmp
ionice -c 1 -n 0 time dd of=$TMPDIR/testzerofile1 if=/dev/zero bs=512MB count=5 &
#time ionice -c 3 -n 4 dd of=$TMPDIR/testzerofile2 if=/dev/zero bs=512MB count=5 &
#time dd of=$TMPDIR/testzerofile1 if=/dev/zero bs=512MB count=5 &
time dd of=$TMPDIR/testzerofile3 if=/dev/zero bs=512MB count=5 &
