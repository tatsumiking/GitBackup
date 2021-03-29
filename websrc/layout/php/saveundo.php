<?php
$str = $_POST['com'];
$rec = $_POST['recno'];
$filename = 'photo/'.$rec.'/undo.txt';
$fp = fopen($filename, 'w');
fputs($fp, $str);
fclose($fp);
?>
