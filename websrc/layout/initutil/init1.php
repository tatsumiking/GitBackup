<?php
$argvs = $_POST['com'];
//$argvs = "0,0000,0000,0000,0,Š”Ž®‰ïŽÐŠÇ—,ŠÇ—ŽÒA,000-0000,ZŠ‚P“ú–{,ZŠ‚QŠØ‘,000-000-00000,abc-def@gmail.com,,,";
$argvs = "0,0001,0001,0001,1,Š”Ž®‰ïŽÐ“V‘,‘å“VŽgƒ~ƒJƒGƒ‹,000-0000,ZŠ‚P“ú–{,ZŠ‚QŠØ‘,000-000-00000,abc-def@gmail.com,,,";
//$argvs = "0,0002,0002,0002,1,Š”Ž®‰ïŽÐ“V‘,ƒGƒ“ƒWƒFƒ‹,1111-1111,ZŠ‚P“V‘,ZŠ‚Q‹ÉŠy,111-111-1111,xxx-111@gmail.com,,,";
mb_convert_variables('UTF-8', 'SJIS', $argvs);
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

$sqlchk = "SELECT * FROM `".$tblname."` WHERE (";
$sqlchk = $sqlchk."(`userno` like '".$a[1]."')";
$sqlchk = $sqlchk.");"; 
//$str1 = $sqlchk;
if($mysql = mysql_pconnect($server,$username,$password)){
	mysql_select_db($dbname, $mysql);
	$rows = mysql_query($sqlchk, $mysql);
	if($rows != NULL){
		$row = mysql_fetch_array($rows);
		if($row != NULL){
			$mysql_result = mysql_close($mysql);
			$str1 = "comp=0,2,,";
			echo $str1;
			return;
		}
	}
	$mysql_result = mysql_close($mysql);
}else{
	$str1 = "comp=0,1,,";
	echo $str1;
	return;
}

$sql = "INSERT INTO `".$tblname."` (";
$sql = $sql."`userno`,`userid`,`password`,`flag`";
$sql = $sql.",`company`,`name`,`postno`,`adrs1`";
$sql = $sql.",`adrs2`,`telno`,`mail`";
$sql = $sql.") VALUES (";
$sql = $sql."'".$a[1]."','".$a[2]."','".$a[3]."','".$a[4]."'";
$sql = $sql.",'".$a[5]."','".$a[6]."','".$a[7]."','".$a[8]."'";
$sql = $sql.",'".$a[9]."','".$a[10]."','".$a[11]."'";
$sql = $sql.");";

if($code == 0){
	mb_convert_variables('eucJP-win', 'UTF-8', $sql);
}
//$str1 = $sql;

if($mysql = mysql_pconnect($server,$username,$password)){
	mysql_select_db($dbname, $mysql);
	$mysql_result = mysql_query($sql, $mysql);
	if($mysql_result == FALSE){
		$mysql_result = mysql_close($mysql);
		$str1 = "comp=0,4,,";
		echo $str1;
		return;
	}
	//echo "<BR>"; var_dump($mysql_result); echo "<BR>";
	$mysql_result = mysql_close($mysql);
}else{
	$str1 = "comp=0,3,,";
	echo $str1;
	return;
}


if($mysql = mysql_pconnect($server,$username,$password)){
	mysql_select_db($dbname, $mysql);
	$rows = mysql_query($sqlchk, $mysql);
	if($rows == NULL){
		$str1 = "comp=0,6,,";
		echo $str1;
		return;
	}else{
		$row = mysql_fetch_array($rows);
		if($row == NULL){
			$str1 = "comp=0,7,,";
		}else{
			$str1 = "comp=1,".$row[0].",,";
		}
	}
	$mysql_result = mysql_close($mysql);
}else{
	$str1 = "comp=0,5,,";
}

//mb_convert_variables('sjis-win', 'UTF-8', $str1);
echo $str1;
?>

