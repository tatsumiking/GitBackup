<?php

//$dbname = "bridal";
//$sql = "";

$dbname = $_POST['dbnm'];
$sql = $_POST['sql'];

$envfilename = "../env/dbenv.txt";
$fp = fopen($envfilename, 'r');
fgets($fp);
$str = fgets($fp); $ary = explode(",", $str); $server = $ary[0];
$str = fgets($fp); $ary = explode(",", $str); $username = $ary[0];
$str = fgets($fp); $ary = explode(",", $str); $password = $ary[0];
fclose($fp);

if($mysql = mysql_connect($server,$username,$password)){
	mysql_select_db($dbname, $mysql);
	$ret = mysql_query($sql, $mysql);
	if($ret == FALSE){
		mysql_close($mysql);
		echo "0,2";
		return;
	}
	$id = mysql_insert_id($mysql);
	mysql_close($mysql);

	$retstr = "1,".$id.",";
}else{
	$retstr = "0,1,";
}
echo $retstr;

?>
