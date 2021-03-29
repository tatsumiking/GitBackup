<?php
//$argvs = $_POST['com'];
$argvs= "1,,";
$a = explode(",", $argvs);

$filename = "../env/code.txt";
$fp = fopen($filename, 'r');
$code = fgets($fp);$code = substr($code, 0, -2);
fclose($fp);

$filename = "../env/dbenv.txt";
$fp = fopen($filename, 'r');
$server = fgets($fp);$server = substr($server, 0, -2);
$username = fgets($fp);$username = substr($username, 0, -2);
$password = fgets($fp);$password = substr($password, 0, -2);
$dbname = fgets($fp);$dbname = substr($dbname, 0, -2);
$tblname = fgets($fp);$tblname = substr($tblname, 0, -2);
fclose($fp);

// カレンダー用ユーザー管理
$sql = "CREATE TABLE `".$tblname;
$sql = $sql."` (`id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY";
$sql = $sql.",`userno` VARCHAR(16),`userid` VARCHAR(32)";
$sql = $sql.",`password` VARCHAR(32),`flag` INT NOT NULL";
$sql = $sql.",`company` VARCHAR(64),`name` VARCHAR(32)";
$sql = $sql.",`postno` VARCHAR(16),`adrs1` VARCHAR(64)";
$sql = $sql.",`adrs2` VARCHAR(64),`telno` VARCHAR(16)";
$sql = $sql.",`mail` VARCHAR(32),`option` VARCHAR(64));";

echo $sql."</BR>";
if($mysql = mysql_pconnect($server,$username,$password)){
	mysql_select_db($dbname, $mysql);
	$mysql_result = mysql_query($sql, $mysql);
	var_dump($mysql_result); echo "</BR>";
	$mysql_result = mysql_close($mysql);
}

$str1 = "comp=".$dbname.",".$tblname.",";
echo $str1;
?>

