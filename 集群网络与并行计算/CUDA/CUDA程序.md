

### CUDA程序

##### 1.向量加实现 A+B

+ 代码名称：vector.cu
+ 运行代码：vector
+ 运行结果：自动生成2个30行的向量，并进行向量各个位置的加法运算
+ 运行结果如图：
+ <img src="C:\Users\12590\AppData\Roaming\Typora\typora-user-images\image-20210107163340447.png" alt="image-20210107163340447" style="zoom:80%;" />



##### 2.矩阵运算 D=A*B+C

+ 代码名称：matrix.cu
+ 运行代码：matrix
+ 运行结果：
+ + 首先生成矩阵ABC，这里取A矩阵的元素A[i * N+j]=i+j;B矩阵的元素为 B[i * N+j]=2*i+j；C矩阵的元素为C[i * N+j]=i + 3 *j
  + 然后进行矩阵运算最后得到最终的结果并打印
+ 运行结果如图：
+ <img src="C:\Users\12590\AppData\Roaming\Typora\typora-user-images\image-20210107164728157.png" alt="image-20210107164728157" style="zoom:80%;" />



##### 3.求解向量到矩阵每一行的欧式距离

+ 代码名称：kernel.cu
+ 运行代码：kernel
+ 运算结果：
+ + 生成一个6行的向量，向量值为vec[i]=i;
  + 生成一个矩阵，矩阵元素的值为mat[i * N + j]=i * N + j；
  + 通过运算得到结果
+ 运算结果如图：

<img src="C:\Users\12590\AppData\Roaming\Typora\typora-user-images\image-20210107164901172.png" alt="image-20210107164901172" style="zoom:80%;" />