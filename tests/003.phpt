--TEST--
Check re-allocation
--SKIPIF--
<?php if (!extension_loaded("ocqprint")) print "skip"; ?>
--FILE--
<?php
echo qprint(str_repeat("æøå",30));
?>
--EXPECTF--
Warning: Had to re-allocate qprint buffer wrote %d bytes in %s.php on line %d
=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=
=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=
=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=
=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=
=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=
=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=
=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=A6=C3=B8=C3=A5=C3=
=A6=C3=B8=C3=A5
