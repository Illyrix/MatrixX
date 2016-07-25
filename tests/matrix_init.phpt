--TEST--
Check for MatrixX initialize and print out
--SKIPIF--
<?php if (!extension_loaded("MatrixX")) print "skip"; ?>
--FILE--
<?php 
$arr = [[3.25, 1.87,2.111],[1.2, 2,33.1]];
$rMatrix = matrix_init($arr);
echo matrix_rows($rMatrix) . matrix_cols($rMatrix) . "<br>";
matrix_dump($rMatrix);
$arr = [1,2,3,-3];
$rMatrix = matrix_init($arr);
echo matrix_rows($rMatrix) . matrix_cols($rMatrix) . "<br>";
matrix_dump($rMatrix);
$arr = [1,2.2,[3],[-3]];
$rMatrix = matrix_init($arr);
echo matrix_rows($rMatrix) . matrix_cols($rMatrix) . "<br>";
matrix_dump($rMatrix);
$arr = [[1],[2.2],[3],[-3]];
$rMatrix = matrix_init($arr);
echo matrix_rows($rMatrix) . matrix_cols($rMatrix) . "<br>";
matrix_dump($rMatrix);
?>
--EXPECT--
32<br><table>3.250000 1.870000 2.111000 <br>1.200000 2.000000 33.100000 <br></table>m:2<br>n:3<br>14<br><table>1.000000 <br>2.000000 <br>3.000000 <br>-3.000000 <br></table>m:4<br>n:1<br>14<br><table>1.000000 <br>2.200000 <br>3.000000 <br>-3.000000 <br></table>m:4<br>n:1<br>14<br><table>1.000000 <br>2.200000 <br>3.000000 <br>-3.000000 <br></table>m:4<br>n:1<br>