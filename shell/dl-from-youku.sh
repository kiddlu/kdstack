#!/bin/sh
web_page=http://i.youku.com/i/UNTc0MzM0Mjcy/videos\?order\=1\&page\=2\&last_item\=50
string="沙滩排球3"

curl -s $web_page | pup 'a' | grep $string  | grep video | busybox tac | sed "s/<a href=\"//g" | sed "s/\" data.*//g" | xargs you-get
