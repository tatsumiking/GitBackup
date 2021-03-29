<?php
$argvs = $_POST['com'];
$ary = explode("|", $argvs);
$fileName = $ary[0] . "." . $ary[1];
$safix = $ary[1];
$img = $ary[2];
if($safix == "png"){
	$img = str_replace('data:image/png;base64,', '', $img);
}else{
	$img = str_replace('data:image/jpeg;base64,', '', $img);
}
$img = str_replace(' ', '+', $img);
$fileData = base64_decode($img);

$ret = file_put_contents($fileName, $fileData);
$ret = file_put_contents($fileName, $fileData);
if($ret == 0){
	$str = "comp=0,".$fileName.",";
}else{
	$str = "comp=1,".$fileName.",";
}
echo $str;
?>
