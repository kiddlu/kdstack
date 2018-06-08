#COMMAND     PID   TID             USER   FD      TYPE             DEVICE SIZE/OFF       NODE NAME
#systemd       1                   root  cwd       DIR                8,1     4096          2 /

#list process open this file lsof abc.txt
lsof /dev/urandom

#
lsof -c zsh
