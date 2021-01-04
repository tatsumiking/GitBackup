<?php   

$filename = $_POST['file'];
$data = $_POST['data'];

//$filename = "user/gest.txt";
//$data = "2021/01/02 12:00,30.56";
mb_convert_variables('SJIS', 'UTF-8', $filename);
error_reporting(0);
$fp = fopen($filename, 'r+');
if($fp == 0){
	$fp = fopen($filename, 'w');
	if($fp == 0){
		$retstr = "0,1,";
		echo $retstr;
		return;	
	}
}else{
	fseek($fp, 0, 2);
}
$len=fwrite($fp,$data);
fwrite($fp,"\r\n");
fclose($fp);
$retstr = "1,".$len.",";
echo $retstr;
?> 
