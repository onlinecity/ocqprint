--TEST--
Check for ocqprint presence
--SKIPIF--
<?php if (!extension_loaded("ocqprint")) print "skip"; ?>
--FILE--
<?php
echo "ocqprint extension is available";
?>
--EXPECT--
ocqprint extension is available
