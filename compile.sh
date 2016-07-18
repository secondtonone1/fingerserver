#!/bin/sh
 killall -9 gameserver

filepath="./"
echo $filepath
cd $filepath/
pwd

i=0
limit=100

let timeinv=5*60*60
#sleep $timeinv

while [ "$i" -le $limit ]
do 
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
yes|cp -rf gameserver Linux/gameserver


cd Linux/


count=`cat /etc/ld.so.conf |grep Server/Linux |wc -l`
echo so num =$count
if [ $count -eq 0 ]
then
"$dir/Linux">>/etc/ld.so.conf
/sbin/ldconfig -v
fi

chmod +x gameserver
./gameserver & 

count=`ps -ef |grep gameserver |grep -v "grep" |wc -l`
echo process num =$count



if [ 0 -eq $count ]
then
echo "gameserver is fail to run!!!"
else
echo "gameserver is running!!!"
fi

echo ""

echo "sleep 24*60*60 then compile and continue running!"
let timeinv24=24*60*60
sleep $timeinv24

done
