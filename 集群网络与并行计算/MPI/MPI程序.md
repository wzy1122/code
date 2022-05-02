### MPI程序

##### 1.  两个矩阵的乘积，如：A * B

+ 代码名称：juzhen.c
+ 运行代码：mpirun -np 4 ./juzhen
+ 运行结果：随机生成两个4*4的矩阵并显示，最后得到矩阵相乘的结果并显示
+ 实现效果如图：
+ <img src="C:\Users\12590\AppData\Roaming\Typora\typora-user-images\image-20210107104257466.png" alt="image-20210107104257466" style="zoom:80%;" />



##### 2.矩阵向量乘法，如： A * T

+ 代码名称：juzhen2.c
+ 运行代码：juzhen2
+ 运行结果：
+ + 输入矩阵A的行数和列数
  + 输入矩阵B的行数和列数
  + 输入矩阵A的元素，显示矩阵A
  + 输入矩阵B的元素，显示矩阵B
  + 得到最终的结果矩阵

+ 实现效果如图：

<img src="C:\Users\12590\AppData\Roaming\Typora\typora-user-images\image-20210107110431715.png" alt="image-20210107110431715" style="zoom:80%;" />



##### 3.Jacobi迭代

+ 代码名称：jacobi.c
+ 运行代码：mpirun -np 4 ./jacobi
+ 运行结果：
+ + 先生成一个2*8的矩阵，在以此矩阵根据每个元素都是其上下左右四个元素的平均值，矩阵四个边界的值不变的规则计算
  + 生成计算后的8*8的jacobi矩阵

+ 实现效果如图：
+ <img src="C:\Users\12590\AppData\Roaming\Typora\typora-user-images\image-20210107111122869.png" alt="image-20210107111122869" style="zoom:80%;" />