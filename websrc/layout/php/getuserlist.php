<?php
$argvs = $_POST['com'];
//$argvs = "0,,";
//mb_convert_variables('UTF-8', 'SJIS', $argvs);
$a = explode(",", $argvs);

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

$sql = "SELECT * FROM `".$tblname."`;";
$str1 = "comp=";
if($mysql = mysql_pconnect($server,$username,$password)){
	mysql_select_db($dbname, $mysql);
	if($code == 2){
		mysql_query("set names utf8");
	}
	$rows = mysql_query($sql, $mysql);
	while(1){
		$row = mysql_fetch_array($rows);
		if($row == NULL){
			break;
		}
		if($code == 0){
			mb_convert_variables('UTF-8', 'eucJP-win', $row);
		}
		$str1 = $str1.$row[1]." ".$row[2];
		$str1 = $str1." ".$row[5]." ".$row[6].":";
		$str1 = $str1.$row[0].",";
	}
	$mysql_result = mysql_close($mysql);
}

//mb_convert_variables('sjis-win', 'UTF-8', $str1);
echo $str1;
?>

