OpenGLshader开发实战学习笔记：第二章 第一个着色器


# 1. 第一个着色器

[openFrameworks](https://openframeworks.cc/) 是一个开源的 C++ 工具包，旨在通过提供一个简单直观的框架来协助创作过程，帮助进行实验。

openFrameworks 被设计为一种通用的“胶水”，将多个常用的库整合在一起，包括：

- **图形**：OpenGL、GLEW、GLUT、libtess2 和 cairo  
- **音频输入、输出与分析**：rtAudio、PortAudio、OpenAL、Kiss FFT 或 FMOD  
- **字体**：FreeType  
- **图像保存与加载**：FreeImage  
- **视频播放与捕获**：Quicktime、GStreamer 和 videoInput  
- **多种实用工具**：Poco  
- **计算机视觉**：OpenCV  
- **3D 模型加载**：Assimp  

代码被编写为高度跨兼容性。目前，openFrameworks支持五个操作系统（Windows、OSX、Linux、iOS、Android）和四个集成开发环境（XCode、Code::Blocks、Visual Studio 和 Eclipse）。API 的设计力求简洁且易于掌握。
openFrameworks 以 MIT 许可证发布。这赋予了每个人在任何环境下使用 openFrameworks 的自由：商业或非商业、公共或私人、开源或闭源。虽然许多 openFrameworks 用户以类似自由的方式将自己的作品回馈给社区，但这并非强制要求。
简而言之，openFrameworks 是一个让用代码创作变得更加容易的工具。
我们的关注点在于OpenGL开发，为了简化学习，将学习重点放在OpenGL上，因此使用openFrameworks来作为我们的开发环境。
原书使用的是[openFrameWorks v0.10.0 版本](https://openframeworks.cc/versions/v0.10.0/of_v0.10.0_vs2017_release.zip)

本学习笔记配套的代码在https://github.com/DragonBaySpark/practical_shader_development，如遇因Visual Studio 及openframeworks的目录原因造成配套的sln项目不能运行，可以自行新建项目，将src及bin/data目录拷贝过去即可

**参考资料**
1. [openFrameWorks官方文档](https://openframeworks.cc/documentation/)
2. [openFrameWorks v0.10.0 版本下载](https://openframeworks.cc/versions/v0.10.0/of_v0.10.0_vs2017_release.zip)

## 1.1. 在windows上安装openFrameWorks
在Visual Studio 中设置常规项目和使用openFrameWorks的区别是是openFrameWorks 自带的项目生成器(project generator)应用程序。这个应用程序可以帮助我们创建一个新的openFrameWorks项目，而不需要手动设置项目的属性。

1. 下载openFrameWorks v0.12.0 版本（目前最新版），解压到本地磁盘。(注：原书中采用的是v0.10.0版本，我在学习时采用了v0.12.0版本。如果遇到问题，可以将版本换回v0.10.0，这样与原书一致)
2. 打开projectGenerator/projectGenerator.exe，按照提示创建项目。
3. 用Visual Studio 2022 打开项目，编译运行即可，无需手动设置项目属性。

注：创建的Visual Studio 项目中的项目属性中会有很多设置与openFrameWroks的目录有关，建议可以将openFrameWorks放在以下目录：D:\openframeworks\of_v0.12.0_vs_release

![](https://easyimage.elyt.cn/i/2025/03/24/5715785763770078984-2.webp)


![](https://easyimage.elyt.cn/i/2025/03/24/5716022074003167618-2.webp)

在Visual Studio 上打开的窗口中点击"确定"  
![](https://easyimage.elyt.cn/i/2025/03/24/5716022494532476025-2.webp)

解决方案中将包含2个项目，我们目前的重点在main.cpp和ofApp.cpp   
![](https://easyimage.elyt.cn/i/2025/03/24/5716022913639912111-2.webp)

不修改代码，直接编译运行，将得到如下的界面 
![](https://easyimage.elyt.cn/i/2025/03/24/5716023451345490321-2.webp)  


## 1.2. 创建第一个三角形 

### 1.2.1. 运行结果
![](https://easyimage.elyt.cn/i/2025/03/24/5716025396588187856-2.webp)
### 1.2.2. main.cpp 
```c++
int main( ){

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(1024, 768);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	settings.setGLVersion(4, 1); //GL 版本设置为4.1
	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();

}

```

### 1.2.3. ofApp.h
```c++
private:
	// ofMesh 用于存储和管理三维模型的顶点、颜色、纹理坐标等数据
	ofMesh mesh;
```

### 1.2.4. ofApp.cpp
```c++
void ofApp::setup(){
	
	// 添加第一个顶点，位于坐标原点 (0, 0, 0)
	mesh.addVertex(glm::vec3(0.0, 0.0, 0.0));

	// 添加第二个顶点，位于坐标 (0, 768, 0)
	mesh.addVertex(glm::vec3(0.0, 768.0f, 0.0));

	// 添加第三个顶点，位于坐标 (1024, 768, 0)
	mesh.addVertex(glm::vec3(1024.0f, 768.0f, 0.0));

}

void ofApp::draw(){
	// 绘制网格，将之前定义的顶点连接起来形成图形
	// 每帧都会重新绘制一次网格，每帧绘制三个顶点
	mesh.draw();
}
```

### 1.2.5. 说明
代码中画的窗口为 1024*768 大小 ，其坐标系为：原点为左上角 (0,0) ,右下角为(1024,768)
![](https://easyimage.elyt.cn/i/2025/03/25/5716163769114041051-2.webp)
在OpenGL中，有各种坐标系，后面会一一说明。
这里先说明第一种：将屏幕像素坐标归一化到[-1,1] 之间 。这个归一化其实也存在一定的问题，这会造成屏幕的宽度和高度的比例不一致，导致图像变形。目前我们暂时不考虑这个问题，后续会解决这个问题。
![](https://easyimage.elyt.cn/i/2025/03/25/5716221863273700688-2.webp)

原始代码为
```c++
mesh.addVertex(glm::vec3(0.0, 0.0, 0.0));
mesh.addVertex(glm::vec3(0.0, 768.0f, 0.0));
mesh.addVertex(glm::vec3(1024.0f, 768.0f, 0.0));
```
更新后的代码如下：
```c++
mesh.addVertex(glm::vec3(-1.0, 1.0, 0.0));
mesh.addVertex(glm::vec3(-1.0, -1.0f, 0.0));
mesh.addVertex(glm::vec3(1.0, -1.0f, 0.0));

```

## 1.3. 第一个顶点着色器
**运行结果**
![](https://easyimage.elyt.cn/i/2025/03/25/5716244035954285401-2.webp)

着色器代码通常放在与C++代码分离的文件中，通常放置在与存储图像或网格等资源相同的文件夹中。
对于openFrameworks，资源目录为bin/data
通常着色器后缀规则：
1. 顶点着色器： 后缀为.vert 
2. 片元着色器:  后缀为.frag 

顶点着色器的代码
```glsl
// 指定GLSL版本为4.1
#version 410
// 声明输入变量：顶点位置（从顶点缓冲区接收）
in vec3 position;
// 主函数：每个顶点都会执行一次
void main()
{
    // 将3D位置转换为齐次坐标系（4D）
    // w分量设为1.0表示这是一个位置点（而不是方向向量）
    gl_Position=vec4(position,1.0);
}
```
目前只介绍我们会用到的几个关键字
1. #version ：指定着色器的版本号
2. in ： 输入变量，从顶点着色器传递到片元着色器
3. out ： 输出变量
4. void ： 函数返回类型，无返回值
5. main ： 主函数，程序的入口点
6. vec2 ： 2D向量类型，包含2个浮点数分量，常用于表示2D坐标、纹理坐标(UV)等。分量可以用 x,y 或 s,t 访问
7. vec3 ： 3D向量类型，包含3个浮点数分量，常用于表示3D位置、法线、颜色(RGB)等。分量可以用 x,y,z 或 r,g,b 访问
8. vec4 ： 4D向量类型，包含4个浮点数分量，常用于表示4D位置、颜色(RGBA)等。分量可以用 x,y,z,w 或 r,g,b,a 访问
9. gl_Position ： 内置变量，用于存储顶点的位置信息，在顶点着色器中设置该变量的值，将决定顶点在屏幕上的位置。

**GLSL 语言参考资料**
1. [OpenGL_Shading_Language](https://www.khronos.org/opengl/wiki/OpenGL_Shading_Language)
2. [WebGL GLSL_Shaders](https://developer.mozilla.org/zh-CN/docs/Games/Techniques/3D_on_the_web/GLSL_Shaders)

## 1.4. 第一个片元着色器
片元着色器的代码
```glsl
#version 410
// 输出变量：片元颜色
out vec4 color;
// 主函数：每个片元都会执行一次
void main()
{
    // 设置片元颜色为红色
    color=vec4(1.0,0.0,0.0,1.0);

}
```
片元着色器中，我们只定义了一个输出变量 color，其类型为 vec4，表示4个浮点数分量，分别表示RGBA颜色值。其值设置为红色，因此，每个片元都会被渲染为红色。


需要修改 ofApp.cpp 中的代码
```c++
void ofApp::setup(){
	
	mesh.addVertex(glm::vec3(-1.0, 1.0, 0.0));
	mesh.addVertex(glm::vec3(-1.0, -1.0f, 0.0));
	mesh.addVertex(glm::vec3(1.0, -1.0f, 0.0));
	shader.load("first_vertex.vert", "first_fragment.frag");
}
//--------------------------------------------------------------
/**
 * @brief 绘制场景的主要函数。
 *
 * 此函数负责使用指定的着色器绘制网格。
 * 它调用着色器的 begin() 方法来激活着色器，
 * 然后绘制网格，最后调用着色器的 end() 方法来停用着色器。
 */
void ofApp::draw(){
    // 开始使用着色器，准备进行渲染
    shader.begin();
    // 绘制之前设置好的网格
    mesh.draw();
    // 结束使用着色器，完成渲染
    shader.end();
}
```		





## 使用顶点添加颜色 
运行效果
![](https://easyimage.elyt.cn/i/2025/03/25/5716250202667488061-2.webp)

### 插值
**插值的基本概念**
在渲染图形时，我们通常只定义了少量的顶点及其属性，例如顶点的位置、颜色、法线等。然而，在屏幕上最终显示的图形是由大量的片元（Fragment）组成的，这些片元需要根据顶点的属性来确定自身的属性值。插值就是用来计算这些位于顶点之间的片元属性值的方法。

**为什么需要插值**
以绘制一个三角形为例，我们只定义了三个顶点的属性，但三角形内部有无数个片元，每个片元都需要有自己的属性值（如颜色、纹理坐标等）才能正确渲染。通过插值，我们可以根据顶点的属性来计算出每个片元的属性值，从而实现平滑的过渡效果。

**插值的工作原理**
在GPU中，当进行渲染时，会根据三角形的三个顶点的属性来计算每个片元的属性值。具体来说，对于每个片元，GPU会考虑它所在的面（由三个顶点构成），并从这三个顶点获取属性数据，然后按照一定的权重混合这些数据，得到该片元的属性值。这个混合过程是线性的，也就是说，属性值在顶点之间是线性变化的。

**示例**
我们有一个三角形，三个顶点分别设置了不同的颜色（如红色、绿色、蓝色），在渲染这个三角形时，GPU会对每个片元进行插值计算。例如，对于三角形内部靠近红色顶点的片元，其颜色会更接近红色；而靠近绿色和蓝色顶点的片元，颜色会相应地偏向绿色和蓝色。通过这种方式，三角形内部的颜色会呈现出平滑的过渡效果。

**顶点着色器和片元着色器中的插值**
- 顶点着色器 ：主要负责处理顶点的位置和属性，通常会将顶点的属性传递给片元着色器。
- 片元着色器 ：接收经过插值后的属性值，并根据这些值来计算每个片元的最终颜色。
![](https://easyimage.elyt.cn/i/2025/03/25/5716261406790129897-2.webp)

### 顶点信息
在 OpenFrameworks 里， ofMesh 对象可用于存储和管理三维模型的顶点、颜色、纹理坐标等数据，这意味着每个顶点可以包含以下几类信息：

#### 1. 位置信息
顶点的位置信息用 glm::vec3 类型来表示，它定义了顶点在三维空间里的位置。
```c++
mesh.addVertex(glm::vec3(0.0, 0.0, 0.0));
```
在顶点着色器中，通过 layout (location=0) in vec3 position; 来定义顶点位置信息的输入变量。

#### 1.4.0.1. 颜色信息
顶点的颜色信息用 glm::vec4 类型来表示，它定义了顶点的 RGBA 颜色值。
```c++
mesh.addColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
```

在顶点着色器中，通过 layout (location=1) in vec4 color; 来定义顶点颜色信息的输入变量。
#### 1.4.0.2. 纹理坐标信息
纹理坐标信息用 glm::vec2 类型来表示，它定义了顶点在纹理图上的位置。
```c++
mesh.addTexCoord(glm::vec2(0.5, 0.5));
```
在顶点着色器中，通过 layout (location=2) in vec2 texCoord; 来定义纹理坐标信息的输入变量。
#### 1.4.0.3. 法线信息
法线信息用 glm::vec3 类型来表示，它定义了顶点的法线向量。
```c++
mesh.addNormal(glm::vec3(0.0, 0.0, 1.0));
```
在顶点着色器中，通过 layout (location=3) in vec3 normal; 来定义法线信息的输入变量。

### 1.4.1. ofApp.cpp
```c++
void ofApp::setup()
{
    // 添加三角形的第一个顶点，位于左上角 (-1, 1, 0)
    mesh.addVertex(glm::vec3(-1.0, 1.0, 0.0));
    // 添加三角形的第二个顶点，位于左下角 (-1, -1, 0)
    mesh.addVertex(glm::vec3(-1.0, -1.0f, 0.0));
    // 添加三角形的第三个顶点，位于右下角 (1, -1, 0)
    mesh.addVertex(glm::vec3(1.0, -1.0f, 0.0));
    // 为第一个顶点添加颜色，红色 (1, 0, 0)
    mesh.addColor(ofFloatColor(1.0, 0.0, 0.0));
    // 为第二个顶点添加颜色，绿色 (0, 1, 0)
    mesh.addColor(ofFloatColor(0.0, 1.0, 0.0));
    // 为第三个顶点添加颜色，蓝色 (0, 0, 1)
    mesh.addColor(ofFloatColor(0.0, 0.0, 1.0));
    
    // 加载顶点着色器和片段着色器
    shader.load("first_vertex.vert", "first_fragment.frag");
}

```

### 1.4.2. first_vertex.vert
```glsl
// 指定GLSL版本为4.1
#version 410
// 声明输入变量：顶点位置（从顶点缓冲区接收）
layout (location=0) in vec3 position;
layout (location=1) in vec4 color;
// 主函数：每个顶点都会执行一次

out vec4 fragCol;
void main()
{
    // 将3D位置转换为齐次坐标系（4D）
    // w分量设为1.0表示这是一个位置点（而不是方向向量）
    gl_Position=vec4(position,1.0);
    // 将颜色输出到片元着色器
    fragCol=color;
}
```


### 1.4.3. first_fragment.frag
```glsl
#version 410
in vec4 fragCol;
// 输出变量：片元颜色
out vec4 color;
// 主函数：每个片元都会执行一次
void main()
{
    // 设置片元颜色为红色
    //color=vec4(1.0,0.0,0.0,1.0);
    color=fragCol;
}

```

## 1.5. Uniform变量
在 GLSL（OpenGL Shading Language）中，uniform 是一种特殊的变量类型，用于在着色器程序和应用程序之间传递数据。

### 1.5.1. Uniform 的特点
1. 全局性: uniform 变量可以在顶点着色器和片段着色器中共享，而不需要通过 varying 传递。
2. 只读性: 在着色器运行期间，uniform 的值是只读的，不能被修改。
3. 固定值: uniform 的值在整个绘制调用期间保持不变，适用于所有顶点和片段。
4. 外部设置: uniform 的值由应用程序（例如 OpenFrameworks 或 OpenGL 应用程序）通过 API 设置。
### 1.5.2. Uniform 的用途
uniform 变量通常用于传递以下类型的数据：
1. 变换矩阵: 如模型矩阵（Model Matrix）、视图矩阵（View Matrix）、投影矩阵（Projection Matrix）。
2. 光照信息: 如光源位置、颜色、强度等。
3. 材质属性: 如漫反射颜色、镜面反射颜色、光泽度等。
4. 时间或动画参数: 如时间戳、动画进度等。
5. 纹理采样器: 如绑定到着色器的纹理单元。
6. 其他全局参数: 如分辨率、摄像机参数等。

### 1.5.3. Uniform 的设置
1. c++代码中调用setUniform1f(), setUniform2f(), setUniform3f(), setUniform4f() 等函数设置uniform浮点数变量。
2. 在着色器中使用 uniform 关键字声明变量。
3. **c++代码和着色器中使用相同的变量名。**
4. c++代码和着色器中使用相同的变量类型。

### 1.5.4. 将三角形的颜色设置为绿色
**运行结果**
![](https://easyimage.elyt.cn/i/2025/03/25/5716276735515824295-2.webp)
#### 1.5.4.1. ofApp.cpp
```c++
void ofApp::draw(){
    // 开始使用着色器，准备进行渲染
    shader.begin();
    // 设置着色器中的统一变量 "fragCol"，将其值设置为一个四维向量，代表颜色 (RGBA)
    // 这里的颜色为绿色（R=0.0, G=1.0, B=0.0），并且完全不透明（A=1.0）
    shader.setUniform4f("fragCol",glm::vec4(0.0, 1.0, 0.0, 1.0));
    // 绘制之前设置好的网格
    mesh.draw();
    // 结束使用着色器，完成渲染
    shader.end();
}
```
#### 1.5.4.2. vertex.vert
```glsl

// 指定GLSL版本为4.1
#version 410
// 声明输入变量：顶点位置（从顶点缓冲区接收）
layout (location=0) in vec3 position;
// 主函数：每个顶点都会执行一次
void main()
{
    // 将3D位置转换为齐次坐标系（4D）
    // w分量设为1.0表示这是一个位置点（而不是方向向量）
    gl_Position=vec4(position,1.0);
}
```

#### 1.5.4.3. fragment.frag
```glsl
#version 410

uniform vec4 fragCol; 
// 输出变量：片元颜色
out vec4 color;

// 主函数：每个片元都会执行一次
void main()
{
    // 设置片元颜色为红色
    //color=vec4(1.0,0.0,0.0,1.0);
    color=fragCol;

}
```


## 1.6. 常见问题
1. 找不到dll  
找不到libcrypto-3-x64.dll 和找不到FreeImage.dll  
清理项目，重新编译。 

![](https://easyimage.elyt.cn/i/2025/03/25/5716219178340326073-2.webp)  
![](https://easyimage.elyt.cn/i/2025/03/25/5716249310018935575-2.webp)  


