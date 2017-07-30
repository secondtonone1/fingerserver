#!/bin/sh
 #killall -9 gameserver

filepath="./"
echo $filepath
cd $filepath/
pwd

i=0
limit=100

let timeinv=5*60*60
#sleep $timeinv

#while [ "$i" -le $limit ]
#do 
i=$(($i+1))

echo update now
svn update
pwd

yes|cp Win32/Binaries/Data/* Linux/Data/
echo copy .dbc

cd PlatformLib/
make
pwd
#yes|cp -rf libplatform.a ../Linux/libplatform.a

cd ../NetworkLib/
make
pwd
#yes|cp -rf libnetwork.a  ../Linux/libnetwork.a

cd ../CommonLib/
make
pwd
#yes|cp -rf libcommon.a  ../Linux/libcommon.a 

cd ..
make
pwd
yes|cp -rf .libs/gameserver Linux/gameserver


cd Linux/

chmod +x gameserver
#./gameserver & 

#done
