# 1. 10 法线贴图

## 1.1. 什么是法线贴图  
我们如果想要在盾牌上实现凹凸感，应该如何做？一种方法是添加更多的顶点来建模更多的细节，但是网格的顶点越多，渲染网格所需的顶点着色器计算就越多，网格占用的内存就越多。大多数游戏不能仅仅因为凹凸和刮痕而为每个物体添加数千个顶点。
![](https://easyimage.elyt.cn/i/2025/04/14/5723626522628071195-2.webp)  

因此，我们用法线贴图来解决这个问题。法线贴图是使用一张纹理来存储法线信息，然后通过纹理采样来得到法线信息，最后将法线信息应用到物体上。

![](https://easyimage.elyt.cn/i/2025/04/14/5723627897797742970-2.webp)  

法线贴图是在每一个像素中存储归一化的向量而不是颜色的纹理。由于不存在负颜色，因此纹理无法表示小于0的值 。为了解决这个问题，我们需要将法线从$[-1,1]$映射到$[0,1]$。 

以下是一个法线贴图的例子：  
![](https://easyimage.elyt.cn/i/2025/04/14/5723629755752782573-2.webp)  
会看到其偏蓝色，其原因为：


### 1.1.1. 法线向量的表示
在三维空间里，法线向量是一个三维向量，通常用 $(x, y, z)$ 来表示，其值的范围是 $[-1, 1]$。这个向量定义了表面上某点的朝向，在光照计算里起着关键作用。

### 1.1.2. 纹理存储限制
纹理（如法线贴图）一般用来存储颜色信息，颜色分量（红、绿、蓝）的取值范围是 $[0, 1]$，没办法直接存储负数。所以，要把范围在 $[-1, 1]$ 的法线向量映射到 $[0, 1]$ 这个范围，才能存进纹理里。

### 1.1.3. 映射规则
映射的公式为：
$$
\text{mappedValue} = \frac{\text{originalValue} + 1}{2}
$$

对于一个没有任何倾斜，垂直于表面的法线向量，其值是 $(0, 0, 1)$。按照上述映射公式进行转换：

- 红色分量：$x = 0$，映射后的值为 $\frac{0 + 1}{2} = 0.5$
- 绿色分量：$y = 0$，映射后的值为 $\frac{0 + 1}{2} = 0.5$
- 蓝色分量：$z = 1$，映射后的值为 $\frac{1 + 1}{2} = 1$
转换后的颜色值是 $(0.5, 0.5, 1)$，在 RGB 颜色空间里，这个颜色偏向蓝色，这是因为蓝色分量的值最大，达到了 1。

### 1.1.4. 切线空间  
法线贴图使用一个称为切线空间的坐标空间，它是相对于网格曲面定义的。对于任何给定点，切线空间中的Z轴指向网格几何体法线向量的方向，这意味着存储的每个法线向量都将具有正的Z值 ，因此，每一像素的蓝色值都大于0.5。因而会更偏向蓝色。

切线空间是一个局部的三维坐标空间，它是相对于模型表面上的每一个点定义的。在这个空间中，每个点都有一组相互垂直的基向量，分别是切线向量（Tangent）、副切线向量（Bitangent，也叫副法线）和法线向量（Normal），通常用 T 、 B 、 N 表示，这三个向量构成了一个右手坐标系。

- 法线向量（N） ：垂直于模型表面，指向表面的外侧，定义了表面的朝向。
- 切线向量（T） ：沿着纹理坐标 U 轴的方向，位于表面的切平面上。
- 副切线向量（B） ：沿着纹理坐标 V 轴的方向，也位于表面的切平面上，并且与切线向量和法线向量都垂直。

![](https://easyimage.elyt.cn/i/2025/04/15/5723763751191056717-2.webp)  
### 1.1.5. 作用
1. 可复用性 ：切线空间中的法线贴图可以应用到不同的模型上。因为切线空间是基于每个模型表面点的局部空间，所以同一张法线贴图可以在不同的模型上产生合理的光照效果，而不需要为每个模型单独创建法线贴图。
2. 方便光照计算 ：在进行光照计算时，将光照向量和视线向量转换到切线空间中，可以简化计算过程。因为在切线空间中，法线向量的方向是固定的（通常是 (0, 0, 1) ），这样可以更方便地进行点积等运算。
3. 支持模型变形 ：当模型进行变形（如骨骼动画）时，切线空间中的法线贴图仍然可以正确地计算光照效果。因为切线空间是随着模型表面的变化而变化的。

## 1.2. 叉积  
副切线向量是一个同时垂直于切线向量和法线向量的向量，它的计算方式是通过对切线向量和法线向量进行叉积得到的。
![](https://easyimage.elyt.cn/i/2025/04/15/5723767355088051478-2.webp)  

如果我们计算左图中两个向量的叉积，得到的向量就是右图中绿色向量（竖直向上的向量）。

GLSL 中的叉积函数是 `cross`，其语法如下：
```glsl
vec3 cross(vec3 a, vec3 b);
```
## 1.3. 法线贴图的工作原理
法线贴图的工作原理是将法线信息存储在一张纹理中，然后通过纹理采样来得到法线信息，最后将法线信息应用到物体上。
从法线贴图中采样法线信息的过程可以分为以下几个步骤：
1. 纹理采样：首先，我们需要根据当前顶点的纹理坐标，从法线贴图中采样出一个颜色值。这个颜色值其范围是 $[0, 1]$。
2. 颜色到法线的转换：由于法线贴图存储的是归一化的向量，所以我们需要将采样出的颜色值转换回一个归一化的向量。这个转换的过程通常是将颜色值的每个分量从 $[0, 1]$ 映射到 $[-1, 1]$。
3. 切线空间转换：由于法线贴图是在切线空间中定义的，所以我们需要将采样出的法线向量转换到模型的切线空间中。这个转换的过程通常是将切线空间中的法线向量与切线空间中的副切线向量和切线空间中的法线向量进行叉积得到的。
4. 法线贴图应用：最后，我们将转换后的法线向量应用到物体上，从而实现凹凸感。这个过程通常是将法线向量与光照向量进行点积，得到一个光照强度值，然后根据这个光照强度值来计算物体的光照效果。

### 1.3.1. TBN矩阵
TBN 矩阵是一个 3x3 的矩阵，它由切线向量、副切线向量和法线向量组成。TBN 矩阵的作用是将法线向量从模型空间转换到切线空间中，从而实现法线贴图的应用。TBN 矩阵可以通过以下公式计算得到：
$$
TBN = \begin{bmatrix}
T_x & B_x & N_x \\
T_y & B_y & N_y \\
T_z & B_z & N_z
\end{bmatrix}
$$
其中，$T_x, T_y, T_z$ 分别是切线向量的 x、y、z 分量，$B_x, B_y, B_z$ 分别是副切线向量的 x、y、z 分量，$N_x, N_y, N_z$ 分别是法线向量的 x、y、z 分量。

openframeworks 默认不支持切线向量，我们采用折中的办法，将切线向量存贮在网格的顶点属性中。在计算 TBN 矩阵时，我们使用顶点属性中的切线向量来计算 TBN 矩阵。
```cpp
layout (location =0) in vec3 pos;
//原本location=1的位置是颜色 ，我们将它改成了切线向量
layout (location =1) in vec4 tan;
layout (location =2) in vec3 nrm;
layout (location =3) in vec2 uv;
```
其中，`pos` 是顶点的位置，`tan` 是顶点的切线向量，`nrm` 是顶点的法线向量，`uv` 是顶点的纹理坐标。
#### 1.3.1.1. 计算 TBN 矩阵
```cpp
//计算切线向量
vec3 T = normalize(normal * tan.xyz, 0.0);
//计算副切线向量
vec3 B = normalize(normal*cross(tan.xyz,nrm) );
//计算法线向量
vec3 N = normalize(normal * nrm);

mat3 TBN = mat3(T, B, N);
```
其中，`normal` 是法线矩阵，`tan.xyz` 是顶点的切线向量。

## 1.4. 法线贴图的应用（带凹凸的盾牌）  
![](https://easyimage.elyt.cn/i/2025/04/16/5724302609351711523-2.webp)  
上图中左图是使用法线贴图之前的效果，右边未使用法线贴图。可以看到，左图使用法线贴图之后，盾牌的凹凸感更加明显，光照效果也更加真实。

### 1.4.1. ofApp.cpp

#### 1.4.1.1. 计算切线向量
以下是计算切线向量的代码，其中 `mesh` 是要计算切线向量的网格。
```cpp
void calcTangents(ofMesh& mesh)
{
	using namespace glm;
	std::vector<vec4> tangents;
	tangents.resize(mesh.getNumVertices());

	uint indexCount = mesh.getNumIndices();

	const vec3* vertices = mesh.getVerticesPointer();
	const vec2* uvs = mesh.getTexCoordsPointer();
	const uint* indices = mesh.getIndexPointer();

	for (uint i = 0; i < indexCount - 2; i += 3)
	{
		const vec3& v0 = vertices[indices[i]];
		const vec3& v1 = vertices[indices[i + 1]];
		const vec3& v2 = vertices[indices[i + 2]];
		const vec2& uv0 = uvs[indices[i]];
		const vec2& uv1 = uvs[indices[i + 1]];
		const vec2& uv2 = uvs[indices[i + 2]];

		vec3 edge1 = v1 - v0;
		vec3 edge2 = v2 - v0;
		vec2 dUV1 = uv1 - uv0;
		vec2 dUV2 = uv2 - uv0;

		float f = 1.0f / (dUV1.x * dUV2.y - dUV2.x * dUV1.y);

		vec4 tan;
		tan.x = f * (dUV2.y * edge1.x - dUV1.y * edge2.x);
		tan.y = f * (dUV2.y * edge1.y - dUV1.y * edge2.y);
		tan.z = f * (dUV2.y * edge1.z - dUV1.y * edge2.z);
		tan.w = 0;
		tan = normalize(tan);

		tangents[indices[i]] += (tan);
		tangents[indices[i + 1]] += (tan);
		tangents[indices[i + 2]] += (tan);
	}

	int numColors = mesh.getNumColors();

	for (int i = 0; i < tangents.size(); ++i)
	{
		vec3 t = normalize(tangents[i]);
		if (i >= numColors)
		{
			mesh.addColor(ofFloatColor(t.x, t.y, t.z, 0.0));
		}
		else
		{
			mesh.setColor(i, ofFloatColor(t.x, t.y, t.z, 0.0));
		}
	}
}
```
#### 1.4.1.2. setup()
1. 调用 `calcTangents` 函数计算切线向量。
2. 加载法线贴图。
```cpp
void ofApp::setup()
{

	ofDisableArbTex();
	ofEnableDepthTest();
	shieldMesh.load("shield.ply");
	calcTangents(shieldMesh); // 计算切线
	shieldShader.load("mesh.vert", "diffuse.frag");

	diffuseImage.load("shield_diffuse.png");
	specImage.load("shield_spec.png");
	normalImage.load("shield_normal.png"); // 法线贴图

	directionalLightData.direction = glm::vec3(1.0f, -1.0f, 0.0f); 
	directionalLightData.color = glm::vec3(1.0f, 1.0f, 1.0f); // 白色
	directionalLightData.intensity = 1.0f; // 强度		
}
```

#### 1.4.1.3. draw()
1. 设置光照参数。
2. 绘制网格。
```cpp
void ofApp::draw()
{
    //其它代码
    shieldShader.setUniformTexture("normalTex", normalImage.getTexture(), 2); // 纹理
    //其它代码
    
}
```
### 1.4.2. mesh.vert
计算TBN矩阵，将TBN矩阵传递给片元着色器。
```cpp
#version 410

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 tan; //tangent 切线 ,借用颜色通道
layout (location = 2) in vec3 nrm; //normal 法线
layout (location = 3) in vec2 uv;

uniform mat4 mvp;
uniform mat3 normalMatrix;
uniform mat4 model; //模型矩阵
out vec2 fragUV;
out vec3 fragNormal;
out vec3 fragWorldPos; //片元的世界坐标

out mat3 TBN; // 切线空间矩阵，用于法线贴图
void main()
{
	gl_Position =  vec4( pos, 1.0);
	gl_Position = mvp * gl_Position;
	fragUV = vec2(uv.x, 1.0-uv.y);
	fragNormal =normalMatrix * nrm;
	fragWorldPos =( model * vec4( pos, 1.0)).xyz; //片元的世界坐标

	vec3 T = normalize(normalMatrix * tan); //切线
	vec3 N = normalize(normalMatrix * nrm); //法线
	vec3 B = cross(N, T); //副切线
	TBN = mat3(T, B, N);
}
```
### 1.4.3. diffuse.frag
**重要代码：**
1. 从法线贴图中采样法线信息。
2. 将法线信息从 $[0, 1]$ 映射到 $[-1, 1]$。
3. TBN与法线向量相乘以得到最终片元使用的法线向量，即从切线空间转换到世界空间。
```cpp
#version 410


uniform vec3 lightDir; // light directio
uniform vec3 lightCol; // light color
//uniform vec3 meshCol;	// mesh color
//uniform vec3 meshSpecCol; // mesh specular color
uniform vec3 rimCol; // rim color
uniform  vec3 cameraPos; //相机的世界坐标
uniform vec3 ambientCol; //环境光
uniform sampler2D diffuseTex; //diffuse texture
uniform sampler2D specularTex; //specular texture
uniform sampler2D normalTex; 


in vec2 fragUV;
in vec3 fragNormal;
out vec4 outCol;
in vec3 fragWorldPos; //片元的世界坐标
in mat3 TBN; // 切线空间矩阵，用于法线贴图


void main()
{
	//outCol = vec4(fragUV, 0.0, 1.0);
	//vec3 normal=normalize(fragNormal);
	vec3 normalTexture=texture(normalTex,fragUV).rgb;
	vec3 normal=normalize(normalTexture*2.0-1.0);
	normal=normalize(TBN*normal);
	

	float diffuse = max(0.0, dot(normal, lightDir));
	vec3 meshCol=texture(diffuseTex,fragUV).rgb;
	vec3 meshSpecCol=texture(specularTex,fragUV).rgb;
	vec3 diffuseCol=meshCol*lightCol*diffuse;


	vec3 toCam=(normalize(cameraPos-fragWorldPos));

	//vec3 reflectDir=normalize(reflect(-lightDir,normal));

	//float specAmt=max(0.0,dot(reflectDir,toCam));
	vec3 halfDir=normalize(lightDir+toCam);
	float specAmt=max(0.0,dot(halfDir,normal));
	specAmt=pow(specAmt,2.0f);
	
	vec3 specCol=meshSpecCol*lightCol*specAmt;

	vec3 ambient=ambientCol*meshCol;

	//outCol = vec4(lightCol* diffuse * meshCol , 1.0);
	outCol = vec4(ambient + diffuseCol + specCol, 1.0);
	

}
```
## 1.5. 水面
![](https://easyimage.elyt.cn/i/2025/04/16/5724333101128095466-2.webp)  

下图是动图，受制于网络，gif图效果较差。建议实际运行查看。
![](https://easyimage.elyt.cn/i/2025/04/16/5724334354004449859-2.gif)

图中能看到水面及反光效果，这是通过法线贴图实现的。
下图为法线贴图  
![](https://easyimage.elyt.cn/i/2025/04/16/5724349610290320833-2.webp)  
核心代码如下：

### 1.5.1. water.vert
```cpp
	float t=time*0.05;
	float t2=time*0.02;
	fragUV = vec2(uv.x+t, 1.0-uv.y)*3.0f;
	fragUV2 = vec2(uv.x+t2, 1.0-uv.y)*2.0f;
```

### 1.5.2. water.frag
```cpp
	vec3 normal=texture(normalTex,fragUV).rgb;
	normal=(normal*2.0-1.0);

	vec3 normal2=texture(normalTex,fragUV2).rgb;
	normal2=(normal2*2.0-1.0);

	normal=normalize(TBN*(normal+normal2));
```

**代码解析**
- t 和 t2 是根据时间 time 计算得到的偏移量， t 的变化速度是 time 的 0.05 倍， t2 的变化速度是 time 的 0.02 倍。通过不同的变化速度，可以模拟出不同频率的水波效果。
- fragUV 和 fragUV2 是经过处理后的纹理坐标。在原始纹理坐标 uv 的基础上， x 分量分别加上 t 和 t2 ，实现纹理在 x 方向上的动态偏移，模拟水波的流动。 y 分量取 1.0 - uv.y 是为了反转纹理的垂直方向。最后分别乘以 3.0f 和 2.0f 来调整纹理的缩放比例
- 将两个采样得到的法线向量 normal 和 normal2 相加，融合不同频率的水波法线信息，模拟更复杂的水波效果。

注：也可以只采用fragUV，不采用fragUV2，也能看到水波效果，但效果不如采用两个纹理坐标得到的好。