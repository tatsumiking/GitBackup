<?php
$argvs = $_POST['com'];
//$argvs = "1,0001,0001,0001,0,Š”Ž®‰ïŽÐ“V‘,“VŽgƒJƒ~ƒGƒ‹,000-0000,ZŠ‚P“ú–{,ZŠ‚QŠØ‘,000-000-00000,abc-def@gmail.com,,,";
//mb_convert_variables('UTF-8', 'SJIS', $argvs);
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

$sqlchk = "SELECT * FROM `".$tblname."` WHERE (";
$sqlchk = $sqlchk."(`userno` like '".$a[1]."')";
$sqlchk = $sqlchk.");";
//$str1 = $sqlchk;
if($mysql = mysql_pconnect($server,$username,$password)){
	mysql_select_db($dbname, $mysql);
	if($code == 2){
		mysql_query("set names utf8");
	}
	$rows = mysql_query($sqlchk, $mysql);
	if($rows != NULL){
		while(1){
			$row = mysql_fetch_array($rows);
			if($row == NULL){
				break;
			}
			if($row[0] != $a[0]){
				$mysql_result = mysql_close($mysql);
				$str1 = "comp=0,2,,";
				echo $str1;
				return;
			}
		}
	}
	$mysql_result = mysql_close($mysql);
}else{
	$str1 = "comp=0,1,1,";
	echo $str1;
	return;
}

$sqlchk = "SELECT * FROM `".$tblname."` WHERE (";
$sqlchk = $sqlchk."(`userid` like '".$a[2]."')";
$sqlchk = $sqlchk.");";
//$str1 = $sqlchk;
if($mysql = mysql_pconnect($server,$username,$password)){
	mysql_select_db($dbname, $mysql);
	if($code == 2){
		mysql_query("set names utf8");
	}
	$rows = mysql_query($sqlchk, $mysql);
	if($rows != NULL){
		while(1){
			$row = mysql_fetch_array($rows);
			if($row == NULL){
				break;
			}
			if($row[0] != $a[0]){
				$mysql_result = mysql_close($mysql);
				$str1 = "comp=0,3,,";
				echo $str1;
				return;
			}
		}
	}
	$mysql_result = mysql_close($mysql);
}else{
	$str1 = "comp=0,1,2,";
	echo $str1;
	return;
}

$sql = "UPDATE `".$tblname."` SET ";
$sql = $sql."`userno`='".$a[1]."',`userid`='".$a[2]."'";
$sql = $sql.",`password`='".$a[3]."',`flag`='".$a[4]."'";
$sql = $sql.",`company`='".$a[5]."',`name`='".$a[6]."'";
$sql = $sql.",`postno`='".$a[7]."',`adrs1`='".$a[8]."'";
$sql = $sql.",`adrs2`='".$a[9]."',`telno`='".$a[10]."'";
$sql = $sql.",`mail`='".$a[11]."'";
$sql = $sql." WHERE (`id`=".$recno.");";

if($code == 0){
	mb_convert_variables('eucJP-win', 'UTF-8', $sql);
}
if($mysql = mysql_pconnect($server,$username,$password)){
	mysql_select_db($dbname, $mysql);
	if($code == 2){
		mysql_query("set names utf8");
	}
	$mysql_result = mysql_query($sql, $mysql);
	if($mysql_result == FALSE){
		$mysql_result = mysql_close($mysql);
		//mb_convert_variables('sjis-win', 'UTF-8', $sql);
		//echo $sql; echo "<BR>";
		$str1 = "comp=0,4,,";
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
