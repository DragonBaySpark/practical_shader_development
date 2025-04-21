
# 1. 深入光照
## 1.1. 平行光
我们在前面的章节中，已经介绍了平行光的基本原理和实现步骤
![](https://easyimage.elyt.cn/i/2025/04/18/5725067152642281522-2.webp)  
平行光的基本原理是，所有的光都从同一个方向照射到物体上，这个方向就是平行光的方向。

## 1.2. 点光源
点光源的基本原理是，所有的光都从一个点向各个方向照射到物体上，这个点就是点光源的位置。  
![](https://easyimage.elyt.cn/i/2025/04/18/5725067383375140253-2.webp)  

### 1.2.1. 点光源的特点
点光源是计算机图形学和现实照明中常见的一种光源类型，它模拟了从一个点向各个方向均匀发射光线的光源。以下是点光源的主要特点：

#### 1.2.1.1. 物理特性方面
1. **位置决定性**：点光源有明确的位置坐标，所有光线从该点向四面八方发射。在三维空间中，其位置可以用一个三维向量 `(x, y, z)` 来精确表示。在场景中移动点光源的位置，会直接改变物体受光的区域和强度分布。
2. **光线发散性**：光线从点光源出发，呈辐射状向周围空间发散传播。随着传播距离的增加，光线覆盖的面积会逐渐增大，这符合光的传播规律。
3. **光照衰减**：点光源的光照强度会随着距离的增加而减弱。根据平方反比定律，光照强度与距离的平方成反比，即距离点光源越远，物体接收到的光照越弱。

#### 1.2.1.2. 渲染效果方面
1. **产生阴影**：由于点光源的光线是从一个点发出的，物体被其照射时会在背后产生明显的阴影。阴影的形状和范围取决于物体的形状、位置以及点光源的位置和强度。
2. **多角度照明**：能从多个角度照亮物体，使物体表面产生丰富的明暗变化，增强物体的立体感和层次感。物体朝向点光源的面会被照亮，而背向的面则处于阴影中。
3. **局部照明效果**：点光源通常用于突出场景中的特定物体或区域，营造局部照明效果。例如，在游戏场景中，可以用点光源模拟火把、吊灯等，使这些发光物体周围的环境更加生动。

#### 1.2.1.3. 计算复杂度方面
1. **计算量较大**：与平行光等简单光源相比，点光源的光照计算更为复杂。因为需要考虑光源位置、物体与光源的距离、光照衰减等多个因素，所以在实时渲染中，大量使用点光源可能会对性能产生较大影响。
2. **可优化性**：为了提高渲染效率，有多种针对点光源的优化算法，如光照探针、光照贴图等。这些方法可以在一定程度上减少点光源的计算量，同时保持较好的光照效果。 

### 1.2.2. 点光源的实现步骤
点光源的实现步骤如下：
1. 计算点光源到物体表面的距离。
2. 计算点光源到物体表面的方向向量。
3. 计算点光源到物体表面的光照强度。
4. 将光照强度应用到物体表面。
   
#### 1.2.2.1. PointLightData
点光源的数据结构如下：
```cpp
struct PointLightData {
	glm::vec3 position;
	glm::vec3 color;
	float intensity;
	float radius;
};
```
#### 1.2.2.2. fragment shader
点光源到物体表面的距离可以通过以下公式计算：
```cpp
    vec3 toLight=(lightPos-fragWorldPos);
	vec3 lightDir=normalize(toLight); // 从片元到光源的向量
	float dist=length(toLight); // 片元到光源的距离
	float fallOff=1.0-(dist/lightRadius); // 衰减
	fallOff=clamp(fallOff,0.0,1.0); // 限制在0到1之间
	vec3 lightColNew=lightCol*fallOff; // 应用衰减到光源颜色
```

#### 1.2.2.3. 通过按键控制点光源
我们可以通过按键控制点光源的位置，以便观察点光源不同位置对于物体的影响，代码如下：
```cpp
void ofApp::keyPressed(int key)
{
	if (key == 'a') {
		pointLightData.position.x -= 0.01f;
	}
	else if (key == 'd') {
		pointLightData.position.x += 0.01f;
	}
	else if (key == 'w') {
		pointLightData.position.y += 0.01f;
	}
	else if (key =='s') {
		pointLightData.position.y -= 0.01f;
	}
	else if (key == 'q') {
		pointLightData.position.z -= 0.01f;
	}
	else if (key == 'e') {
		pointLightData.position.z += 0.01f;
	}

	printf("pointLightData.position.x = %f , pointLightData.position.y = %f, pointLightData.position.z = %f\n", pointLightData.position.x, pointLightData.position.y, pointLightData.position.z);
	
	
}
```
以下两张图分别是点光源位置不同时，对于物体的影响。
![](https://easyimage.elyt.cn/i/2025/04/19/5725264110153635897-2.webp)  
![](https://easyimage.elyt.cn/i/2025/04/19/5725264265078640979-2.webp)  

## 1.3. 聚光灯

![](https://easyimage.elyt.cn/i/2025/04/19/5725264691937153988-2.webp)  
聚光灯是一种特殊的光源，它从某个方向照射物体，并在物体表面产生锥形的光照区域。聚光灯的原理和实现步骤如下：
1. 计算聚光灯到物体表面的距离。
2. 计算聚光灯到物体表面的方向向量。
3. 计算聚光灯到物体表面的光照强度。
4. 计算聚光灯到物体表面的方向向量与聚光灯方向的角度，可以转换为余弦值。即：cosθ = dot(lightDir,lightDirToFragment)
5. 判断聚光灯是否能照射到物体上，即cosθ是否大于聚光灯的lightCutoff值（角度<聚光灯的角度）。
6. 将光照强度应用到物体表面。

以下是运行效果：
从中可以看到，聚光灯的光照区域是一个圆形的区域。
![](https://easyimage.elyt.cn/i/2025/04/21/5726053629454983060-2.webp)  

### 1.3.1. 核心代码（fragment shader）
```cpp
    vec3 toLight = lightPos - fragWorldPos; 
	vec3 lightDir = normalize(toLight);
	float angle = dot(lightConeDir, -lightDir);
	float falloff = 0.0;
	if (angle > lightCutoff)
	{
		falloff = 1.0;
	}

	float diffAmt = diffuse(lightDir, nrm) * falloff;
```
### 1.3.2. 完整代码（fragment shader）
```cpp
#version 410

uniform vec3 lightCol;
uniform vec3 lightConeDir;
uniform vec3 lightPos;
uniform float lightCutoff; 
uniform vec3 cameraPos;
uniform vec3 ambientCol;
uniform sampler2D diffuseTex;
uniform sampler2D specTex;
uniform sampler2D normTex;
uniform samplerCube envMap;

in vec3 fragNrm;
in vec3 fragWorldPos;
in vec2 fragUV;
in mat3 TBN;
out vec4 outCol;


float diffuse(vec3 lightDir, vec3 nrm)
{
	float diffAmt = max(0.0, dot(nrm, lightDir));
	return diffAmt;
}

float specular(vec3 lightDir, vec3 viewDir, vec3 nrm, float shininess)
{
	vec3 halfVec = normalize(viewDir + lightDir);
	float specAmt = max(0.0, dot(halfVec, nrm));
	return pow(specAmt, shininess);
}

void main(){	
	vec3 nrm = texture(normTex, fragUV).rgb;
	nrm = normalize(nrm * 2.0 - 1.0);   
	nrm = normalize(TBN * nrm); 
	vec3 viewDir = normalize( cameraPos - fragWorldPos); 

	vec3 envSample = texture(envMap, reflect(-viewDir, nrm)).xyz;
	vec3 sceneLight = mix(lightCol, envSample + lightCol * 0.5, 0.5);	

	vec3 toLight = lightPos - fragWorldPos; 
	vec3 lightDir = normalize(toLight);
	float angle = dot(lightConeDir, -lightDir);
	float falloff = 0.0;
	if (angle > lightCutoff)
	{
		falloff = 1.0;
	}

	float diffAmt = diffuse(lightDir, nrm) * falloff;
	float specAmt = specular(lightDir, viewDir, nrm, 4.0) * falloff;

	vec3 diffCol = texture(diffuseTex, fragUV).xyz * sceneLight * diffAmt;

	float specMask = texture(specTex, fragUV).x;
	vec3 specCol = specMask * sceneLight * specAmt;
	
	outCol = vec4(diffCol + specCol + ambientCol, 1.0);

}
```

## 1.4. 多个灯源（shader中固定实现）
运行效果：  
![](https://easyimage.elyt.cn/i/2025/04/21/5726057742938938964-2.webp)  
采用在shader中固定实现，这种方式我们**不推荐**，因为这样会使得shader代码变得复杂，而且效率不高。但这种方式可以让我们了解，多个光源是如何在shader中实现的。

### 1.4.1. 实现步骤
1. 定义多个光源的数据结构。
2. 在fragment shader中，遍历所有的光源，计算每个光源对物体的光照强度。
3. 将所有光源对物体的光照强度相加，得到最终的光照强度。


### 1.4.2. 核心代码（fragment shader）
```cpp
vec3 finalColor = vec3(0,0,0);
//遍历所有的平行光源
for (int i = 0; i < NUM_DIR_LIGHTS; ++i) 
	{
		DirectionalLight light = directionalLights[i];
		vec3 sceneLight = mix(light.color, envSample + light.color * 0.5, 0.5);	

		float diffAmt = diffuse(light.direction, nrm);
		float specAmt = specular(light.direction, viewDir, nrm, 4.0) * specMask;
	
		vec3 specCol = specMask * sceneLight * specAmt;

		finalColor += diffuseColor * diffAmt * sceneLight;
		finalColor += specCol * light.color;
	}
//遍历所有的点光源
//...
```
### 1.4.3. 完整代码（fragment shader）
```cpp
#version 410

struct DirectionalLight {
	vec3 direction;
	vec3 color;
};  

struct PointLight {    
	vec3 position;
	vec3 color;
	float radius;
};  

struct SpotLight{
	vec3 position;
	vec3 direction;
	vec3 color;
	float cutoff;
};

#define NUM_DIR_LIGHTS 1
#define NUM_POINT_LIGHTS 2
#define NUM_SPOT_LIGHTS 2

uniform DirectionalLight directionalLights[NUM_DIR_LIGHTS];
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLights[NUM_SPOT_LIGHTS];
uniform sampler2D diffuseTex;
uniform sampler2D specTex;
uniform sampler2D normTex;
uniform samplerCube envMap;
uniform vec3 cameraPos;
uniform vec3 ambientCol;

in vec3 fragNrm;
in vec3 fragWorldPos;
in vec2 fragUV;
in mat3 TBN;

out vec4 outCol;

float diffuse(vec3 lightDir, vec3 nrm)
{
	float diffAmt = max(0.0, dot(nrm, lightDir));
	return diffAmt;
}

float specular(vec3 lightDir, vec3 viewDir, vec3 nrm, float shininess)
{
	vec3 halfVec = normalize(viewDir + lightDir);
	float specAmt = max(0.0, dot(halfVec, nrm));
	return pow(specAmt, shininess);
}

void main(){	
	vec3 nrm = texture(normTex, fragUV).rgb;
	nrm = normalize(nrm * 2.0 - 1.0);   
	nrm = normalize(TBN * nrm); 
	vec3 viewDir = normalize( cameraPos - fragWorldPos); 	

	vec3 envSample = texture(envMap, reflect(-viewDir, nrm)).xyz;
	float specMask = texture(specTex, fragUV).x;
	vec3 diffuseColor = texture(diffuseTex, fragUV).xyz;

	vec3 finalColor = vec3(0,0,0);

	for (int i = 0; i < NUM_DIR_LIGHTS; ++i) 
	{
		DirectionalLight light = directionalLights[i];
		vec3 sceneLight = mix(light.color, envSample + light.color * 0.5, 0.5);	

		float diffAmt = diffuse(light.direction, nrm);
		float specAmt = specular(light.direction, viewDir, nrm, 4.0) * specMask;
	
		vec3 specCol = specMask * sceneLight * specAmt;

		finalColor += diffuseColor * diffAmt * sceneLight;
		finalColor += specCol * light.color;
	}

	for (int i = 0; i < NUM_POINT_LIGHTS; ++i)
	{
		PointLight light = pointLights[i];
		vec3 sceneLight = mix(light.color, envSample + light.color * 0.5, 0.5);	

		vec3 toLight = light.position - fragWorldPos; 
		vec3 lightDir = normalize(toLight);
		float distToLight = length(toLight); 
		float falloff = 1.0 - (distToLight / light.radius);

		float diffAmt = diffuse(lightDir, nrm) * falloff;
		float specAmt = specular(lightDir, viewDir, nrm, 4.0) * specMask * falloff;
		vec3 specCol = specMask * sceneLight * specAmt;

		finalColor += diffAmt * sceneLight * diffuseColor;
		finalColor += specCol;
	}

	for (int i = 0; i < NUM_SPOT_LIGHTS; ++i)
	{
		SpotLight light = spotLights[i];
		vec3 sceneLight = mix(light.color, envSample + light.color * 0.5, 0.5);	

		vec3 toLight = light.position - fragWorldPos; 
		vec3 lightDir = normalize(toLight);
		float angle = dot(light.direction, -lightDir);
		float falloff = (angle > light.cutoff) ? 1.0 : 0.0;

		float diffAmt = diffuse(lightDir, nrm) * falloff;
		float specAmt = specular(lightDir, viewDir, nrm, 4.0) * specMask * falloff;

		vec3 specCol = specMask * sceneLight * specAmt;

		finalColor += diffAmt * sceneLight * diffuseColor;
		finalColor += specCol;
	}
	outCol = vec4(finalColor + ambientCol, 1.0);
}
```
## 1.5. Multi-pass（多通道）
![](https://easyimage.elyt.cn/i/2025/04/21/5726060477230878993-2.webp)  

在着色器编程里，Multi-pass（多通道）着色是一种渲染技术，它将一个复杂的渲染任务拆分成多个连续的渲染通道（pass）来完成。每个通道都有特定的渲染目标和操作，最终把各个通道的结果组合起来，实现复杂的视觉效果。下面从原理、工作流程、应用场景和优缺点几个方面详细介绍。

### 1.5.1. 原理
Multi-pass 着色的核心思想是把复杂的渲染效果分解为多个简单的步骤，每个步骤对应一个渲染通道。在每个通道中，GPU 会对场景或者特定对象进行渲染，将结果存储在帧缓冲（Framebuffer）或者纹理中，后续的通道可以利用这些中间结果进行进一步处理，最终得到完整的渲染效果。

### 1.5.2. 工作流程
1. 初始化 ：设置好初始的渲染状态，比如视口大小、清除颜色缓冲区和深度缓冲区等。
2. 通道渲染 ：依次执行各个渲染通道，每个通道都有自己的着色器程序、渲染目标和渲染参数。在每个通道结束后，可能需要保存渲染结果到纹理或者帧缓冲中。
3. 结果合并 ：最后一个通道或者后续处理步骤会把前面各个通道的结果进行合并，生成最终的渲染图像。

### 1.5.3. 优缺点
#### 1.5.3.1. 优点
- 实现复杂效果 ：能够实现单通道难以完成的复杂视觉效果，提升渲染质量。
- 模块化设计 ：将渲染任务分解为多个通道，每个通道的逻辑相对简单，便于开发和维护。 
### 1.5.4. 缺点
- 性能开销大 ：每个通道都需要进行一次完整的渲染过程，会增加 GPU 的计算量和内存开销，降低渲染性能。
- 资源占用多 ：需要额外的帧缓冲和纹理来存储中间结果，增加了内存资源的占用。

### 1.5.5. 概念
#### 1.5.5.1. glDepthFunc(GL_LEQUAL)
glDepthFunc(GL_LEQUAL); 是 OpenGL 里用于设置深度测试函数的调用语句。深度测试是渲染管线中非常关键的一部分，它依据片段的深度值来决定新片段是否能覆盖帧缓冲区里已有的片段。

**函数解释**
- glDepthFunc 是 OpenGL 的一个函数，作用是指定深度比较函数。该函数会根据新片段和深度缓冲区中已存片段的深度值，判断新片段是否可以替换掉已有的片段。
- GL_LEQUAL 是众多深度比较函数中的一种，代表“小于或等于”。当调用 glDepthFunc(GL_LEQUAL) 后，若新传入片段的深度值小于或等于深度缓冲区中存储的深度值，深度测试就会通过，这个新片段就可能会被绘制到屏幕上；反之，若不满足条件，该片段就会被丢弃。
- GL_LESS 若新片段的深度值小于深度缓冲区中存储的深度值，深度测试通过。

调用glDepthFunc(GL_LEQUAL) 函数后，就不会忽略片元被绘制到前面通道已经绘制的同一个点上


### 1.5.6. 代码
#### 1.5.6.1. 核心代码(ofApp.cpp)
```cpp
void ofApp::beginRenderingPointLights()
{
	ofEnableAlphaBlending();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	glDepthFunc(GL_LEQUAL);
}

void ofApp::endRenderingPointLights()
{
	ofDisableAlphaBlending();
	ofDisableBlendMode();
	glDepthFunc(GL_LESS);
}
void ofApp::draw() {

	using namespace glm;

	
	cam.pos = glm::vec3(0, 0.75f, 1.0);

	mat4 proj = perspective(cam.fov, 1024.0f / 768.0f, 0.01f, 10.0f);
	mat4 view = inverse(translate(cam.pos));

	drawSkybox(proj, view);

	drawWater(dirLight, proj, view);
	drawShield(dirLight, proj, view);

	beginRenderingPointLights();
	for (int i = 0; i < pointLights.size(); ++i)
	{
		drawWater(pointLights[i], proj, view);
		drawShield(pointLights[i], proj, view);
	}

	endRenderingPointLights();
	
}
```
#### 1.5.6.2. 完整代码(ofApp.cpp) 
```cpp
#include "ofApp.h"
#include <vector>
#include "ofMainLoop.h"
//--------------------------------------------------------------

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

void ofApp::setup() {

	ofSetVerticalSync(false);
	ofDisableArbTex();
	ofEnableDepthTest();

	planeMesh.load("plane.ply");
	shieldMesh.load("shield.ply");
	cubeMesh.load("cube.ply");

	dirLightShaders[0].load("mesh.vert", "dirLight.frag");
	pointLightShaders[0].load("mesh.vert", "pointLight.frag");

	dirLightShaders[1].load("water.vert", "dirLightWater.frag");
	pointLightShaders[1].load("water.vert", "pointLightWater.frag");


	diffuseTex.load("shield_diffuse.png");
	specTex.load("shield_spec.png");
	nrmTex.load("shield_normal.png");
	waterNrm.load("water_nrm.png");
	waterNrm.getTexture().enableMipmap();
	waterNrm.getTexture().generateMipmap();

	waterNrm.getTexture().setTextureMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	waterNrm.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);

	cam.pos = glm::vec3(0, 0.75f, 1.0f);
	cam.fov = glm::radians(90.0f);

	calcTangents(shieldMesh);
	calcTangents(planeMesh);

	skyboxShader.load("skybox.vert", "skybox.frag");
	cam.pos = glm::vec3(0, 0.75f, 1.0f);
	cam.fov = glm::radians(90.0f);

	cubemap.load("night_front.jpg", "night_back.jpg",
		"night_right.jpg", "night_left.jpg",
		"night_top.jpg", "night_bottom.jpg");

	PointLight pl0;
	pl0.color = glm::vec3(1, 0, 0);
	pl0.radius = 1.0f;
	pl0.position = glm::vec3(-0.5, 0.35, 0.25);
	pl0.intensity = 3.0;

	PointLight pl1;
	pl1.color = glm::vec3(0, 1, 0);
	pl1.radius = 1.0f;
	pl1.position = glm::vec3(0.5, 0.35, 0.25);
	pl1.intensity = 3.0;

	PointLight pl2;
	pl2.color = glm::vec3(0, 0, 1);
	pl2.radius = 1.0f;
	pl2.position = glm::vec3(0.0, 0.7, 0.25);
	pl2.intensity = 3.0;

	pointLights.push_back(pl0);
	pointLights.push_back(pl1);
	pointLights.push_back(pl2);

	dirLight.color = glm::vec3(1, 1, 0);
	dirLight.intensity = 0.25f;
	dirLight.direction = glm::vec3(0, 0, -1);

}

glm::vec3 getLightDirection(DirectionalLight& l)
{
	return glm::normalize(l.direction * -1.0f);
}

glm::vec3 getLightColor(DirectionalLight& l)
{
	return l.color * l.intensity;
}

glm::vec3 getLightColor(PointLight& l)
{
	return l.color * l.intensity;
}


//--------------------------------------------------------------
void ofApp::update()
{
}

void ofApp::drawWater(Light& light, glm::mat4& proj, glm::mat4& view)
{
	using namespace glm;

	static float t = 0.0f;
	t += ofGetLastFrameTime();

	vec3 right = vec3(1, 0, 0);
	mat4 rotation = rotate(radians(-90.0f), right);
	mat4 model = rotation * scale(vec3(10.0, 10.0, 10.0));

	mat4 mvp = proj * view * model;
	mat3 normalMatrix = mat3(transpose(inverse(model)));
	ofShader shd = light.isPointLight() ? pointLightShaders[1] : dirLightShaders[1];

	shd.begin();
	light.apply(shd);

	shd.setUniformMatrix4f("mvp", mvp);
	shd.setUniformMatrix4f("model", model);
	shd.setUniformMatrix3f("normal", normalMatrix);
	shd.setUniform3f("meshSpecCol", glm::vec3(1, 1, 1));
	shd.setUniformTexture("normTex", waterNrm, 0);;
	shd.setUniformTexture("envMap", cubemap.getTexture(), 1);
	shd.setUniform1f("time", t);
	shd.setUniform3f("ambientCol", glm::vec3(0.0, 0.0, 0.0));
	shd.setUniform3f("cameraPos", cam.pos);
	planeMesh.draw();
	shd.end();
}


void ofApp::drawSkybox(glm::mat4& proj, glm::mat4& view)
{
	using namespace glm;

	mat4 model = translate(cam.pos);
	mat4 mvp = proj * view * model;

	ofShader& shd = skyboxShader;
	glDepthFunc(GL_LEQUAL);
	shd.begin();
	shd.setUniformMatrix4f("mvp", mvp);
	shd.setUniformTexture("envMap", cubemap.getTexture(), 0);
	cubeMesh.draw();
	shd.end();
	glDepthFunc(GL_LESS);
}

void ofApp::drawShield(Light& light, glm::mat4& proj, glm::mat4& view)
{
	using namespace glm;

	mat4 model = translate(vec3(0.0, 0.75, 0.0f));
	mat4 mvp = proj * view * model;
	mat3 normalMatrix = mat3(transpose(inverse(model)));

	ofShader shd = light.isPointLight() ? pointLightShaders[0] : dirLightShaders[0];

	shd.begin();
	light.apply(shd);
	shd.setUniformMatrix4f("mvp", mvp);
	shd.setUniformMatrix4f("model", model);
	shd.setUniformMatrix3f("normal", normalMatrix);
	shd.setUniform3f("meshSpecCol", glm::vec3(1, 1, 1));
	shd.setUniformTexture("diffuseTex", diffuseTex, 0);;
	shd.setUniformTexture("specTex", specTex, 1);
	shd.setUniformTexture("normTex", nrmTex, 2);
	shd.setUniformTexture("envMap", cubemap.getTexture(), 3);

	shd.setUniform3f("ambientCol", glm::vec3(0.0, 0.0, 0.0));
	shd.setUniform3f("cameraPos", cam.pos);
	shieldMesh.draw();
	shd.end();
}


void ofApp::beginRenderingPointLights()
{
	ofEnableAlphaBlending();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	glDepthFunc(GL_LEQUAL);
}

void ofApp::endRenderingPointLights()
{
	ofDisableAlphaBlending();
	ofDisableBlendMode();
	glDepthFunc(GL_LESS);
}


//--------------------------------------------------------------
void ofApp::draw() {

	using namespace glm;

	
	cam.pos = glm::vec3(0, 0.75f, 1.0);

	mat4 proj = perspective(cam.fov, 1024.0f / 768.0f, 0.01f, 10.0f);
	mat4 view = inverse(translate(cam.pos));

	drawSkybox(proj, view);

	drawWater(dirLight, proj, view);
	drawShield(dirLight, proj, view);

	beginRenderingPointLights();
	for (int i = 0; i < pointLights.size(); ++i)
	{
		drawWater(pointLights[i], proj, view);
		drawShield(pointLights[i], proj, view);
	}

	endRenderingPointLights();
	
}



```
#### 1.5.6.3. dirLight.frag
```cpp
#version 410

uniform vec3 lightDir;
uniform vec3 lightCol;
uniform vec3 cameraPos;
uniform vec3 ambientCol;
uniform sampler2D diffuseTex;
uniform sampler2D specTex;
uniform sampler2D normTex;
uniform samplerCube envMap;
in vec3 fragNrm;
in vec3 fragWorldPos;
in vec2 fragUV;
in mat3 TBN;

out vec4 outCol;

void main()
{
	vec3 nrm = texture(normTex, fragUV).rgb;
	nrm = normalize(nrm * 2.0 - 1.0);   
	nrm = normalize(TBN * nrm); 
	vec3 diffCol = vec3(0,0,0);
	vec3 specCol = vec3(0,0,0);
	vec3 viewDir = normalize( cameraPos - fragWorldPos); 	
	vec3 halfVec = normalize(viewDir + lightDir);
	
	vec3 envSample = texture(envMap, reflect(-viewDir, nrm)).xyz;
	vec3 sceneLight = mix(lightCol, envSample + lightCol * 0.5, 0.5);

	float diffAmt = max(0.0, dot(nrm, lightDir));
	diffCol += texture(diffuseTex, fragUV + nrm.xy).xyz * sceneLight * diffAmt;

	float specAmt = max(0.0, dot(halfVec, nrm));
	float specBright = pow(specAmt, 4.0);
	specCol += texture(specTex, fragUV + nrm.xy).x * sceneLight * specBright;

	outCol = vec4(diffCol + specCol + ambientCol, 1.0);

}
```

#### 1.5.6.4. dirLightWater.frag
```cpp
#version 410

uniform vec3 lightDir;
uniform vec3 lightCol;
uniform vec3 cameraPos;
uniform vec3 ambientCol;
uniform sampler2D normTex;
uniform samplerCube envMap;
in vec3 fragNrm;
in vec3 fragWorldPos;
in vec2 fragUV;

in mat3 TBN;

out vec4 outCol;

in vec2 fragUV2;
void main()
{
	vec3 nrm = texture(normTex, fragUV).rgb;
	nrm = (nrm * 2.0 - 1.0);   
	
	vec3 nrm2 = texture(normTex, fragUV2).rgb;
	nrm2 = (nrm2 * 2.0 - 1.0);   
	
	nrm = normalize(TBN * (nrm + nrm2)); 

	vec3 viewDir = normalize( cameraPos - fragWorldPos); 	
	vec3 halfVec = normalize(viewDir + lightDir);

	vec3 envSample = texture(envMap, reflect(-viewDir, nrm)).xyz;
	vec3 sceneLight = mix(lightCol, envSample + lightCol * 0.5, 0.5);


	float diffAmt = max(0.0, dot(nrm, lightDir));
	vec3 diffCol = sceneLight * diffAmt;

	float specAmt = max(0.0, dot(halfVec, nrm));
	float specBright = pow(specAmt, 512.0);
	vec3 specCol = sceneLight * specBright;

	outCol = vec4(diffCol + specCol + ambientCol, 1.0); 
}
```

#### 1.5.6.5. pointLight.frag
```cpp
#version 410 

uniform vec3 meshCol;
uniform vec3 lightPos; 
uniform float lightRadius;
uniform vec3 lightCol;
uniform vec3 cameraPos;
uniform vec3 ambientCol;
uniform sampler2D diffuseTex;
uniform sampler2D specTex;
uniform sampler2D normTex;
uniform samplerCube envMap;

in vec3 fragNrm;
in vec3 fragWorldPos;
in vec2 fragUV;
in mat3 TBN;
out vec4 outCol;

float diffuse(vec3 lightDir, vec3 nrm)
{
	float diffAmt = max(0.0, dot(nrm, lightDir));
	return diffAmt;
}

float specular(vec3 lightDir, vec3 viewDir, vec3 nrm, float shininess)
{
	vec3 halfVec = normalize(viewDir + lightDir);
	float specAmt = max(0.0, dot(halfVec, nrm));
	return pow(specAmt, shininess);
}

void main()
{	
	vec3 nrm = texture(normTex, fragUV).rgb;
	nrm = normalize(nrm * 2.0 - 1.0);   
	nrm = normalize(TBN * nrm); 
	vec3 viewDir = normalize( cameraPos - fragWorldPos); 	

	vec3 envSample = texture(envMap, reflect(-viewDir, nrm)).xyz;
	vec3 sceneLight = mix(lightCol, envSample + lightCol * 0.5, 0.5);

	//manual light direction calculation
	vec3 toLight = lightPos - fragWorldPos; 
	vec3 lightDir = normalize(toLight);
	float distToLight = length(toLight); 
	float falloff = 1.0 - (distToLight / lightRadius);

	float diffAmt = diffuse(lightDir, nrm) * falloff; 
	float specAmt = specular(lightDir, viewDir, nrm, 4.0) * falloff; 

	vec3 diffCol = texture(diffuseTex, fragUV).xyz * sceneLight * diffAmt;

	float specMask = texture(specTex, fragUV).x;
	vec3 specCol = specMask * sceneLight * specAmt;

	outCol = vec4(diffCol + specCol, 1.0);
}

```

#### 1.5.6.6. pointLightWater.frag
```cpp
#version 410

uniform vec3 lightPos; 
uniform float lightRadius;
uniform vec3 lightCol;
uniform vec3 cameraPos;
uniform vec3 ambientCol;
uniform sampler2D normTex;
uniform samplerCube envMap;

in vec3 fragNrm;
in vec3 fragWorldPos;
in vec2 fragUV;
in vec2 fragUV2;
in mat3 TBN;

out vec4 outCol;

float diffuse(vec3 lightDir, vec3 nrm)
{
	float diffAmt = max(0.0, dot(nrm, lightDir));
	return diffAmt;
}

float specular(vec3 lightDir, vec3 viewDir, vec3 nrm, float shininess)
{
	vec3 halfVec = normalize(viewDir + lightDir);
	float specAmt = max(0.0, dot(halfVec, nrm));
	return pow(specAmt, shininess);
}

void main()
{
	vec3 nrm = texture(normTex, fragUV).rgb;
	nrm = (nrm * 2.0 - 1.0);   
	
	vec3 nrm2 = texture(normTex, fragUV2).rgb;
	nrm2 = (nrm2 * 2.0 - 1.0);   
	
	nrm = normalize(TBN * (nrm + nrm2)); 

	//manual light direction calculation
	vec3 toLight = lightPos - fragWorldPos; 
	vec3 lightDir = normalize(toLight);
	float distToLight = length(toLight); 
	float falloff = 1.0 - (distToLight / lightRadius);
	vec3 viewDir = normalize( cameraPos - fragWorldPos); 	

	float diffAmt = diffuse(lightDir, nrm) * falloff; 
	float specAmt = specular(lightDir, viewDir, nrm, 512.0) * falloff; 

	vec3 diffCol = texture(envMap, (reflect(-viewDir, nrm))).xyz * lightCol * diffAmt;
	vec3 specCol = lightCol * specAmt;

	outCol = vec4(diffCol + specCol, 1.0); 
}
```