<?php

//$dbname = "bridal";
//$konreitable = "bridaluser";
//$krno = "000001";
//$krpw = "";

$dbname = $_POST['dbnm'];
$konreitable = $_POST['tble'];
$krno = $_POST['krno'];
$krpw = $_POST['krpw'];

$envfilename = "env/dbenv.txt";
$fp = fopen($envfilename, 'r');
fgets($fp);
$str = fgets($fp); $ary = explode(",", $str); $server = $ary[0];
$str = fgets($fp); $ary = explode(",", $str); $username = $ary[0];
$str = fgets($fp); $ary = explode(",", $str); $password = $ary[0];
fclose($fp);

$sqlslct = "SELECT * FROM ".$konreitable." WHERE (userno=".$krno.");";

$sqlkhn = "INSERT INTO ".$konreitable."";
$sqlkhn = $sqlkhn." (userno,username,password)";
$sqlkhn = $sqlkhn." VALUES (".$krno.",'".$krno."','".$krpw."');";

if($mysql = mysql_pconnect($server,$username,$password)){
	mysql_select_db($dbname, $mysql);

	$rows = mysql_query($sqlslct, $mysql);
	if($rows != NULL){
		$row = mysql_fetch_array($rows);
		if($row != NULL){
			mysql_close($mysql);
			echo "1,".$row[id].",";
			return;
		}
	}
	$ret = mysql_query($sqlkhn, $mysql);
	if($ret == FALSE){
		mysql_close($mysql);
		echo "0,2,";
		return;
	}
	$id = mysql_insert_id($mysql);

	$gesttable = "ge".$krno;
	$sql = "CREATE TABLE ".$gesttable."";
	$sql = $sql." (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY";
	$sql = $sql.",flag INT NOT NULL,tno INT NOT NULL,sno INT NOT NULL";
	$sql = $sql.",name1 VARCHAR(24),name2 VARCHAR(24),yomi VARCHAR(48)";
	$sql = $sql.",sama VARCHAR(15),kind VARCHAR(30),skind VARCHAR(30)";
	$sql = $sql.",adrsno VARCHAR(24),adrs1 VARCHAR(90),adrs2 VARCHAR(90)";
	$sql = $sql.",adrs3 VARCHAR(90),telno VARCHAR(24),kt1 VARCHAR(60)";
	$sql = $sql.",kt2 VARCHAR(60),kt3 VARCHAR(60),kt4 VARCHAR(60)";
	$sql = $sql.",gno INT NOT NULL,sub1 VARCHAR(96),sub2 VARCHAR(96)";
	$sql = $sql.",gift VARCHAR(30),dish VARCHAR(30));";
// echo $sql;
	$ret = mysql_query($sql, $mysql);
	if($ret == FALSE){
		mysql_close($mysql);
		$retstr = "0,5,";
		echo $retstr;
		return;
	}

	$layouttable = str_replace("ge", "tb", $gesttable);
	$sql = "CREATE TABLE `".$layouttable."` (`id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY";
	$sql = $sql.",`dx` INT NOT NULL,`dy` INT NOT NULL);";
	$ret = mysql_query($sql, $mysql);
	if($ret == FALSE){
		mysql_close($mysql);
		$retstr = "0,6,";
		echo $retstr;
		return;
	}

	$strtable = str_replace("ge", "ss", $gesttable);
	$sql = "CREATE TABLE `".$strtable."` (`id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY";
	$sql = $sql.",`leftstr` VARCHAR(192),`rightstr` VARCHAR(192));";
	$ret = mysql_query($sql, $mysql);
	if($ret == FALSE){
		mysql_close($mysql);
		$retstr = "0,7,";
		echo $retstr;
		return;
	}

	$mysql_result = mysql_close($mysql);
	
	$folder = "list/".$gesttable;
	if(!file_exists($folder)){
		mkdir($folder, 0777);
	}
	$retstr = "1,".$id.",";
}else{
	$retstr = "0,1,";
}
echo $retstr;
?>
