<?php
$file="../photo/2/photo.jpg";
chmod($file, 0777);
$ret = unlink($file);
echo $file." ".$ret;
?>
