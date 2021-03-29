<?php
$MAXIMUM_FILESIZE = 1024 * 1024 * 8; // 8M
$name=$_FILES['Filedata']['name'];
//mb_convert_variables('UTF-8', 'SJIS', $name);
$localname=$_FILES['Filedata']['tmp_name'];
if($_FILES['Filedata']['size'] <= $MAXIMUM_FILESIZE){
	$argvs = $_POST['com'];
	//$argvs = "2,,";
	$a = explode(",", $argvs);
	$folder = $a[0];
	$path = "photo/".$folder;
	$ret=mkdir($path, 0777, true);
	$dir=getcwd();
	$file=$dir."/".$path."/".$name;
	move_uploaded_file($localname,$file);
	//Œ ŒÀÝ’è
	chmod($file, 0777);
}
?>
