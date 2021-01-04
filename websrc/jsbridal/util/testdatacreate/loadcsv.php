<?php

//$dbname = "bridal";
//$konreitable = "bridaluser";
//$recid = 2;
//$konreino = "000001";

$dbname = $_POST['dbnm'];
$konreitable = $_POST['krtbl'];
$recid = $_POST['recid'];
$konreino = $_POST['krno'];

$gesttable = "ge".$konreino;

$envfilename = "env/dbenv.txt";
$fp = fopen($envfilename, 'r');
fgets($fp);
$str = fgets($fp); $ary = explode(",", $str); $server = $ary[0];
$str = fgets($fp); $ary = explode(",", $str); $username = $ary[0];
$str = fgets($fp); $ary = explode(",", $str); $password = $ary[0];
$str = fgets($fp); $ary = explode(",", $str); $saveurl = $ary[0];
fclose($fp);

$csvfilename = "temp/upload/".$gesttable.".csv";

if($mysql = mysql_connect($server,$username,$password)){
	mysql_select_db($dbname, $mysql);
	$fp = fopen($csvfilename, 'r');
	if($fp == FALSE){
		mysql_close($mysql);
		$retstr = "0,2,";
		echo $retstr;
		return;
	}
	$str = fgets($fp);
	$str = mb_convert_encoding($str, 'UTF-8', 'sjis-win');	
	$a = split(',',$str);
	$sql = "UPDATE ".$konreitable." SET tablelayout='".$a[0]."'";
	$sql = $sql.",kyosiki='".$a[1]."',hirouen='".$a[2]."'";
	$sql = $sql.",kaijyou='".$a[3]."',mukotori='".$a[4]."'";
	$sql = $sql.",sinrozoku='".$a[5]."',sinroname1='".$a[6]."',sinroname2='".$a[7]."'";
	$sql = $sql.",sinpuzoku='".$a[8]."',sinpuname1='".$a[9]."',sinpuname2='".$a[10]."'";
	$sql = $sql.",sinrodish='".$a[11]."',sinpudish='".$a[12]."'";
	$sql = $sql.",sinrosub='".$a[13]."',sinpusub='".$a[14]."'";
	$sql = $sql.",paperlocate='".$a[15]."',papersize='".$a[16]."'";
	$sql = $sql.",ryoukekind='".$a[17]."',tablekind='".$a[18]."'";
	$sql = $sql.",takasagokind='".$a[19]."',nametype='".$a[20]."',textsize='".$a[21]."'";
	$sql = $sql." WHERE (id=".$recid.");";
// echo $sql;
	$ret = mysql_query($sql, $mysql);
	if($ret == FALSE){
		mysql_close($mysql);
		fclose($fp);
		$retstr = "0,3,";
		echo $retstr;
		return;
	}


	$gestcount=0;
	while(1){
		if(feof($fp)){
			break;
		}
		$str = fgets($fp);
		$str = mb_convert_encoding($str, 'UTF-8', 'sjis-win');
		$a = split(',',$str);
		if(strcmp($a[1],"start") == 0){
			break;
		}
		if($a[0] != ""){
			$kubun = mb_convert_encoding($str1, 'sjis-win', 'UTF-8');
			$kubun = substr($kubun, 0, 4);
			if($kubun == "新郎"){
				$flag = 1;
			}else if($kubun == "新婦"){
				$flag = 2;
			}else{
				$flag = 0;
			}
			if($gestcount == 0){
				$datasql = "";
			}else{
				$datasql=$datasql.",";
			}
		
			$datasql=$datasql."('".$flag."','".$a[19]."','".$a[20]."'";	// tno,sno
			$datasql=$datasql.",'".$a[0]."','".$a[1]."','".$a[2]."','".$a[3]."'";
			$datasql=$datasql.",'".$a[4]."','".$a[5]."','".$a[6]."','".$a[7]."'";
			$datasql=$datasql.",'".$a[8]."','".$a[9]."','".$a[10]."'";
			$datasql=$datasql.",'".$a[11]."','".$a[12]."','".$a[13]."','".$a[14]."'";
			$datasql=$datasql.",'".$a[15]."','".$a[16]."'";
			$datasql=$datasql.",'".$a[17]."','".$a[18]."'";
			$datasql=$datasql.")";
			$gestcount++;
		}
	}
	$sql="INSERT INTO ".$gesttable." (";
	$sql=$sql."flag,tno,sno";
	$sql=$sql.",name1,name2,yomi,sama";
	$sql=$sql.",kind,skind,adrsno,adrs1";
	$sql=$sql.",adrs2,adrs3,telno";
	$sql=$sql.",kt1,kt2,kt3,kt4";
	$sql=$sql.",gno,sub1,gift,dish)";
	$sql=$sql." VALUES ".$datasql.";";
// echo $sql;
	$ret = mysql_query($sql, $mysql);
	if($ret == FALSE){
		mysql_close($mysql);
		fclose($fp);
		$retstr = "0,4,";
		echo $retstr;
		return;
	}

	$tablecount=0;
	$datasql = "";
	while(1){
		$str = fgets($fp);
		$str = mb_convert_encoding($str, 'UTF-8', 'sjis-win');
		$a = split(',',$str);
		if(strcmp($a[1],"end") == 0){
			break;
		}
		if($tablecount == 0){
			$datasql = "";
		}else{
			$datasql=$datasql.",";
		}
		$datasql = $datasql."(".$a[0].",".$a[1].")";
		$tablecount++;
	}
	$datasql = $datasql.",(0,0),(0,0),(0,0),(0,0),(0,0)";
	$layouttable = "tb".$konreino;
	$sql="INSERT INTO ".$layouttable." (dx,dy)";
	$sql=$sql." VALUES ".$datasql.";";
	$ret = mysql_query($sql, $mysql);
	if($ret == FALSE){
		mysql_close($mysql);
		fclose($fp);
		$retstr = "0,5,";
		echo $retstr;
		return;
	}

	$str = fgets($fp);
	$str = mb_convert_encoding($str, 'UTF-8', 'sjis-win');
	echo $str."<BR>";
	$a = split(',',$str);
	$strtable = "ss".$konreino;
	$sql="INSERT INTO ".$strtable." (leftstr,rightstr)";
	$sql=$sql." VALUES ('".$a[0]."','".$a[1]."');";
	echo $sql."<BR>";
	$ret = mysql_query($sql, $mysql);
	if($ret == FALSE){
		mysql_close($mysql);
		fclose($fp);
		$retstr = "0,6,";
		echo $retstr;
		return;
	}
	fclose($fp);
	mysql_close($mysql);
	$retstr = "1,".$gestcount.",".$tablecount.",";
}else{
	$retstr = "0,1,";
}
echo $retstr;
?>

