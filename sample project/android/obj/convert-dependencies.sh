#!/bin/sh
# AUTO-GENERATED FILE, DO NOT EDIT!
if [ -f $1.org ]; then
  sed -e 's!^C:/Users/david\.landeros/Documents/git/usr/bin!/bin!ig;s! C:/Users/david\.landeros/Documents/git/usr/bin! /bin!ig;s!^C:/Users/DAVID~1\.LAN/AppData/Local/Temp!/tmp!ig;s! C:/Users/DAVID~1\.LAN/AppData/Local/Temp! /tmp!ig;s!^C:/Users/david\.landeros/Documents/git/!/!ig;s! C:/Users/david\.landeros/Documents/git/! /!ig;s!^D:!/d!ig;s! D:! /d!ig;s!^C:!/c!ig;s! C:! /c!ig;' $1.org > $1 && rm -f $1.org
fi
