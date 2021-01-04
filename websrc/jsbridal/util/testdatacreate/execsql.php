<?php

$dbname = $_POST['dbnm'];
$sql = $_POST['sql'];

$envfilename = "env/dbenv.txt";
$fp = fopen($envfilename, 'r');
fgets($fp);
$str = fgets($fp); $ary = explode(",", $str); $server = $ary[0];
$str = fgets($fp); $ary = explode(",", $str); $username = $ary[0];
$str = fgets($fp); $ary = explode(",", $str); $password = $ary[0];
fclose($fp);

if($mysql = mysql_pconnect($server,$username,$password)){
	mysql_select_db($dbname, $mysql);
	$mysql_result = mysql_query($sql, $mysql);
	mysql_close($mysql);
	if($mysql_result == FALSE){
		$retstr = "0,2,";
		echo $retstr;
		return;
	}
	$retstr = "1.0";
}else{
	$retstr = "0,1,,";
}
echo $retstr;
?> 
