<?php

$MAXIMUM_FILESIZE = 1024 * 1024 * 8; // 8M
$name=$_FILES['Filedata']['name'];
//mb_convert_variables('UTF-8', 'SJIS', $name);
$localname=$_FILES['Filedata']['tmp_name'];
if ($_FILES['Filedata']['size'] <= $MAXIMUM_FILESIZE){
	$argvs = $_POST['com'];
	//$argvs = "test.gif,";
	$a = explode(",", $argvs);
	$filename = $a[0];
	$dir=getcwd();
	$file=$dir."/yktemplate/".$filename;
	move_uploaded_file($_FILES['Filedata']['tmp_name'],$file);
	//�����ݒ�
	chmod($file, 0777);

	$soutfile=$dir."/yktemplate/S".$filename;
	$loutfile=$dir."/yktemplate/L".$filename;
	$routfile=$dir."/yktemplate/R".$filename;
	$im_inp = ImageCreateFromGif($file);
	$alpha = imagecolortransparent($im_inp);  // ���摜���瓧�ߐF���擾����
	$xsize = ImageSX($im_inp);    // �ǂݍ��񂾉摜�̉��T�C�Y���擾
	$ysize = ImageSY($im_inp);    // �ǂݍ��񂾉摜�̏c�T�C�Y���擾
	$xsize1 = $xsize / 2;
	$xsize2 = $xsize - $xsize1;

	$im_out = ImageCreateTrueColor($xsize1, $ysize);
	Imagefill($im_out, 0, 0, $alpha);
	Imagecolortransparent($im_out, $alpha);        
	ImageCopy($im_out, $im_inp, 0, 0, 0, 0, $xsize1, $ysize);
	ImageGif($im_out, $loutfile);
	imagedestroy($im_out);

	$im_out = ImageCreateTrueColor($xsize2, $ysize);
	Imagefill($im_out, 0, 0, $alpha);
	Imagecolortransparent($im_out, $alpha);
	ImageCopy($im_out, $im_inp, 0, 0, $xsize1, 0, $xsize2, $ysize);
	ImageGif($im_out, $routfile);
	imagedestroy($im_out);

	$ox = 900;
	$oy = 600;
	$im_out = ImageCreateTrueColor($ox, $oy);
	Imagefill($im_out, 0, 0, $alpha);
	Imagecolortransparent($im_out, $alpha);
	ImageCopyResized($im_out, $im_inp, 0, 0, 0, 0, $ox, $oy, $xsize, $ysize);
	ImageGif($im_out, $soutfile);
	imagedestroy($im_out);

	imagedestroy($im_inp);
}

?>

