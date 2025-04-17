# 1. 漫反射光照

法线是垂直于表面并向外的向量，它决定了表面如何反射光线。


![](https://easyimage.elyt.cn/i/2025/04/07/5721083269286468194-2.webp)  
圆环网格的法线---蓝色线段显示了每个顶点的法线方向。

目前我们已经知道网格中存储了网格中每个顶点的位置、颜色和纹理坐标信息 。在openFrameworks中，是在location=2中存储法线向量。以下是顶点着色器中关于法线的部分代码：
```cpp
layout(location =2) in vec3 normal; //法线
```
**注意**：不同的引擎和框架可能会以不同的方式存储法线。
## 1.1. 将法线向量做为颜色输出  
运行结果如下：  
![](https://easyimage.elyt.cn/i/2025/04/09/5721587870683109314-2.webp)  

### 1.1.1. 顶点着色器
```cpp
#version 410

layout (location = 0) in vec3 pos;
layout (location = 2) in vec3 nrm; //normal 法线
layout (location = 3) in vec2 uv;

uniform mat4 mvp;
out vec2 fragUV;
out vec3 fragNormal;
void main()
{
	gl_Position =  vec4( pos, 1.0);
	gl_Position = mvp * gl_Position;
	fragUV = vec2(uv.x, 1.0-uv.y);
	fragNormal = nrm;
}
```
顶点着色器中，我们把法线向量作为输出，输出到片段着色器中。
### 1.1.2. 片段着色器
```cpp
#version 410

in vec2 fragUV;
in vec3 fragNormal;
out vec4 outCol;

void main()
{
	//outCol = vec4(fragUV, 0.0, 1.0);
	vec3 normal=normalize(fragNormal);
	outCol = vec4(normal, 1.0);

}
```
由于从顶点着色器中输出的法线向量会经过插值传到片段着色器中，因此可能会造成片段着色器中得到的法向向量长度不一定为1，所以在片段着色器中需要对法线向量进行归一化处理，即调用函数 normalize()。所谓归一化，就是将向量除以向量的模，使得向量的模为1。归一化后的向量称为单位向量。
对于运行结果，我们可以看到，网格的某些区域为黑色，这是因为这些片元的法线向量分量都是负值，因为没有负的颜色值，所以这些片元的颜色值为黑色。

## 1.2. ply 文件格式解析
PLY（Polygon File Format）是一种用于存储三维物体的文件格式，它可以包含顶点、面等几何信息。下面我们逐行解释你提供的 PLY 文件内容：
### 1.2.1. 示例
```c++
ply
format ascii 1.0
comment Created in Blender version 4.3.2
element vertex 14
property float x
property float y
property float z
property float nx
property float ny
property float nz
property float s
property float t
element face 12
property list uchar uint vertex_indices
end_header
-0.5 0.5 0.5 -0.57732844 0.57736117 0.57736117 0.875 0.5
0.5 -0.5 0.5 0.57732844 -0.57736117 0.57736117 0.625 0.75
0.5 0.5 0.5 0.5773721 0.57733935 0.57733935 0.625 0.5
-0.5 -0.5 -0.5 -0.57732844 -0.57736117 -0.57736117 0.375 1
0.5 -0.5 -0.5 0.5773721 -0.57733935 -0.57733935 0.375 0.75
-0.5 -0.5 0.5 -0.5773721 -0.57733935 0.57733935 0.625 0
-0.5 0.5 -0.5 -0.5773721 0.57733935 -0.57733935 0.375 0.25
-0.5 -0.5 -0.5 -0.57732844 -0.57736117 -0.57736117 0.375 0
0.5 0.5 -0.5 0.57732844 0.57736117 -0.57736117 0.375 0.5
-0.5 -0.5 -0.5 -0.57732844 -0.57736117 -0.57736117 0.125 0.75
-0.5 0.5 -0.5 -0.5773721 0.57733935 -0.57733935 0.125 0.5
-0.5 0.5 0.5 -0.57732844 0.57736117 0.57736117 0.625 0.25
-0.5 -0.5 0.5 -0.5773721 -0.57733935 0.57733935 0.875 0.75
-0.5 -0.5 0.5 -0.5773721 -0.57733935 0.57733935 0.625 1
3 0 1 2
3 1 3 4
3 5 6 7
3 8 9 10
3 2 4 8
3 11 8 6
3 0 12 1
3 1 13 3
3 5 11 6
3 8 4 9
3 2 1 4
3 11 2 8

```


PLY（Polygon File Format）是一种用于存储三维物体的文件格式，它可以保存物体的顶点、面等几何信息。下面我们逐行解释你提供的PLY文件内容：

### 1.2.2. 文件头部分
```c++
ply
```
这是PLY文件的起始标识，表明该文件是一个PLY格式的文件。

```c++
format ascii 1.0
```
指定文件的格式为ASCII编码，版本为1.0。ASCII格式意味着文件中的数据以文本形式存储，易于人类阅读和编辑。

```c++
comment Created in Blender version 4.3.2
```
这是一个注释行，说明该PLY文件是在Blender 4.3.2版本中创建的。注释行用于提供额外的信息，不会影响文件的解析。

```c++
element vertex 14
```
定义了文件中顶点（vertex）元素的数量为14个。顶点是三维物体的基本组成部分，代表了物体表面上的点。

```c++
property float x
property float y
property float z
```
定义了每个顶点的位置属性，分别用`x`、`y`、`z`三个浮点数表示三维空间中的坐标。

```c++
property float nx
property float ny
property float nz
```
定义了每个顶点的法线属性，分别用`nx`、`ny`、`nz`三个浮点数表示顶点的法线向量。法线向量用于计算光照效果，确定顶点表面的朝向。

```c++
property float s
property float t
```
定义了每个顶点的纹理坐标属性，分别用`s`和`t`两个浮点数表示。纹理坐标用于将纹理映射到物体表面。

```c++
element face 12
```
定义了文件中面（face）元素的数量为12个。面是由多个顶点组成的多边形，通常用于表示物体的表面。

```c++
property list uchar uint vertex_indices
```
定义了每个面的属性，是一个列表，列表中的每个元素表示一个顶点的索引。`uchar`表示列表的长度使用无符号字符类型存储，`uint`表示顶点索引使用无符号整数类型存储。

```c++
end_header
```
标志着文件头部分的结束，接下来是具体的数据部分。

### 1.2.3. 数据部分
#### 1.2.3.1. 顶点数据
```c++
-0.5 0.5 0.5 -0.57732844 0.57736117 0.57736117 0.875 0.5
0.5 -0.5 0.5 0.57732844 -0.57736117 0.57736117 0.625 0.75
...
```
每行代表一个顶点的数据，按照文件头中定义的属性顺序排列，依次为`x`、`y`、`z`、`nx`、`ny`、`nz`、`s`、`t`。

#### 1.2.3.2. 面数据
```c++
3 0 1 2
3 1 3 4
...
```
每行代表一个面的数据，第一个数字表示该面所包含的顶点数量，后面的数字是顶点的索引，对应前面顶点数据中的行号（从0开始计数）。例如，`3 0 1 2`表示这个面由第0、1、2个顶点组成，是一个三角形面。

综上所述，这个PLY文件描述了一个包含14个顶点和12个面的三维物体，每个顶点包含位置、法线和纹理坐标信息，每个面由多个顶点组成。

## 1.3. 使用法线平滑(smooth)着色与平面(flat)着色
### 1.3.1. 平滑着色
当我们对立方体采用将法线向量做为颜色输出时，可以看到，立方体的同一个面的颜色有渐变，而不是纯色。

![](https://easyimage.elyt.cn/i/2025/04/10/5722091960374661758-2.webp)  

之所以不为纯色的原因是法线向量是经过插值的。下图是 立方体在blender 中的顶点的法线向量（图中蓝色细线）。  
![](https://easyimage.elyt.cn/i/2025/04/10/5722093420902617942-2.webp)  

如果立方体的顶点采用了复用方式，这样一个顶点上的法线向量就会被多个面共享，那么这些面的法线向量就会被插值，得到一个渐变的颜色。

### 1.3.2. 平面着色
在blender中，我们将立方体的每个面做分离，这样，顶点将不会被复用，在之前每个顶点的位置处将形成3个顶点，每个顶点有自己的法线向量。
这样，虽然还是会进行插值，但同一个面上每个片元的法线向量都是相同的，所以颜色也是相同的。
![](https://easyimage.elyt.cn/i/2025/04/10/5722112598309081593-2.webp)  

下图右边的立方体每个面都是纯色  
![](https://easyimage.elyt.cn/i/2025/04/10/5722113558381072930-2.webp)  


## 1.4. 法线矩阵  
法线矩阵（Normal Matrix）是用于将法线向量从模型空间（Model Space）变换到世界空间（World Space）的矩阵。
由于法线向量表示的是表面的方向，它的变换规则和普通的位置向量有所不同。当模型矩阵包含非均匀缩放时，直接使用模型矩阵变换法线向量会导致法线方向错误，因此需要使用法线矩阵来确保法线向量在变换后仍然垂直于物体表面。
法线矩阵是模型矩阵的逆转置矩阵的左上角 3x3 子矩阵。数学表达式如下：
$$ [ M_{normal} = (M_{model}^{-1})^T ] $$

在 openFrameworks 中，可以使用 glm 库中的函数来计算法线矩阵。以下是一个示例代码：

```c++
mat3 normalMatrix = transpose(inverse(mat3(model)));
```

## 1.5. 光线  
![](https://easyimage.elyt.cn/i/2025/04/12/5722728321322586333-2.webp)  
光线照射片元  

## 1.6. 点积  
点积是两个向量之间的一种乘法运算，其结果是一个标量（单一数值）。公式为：
$$
\mathbf{a} \cdot \mathbf{b} = |\mathbf{a}| |\mathbf{b}| \cos\theta
$$
其中：
- $|\mathbf{a}|$ 和 $|\mathbf{b}|$ 分别是向量的模（长度）。
- $\theta$ 是两个向量之间的夹角。

在二维或三维空间中，点积也可以用分量表示：
$$
\mathbf{a} \cdot \mathbf{b} = a_1 b_1 + a_2 b_2 + a_3 b_3
$$

**几何意义**：点积可以用来计算两个向量之间的夹角，或者判断两个向量是否正交（如果点积为0，则两向量正交）。

**应用**：
1. 计算投影长度。
2. 判断两个向量的方向关系。

当两个向量都是标准化的（模长为1），点积可以用来计算两个向量之间的夹角：
1. 如果点积为0，则两向量正交。
2. 如果点积为1，则两向量平行。
3. 如果点积为正值 ，则两向量夹角小于90度。
4. 如果点积为负值，则两向量夹角大于90度。


## 1.7. 点积着色  
我们将要创建的第一种光照类型称为漫反射光照。，其工作原理是将给定片元的法线与光线的方向进行比较，这两个向量间的夹角越小，光线照在网格曲面的这一点上就越垂直。


![](https://easyimage.elyt.cn/i/2025/04/12/5722730146381700135-2.webp)  
使用漫反射光照的效果 

为了方便计算 我们将灯光方向反转，即由片元指向光源。
![](https://easyimage.elyt.cn/i/2025/04/12/5722730637354340567-2.webp)  
另外，我们可能得到负的点积值，而颜色值不能为负，因此我们使用点积值与0.0的最大值来作为颜色值。
```c++
float dotProduct = max(dot(normal, lightDirection), 0.0);
```  

## 1.8. 第一个平行光  
![](https://easyimage.elyt.cn/i/2025/04/12/5722778087016371524-2.webp)  


### 1.8.1. ofApp.h
增加平行光结构体
```c++
struct DirectionalLightData {
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
};
```
### 1.8.2. ofApp.cpp
```c++

glm::vec3 ofApp::getLightDirection(const  DirectionalLightData & lightData) {
	using namespace glm;
	return normalize(lightData.direction*-1.0f); // 注意，这里的方向是反的，因为我们的光源是指向场景的
}

void ofApp::setup()
{

	ofDisableArbTex();
	ofEnableDepthTest();
	torusMesh.load("torus.ply");
	torusShader.load("mesh.vert", "diffuse.frag");

	directionalLightData.direction = glm::vec3(0.0f, -1.0f, 0.0f); // 向下的方向
	directionalLightData.color = glm::vec3(1.0f, 1.0f, 1.0f); // 白色
	directionalLightData.intensity = 1.0f; // 强度		
}
//--------------------------------------------------------------

void ofApp::draw()
{
	using namespace glm;
	cameraData.position=vec3(0, 0, 1);
	cameraData.fov=radians(90.f);
	float aspect=1024.0f/768.0f;

	//mat4 model=glm::rotate(1.0f,vec3(1,1,1))*scale(vec3(0.5,0.5,0.5));
	mat4 model = glm::rotate(glm::radians(-45.0f), vec3(1,0, 0)) * scale(vec3(0.5, 0.5, 0.5));
	//mat4 model = glm::mat4(1.0f)*scale(vec3(0.5, 0.5, 0.5));
	mat4 view=glm::inverse(glm::translate(cameraData.position));
	mat4 proj=glm::perspective(cameraData.fov,aspect,0.01f,100.0f);
	mat4 mvp=proj*view*model;

	vec3 lightDirection = getLightDirection(directionalLightData); // 获取光源方向


	mat3 normalMatrix = glm::transpose(glm::inverse(mat3(model)));
	torusShader.begin();
	torusShader.setUniformMatrix4f("mvp", mvp);
	torusShader.setUniformMatrix3f("normalMatrix", normalMatrix);

	torusShader.setUniform3f("lightDir", lightDirection);
	torusShader.setUniform3f("lightCol", directionalLightData.color*directionalLightData.intensity);

	torusShader.setUniform3f("meshCol", vec3(1,0,0));
	torusMesh.draw();
	torusShader.end();
}
```


### 1.8.3. mesh.vert
```c++
#version 410

layout (location = 0) in vec3 pos;
layout (location = 2) in vec3 nrm; //normal 法线
layout (location = 3) in vec2 uv;

uniform mat4 mvp;
uniform mat3 normalMatrix;
out vec2 fragUV;
out vec3 fragNormal;
void main()
{
	gl_Position =  vec4( pos, 1.0);
	gl_Position = mvp * gl_Position;
	fragUV = vec2(uv.x, 1.0-uv.y);
	fragNormal =normalMatrix * nrm;
}
```
### 1.8.4. diffuse.frag
```c++
#version 410


uniform vec3 lightDir; // light directio
uniform vec3 lightCol; // light color
uniform vec3 meshCol;	// mesh color


in vec2 fragUV;
in vec3 fragNormal;
out vec4 outCol;

void main()
{
	//outCol = vec4(fragUV, 0.0, 1.0);
	vec3 normal=normalize(fragNormal);

	float diffuse = max(0.0, dot(normal, lightDir));
	outCol = vec4(lightCol* diffuse * meshCol , 1.0);
	

}
```
核心代码如下：
即平行光的方向和片元的法线方向进行点积计算，得到一个标量值，这个值 表示了片元对光源的漫反射程度，然后乘以光源的颜色，再乘以片元的颜色，得到最终的颜色值。
```c++
float diffuse = max(0.0, dot(normal, lightDir));
outCol = vec4(lightCol* diffuse * meshCol, 1.0);
```

## 1.9. 创建轮廓光照效果
轮廓光照（Rim Lighting），也被称作边缘光照，常被用于突出物体的轮廓，让物体在场景里更具立体感与视觉吸引力。
轮廓光照的核心在于计算物体表面法线向量和相机视角向量之间的夹角。当物体表面的法线方向与相机视角方向接近垂直时，也就是物体的边缘部分，轮廓光照会产生较强的光照效果。
![](https://easyimage.elyt.cn/i/2025/04/12/5722803557774659611-2.webp)  

### 1.9.1. ofApp.cpp
重点增加与轮廓光照相关的代码：
```c++
	torusShader.setUniformMatrix4f("setUniformMatrix4f", model); // 模型矩阵
	torusShader.setUniform3f("rimCol", vec3(0,1,0)); //轮廓颜色
	torusShader.setUniform3f("cameraPos",cameraData.position); //相机位置

```c++
完整代码如下：	
void ofApp::draw()
{
	using namespace glm;
	cameraData.position=vec3(0, 0, 1);
	cameraData.fov=radians(90.f);
	float aspect=1024.0f/768.0f;

	//mat4 model=glm::rotate(1.0f,vec3(1,1,1))*scale(vec3(0.5,0.5,0.5));
	mat4 model = glm::rotate(glm::radians(-45.0f), vec3(1,0, 0)) * scale(vec3(0.5, 0.5, 0.5));
	//mat4 model = glm::mat4(1.0f)*scale(vec3(0.5, 0.5, 0.5));
	mat4 view=glm::inverse(glm::translate(cameraData.position));
	mat4 proj=glm::perspective(cameraData.fov,aspect,0.01f,100.0f);
	mat4 mvp=proj*view*model;

	vec3 lightDirection = getLightDirection(directionalLightData); // 获取光源方向


	mat3 normalMatrix = glm::transpose(glm::inverse(mat3(model)));
	torusShader.begin();
	torusShader.setUniformMatrix4f("mvp", mvp);
	torusShader.setUniformMatrix3f("normalMatrix", normalMatrix);

	torusShader.setUniform3f("lightDir", lightDirection);
	torusShader.setUniform3f("lightCol", directionalLightData.color*directionalLightData.intensity);

	torusShader.setUniform3f("meshCol", vec3(1,0,0));
	torusShader.setUniformMatrix4f("setUniformMatrix4f", model); // 模型矩阵
	torusShader.setUniform3f("rimCol", vec3(0,1,0)); //轮廓颜色
	torusShader.setUniform3f("cameraPos",cameraData.position); //相机位置

	torusMesh.draw();
	torusShader.end();
}
```

### 1.9.2. mesh.vert
核心代码如下：
```c++
fragWorldPos =( model * vec4( pos, 1.0)).xyz; //片元的世界坐标
```

完成代码如下：
```c++
#version 410

layout (location = 0) in vec3 pos;
layout (location = 2) in vec3 nrm; //normal 法线
layout (location = 3) in vec2 uv;

uniform mat4 mvp;
uniform mat3 normalMatrix;
uniform mat4 model; //模型矩阵
out vec2 fragUV;
out vec3 fragNormal;
out vec3 fragWorldPos; //片元的世界坐标
void main()
{
	gl_Position =  vec4( pos, 1.0);
	gl_Position = mvp * gl_Position;
	fragUV = vec2(uv.x, 1.0-uv.y);
	fragNormal =normalMatrix * nrm;
	fragWorldPos =( model * vec4( pos, 1.0)).xyz; //片元的世界坐标
}
```
### 1.9.3. diffuse.frag

核心代码如下：
```c++
	vec3 toCam=(normalize(cameraPos-fragWorldPos));
	float rimAmt=1.0-dot(normal,toCam);
	
	rimAmt=pow(rimAmt,3.0);
	
	vec3 rimLightCol=rimCol*rimAmt;

	outCol = vec4(lightCol* diffuse * meshCol +rimLightCol , 1.0);
```
在轮廓光照的计算里，rimAmt 代表轮廓光照的强度。rimAmt 的初始值是 1.0 - dot(normal, toCam)，也就是 1 减去法线向量和相机视角向量的点积。这个值越趋近于 1，表明片元越靠近物体的边缘；越趋近于 0，则说明片元越靠近物体的中心。 对 rimAmt 进行幂运算，能够调整轮廓光照的衰减速度。当使用 pow(rimAmt, 3.0) 时，会让边缘的光照更亮，而远离边缘的部分光照衰减得更快，从而让轮廓更加明显。下面通过不同次幂的效果对比来理解： • 一次方（无幂运算）：轮廓光照会比较平缓地从边缘过渡到中心。 • 三次方：边缘的光照强度会显著增强，而靠近中心的部分光照会迅速减弱，使得轮廓更加突出。

完整代码如下：

```c++
#version 410


uniform vec3 lightDir; // light directio
uniform vec3 lightCol; // light color
uniform vec3 meshCol;	// mesh color
uniform vec3 rimCol; // rim color
uniform  vec3 cameraPos; //相机的世界坐标


in vec2 fragUV;
in vec3 fragNormal;
out vec4 outCol;
in vec3 fragWorldPos; //片元的世界坐标

void main()
{
	//outCol = vec4(fragUV, 0.0, 1.0);
	vec3 normal=normalize(fragNormal);

	float diffuse = max(0.0, dot(normal, lightDir));

	vec3 toCam=(normalize(cameraPos-fragWorldPos));

	float rimAmt=1.0-dot(normal,toCam);
	//float rimAmt=dot(normal,toCam);
	rimAmt=pow(rimAmt,3.0);
	
	vec3 rimLightCol=rimCol*rimAmt;

	outCol = vec4(lightCol* diffuse * meshCol +rimLightCol , 1.0);
	

}
```

## 1.10. Phong光照

Phong光照模型是一种经典的光照模型，用于模拟真实世界中的光照效果，它将光照分为环境光（Ambient）、漫反射光（Diffuse）和镜面反射光（Specular）三个部分。最终的光照颜色是这三部分光照颜色的总和。下面详细介绍各部分的数学公式及整体公式。

### 1.10.1. 环境光（Ambient）
环境光模拟了场景中全局的、均匀的光照，它不依赖于光源的位置和物体的朝向。环境光的计算公式如下：
$$
I_{ambient} = k_a \times I_a
$$
其中：
- $I_{ambient}$ 是环境光的颜色强度。
- $k_a$ 是物体的环境光反射系数，取值范围通常在 $[0, 1]$ 之间，它表示物体对环境光的反射能力。
- $I_a$ 是环境光的颜色强度。

### 1.10.2. 漫反射光（Diffuse）
漫反射光模拟了光线在物体表面的均匀散射，它取决于光线的方向和物体表面法线的夹角。漫反射光的计算公式基于 Lambert 余弦定律：
$$
I_{diffuse} = k_d \times I_l \times \max(0, \mathbf{N} \cdot \mathbf{L})
$$
其中：
- $I_{diffuse}$ 是漫反射光的颜色强度。
- $k_d$ 是物体的漫反射系数，取值范围通常在 $[0, 1]$ 之间，它表示物体对漫反射光的反射能力。
- $I_l$ 是光源的颜色强度。
- $\mathbf{N}$ 是物体表面的法线向量，且为单位向量。
- $\mathbf{L}$ 是从物体表面指向光源的单位向量。
- $\mathbf{N} \cdot \mathbf{L}$ 是 $\mathbf{N}$ 和 $\mathbf{L}$ 的点积，$\max(0, \mathbf{N} \cdot \mathbf{L})$ 确保结果不会为负数。

### 1.10.3. 镜面反射光（Specular）
镜面反射光模拟了光线在物体表面的镜面反射效果，它取决于观察者的位置。镜面反射光的计算公式如下：
$$
I_{specular} = k_s \times I_l \times (\max(0, \mathbf{R} \cdot \mathbf{V}))^n
$$
其中：
- $I_{specular}$ 是镜面反射光的颜色强度。
- $k_s$ 是物体的镜面反射系数，取值范围通常在 $[0, 1]$ 之间，它表示物体对镜面反射光的反射能力。
- $I_l$ 是光源的颜色强度。
- $\mathbf{R}$ 是反射光线的单位向量，可以通过公式 $\mathbf{R} = 2(\mathbf{N} \cdot \mathbf{L})\mathbf{N} - \mathbf{L}$ 计算得到。
- $\mathbf{V}$ 是从物体表面指向观察者的单位向量。
- $n$ 是高光指数（Shininess），它控制了镜面反射的范围，值越大，高光越集中。

### 1.10.4. Phong光照模型整体公式
最终的光照颜色 $I_{total}$ 是环境光、漫反射光和镜面反射光的总和：
$$
I_{total} = I_{ambient} + I_{diffuse} + I_{specular}
$$
将前面的公式代入可得：
$$
I_{total} = k_a \times I_a + k_d \times I_l \times \max(0, \mathbf{N} \cdot \mathbf{L}) + k_s \times I_l \times (\max(0, \mathbf{R} \cdot \mathbf{V}))^n
$$

## 1.11. Blinn-Phong光照模型
Blin-Phong光照模型是对经典Phong光照模型的改进，它被提出主要是为了解决Phong模型在某些场景下存在的计算效率和视觉效果问题，下面从几个方面详细说明：

### 1.11.1. 计算效率更高
- **Phong模型的计算瓶颈**：在Phong光照模型中，计算镜面反射光时需要先计算反射光线向量 $\mathbf{R}$，其计算公式为 $\mathbf{R} = 2(\mathbf{N} \cdot \mathbf{L})\mathbf{N} - \mathbf{L}$，其中 $\mathbf{N}$ 是法线向量，$\mathbf{L}$ 是从物体表面指向光源的向量。之后还要计算反射光线向量 $\mathbf{R}$ 与从物体表面指向观察者的向量 $\mathbf{V}$ 的点积 $(\max(0, \mathbf{R} \cdot \mathbf{V}))^n$。计算反射光线向量 $\mathbf{R}$ 涉及多次向量运算，在实时渲染中，当场景中有大量物体或复杂光照时，会带来较大的计算开销。
- **Blin-Phong的优化**：Blin-Phong模型引入了半程向量 $\mathbf{H}$，它是光源方向向量 $\mathbf{L}$ 和观察方向向量 $\mathbf{V}$ 的中间向量，计算公式为 $\mathbf{H} = \frac{\mathbf{L} + \mathbf{V}}{\vert\mathbf{L} + \mathbf{V}\vert}$。然后直接计算半程向量 $\mathbf{H}$ 与法线向量 $\mathbf{N}$ 的点积 $(\max(0, \mathbf{N} \cdot \mathbf{H}))^n$ 来得到镜面反射效果。相比Phong模型，Blin-Phong模型避免了反射光线向量 $\mathbf{R}$ 的计算，减少了向量运算次数，从而提高了计算效率。

### 1.11.2. 视觉效果更符合预期
- **Phong模型的视觉缺陷**：在Phong模型中，当观察者与光源的夹角较大时，镜面反射高光区域可能会变得非常小甚至消失，导致视觉效果不够自然。
- **Blin-Phong的改进效果**：Blin-Phong模型使用半程向量来计算镜面反射，在很多情况下能产生更平滑、更明显的高光效果。半程向量的引入使得高光区域在不同观察角度下变化更加自然，即使观察者与光源夹角较大时，也能保持较好的视觉效果，更符合人眼对光照的感知。

### 1.11.3. 实时渲染的需求
在游戏开发、虚拟现实等实时渲染场景中，需要在保证视觉效果的前提下尽可能提高渲染效率。Blin-Phong光照模型在计算效率和视觉效果之间找到了较好的平衡，能够在有限的硬件资源下实现高质量的光照渲染，因此被广泛应用于实时渲染领域。

综上所述，Blin-Phong光照模型的出现是为了克服Phong模型的不足，满足实时渲染对效率和视觉效果的双重要求。 

### 1.11.4. Blinn-Phong 镜面反射光计算 
计算半程向量 $\mathbf{H}$，然后通过半程向量 $\mathbf{H}$ 与法线向量 $\mathbf{N}$ 的点积来计算镜面反射光。

- 计算半程向量：$\mathbf{H} = \frac{\mathbf{L} + \mathbf{V}}{\vert\mathbf{L} + \mathbf{V}\vert}$
- 计算镜面反射光：$I_{specular} = k_s \times I_l \times (\max(0, \mathbf{N} \cdot \mathbf{H}))^n$
  其中，$I_{specular}$ 是镜面反射光的颜色强度，$k_s$ 是物体的镜面反射系数，$n$ 是高光指数，控制镜面反射的范围。