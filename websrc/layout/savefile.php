<?php 
$filename = $_POST['file'];
$data = $_POST['data'];
mb_convert_variables('SJIS', 'UTF-8', $data);
$fp = fopen($filename, 'w');
if($fp != 0){
	$len = fwrite($fp, $data);
	fclose($fp);
}
$str = "comp=1,".$len.",";
echo $str;
?> 
