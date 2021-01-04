<?php

$csvfilename = $_POST['file'];
$base64data = $_POST['data'];
$base64data = str_replace(' ' , '+' , $base64data);
$ary = split(',',$base64data);
$textdata = base64_decode($ary[1]);
$ret = file_put_contents($csvfilename, $textdata);
if($ret == 0){
	$str = "0,".$csvfilename.",";
}else{
	$str = "1,".$csvfilename.",";
	
}
chmod($csvfilename, 0777);
echo $str;

?>

