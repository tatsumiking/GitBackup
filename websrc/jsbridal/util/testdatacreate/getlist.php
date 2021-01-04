<?php

$dbname = $_POST['dbnm'];
$sqlstr = $_POST['sql'];
//$dbname = "bridal";
//$sqlstr = "SELECT * FROM bridaluser;";
//$dbname = "";
//$sqlstr = "SHOW DATABASES;";
//$sqlstr = "SHOW TABLES FROM bridal;";

$envfilename = "env/dbenv.txt";
$fp = fopen($envfilename, 'r');
fgets($fp);
$str = fgets($fp); $ary = explode(",", $str); $server = $ary[0];
$str = fgets($fp); $ary = explode(",", $str); $username = $ary[0];
$str = fgets($fp); $ary = explode(",", $str); $password = $ary[0];
fclose($fp);

if($mysql = mysql_pconnect($server,$username,$password)){
	if($dbname != ""){
		mysql_select_db($dbname, $mysql);
	}
	$rows = mysql_query($sqlstr, $mysql);
	mysql_close($mysql);
	if($rows == NULL){
		$retstr = "0,2,"."<".$sqlstr.">";
		echo $retstr;
		return;
	}
	$retstr = "";
	while(1){
		$row = mysql_fetch_array($rows);
		if($row == NULL){
			break;
		}
		$max = count($row) / 2;
		for($idx = 0; $idx < $max; $idx++){
			if($idx != 0){
				$retstr = $retstr.",";
			}
			$retstr = $retstr.$row[$idx];
		}
		$retstr = $retstr.";";
	}
}else{
	$retstr = "0,1,,";
	echo $retstr."<".$sqlstr.">";
	return;
}
//mb_convert_variables('sjis-win', 'UTF-8', $str1);
echo $retstr;
?> 
