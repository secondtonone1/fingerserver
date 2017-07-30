

#pkill gameserver

#service crond start

todate=$(date +%Y%m%d_%2M%2S)

#/home/redis-3.0.7/src/redis-server /home/redis-3.0.7/redis.conf &

pkill gameserver
echo $todate
touch $todate.log

echo "now path is ...."
pwd
./gameserver &>$todate.log & 

tail -f $todate.log
