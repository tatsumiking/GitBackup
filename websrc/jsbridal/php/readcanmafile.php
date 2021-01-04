<?php   
$filename = $_POST['file'];
error_reporting(0);
$fp = fopen($filename, 'r');
$retstr = "0,";
if($fp != 0){
	$retstr=fread($fp,1048576);
	fclose($fp);
	mb_convert_variables('UTF-8', 'SJIS', $retstr);
}
echo $retstr;
?> 
