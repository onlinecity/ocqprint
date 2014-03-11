--TEST--
Check for QP edge cases
--SKIPIF--
<?php if (!extension_loaded("ocqprint")) print "skip"; ?>
--FILE--
<?php
$s = str_repeat("a",75)."."."eod";
var_dump($d = qprint($s));

$s = str_repeat("_",71)." \n"."eod";
var_dump($d = qprint($s));

$s = str_repeat("_",72)."\t\n"."eod";
var_dump($d = qprint($s));
?>
--EXPECTF--
string(%d) "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa=
=2Eeod"
string(%d) "_______________________________________________________________________=20
eod"
string(%d) "________________________________________________________________________=09
eod"
