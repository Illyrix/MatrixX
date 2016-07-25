## MatrixX
---------------------------
MatrixX is a simple extension for PHP to help you do operation to matrix, based on PHP 5.6.12.

### Install
* On Windows:  
Create a new project in Visual Studio to generate a dll. Import source code into the project, and compile.
If you have set correctly, put the generated dll into `Path/To/PHP/Installed/ext`. Then add `extension=php_MatrixX.dll` in `php.ini`. Restart your web server (or php-fpm).
* On Linux:
Use `phpize` to make and install a file with a .so suffix. As the same as on Windows, enable extension in `php.ini`.  
**Notice:** It has NOT been test on Linux.

### Document

* `matrix_init`

MatrixX provide a resource type to save matrix. To create it, you have to use `matrix_init()` to initialize one.
```
$arr = [[3.25, 1.87, 2.111], [1.2, 2 ,33.1]];
$rMatrix = matrix_init($arr);
matrix_dump($rMatrix);
```
`matrix_init()` receive one param, should be an array or another matrix resource. If the param was an array, make sure the dimension of it is 1 or 2.  
This function will return a resource if input is valid, and `false` if invalid.

* `matrix_dump`

`matrix_dump()` will print out the information of input resource, include columns, rows and the matrix.  
The example above will print on sreen like this:
>3.250000 1.870000 2.111000  
1.200000 2.000000 33.100000  
m:2  
n:3  

`matrix_dump()` need a resource as param, and return false for invalid input, true for valid.

* `matrix_rows`

`matrix_rows()` will return the rows of matrix in resource input, or false if input is invalid.

* `matrix_cols`

This function is  similar to `matrix_rows()`, the different is it returns the columns.
```
$arr = [[3.25, 1.87,2.111],[1.2, 2,33.1]];
$rMatrix = matrix_init($arr);
echo "rMat.rows:" . matrix_rows($rMatrix) . " " .
    "rMat.cols:" . matrix_cols($rMatrix) . PHP_EOL;
```
Output looks like this:
>rMat.rows:3 rMat.cols:2

* `matrix_current`

It will return an array with the same structure and elements of the matrix.  
Based on the array above:
```
var_dump(matrix_current($rMatrix));
```
And ouptut is:
>array (size=2)  
  0 =>   
    array (size=3)  
      0 => float 3.25  
      1 => float 1.87  
      2 => float 2.111  
  1 =>   
    array (size=3)  
      0 => float 1.2  
      1 => float 2  
      2 => float 33.1  

* `matrix_destroy`

It will release the resource. A resource as the param is necessary.  
**Notice:** You should not destroy one resource twice.

* `matrix_add`

2 params need, they could be one is real number, and other one is matrix resource; or both resources. 
It will return a matrix which is the addition of inputs. Especially if one of params is a real number, the result is all elements in other matrix will add this real number.
```
$arr = [[3.25, 1.87,2.111],[1.2, 2,33.1]];
$rMatrix = matrix_init($arr);
matrix_dump(matrix_add($rMatrix, 10))
```
This will have output:
>13.250000 11.870000 12.111000  
11.200000 12.000000 43.100000  
m:2  
n:3

**Notice:** Do NOT try to add two matrices with different columns or rows. It will cause a warning and return `false`.

* `matrix_subtract`

The same requirement as `matrix_add()`. But the return value is the remainder.

* `matrix_product`

It can do product of 2 matrices (if they can) or one matrix and a real number.
```
$arr1 = [[3.25, 1.87,2.111],[1.2, 2,33.1]];
$arr2 = [[2.00,0.3],[3.1,100],[1,12]];
$rMatrix1 = matrix_init($arr1);
$rMatrix2 = matrix_init($arr2);
matrix_dump(matrix_product($rMatrix1, $rMatrix2));
```
The return value is the product.
>6.860000 4.340000 14.152000  
130.075000 205.797000 3316.544100  
17.650000 25.870000 399.311000  
m:3  
n:3

**NOTICE:** If the 2 input matrices do NOT fit the condition of doing product, a warning will be sent out and return `false`.

### Support
Other functions will be add in future.