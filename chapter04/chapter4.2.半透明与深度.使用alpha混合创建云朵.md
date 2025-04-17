
## 1. 使用alpha混合创建云朵  
运行结果  
![](https://easyimage.elyt.cn/i/2025/03/29/5717727567008504969-2.webp)  

### 1.1. alpha 混合的工作原理
alpha 混合是一种用于创建半透明效果的技术，它通过将当前像素的颜色与背景颜色进行混合来实现。

伪代码如下：
```cpp

   vec4 finalColor = src * src.a + dst * (1.0 - src.a); // 混合公式
```
src 和 dst 是需要进行混合的两个片元的颜色值的名称。dst 是混合发生时后置缓冲的当前值，src 是需要混合的片元的颜色值。混合公式决定了最终的颜色值。

片元写入后置缓冲的顺序很重要，需要把不透明的片元先写入后置缓冲，再把透明的片元写入后置缓冲。

### 1.2. ofApp.cpp
```cpp
/**
 * @brief 初始化应用程序的设置
 *
 * 此函数在应用程序启动时调用，用于设置初始状态。
 * 它执行以下操作：
 * 1. 禁用任意纹理坐标模式。
 * 2. 构建角色网格并加载角色图像。
 * 3. 构建森林网格并加载森林图像。
 * 4. 加载角色着色器。
 * 5. 构建云网格并加载云图像。
 * 6. 加载云着色器。
 */
void ofApp::setup()
{
	// 禁用任意纹理坐标模式，确保纹理坐标与标准OpenGL行为一致
	ofDisableArbTex();
	// 构建角色网格，宽度为0.25，高度为0.5，位置为(0.0, -0.2, 0.0)
	buildMesh(charMesh, 0.25, 0.5, glm::vec3(0.0, -0.2, 0.0));
	// 加载角色图像
	alienImg.load("alien.png");
	// 构建森林网格，宽度为2.0，高度为2.0，位置为(0.0, 0.0, 0.5)
	buildMesh(forestMesh, 2.0, 2.0, glm::vec3(0.0, 0.0, 0.5));
	// 加载森林图像
	forestImg.load("forest.png");
	// 加载角色着色器，顶点着色器为passthrough.vert，片段着色器为alphaTest.frag
	charShader.load("passthrough.vert", "alphaTest.frag");
	// 构建云网格，宽度为0.5，高度为0.24，位置为(-0.55, 0.0, 0)
	buildMesh(cloudMesh, 0.5, 0.24, glm::vec3(-0.55, 0.0, 0));
	// 加载云图像
	cloudImg.load("cloud.png");
	// 加载云着色器，顶点着色器为cloud.vert，片段着色器为cloud.frag
	cloudShader.load("cloud.vert", "cloud.frag");
}
//--------------------------------------------------------------
/**
 * @brief 绘制应用程序的场景
 *
 * 此函数在每一帧被调用，用于绘制应用程序的场景。
 * 它执行以下操作：
 * 1. 启用深度测试，确保正确的深度排序。
 * 2. 开始使用角色着色器，并设置角色纹理和森林纹理进行绘制。
 * 3. 结束使用角色着色器。
 * 4. 启用Alpha混合，以支持半透明效果。
 * 5. 开始使用云着色器，并设置云纹理进行绘制。
 * 6. 结束使用云着色器。
 */
void ofApp::draw()
{
    // 启用深度测试，确保正确的深度排序
    ofEnableDepthTest();
    // 开始使用角色着色器
    charShader.begin();
    // 设置角色纹理
    charShader.setUniformTexture("tex", alienImg, 0);
    // 绘制角色网格
    charMesh.draw();
    // 设置森林纹理
    charShader.setUniformTexture("tex", forestImg, 0);
    // 绘制森林网格
    forestMesh.draw();
    // 结束使用角色着色器
    charShader.end();

    // 启用Alpha混合，以支持半透明效果
    ofEnableAlphaBlending();
    // 开始使用云着色器
    cloudShader.begin();
    // 设置云纹理
    cloudShader.setUniformTexture("tex", cloudImg, 0);
    // 绘制云网格
    cloudMesh.draw();
    // 结束使用云着色器
    cloudShader.end();
}
```

### 1.3. cloud.vert
与之前的顶点着色器相同，用于处理顶点位置和纹理坐标，不再给出代码。

### 1.4. cloud.frag
```cpp  
void main()
{
// 从纹理中采样颜色值，并将其赋值给输出颜色
outCol = texture(tex, fragUV);
// 限制输出颜色的透明度值不超过0.8，实现半透明的alpha混合效果
outCol.a = min(outCol.a, 0.8); // alpha blending for translucide
}
```

注意：
1. 在着色器中，尽量不用if语句，因为if语句会导致着色器的性能下降。
2. 尽量使用内置函数，如min、max、abs、sin、cos等，这些函数在GPU上执行效率更高。

## 2. 使用加法混合添加太阳光  
运行结果  
![](https://easyimage.elyt.cn/i/2025/03/29/5717802516406605883-2.webp)  

下图是太阳的图，可以看到比较暗  
![](https://easyimage.elyt.cn/i/2025/03/29/5717802909568079021-2.webp)  

我们采用加法混合的方式来添加太阳光，加法混合的公式如下：  
```cpp
vec4 finalColor = src + dst; // 混合公式
```
这样，我们的画布上有以下物体：
1、不透明的物体，如角色和森林。
2、半透明的物体，如云。
3、太阳光。

在shader中我们要遵循的原则：  
1、对于不透明的物体，我们需要开启深度测试，确保正确的深度排序。同时关闭Alpha混合，以避免混合效果。  
2、对于半透明的物体，我们需要开启Alpha混合，以支持半透明效果。通过调整Alpha的值来控制透明度。  
3、对于太阳，我们需要开启加法混合，以添加太阳光。  

### 2.1. ofApp.cpp
```cpp
void ofApp::draw()
{

	ofEnableDepthTest(); // 开启深度测试
	ofDisableAlphaBlending(); // 关闭alpha混合
	charShader.begin();
	charShader.setUniformTexture("tex", alienImg, 0);
	charMesh.draw();
	charShader.setUniformTexture("tex", forestImg, 0);
	forestMesh.draw();
	charShader.end();
	
	ofDisableDepthTest(); // 关闭深度测试
	ofEnableBlendMode(OF_BLENDMODE_ALPHA); // 开启alpha混合
	cloudShader.begin();
	cloudShader.setUniformTexture("tex", cloudImg, 0);
	cloudMesh.draw();
	cloudShader.end();

	ofEnableBlendMode(OF_BLENDMODE_ADD); 
	sunShader.begin();
	sunShader.setUniformTexture("tex", sunImg, 0);
	sunMesh.draw();
	sunShader.end();
}
```
## 3. 使用精灵表制作动画  
运行结果  
![](https://easyimage.elyt.cn/i/2025/03/29/5717829679142934579-2.gif)

下图是我们使用的精灵表  
![](https://easyimage.elyt.cn/i/2025/03/29/5717806790201054215-2.webp)  
单个精灵的尺寸为(0.28,0.19)

要从精灵表中选择单个帧，首先要将四边形的UV坐标乘以二维向量，以便将它们缩放到精灵表纹理中单个动画帧的大小。正确缩放后，还需要有一个偏移量，以便将它们定位到正确的位置。
![](https://easyimage.elyt.cn/i/2025/03/29/5717807801653270713-2.webp)  

### 3.1. alien.vert
```cpp
#version 410

layout (location = 0) in vec3 pos;
layout (location = 3) in vec2 uv;

uniform vec2 size; //sprite size
uniform vec2 offset ;// 在精灵表中的偏移量
out vec2 fragUV;

void main()
{
	gl_Position =  vec4( pos, 1.0);
	fragUV = vec2(uv.x, 1.0-uv.y)*size+(offset*size);  // 缩放UV坐标并加上偏移量
}
```

### 3.2. alien.frag
```cpp
#version 410
uniform sampler2D tex;

in vec2 fragUV;

out vec4 outCol;

void main()
{
	outCol = texture(tex, fragUV);
	if (outCol.a < 1.0)
		discard;
	
}
``` 
### 3.3. ofApp.cpp
```cpp
void ofApp::draw()
{
	static float frame=0.0;

	ofEnableDepthTest();
	ofDisableAlphaBlending(); // 关闭alpha混合
	charShader.begin();
	charShader.setUniformTexture("tex", alienImg, 0);
	charShader.setUniform2f("size",glm::vec2(0.28,0.19));

	frame+=0.02;
	if(frame>10.0){
		frame=0.0;
	}
	charShader.setUniform2f("offset",glm::vec2((int)frame/3,(int)frame%3));
	charMesh.draw();
	charShader.end();
    // 省略其他绘制代码
}
