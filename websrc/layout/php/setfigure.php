<?php
$argvs = $_POST['com'];
//$argvs = "10,temp.gif,,";
//mb_convert_variables('UTF-8', 'SJIS', $argvs);
$a = explode(",", $argvs);
$setidx = $a[0];
$name = $a[1];
$path = getcwd();
$dstfilename = "list/figure.txt";
$srcfilename = "list/figure.bak";
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
	if($idx == $setidx){
		$setflag = 1;
		fputs($dstfp, $name);
		fputs($dstfp, "\r\n");
	}else{
		fputs($dstfp, $str);
	}
	$idx = $idx + 1;
}
if($setflag == 0){
	fputs($dstfp, $name);
	fputs($dstfp, "\r\n");
}

fclose($dstfp);
fclose($srcfp);
$str = "comp=1,,,";
echo $str;
?>
