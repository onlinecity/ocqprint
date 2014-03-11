--TEST--
Check for output
--SKIPIF--
<?php if (!extension_loaded("ocqprint")) print "skip"; ?>
--FILE--
<?php
echo qprint("Hello æøå \n.foo.bar\naaa");
?>
--EXPECT--
Hello =C3=A6=C3=B8=C3=A5=20
=2Efoo.bar
aaa
