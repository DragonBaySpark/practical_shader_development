**OpenGLshader开发实战学习笔记：第三章 使用纹理**

# 1. 使用纹理
在计算机图形学的着色器（Shader）里，纹理（Texture）是一种至关重要的元素，它能够为渲染的物体增添丰富的细节和真实感。  
**定义**
纹理本质上是一个图像，它可以是二维的（2D纹理）、三维的（3D纹理），甚至是立方体映射（Cube Map）。在渲染时，纹理会被“贴”到物体表面，从而让物体呈现出各种外观，比如木头、石头、金属等材质的质感。 

在三维网格上使用的纹理通常看起来不像它们最终要使用的对象。但**不论纹理要应用到哪个网格，着色器的代码是一样的**。 

**工作原理**
- 纹理坐标 ：为了把纹理正确地映射到物体表面，每个顶点都需要有对应的纹理坐标（Texture Coordinates），也被称作UV坐标。纹理坐标是一个二维向量，其取值范围通常是从 (0, 0) 到 (1, 1) ， (0, 0) 代表纹理的左下角， (1, 1) 代表纹理的右上角。
- 插值 ：在渲染过程中，GPU会对顶点的纹理坐标进行插值计算，从而得到每个片元（Fragment）的纹理坐标。这样，每个片元就能根据自己的纹理坐标从纹理图像中采样到对应的颜色值。
- 采样 ：根据片元的纹理坐标，从纹理图像中获取对应的颜色值，这个过程叫做纹理采样（Texture Sampling）。在着色器中，通常会使用内置的采样函数（如 texture() ）来完成这个操作

**示例**

