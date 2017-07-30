<?php
error_reporting( E_ALL&~E_NOTICE );
date_default_timezone_set('PRC');

$serverId = 1;
$backupdir = "/backup/backupfile/";
$dir = "/home/boc_a/Server/Linux/Logs/";
$serverDB = "192.168.1.40:3306";
$user = "zjml";
$password = "mysqlpassword@zjml";
$matchStr = "logfile";
$fileSize = 100*1024*1024;
$date = date("Ymdh");
// print_r("$date");
$finalLogTime = null;

$confFile = $dir."php_import.conf";

echo $dir;
$fileList = array();
$fileNameList = array();
foreach(scandir($dir) as $afile)
{
	if($afile=='.'||$afile=='..') 
		continue; 
	if(!is_dir($dir.'/'.$afile)) 
	{ 
		if (strpos($afile, $matchStr) === false)
		{
		}
		else 
		{
			if($afile== $matchStr)
			{
				$fileNameList = "{$afile}";
				$fileList["target"] = "{$dir}{$afile}";
			}
			else
			{
				
				print_r("-getfile-\n");
				$fileList[] = "{$dir}{$afile}";
				print_r("{$dir}{$afile}");
			}
		}
	} 
}

//print_r($fileList);
 print_r("---- file name end !!!!---\n");

$lastLogTime = null;
//TODO
$myfile = fopen($confFile, "r") or die("Unable to open file!");
$lastLogTime = fread($myfile,filesize($confFile));
fclose($myfile);


$finalLogTime = $lastLogTime;
$logContent = array();
foreach ($fileList as $key => $fileName) {
	print_r("---filename--");
	print_r($fileName);
	$fileContent = file($fileName);
	foreach ($fileContent as $rowIdx => $rowContent) {
		$rowArr = explode(",", $rowContent);
		
		//TODO
		 if ($rowArr[1] < $lastLogTime) {
			continue;
			
		 }
		if($finalLogTime < $rowArr[1])
		{
			$finalLogTime = $rowArr[1];
		}

		$rowElementCount = count($rowArr);
		if($rowElementCount < 10){
			continue;
		}

		$operateParamStr = "";
		for ($i=10; $i<=$rowElementCount; $i++) { 
			$operateParamStr = $operateParamStr . $rowArr[$i];
			if ($i != $rowElementCount)
				$operateParamStr = $operateParamStr . ",";
		}

		$timeArr = explode(" ", $rowArr[1]);
		$log_date = $timeArr[0];
		$log_time = $timeArr[1];

		$logContent[] = array(
			'date' => $log_date, 
			'time' => $log_time, 
			'datetime' => $rowArr[1],
			'playerId' => $rowArr[6],
			'playerName' => $rowArr[5],
			'serverID' => $rowArr[4],
			'power' => $rowArr[7],
			'level' => $rowArr[8],
			'viplevel' => $rowArr[9],
			'operateType' => $rowArr[3],
			'operateParams' => $operateParamStr,
		);
		if ($key == "target") {
			$finalLogTime = $rowArr[1];
		}
	}
}

print_r("---- inser sql 22 !!!!---\n");

$accountDB = mysql_pconnect(trim(strval($serverDB)), $user, $password);
if (!$accountDB) {
	echo "Error account db";
	exit;
}
$selectAccountdb = mysql_select_db("BOC_ACCOUNT", $accountDB);
if (!$selectAccountdb) {
	echo "Error db select account";
	exit;
}
mysql_query("set names 'utf8';");


$serverConfigArray = array();
$result = mysql_query("select * from ServerConfig where serverIdx=".$serverId);
if ($result) {
	while($row = mysql_fetch_array($result, MYSQL_ASSOC))
		$serverConfigArray[] = $row;
}

if($serverConfigArray[0] == NULL) {
	echo "Error not find server";
	exit;
}

if ($serverConfigArray[0]["dbIP"] == NULL || $serverConfigArray[0]["dbPort"] == NULL || $serverConfigArray[0]["dbName"] == NULL) {
	echo "Error server config";
	exit;
}


$conAddr = $serverConfigArray[0]["dbIP"] . ":" . $serverConfigArray[0]["dbPort"];
$dbConnect = mysql_pconnect(trim(strval($conAddr)), $user, $password);
if (!$dbConnect) {
	echo "Error server db";
	exit;
}
$selectdb = mysql_select_db(trim(strval($serverConfigArray[0]["logDBName"])), $dbConnect);
if (!$selectdb) {
	echo "Error db select log";
	exit;
}
mysql_query("set names 'utf8';");


$prevDBTblName = null;
foreach ($logContent as $key => $logInfo) {
	$tableName = "server_log_" . $logInfo["date"];
	
	 echo "\n tableName: ".$tableName;	

	$createTableSql = "create table if not exists `".$tableName."` (
		`id` bigint UNSIGNED NOT NULL AUTO_INCREMENT,
		`time` varchar(255) NOT NULL,
		`uuid` int NOT NULL,
		`name` varchar(255),
		`operateType` int,
		`operateParam` varchar(255),
		`power` int,
		`level` int,
		`viplevel` int,
		`serverId` int,
		PRIMARY KEY (`id`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;";
	echo "\ncreateTableSql =  ".$createTableSql;
	
	mysql_query($createTableSql);
	if ($prevDBTblName != $tableName) {
		$deleteSql = "delete from `".$tableName."` where `time`='".$lastLogTime."';";
		mysql_query($deleteSql);
		$prevDBTblName = $tableName;
	}

	$insertTableSql = "insert into `".$tableName."` (`time`, `uuid`, `name`, `operateType`, `operateParam`, `power`, `level`, `viplevel`, `serverId`) values (";
	$insertTableSql .= "\"" . $logInfo['datetime'] . "\", ";
	$insertTableSql .= $logInfo['playerId'] . ", ";
	$insertTableSql .= "\"" . $logInfo['playerName'] . "\", ";
	$insertTableSql .= $logInfo['operateType'] . ", ";
	$insertTableSql .= "\"" . $logInfo['operateParams'] . "\", ";
	$insertTableSql .= $logInfo['power'] . ", ";
	$insertTableSql .= $logInfo['level'] . ", ";
	$insertTableSql .= $logInfo['viplevel'] . ", ";
	$insertTableSql .= $logInfo['serverID'];
	$insertTableSql .= ");";
	echo "\n".$insertTableSql;
	mysql_query($insertTableSql);
}

foreach ($fileNameList as $key => $fileName) {
	if ($key != "target") {
		system("mv ".$fileName." {$backupdir}.{$fileName}.{$date}");
	}
}

//TODO
// $finalLogTime
$myfile = fopen($confFile, "w") or die("Unable to open file!");
fwrite($myfile, $finalLogTime);
 print_r("----write file---\n");
fclose($myfile);



echo "OK";


?>
