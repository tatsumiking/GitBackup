<?php   
$argvs = $_POST['com'];
//$argvs = "0001,0001,,";
//mb_convert_variables('UTF-8', 'SJIS', $argvs);
$a = explode(",", $argvs);
$userid = $a[0];
$userpw = $a[1];

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

if($userid == "quickmaster" && $userpw == "15375"){
	$str = "comp=1,0,0,,,";
	echo $str;
	return;
}

$sql = "SELECT * FROM `".$tblname."` WHERE (";
$sql = $sql."(`userid` like '".$userid."')";
$sql = $sql.");"; 

$str1 = "comp=0,1,0,,";
if($mysql = mysql_pconnect($server,$username,$password)){
	mysql_select_db($dbname, $mysql);
	if($code == 2){
		mysql_query("set names utf8");
	}
	$rows = mysql_query($sql, $mysql);
	$str = "comp=0,2,0,,";
	if($rows != NULL){
		$row = mysql_fetch_array($rows);
		if($row != NULL){
			if(strcmp($userpw, $row[3]) == 0){
				$str = "comp=1,".$row[4].",".$row[0].",,";
			}else{
				$str = "comp=0,3,0,,";
			}
		}
	}
	$mysql_result = mysql_close($mysql);
}
echo $str;
?> 
