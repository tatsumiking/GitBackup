<?php

$MAXIMUM_FILESIZE = 1024 * 1024 * 8; // 8M
$name=$_FILES['Filedata']['name'];
//mb_convert_variables('UTF-8', 'SJIS', $name);
$localname=$_FILES['Filedata']['tmp_name'];
if ($_FILES['Filedata']['size'] <= $MAXIMUM_FILESIZE){
	$argvs = $_POST['com'];
	//$argvs = "template/test.gif,";
	//$argvs = "figure/test.gif,";
	$a = explode(",", $argvs);
	$filename = $a[0];
	$dir=getcwd();
	$file=$dir."/".$filename;
	move_uploaded_file($_FILES['Filedata']['tmp_name'],$file);
	//Œ ŒÀÝ’è
	chmod($file, 0777);
}

?>

