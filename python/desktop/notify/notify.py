#!/usr/bin/python
#coding:utf-8

import pynotify
import time

ISOTIMEFORMAT='%Y-%m-%d %X'
title=time.strftime(ISOTIMEFORMAT, time.localtime(time.time()))

msg="Hell World, 你好世界"

pynotify.init("pynotify")
notify = pynotify.Notification(title, msg)
notify.show()
