#!/usr/bin/bash

cd client; make clean; make
ln -s client ../clientLink
cd ../libsrc; make clean; make
libroot=$(pwd)
cd ../server; make clean; make
ln -s userServer ../userServerLink
root=$(pwd)
#현재 서버루트 확인
sed "s!/home/mobis/board/server!$root!g" menu_board.txt > menu_board1.txt
mv menu_board1.txt menu_board.txt

sed "s!/home/mobis/board/server!$root!g" menu_databoard.txt > menu_databoard1.txt
mv menu_databoard1.txt menu_databoard.txt

sed "s!/home/mobis/board/server!$root!g" menu_login.txt > menu_login1.txt
mv menu_login1.txt menu_login.txt

sed "s!/home/mobis/board/server!$root!g" menu_two.txt > menu_two1.txt
mv menu_two1.txt menu_two.txt

export LD_LIBRARY_PATH=$libroot:$LD_LIBRARY_PATH