![](https://easyimage.elyt.cn/i/2025/03/27/5716886517813615709-2.webp)  
上图中左边的纹理用于三维网格；右边的纹理用于二维平面  
## 1.1. 创建四边形
我们要显示的纹理是一个四边形，我们需要创建一个四边形网格，然后为每个顶点指定纹理坐标。

![](https://easyimage.elyt.cn/i/2025/03/27/5716914505343048905-2.webp)  
上图是由四个顶点组成的四边形，其本质是两个三角形组成，**大多数游戏引擎都使用只有三角形面的网格**。
这两个三角形的存贮可以有以下方式：
1. 采用6个顶点坐标，分别是两个三角形的3个顶点坐标，当然有2个顶点坐标是重复的
2. 采用4个顶点坐标，其中两个顶点坐标是重复的
第二种方式更节省空间，因为重复的顶点坐标只需要存储一次。因此，我们采用一种称为索引数组的方式来存储顶点的索引，索引数组是一个整数数组，其中每个元素都是一个顶点的索引。即采用顶点0,1,2 及顶点2,3,0 来组成两个三角形面。**均采用逆时针顺序来定义三角形面**。



### 1.1.1. 代码
以下是运行结果  
![](https://easyimage.elyt.cn/i/2025/03/27/5716914086688594428-2.webp)  

#### 1.1.1.1. ofApp.cpp
```cpp

/**
 * @brief 初始化场景，创建一个带有颜色的四边形网格
 *
 * 该函数完成以下工作:
 * 1. 添加四个顶点构成一个四边形(-1,-1)到(1,1)
 * 2. 为每个顶点设置不同的颜色(红、绿、蓝、白)
 * 3. 添加索引定义两个三角形组成四边形
 * 4. 加载顶点着色器和片段着色器
 */
void ofApp::setup()
{
    // 添加第一个顶点，位于四边形左下角 (-1, -1, 0)
    mesh.addVertex(glm::vec3(-1.0, -1.0f, 0.0));
    // 添加第二个顶点，位于四边形右下角 (1, -1, 0)
    mesh.addVertex(glm::vec3(1.0, -1.0f, 0.0));
    // 添加第三个顶点，位于四边形右上角 (1, 1, 0)
    mesh.addVertex(glm::vec3(1.0, 1.0f, 0.0));
    // 添加第四个顶点，位于四边形左上角 (-1, 1, 0)
    mesh.addVertex(glm::vec3(-1.0, 1.0f, 0.0));
    // 为第一个顶点添加颜色，红色 (1, 0, 0)
    mesh.addColor(ofFloatColor(1.0, 0.0, 0.0));
    // 为第二个顶点添加颜色，绿色 (0, 1, 0)
    mesh.addColor(ofFloatColor(0.0, 1.0, 0.0));
    // 为第三个顶点添加颜色，蓝色 (0, 0, 1)
    mesh.addColor(ofFloatColor(0.0, 0.0, 1.0));
    // 为第四个顶点添加颜色，白色 (1, 1, 1)
    mesh.addColor(ofFloatColor(1.0, 1.0, 1.0));

    // 定义一个包含 6 个索引的数组，用于描述两个三角形的顶点连接顺序
    ofIndexType indices[6] = { 0, 1, 2,  2, 3 ,0};
    // 将索引数组添加到网格中，指定使用 6 个索引
    mesh.addIndices(indices, 6);
    
    // 加载顶点着色器和片段着色器
    shader.load("vertex.vert", "fragment.frag");
}
```
#### 1.1.1.2. vertex.vert
```cpp

// 指定GLSL版本为4.1
#version 410
// 声明输入变量：顶点位置（从顶点缓冲区接收）
layout (location=0) in vec3 position;
layout (location=1) in vec4 color;

out vec4 fragCol;
// 主函数：每个顶点都会执行一次
void main()
{
    // 将3D位置转换为齐次坐标系（4D）
    // w分量设为1.0表示这是一个位置点（而不是方向向量）
    gl_Position=vec4(position,1.0);
    // 将颜色输出到片元着色器
    fragCol=color;
}
```
#### 1.1.1.3. fragment.frag
```cpp
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

## 1.2. UV坐标
纹理坐标是一个二维向量，其取值范围通常是从 (0, 0) 到 (1, 1) ， (0, 0) 代表纹理的左下角， (1, 1) 代表纹理的右上角。
到目前为止，我们接触了以下坐标：
1. 屏幕像素坐标 ：屏幕上的每个像素都有一个对应的坐标，通常是从 (0, 0) 到 (屏幕宽度, 屏幕高度) 。
2. 归一化设备坐标 ：OpenGL使用的坐标系，其范围是从 (-1, -1) 到 (1, 1) 。
3. UV坐标 ：纹理坐标，其范围是从 (0, 0) 到 (1, 1) 。

下图为 屏幕像素坐标
![](https://easyimage.elyt.cn/i/2025/03/25/5716163769114041051-2.webp)  

下图为 归一化设备坐标
![](https://easyimage.elyt.cn/i/2025/03/25/5716221863273700688-2.webp)  

下图为 UV坐标
![](https://easyimage.elyt.cn/i/2025/03/27/5716932415708993723-2.webp)  

UV坐标与纹理的映射关系  
![](https://easyimage.elyt.cn/i/2025/03/27/5716934287517815247-2.webp)  
比如我们用的鹦鹉的纹理，其大小为 300x300，那么其UV坐标范围就是 (0, 0) 到 (1, 1) 。
鹦鹉的纹理左下角映射到四边形的左下角，其UV坐标就是 (0, 0) 。
如果四边形的宽高比与鹦鹉的宽高比不一致，那么鹦鹉的纹理就会被拉伸或压缩。

### 1.2.1. 运行结果 
![](https://easyimage.elyt.cn/i/2025/03/27/5716994276085210583-2.webp)  
显示的纹理是上下颠倒的，这个在后面会解决。
**ofDisableArbTex();**
是一个用于禁用 ARB 纹理模式的工具，它可以帮助开发者确保纹理坐标在 [0, 1] 范围内正常工作，避免因纹理坐标系统不一致导致的纹理映射问题
#### 1.2.1.1. main.cpp
```cpp
int main( ){

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	//窗口大小为 768x768 ，单位为像素，与纹理的宽高比一致，避免图像拉伸
	settings.setSize(768, 768); 
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	settings.setGLVersion(4, 1);// OPENGL 4.1
	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();

}
```

#### 1.2.1.2. ofApp.cpp
```cpp
/**
 * @brief 初始化OpenFrameworks应用程序
 *
 * 该函数完成以下初始化工作:
 * - 禁用ARB纹理模式以确保纹理坐标正常工作
 * - 加载顶点和片段着色器
 * - 加载图像文件
 * - 创建一个包含4个顶点的矩形网格
 * - 为顶点设置纹理坐标
 * - 添加三角形索引以构建矩形
 *
 * 这是应用程序启动时自动调用的设置函数。它构建了一个基本的纹理渲染网格,
 * 用于后续在OpenGL中进行2D纹理渲染。
 */
void ofApp::setup()
{
    // 禁用ARB纹理模式，确保纹理坐标在[0, 1]范围内正常工作
    ofDisableArbTex();
    // 加载顶点着色器文件a.vert和片段着色器文件a.frag
    shader.load("a.vert","a.frag");
    // 加载名为parrot.png的图像文件
    image.load("parrot.png");

    // 添加四边形的四个顶点
    // 左下角顶点
    mesh.addVertex(glm::vec3(-1,-1,0));
    // 右下角顶点
    mesh.addVertex(glm::vec3(1,-1,0));
    // 右上角顶点
    mesh.addVertex(glm::vec3(1,1,0));
    // 左上角顶点
    mesh.addVertex(glm::vec3(-1,1,0));

    // 为四个顶点添加纹理坐标
    // 左下角纹理坐标
    mesh.addTexCoord(glm::vec2(0,0));
    // 右下角纹理坐标
    mesh.addTexCoord(glm::vec2(1,0));
    // 右上角纹理坐标
    mesh.addTexCoord(glm::vec2(1,1));
    // 左上角纹理坐标
    mesh.addTexCoord(glm::vec2(0,1));

    // 定义两个三角形的索引，用于构成四边形
    ofIndexType indices[] = {
        0,1,2,2,3,0
    };

    // 将索引添加到网格中
    mesh.addIndices(indices,6);
}

/**
 * @brief 绘制函数,负责渲染场景
 *
 * 该函数使用着色器对网格进行渲染:
 * 1. 激活着色器程序
 * 2. 设置纹理采样器uniform变量
 * 3. 绘制网格
 * 4. 结束着色器使用
 */
void ofApp::draw()
{

    // 开始使用着色器，准备进行渲染
    shader.begin();
    shader.setUniformTexture("tex", image, 0);
    mesh.draw();

    
    shader.end();
}
```
#### 1.2.1.3. vertex.vert
```cpp
// 指定GLSL版本为4.1
#version 410
// 声明输入变量：顶点位置（从顶点缓冲区接收）
layout (location=0) in vec3 position;
layout (location=3) in vec2 uv;

out vec2 fragUV;
// 主函数：每个顶点都会执行一次
void main()
{
    // 将3D位置转换为齐次坐标系（4D）
    // w分量设为1.0表示这是一个位置点（而不是方向向量）
    gl_Position=vec4(position,1.0);
    // 将2D纹理坐标传递给片元着色器
    fragUV=uv;
}
```
#### 1.2.1.4. fragment.frag
```cpp
#version 410
// 添加 location 限定符来满足 SPIR-V 的要求
layout (location = 0) in vec2 fragUV;

uniform sampler2D tex;
// 输出变量：片元颜色
out vec4 color;
// 主函数：每个片元都会执行一次
void main()
{
    color=texture(tex,fragUV);
   
}
```

### 1.2.2. 鹦鹉倒置的原因
OpenGL 期望图像数据从最下面一行像素开始存储，但几乎每种 2D 图像格式都首先存储最上面一行的内容。将纹理倒置是在编写图形代码时会遇到的常见问题。
由于 UV 坐标从 0 到 1，因此翻转图像的 UV 所需要做的一切
coordinates 是从 1.0 中减去 Y 分量。这意味着我们图像的底部（以前是 Y 坐标 0）现在将变为 Y 坐标 1，反之亦然。经过这个小改动，图像现在将正确显示。  
![](https://easyimage.elyt.cn/i/2025/03/27/5716997924064337855-2.webp)  
#### 1.2.2.1. vertex.vert
```cpp
//fragUV=uv; // 直接传递纹理坐标，纹理显示时将会倒置
fragUV=vec2(uv.x,1.0-uv.y); // 翻转纹理坐标的y分量
```

## 1.3. 滚动UV坐标
我们可以通过改变UV坐标来实现滚动效果。  
![](https://easyimage.elyt.cn/i/2025/03/27/5716999962265393582-2.webp)  
#### 1.3.0.1. vertex.vert
```cpp
// 指定GLSL版本为4.1
#version 410
// 声明输入变量：顶点位置（从顶点缓冲区接收）
layout (location=0) in vec3 position;
layout (location=3) in vec2 uv;

out vec2 fragUV;
// 主函数：每个顶点都会执行一次
void main()
{
    // 将3D位置转换为齐次坐标系（4D）
    // w分量设为1.0表示这是一个位置点（而不是方向向量）
    gl_Position=vec4(position,1.0);
    // 将2D纹理坐标传递给片元着色器
    //fragUV=uv; // 直接传递纹理坐标，纹理显示时将会倒置
    fragUV=vec2(uv.x,1.0-uv.y)+vec2(0.25,0.0); // 翻转纹理坐标的y分量
}
```

## 1.4. 纹理的 wrap 模式
对于使用大于 1 的水平 UV 坐标的片段，纹理最右侧边缘的颜色似乎是重复的。在后台，GPU 正在固定我们的顶点坐标，以将它们保持在适当的范围内。这意味着，任何尝试对 X 值大于 1.0 的纹理坐标进行采样的片段都会将该值固定到最大有效纹理坐标 （1.0）。在图形术语中，纹理对使用非 0-1 坐标采样的响应方式称为 wrap mode。
常见的 wrap 模式有 repeat 、 clamp 、 mirrored repeat 等。
### 1.4.1. repeat
repeat 模式是默认的 wrap 模式，它会重复纹理的边缘，从而形成一个无缝的平铺效果。
当纹理坐标超出 [0, 1] 范围时，纹理会重复。也就是说，超出部分会使用纹理的相同内容进行填充，类似于将纹理平铺扩展。
若 UV 坐标为 (1.25, 0.0) ，会转换为 (0.25, 0.0) ，继续从纹理起始处开始采样，实现纹理的重复效果

### 1.4.2. clamp

任何超出 [0, 1] 范围的纹理坐标都会被固定到边界值。即尝试对 X 值大于 1.0 的纹理坐标进行采样的片段，会将该值固定到最大有效纹理坐标 (1.0) ；小于 0 的则固定到 0 。

若 UV 坐标为 (1.25, 0.0) ，会将其固定为 (1.0, 0.0) ，只采样纹理边界的颜色。

### 1.4.3. mirrored repeat

纹理坐标超出 [0, 1] 范围时，纹理会以镜像的方式重复。在奇数倍的整数边界处会进行镜像翻转，偶数倍时正常重复。

例如在 [1, 2] 区间内，纹理会以镜像的形式显示；在 [2, 3] 区间又会恢复正常显示，如此交替。

### 1.4.4. repeat 模式运行结果
![](https://easyimage.elyt.cn/i/2025/03/27/5717004959250128578-2.webp)  
```c++
void ofApp::setup()
{
    // other code...
    // 设置图像纹理的环绕模式为GL_REPEAT，这意味着当纹理坐标超出[0, 1]范围时，纹理会重复显示
    // 第一个参数GL_REPEAT用于设置纹理的水平环绕模式
    // 第二个参数GL_REPEAT用于设置纹理的垂直环绕模式
    image.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
}
```
## 1.5. 让纹理动起来
![](https://easyimage.elyt.cn/i/2025/03/27/5717017417566131148-2.gif)  

只需让纹理坐标在 [0, 1] 范围内循环移动，就可以实现纹理的滚动效果。

将时间变量 time 作为 uniform 传入着色器，然后使用它来计算纹理坐标。
```c++
void ofApp::draw()
{
    // 开始使用着色器，准备进行渲染
    shader.begin();
    shader.setUniformTexture("tex", image, 0);
    shader.setUniform1f("time", ofGetElapsedTimef()); // Pass time as uniform
    mesh.draw();
    shader.end();
}
```

#### 1.5.0.1. vertex.vert
```cpp 
// 指定GLSL版本为4.1
#version 410
uniform float time; // 从C++传入的时间变量
// 声明输入变量：顶点位置（从顶点缓冲区接收）
layout (location=0) in vec3 position;
layout (location=3) in vec2 uv;

out vec2 fragUV;
// 主函数：每个顶点都会执行一次
void main()
{
    // 将3D位置转换为齐次坐标系（4D）
    // w分量设为1.0表示这是一个位置点（而不是方向向量）
    gl_Position=vec4(position,1.0);
    // 将2D纹理坐标传递给片元着色器
    fragUV=vec2(uv.x,1.0-uv.y)+vec2(1.0f,0.0f)*time;  
}
```

## 1.6. 改变亮度
![](https://easyimage.elyt.cn/i/2025/03/27/5717024230449617853-2.webp)  
上图中左边是亮度*0.5的效果，右边是亮度*1.5的效果
```cpp
void main()
{
    //color=texture(tex,fragUV);
    color=texture(tex,fragUV)*0.5; // 亮度加大
    //color=texture(tex,fragUV)*0.5; // 亮度减小
   
}
```
可以通过在setup()函数中设置 ofDisableAlphaBlending();来关闭透明度混合。

## 1.7. 颜色加减法
均匀地修改颜色的组成部分会导致该颜色均匀地变得更亮或更暗。修改某些颜色通道而不是其他通道会生成全新的颜色

不论加法还是减法，结果会归一化到 [0, 1] 范围内。即超过 1.0 的值会归一化到 1.0，小于 0.0 的值会归一化到 0.0。  
![](https://easyimage.elyt.cn/i/2025/03/27/5717052485382708781-2.webp)  

### 1.7.1. 颜色加法
#### 1.7.1.1. ofApp.cpp
```c++
    shader.setUniform4f("multiply",glm::vec4(1,1,1,1));
    shader.setUniform4f("add", glm::vec4(0.5,0.5,0.5,1.0));
```
#### 1.7.1.2. fragment.frag
```cpp

#version 410
// 添加 location 限定符来满足 SPIR-V 的要求
layout (location = 0) in vec2 fragUV;


uniform sampler2D tex;
uniform vec4 multiply;  
uniform vec4 add;
// 输出变量：片元颜色
out vec4 color;
// 主函数：每个片元都会执行一次
void main()
{
    //首先执行乘法，然后执行加法。这是着色器代码中常见的优化。GPU 可以执行所谓的 “MAD” 运算，即 “乘法，然后加法”。     
    color=texture(tex,fragUV)*multiply+add; 
   
}
```
#### 1.7.1.3. glm::vec4(0.5,0.5,0.5,1.0)
```c++
    shader.setUniform4f("multiply",glm::vec4(1,1,1,1));
    shader.setUniform4f("add", glm::vec4(0.5,0.5,0.5,1.0));
```
![](https://easyimage.elyt.cn/i/2025/03/27/5717034697519798721-2.webp)  

#### 1.7.1.4. glm::vec4(0.25,0.25,0.25,1.0)
```c++
    shader.setUniform4f("multiply",glm::vec4(1,1,1,1));
    shader.setUniform4f("add", glm::vec4(0.25,0.25,0.25,1.0));
```
![](https://easyimage.elyt.cn/i/2025/03/27/5717035487995109022-2.webp)  

#### 1.7.1.5. glm::vec4(1.0,0.0,0.0,1.0)
```c++
    shader.setUniform4f("multiply",glm::vec4(1,1,1,1));
    shader.setUniform4f("add", glm::vec4(1.0,0.0,0.0,1.0));
```
![](https://easyimage.elyt.cn/i/2025/03/27/5717035807546546181-2.webp)  


## 1.8. mix() 函数
用于在两个值之间进行线性插值。
```cpp
/**函数原型：表示插值因子，取值范围通常在 [0, 1] 之间。当 a 为 0 时，返回 x ；当 a 为 1 时，返回 y ；当 a 在 (0, 1) 之间时，返回 x 和 y 之间的线性插值结果
**/
float mix(float x, float y, float a) {
    return x * (1.0 - a) + y * a;
}
```
实验：
将两个纹理混合在一起，其中一个是黑白棋盘格纹理，另一个是鹦鹉纹理。
下图是黑白棋盘格纹理  
![](https://easyimage.elyt.cn/i/2025/03/27/5717062754188139655-2.webp)  

下图是混合后的纹理  
![](https://easyimage.elyt.cn/i/2025/03/27/5717060848703249181-2.webp)  

本质上是颜色的线性插值，即颜色的混合。

```cpp
#version 410
// 添加 location 限定符来满足 SPIR-V 的要求
layout (location = 0) in vec2 fragUV;

uniform sampler2D checkerTex;
uniform sampler2D parrotTex;

// 输出变量：片元颜色
out vec4 color;
// 主函数：每个片元都会执行一次
void main()
{
    vec4 color1=texture(checkerTex,fragUV);
    vec4 color2=texture(parrotTex,fragUV);
    color=mix(color1,color2,color1.r);
}
```
采用 color=mix(color1,color2,color1.r); 方式时，当 color1.r 为 1 时，返回 color2；当 color1.r 为 0 时，返回 color1。
而黑白棋盘格纹理的 白色部分r 分量为 1，因此返回的是鹦鹉纹理。
而黑色部分r 分量为 0，因此返回的是黑白棋盘格纹理（即黑色纹理）。