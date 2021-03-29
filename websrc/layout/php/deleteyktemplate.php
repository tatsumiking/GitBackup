<?php
$argvs = $_POST['com'];
//$argvs = "12,0013.gif,";
//mb_convert_variables('UTF-8', 'SJIS', $argvs);
$a = explode(",", $argvs);
$delidx = $a[0];
$delname = $a[1];
$path = getcwd();
$filename = $path."/yktemplate/".$delname;
unlink($filename);
$filename = $path."/yktemplate/S".$delname;
unlink($filename);
$filename = $path."/yktemplate/R".$delname;
unlink($filename);
$filename = $path."/yktemplate/L".$delname;
unlink($filename);

$dstfilename = "list/yktemplate.txt";
$srcfilename = "list/yktemplate.bak";
copy($dstfilename, $srcfilename);
$srcfp = fopen($srcfilename, 'r');
$dstfp = fopen($dstfilename, 'w');
$idx = 0;
$setflag = 0;
while(1){
	$str = fgets($srcfp, 256);
	if($str == ""){
		break;
	}
	if($idx != $delidx){
		fputs($dstfp, $str);
	}
	$idx = $idx + 1;
}
fclose($dstfp);
fclose($srcfp);
$str1 = "comp=1,,";
echo $str1;
?>
