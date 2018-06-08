#!/bin/bash

cat `readlink -f $0`

txt_file=`mktemp --suffix=.txt`
trap "rm -f $txt_file" EXIT

cat <<EOF > $txt_file

backing-dev.c,5238
static atomic_long_t bdi_seq = ATOMIC_LONG_INIT(0);bdi_seq15,324
struct backing_dev_info noop_backing_dev_info = {noop_backing_dev_info17,377
EXPORT_SYMBOL_GPL(noop_backing_dev_info);noop_backing_dev_info21,496
static struct class *bdi_class;bdi_class23,539
DEFINE_SPINLOCK(bdi_lock);bdi_lock29,662
LIST_HEAD(bdi_list);bdi_list30,689
struct workqueue_struct *bdi_wq;bdi_wq33,764
static struct dentry *bdi_debug_root;bdi_debug_root39,877
EOF

cat -v $txt_file | sed -e "s/\^A/  /" -e "s/\^?/ /"
