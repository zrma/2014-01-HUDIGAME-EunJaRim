<?php
$connect = mysqli_connect("server!!!", "ID!!!!", "PASS!!!!") or die(mysqli_connect_error());

mysqli_select_db($connect,'DB_NAME!!!!!!') or die(mysqli_error());
//mysql_set_charset("utf8");
?>