#!/bin/sh

#http://stackoverflow.com/questions/115983/how-can-i-add-an-empty-directory-to-a-git-repository

find . -type d -empty -exec touch {}/.gitignore \;
#find . -name .git -prune -o -type d -empty -exec touch {}/.gitignore \;
#find * -type d -empty -exec touch {}/.gitignore \;
#find . -name .DS_Store -exec rm {} \;
