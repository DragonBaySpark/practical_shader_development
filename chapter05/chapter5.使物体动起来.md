

# 1. 使物体动起来  
**运行结果**
![](https://easyimage.elyt.cn/i/2025/04/06/5720665042320366338-2.gif)    
能看到云朵在绕着自己的Z轴旋转。  
## 1.1. 网格的对象空间 
定义网格顶点的坐标系称为网格的对象空间。  
比如下图，定义了一个四边形，其顶点坐标见图中所示。  
![](https://easyimage.elyt.cn/i/2025/04/06/5720505969755035794-2.webp)  

其在世界空间中的坐标为：
![](https://easyimage.elyt.cn/i/2025/04/06/5720506749786526098-2.webp)  


## 1.2. 缩放 

缩放是指改变物体的大小。在计算机图形学中，我们可以使用矩阵来表示缩放。为了按我们预期的方式缩放物体，网格的中心需要是(0, 0, 0)。即**缩放矩阵是针对网格的中心点进行缩放的，而不是针对世界空间的原点进行缩放**。



### 1.2.1. 二维空间的缩放矩阵
在二维空间中，我们可以使用一个缩放矩阵来改变物体的大小。
缩放矩阵的形式为：
```c++
S = |sx 0|
    |0 sy|
```
其中，sx 和 sy 分别表示物体在 x 轴和 y 轴上的缩放比例。
对于任意点 A(x, y)，缩放后的坐标 A'(x', y') 可以通过矩阵乘法得到：
```c++
A' = S * A
```
展开后得到缩放公式：
```c++
x' = sx * x
y' = sy * y
```

### 1.2.2. 三维空间的缩放矩阵
在三维空间中，我们需要分别考虑物体在 x 轴、y 轴和 z 轴上的缩放。
缩放矩阵的形式为：
```c++
S = |sx  0   0   0 |
    |0   sy  0   0 |
    |0   0   sz  0 |
    |0   0   0   1 |
```
其中，sx、sy 和 sz 分别表示物体在 x 轴、y 轴和 z 轴上的缩放比例。
对于任意点 A(x, y, z)，缩放后的坐标 A'(x', y', z') 可以通过矩阵乘法得到：
```c++
A' = S * A
```
展开后得到缩放公式：
```c++
x' = sx * x
y' = sy * y
z' = sz * z
```


## 1.3. 旋转
旋转是指改变物体的方向。在计算机图形学中，我们可以使用矩阵来表示旋转。
旋转分为公转和自转。  
1. 公转是指物体绕着一个固定的点（如世界空间）旋转。  
2. 自转是指物体绕着自己的轴旋转。  


本文中说的旋转是自转，为了按我们预期的方式旋转物体，网格的中心需要是(0, 0, 0)。即**旋转矩阵是针对网格的中心点进行旋转的，而不是针对世界空间的原点进行旋转的**。

### 1.3.1. 二维空间的旋转矩阵

从向量角度看:
![](https://easyimage.elyt.cn/i/2025/04/01/5718675402524729065-2.webp)  

点A可以理解为向量A1+A2,其中 A1 为(x,0),A2 为(0,y)。
将坐标轴旋转θ角度后，点A'的坐标为(x',y')。

![](https://easyimage.elyt.cn/i/2025/04/01/5718679985468743464-2.webp)
将坐标轴以逆时针旋转θ角度后，点A'的坐标为(x',y')。
点A'可以理解为向量A1'+A2',其中
```c++
A1' = (x*cos(θ),x*sin(θ))
A2' = (-y*sin(θ),y*cos(θ))
```
**注意：从图中也可以看出A2'的x坐标为 -y*sin(θ) **

A'=A1'+A2' ,所以
```c++
x' = x * cos(θ) - y * sin(θ)
y' = x * sin(θ) + y * cos(θ)
```  
在二维空间中，旋转矩阵可以表示为：

$R(\theta) = \begin{pmatrix} 
\cos\theta & -\sin\theta \\
\sin\theta & \cos\theta
\end{pmatrix}$

对于任意点 A(x, y)，旋转后的坐标 A'(x', y') 可以通过矩阵乘法得到：

$\begin{pmatrix} 
x' \\
y'
\end{pmatrix} = 
\begin{pmatrix} 
\cos\theta & -\sin\theta \\
\sin\theta & \cos\theta
\end{pmatrix}
\begin{pmatrix} 
x \\
y
\end{pmatrix}$

展开后得到旋转公式：
```c++
x' = x * cos(θ) - y * sin(θ)
y' = x * sin(θ) + y * cos(θ)
```
### 1.3.2. 三维空间的旋转矩阵

在三维空间中，我们需要分别考虑绕 X 轴、Y 轴和 Z 轴的旋转。每个轴的旋转都有其对应的旋转矩阵：

绕 X 轴旋转 θ 角度的矩阵：

$R_x(\theta) = \begin{pmatrix} 
1 & 0 & 0 & 0 \\
0 & \cos\theta & -\sin\theta & 0 \\
0 & \sin\theta & \cos\theta & 0 \\
0 & 0 & 0 & 1
\end{pmatrix}$

绕 Y 轴旋转 θ 角度的矩阵：

$R_y(\theta) = \begin{pmatrix}
\cos\theta & 0 & \sin\theta & 0 \\
0 & 1 & 0 & 0 \\
-\sin\theta & 0 & \cos\theta & 0 \\
0 & 0 & 0 & 1
\end{pmatrix}$

绕 Z 轴旋转 θ 角度的矩阵：

$R_z(\theta) = \begin{pmatrix}
\cos\theta & -\sin\theta & 0 & 0 \\
\sin\theta & \cos\theta & 0 & 0 \\
0 & 0 & 1 & 0 \\
0 & 0 & 0 & 1
\end{pmatrix}$

对于任意点 P(x, y, z)，旋转后的坐标 P'(x', y', z') 可以通过矩阵乘法得到：

$P' = R \cdot P$

如果需要进行多个轴的组合旋转，最终的旋转矩阵是各个轴向旋转矩阵的乘积。注意矩阵乘法的顺序会影响最终结果：

$R_{total} = R_z \cdot R_y \cdot R_x$

在实际应用中，我们通常会使用四阶矩阵（4x4）来表示这些变换，以便与其他变换（如平移、缩放）进行组合。

## 1.4. 平移 
对于在三维空间中平移矩阵 $T$ 可以表示为：
```c++
| 1  0  0  dx |
| 0  1  0  dy |
| 0  0  1  dz |
| 0  0  0  1  |
```
其中，$dx$、$dy$ 和 $dz$ 分别表示沿 $x$ 轴、$y$ 轴和 $z$ 轴的平移距离。
对于任意点 $P(x, y, z)$，平移后的坐标 $P'(x', y', z')$ 可以通过矩阵乘法得到：

$$P' = T \cdot P$$

展开后得到平移公式：
```c++
x' = x + dx
y' = y + dy
z' = z + dz
```

## 1.5. 组合变换
在计算机图形学中，我们经常需要将多个变换组合在一起，以创建更复杂的变换效果。例如，我们可以将缩放、旋转和平移组合在一起，以创建更复杂的变换效果。
其处理过程为：**先进行缩放，再进行旋转，最后进行平移**  

由于矩阵乘法不满足交换律，所以变换的顺序十分关键。按照“先缩放，再旋转，最后平移”的顺序，最终的变换矩阵 M 为：

```c++
M = T * R * S
```

其中，T 是平移矩阵，R 是旋转矩阵，S 是缩放矩阵。
## 1.6. ofApp.cpp
```c++
void ofApp::draw()
{
    //...省略其他代码
	glm::mat4 T = glm::translate(glm::mat4(1.0), glm::vec3(-0.55f, 0.0, 0.0));
	glm::mat4 R = glm::rotate(glm::mat4(1.0), (float)(ofGetElapsedTimef()*10.0*glm::pi<float>()/180.0), glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 S = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 1.0));
	glm::mat4 M = T * R * S;
	cloudShader.begin();
	cloudShader.setUniformTexture("tex", cloudImg, 0);
	cloudShader.setUniformMatrix4f("model",  M);
	cloudMesh.draw();
	cloudShader.end();

    //...省略其他代码
}
```
## 1.7. cloud.vert
```c++
#version 410

layout (location = 0) in vec3 pos;
layout (location = 3) in vec2 uv;

uniform mat4 model;
out vec2 fragUV;

void main()
{
	gl_Position =  vec4( pos, 1.0);
	gl_Position = model * gl_Position;
	fragUV = vec2(uv.x, 1.0-uv.y);
}

```
**重点**
1. 在cpp中，我们使用 glm::translate、glm::rotate 和 glm::scale 函数来创建平移、旋转和缩放矩阵，并将它们组合在一起形成最终的变换矩阵 M。
2. 在vert中，我们将最终的变换矩阵 M 传递给 shader 中的 mvp 变量。
3. 在 shader 中，我们使用 mvp 变量来对顶点坐标进行变换。
4. 在 shader 中，我们使用 gl_Position 变量来存储变换后的顶点坐标。


## 1.8. 参考 
1. https://blog.csdn.net/zhangxiao13627093203/article/details/81204856