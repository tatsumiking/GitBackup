<?php   
$argvs = $_POST['com'];
//$argvs = "0004,,,";
//mb_convert_variables('UTF-8', 'SJIS', $argvs);
$a = explode(",", $argvs);
$userno = $a[0];

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

$sqlchk = "SELECT * FROM `".$tblname."` WHERE (";
$sqlchk = $sqlchk."(`userno` like '".$userno."')";
$sqlchk = $sqlchk.");";

if($code == 0){
	mb_convert_variables('eucJP-win', 'UTF-8', $sql);
}

$str1 = "comp=0,1,0,,";
if($mysql = mysql_pconnect($server,$username,$password)){
	mysql_select_db($dbname, $mysql);
	if($code == 2){
		mysql_query("set names utf8");
	}
	$rows = mysql_query($sqlchk, $mysql);
	$str = "comp=0,2,0,,";
	if($rows != NULL){
		$row = mysql_fetch_array($rows);
		if($code == 0){
			mb_convert_variables('UTF-8', 'eucJP-win', $row);
		}
		if($row != NULL){
			$str = "comp=1,".$row[0].",".$row[1].",,";
		}
	}
	$mysql_result = mysql_close($mysql);
}
echo $str;
?> 
