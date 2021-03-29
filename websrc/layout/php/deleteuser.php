<?php
$argvs = $_POST['com'];
//$argvs = "2,,";	// recno
$a = explode(",", $argvs);
$recno = $a[0];

$filename = "env/code.txt";
$fp = fopen($filename, 'r');
$code = fgets($fp);$code = substr($code, 0, -2);
fclose($fp);

$filename = "env/dbenv.txt";
$fp = fopen($filename, 'r');
$server = fgets($fp);$server = substr($server, 0, -2);
$username = fgets($fp);$username = substr($username, 0, -2);
$password = fgets($fp);$password = substr($password, 0, -2);
$dbname = fgets($fp);$dbname = substr($dbname, 0, -2);
$tblname = fgets($fp);$tblname = substr($tblname, 0, -2);
fclose($fp);

$sql = "DELETE FROM `".$tblname."` WHERE `id` = ".$recno." LIMIT 1;";
//echo $sql."<BR>";
if($mysql = mysql_pconnect($server,$username,$password)){
	mysql_select_db($dbname, $mysql);
	if($code == 2){
		mysql_query("set names utf8");
	}
	$mysql_result = mysql_query($sql, $mysql);
	if($mysql_result == FALSE){
		$mysql_result = mysql_close($mysql);
		$str1 = "comp=0,2,,";
		echo $str1;
		return;
	}
	$mysql_result = mysql_close($mysql);
}else{
	$str1 = "comp=0,1,,";
	echo $str1;
	return;
}

$str1 = "comp=1,".$recno.",,";
echo $str1;
?>
