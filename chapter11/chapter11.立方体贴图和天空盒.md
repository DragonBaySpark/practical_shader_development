
# 1. 立方体贴图和天空盒

## 1.1. 什么是立方体贴图

立方体贴图（Cube Map）是一种纹理，它由六个纹理图像组成，每个纹理图像对应一个方向。这些方向通常是立方体的六个面，分别是“前面”，“后面”，“右面”，“左面”，“上面”，“下面”。  
![](https://easyimage.elyt.cn/i/2025/04/17/5724492200805407221-2.webp)  

### 1.1.1. 前面
![](https://easyimage.elyt.cn/i/2025/04/17/5724539489326469165-2.webp)  
### 1.1.2. 后面
![](https://easyimage.elyt.cn/i/2025/04/17/5724539571090233564-2.webp)  
### 1.1.3. 右面  
![](https://easyimage.elyt.cn/i/2025/04/17/5724539641420322843-2.webp)  
### 1.1.4. 左面  
![](https://easyimage.elyt.cn/i/2025/04/17/5724539713050644592-2.webp)  
### 1.1.5. 上面  
![](https://easyimage.elyt.cn/i/2025/04/17/5724539793635811062-2.webp)  
### 1.1.6. 下面
![](https://easyimage.elyt.cn/i/2025/04/17/5724539855334021306-2.webp)  

## 1.2. 如何对立方体贴图进行采样？
### 1.2.1. 立方体贴图的存储结构
立方体贴图（Cube Map）本质上是由六个 2D 纹理组成，它们分别对应一个立方体的六个面，即前面（Positive Z）、后面（Negative Z）、右面（Positive X）、左面（Negative X）、上面（Positive Y）和下面（Negative Y）。这六个纹理按照特定的方向排列，形成一个包围空间的立方体。在 OpenGL 中，立方体贴图使用 GL_TEXTURE_CUBE_MAP 类型来表示，每个面都有对应的纹理目标，如下所示：

- GL_TEXTURE_CUBE_MAP_POSITIVE_X ：右面
- GL_TEXTURE_CUBE_MAP_NEGATIVE_X ：左面
- GL_TEXTURE_CUBE_MAP_POSITIVE_Y ：上面
- GL_TEXTURE_CUBE_MAP_NEGATIVE_Y ：下面
- GL_TEXTURE_CUBE_MAP_POSITIVE_Z ：前面
- GL_TEXTURE_CUBE_MAP_NEGATIVE_Z ：后面
### 1.2.2. 方向向量的作用
对立方体贴图采样时，需要使用一个三维的方向向量。这个向量从立方体的中心出发，指向空间中的某个方向。OpenGL 根据这个方向向量来确定应该从立方体贴图的哪个面进行采样。具体来说，方向向量的最大分量决定了采样的面，例如：

- 如果 x 分量的绝对值最大且为正，那么从右面采样。
- 如果 x 分量的绝对值最大且为负，那么从左面采样。
- 以此类推，根据 y 和 z 分量的正负和大小确定其他面。
- 在顶点着色器中，每个顶点的位置也可以看为从网络模型空间原点开始的一个向量，由于立方体网格中的顶点都以原点为中心，因此网格的顶点位置也可以看为从立方体中心出发的一个向量。

### 1.2.3. 顶点着色器中的计算
```cpp
#version 410
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    // 将顶点位置转换到裁剪空间
    gl_Position = projection * view * vec4(aPos, 1.0);
}
```
### 1.2.4. 片段着色器中的计算
```cpp
#version 410
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    // 对立方体贴图进行采样
    FragColor = texture(skybox, TexCoords);
}
```


## 1.3. 在openFrameworks 中加载立方体贴图
由于openFrameworks 中没有提供直接加载立方体贴图的函数，我们需要自己实现。具体见附书代码。
此处仅列出如何使用
```cpp
ofxEasyCubemap cubemap; // 创建立方体贴图对象
cubemap.load(	"cube_front.jpg", "cube_back.jpg", 
					"cube_right.jpg", "cube_left.jpg", 
					"cube_top.jpg", "cube_bottom.jpg"); // 加载立方体贴图

// 需要显式地调用getTexture()函数，才能获取到纹理对象  
shd.setUniformTexture("envMap", cubemap.getTexture(), 0);

```

## 1.4. 立方体贴图效果  
![](https://easyimage.elyt.cn/i/2025/04/17/5724538953789347062-2.webp)  

### 1.4.1. ofApp.cp
```cpp
void ofApp::setup()
{

	ofDisableArbTex();
	ofEnableDepthTest();
	//cubemapMesh.load("cube.ply");
	cubemapMesh.load("cube_28_vertex.ply");
	
	cubemapShader.load("cubemap.vert", "cubemap.frag");

	cubemap.load(	"cube_front.jpg", "cube_back.jpg", 
		"cube_right.jpg", "cube_left.jpg", 
		"cube_top.jpg", "cube_bottom.jpg"); // 加载立方体贴图

	directionalLightData.direction = glm::vec3(0.5f, -1.0f, -1.0f); 
	directionalLightData.color = glm::vec3(1.0f, 1.0f, 1.0f); // 白色
	directionalLightData.intensity = 1.0f; // 强度		


}

void ofApp::drawCubemap()
{
	using namespace glm;
	cameraData.position=vec3(0, 0.0f, 1.0f);
	cameraData.fov=radians(90.f);
	float aspect=1024.0f/768.0f;

	//mat4 model=glm::rotate(1.0f,vec3(1,1,1))*scale(vec3(0.5,0.5,0.5));
	mat4 model = glm::rotate(glm::radians(45.0f), vec3(1,1, 0)) * scale(vec3(0.5, 0.5, 0.5));
	//mat4 model = glm::mat4(1.0f)*scale(vec3(0.5, 0.5, 0.5));
	//mat4 view=glm::inverse(glm::translate(cameraData.position))*rotate(glm::radians(-10.0f*(float)(ofGetElapsedTimef())), vec3(1,0, 0));
	mat4 view = glm::inverse(glm::translate(cameraData.position)) * rotate(glm::radians(-10.0f ), vec3(1, 0, 0));
	mat4 proj=glm::perspective(cameraData.fov,aspect,0.01f,100.0f);
	mat4 mvp=proj*view*model;

	vec3 lightDirection = getLightDirection(directionalLightData); // 获取光源方向


	mat3 normalMatrix = glm::transpose(glm::inverse(mat3(model)));
	cubemapShader.begin();
	cubemapShader.setUniformMatrix4f("mvp", mvp);
	

	cubemapShader.setUniform3f("lightDir", lightDirection);
	cubemapShader.setUniform3f("lightCol", directionalLightData.color*directionalLightData.intensity);
	cubemapShader.setUniformMatrix4f("setUniformMatrix4f", model); // 模型矩阵
	
	cubemapShader.setUniformTexture("cubemap", cubemap.getTexture(), 0); // 立方体贴图
	
	
	cubemapMesh.draw();
	cubemapShader.end();
}
```
### 1.4.2. cubemap.vert
```cpp
#version 410

layout (location = 0) in vec3 pos;

uniform mat4 mvp;

out vec3 texCoord;
void main() {
    gl_Position = mvp * vec4(pos, 1.0);
    texCoord = pos;
}
```
### 1.4.3. cubemap.frag
```cpp
#version 410

in vec3 texCoord;
uniform samplerCube cubemap;

out vec4 fragColor;

void main() {
    fragColor = texture(cubemap, texCoord);
    //fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
```

## 1.5. 天空盒

![](https://easyimage.elyt.cn/i/2025/04/17/5724548257904858082-2.webp)  

天空盒（Skybox）是一种用于模拟天空的技术，它通常由一个立方体组成，每个面都有一个纹理图像。天空盒的纹理图像通常是天空的全景图像，它可以包含天空的所有信息，例如天空的颜色，天空的亮度，天空的纹理等。

### 1.5.1. cube.ply
以下是天空盒使用的立方体网格数据（部份），从中可以看到，其面数并不为6面，以便于展示时类似于天空，而不是明显的立方体
```cpp
3 0 1 2
3 3 4 5
3 6 7 8
3 9 10 11
3 12 13 14
3 5 15 16
3 0 2 17
3 3 5 18
3 6 8 19
3 9 11 20
3 12 14 21
3 5 16 22
```

通过这种方式创建的水面，看起来与背景图像不协调，因此，我们还需要对水面进行一些处理，使其看起来更加自然。

## 1.6. 创建立方体贴图反射  
立方体贴图反射是计算机图形学中用于模拟物体表面反射环境效果的一种技术，借助立方体贴图来呈现物体周围环境在其表面的反射影像，让渲染出的物体更具真实感。下面从原理、实现步骤和应用场景来详细介绍。

### 1.6.1. 原理
- 立方体贴图 ：由六个 2D 纹理构成，分别对应立方体的六个面（前、后、左、右、上、下），这些纹理能存储环境的全景信息。例如，在一个室外场景里，立方体贴图可以存储天空、地面、周围建筑等环境信息。
- 反射向量计算 ：对于物体表面的每个点，依据该点的法线向量和视线方向向量计算出反射向量。这个反射向量就像现实中光线照射到物体表面后反射出去的方向。
- 采样立方体贴图 ：把反射向量作为方向向量，对立方体贴图进行采样，获取反射的颜色值，将该颜色值应用到物体表面的着色计算中。  

下图展示了立方体贴图反射的运行结果
![](https://easyimage.elyt.cn/i/2025/04/17/5724722431935913036-2.webp)    

#### 1.6.1.1. water.frag
核心代码：  
```cpp
vec3 viewDir=normalize(cameraPos-fragWorldPos);	// 视线方向
vec3 reflectDir=reflect(-viewDir,normal);	// 反射方向
vec3 diffuseCol=texture(envTex,reflectDir).xyz*lightCol*diffuse;
```
