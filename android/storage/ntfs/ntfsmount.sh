#!/system/bin/sh
# By munjeni @ 2014

option="${1}"

case "$option" in
  "mount")
    echo "ntfs volume is mounted and selinux is permisive"
    setenforce 0
    ntfs-3g -o rw,uid=1023,gid=1023,fmask=0007,dmask=0007,umask=0007,permissions,allow_other,no_def_opts,nls=utf8 /dev/block/sda1 /mnt/media_rw/usbdisk
    mount -t sdcardfs -o rw,nosuid,nodev,noatime,nodiratime,uid=1023,gid=1023,derive=unified /mnt/media_rw/usbdisk /storage/usbdisk
    ;;
  "umount")
    echo "ntfs volume is unmounted and selinux is enforced"
    umount /storage/usbdisk
    umount /mnt/media_rw/usbdisk
    setenforce 1
    ;;
  *)
    echo "usage: `basename ${0}` [mount] | [umount]" 
    exit 1    
    ;;
esac
