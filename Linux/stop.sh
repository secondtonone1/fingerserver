#!/bin/bash

function stopserver()
{
	for i in `ps naux|grep $1|grep -v 'grep'|awk '{print $1,$2}'|grep $UID|awk '{print $2}'`
	do
		echo "$1 process $i has been killed!"
		kill -15 $i
	done 
}

stopserver gameserver 

