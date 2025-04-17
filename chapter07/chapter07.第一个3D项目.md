

# 1. 第一个3D项目
**运行结果**
![](https://easyimage.elyt.cn/i/2025/04/07/5721054595665368861-2.webp)  
## 1.1. 加载网格 
实际项目中，我们通常会使用3D建模软件（如Blender）来创建3D模型，然后导出成3D模型文件。常见的3D模型文件格式：
| 格式名称       | 扩展名       | 特点                                                                                     | 支持动画 | 支持材质与纹理 | 开源/专有 | 常见用途                           |
|----------------|--------------|------------------------------------------------------------------------------------------|----------|----------------|-----------|------------------------------------|
| OBJ            | .obj         | 简单、通用，支持顶点、法线和纹理坐标，不包含场景信息                                      | ❌        | ✅              | 开源      | 静态模型交换、基础建模             |
| FBX            | .fbx         | 功能强大，支持复杂动画、骨骼绑定、材质等，广泛用于行业工具间的数据交换                  | ✅        | ✅              | 专有      | 游戏开发、影视制作、动画           |
| STL            | .stl         | 仅存储几何信息（三角网格），主要用于3D打印                                               | ❌        | ❌              | 开源      | 3D打印、快速原型设计               |
| COLLADA (DAE)  | .dae         | XML格式，跨平台，支持复杂场景、动画和材质                                                | ✅        | ✅              | 开源      | 跨软件数据交换、游戏开发           |
| glTF           | .gltf / .glb | 高效、轻量级，适合实时渲染，支持PBR材质、动画和场景图                                    | ✅        | ✅              | 开源      | Web 3D、实时渲染、AR/VR 应用       |
| 3DS            | .3ds         | 老式格式，支持基本几何、材质和简单动画                                                   | ✅        | ✅              | 专有      | 早期3D建模软件间的兼容性           |
| PLY            | .ply         | 存储顶点和面信息，支持颜色和法线属性，常用于点云和扫描数据                               | ❌        | ❌              | 开源      | 科学计算、点云处理                 |
| X3D            | .x3d         | XML格式，继承自VRML，支持交互性和复杂场景                                                 | ✅        | ✅              | 开源      | Web 3D、教育、科学研究             |
| STEP           | .step / .stp | 工业标准，精确描述几何形状，用于CAD系统                                                  | ❌        | ❌              | 开源      | 工程设计、制造                     |
| IGES           | .igs / .iges | 工业标准，用于CAD数据交换，支持复杂曲面和实体                                           | ❌        | ❌              | 开源      | 工程设计、制造业                   |
| Blend          | .blend       | Blender原生格式，包含所有场景数据（几何、动画、材质等），但与其他软件兼容性较差          | ✅        | ✅              | 开源      | Blender项目保存、内部开发          |
| USD / USDZ     | .usd / .usdz | 高效、可扩展，支持复杂场景、动画和材质，优化用于AR/VR                                   | ✅        | ✅              | 开源      | AR/VR、电影制作、实时渲染          |

本书以ply格式的网格文件为例进行讲解。

### 1.1.1. ply格式
ply格式是一种文本格式，用于存储几何数据。ply文件由三部分组成：文件头、顶点数据和面数据。
以下是一个ply文件的示例：
```c++
ply
format ascii 1.0
comment Created by Blender 2.78 (sub 0) - www.blender.org, source file: ''
element vertex 2888
property float x
property float y
property float z
property float nx
property float ny
property float nz
property float s
property float t
element face 4608
property list uchar uint vertex_indices
end_header
1.230615 0.000000 0.062500 0.966216 0.000000 0.257637 0.885800 0.533702
1.236315 0.081032 0.000000 0.997833 0.065401 0.000000 0.876726 0.518244
1.227978 0.080486 0.062500 0.964141 0.063173 0.257637 0.889176 0.520619
1.220087 0.160628 0.062500 0.957945 0.126102 0.257637 0.892807 0.508354
1.196345 0.157502 0.120813 0.859737 0.113163 0.498032 0.905209 0.510896
1.204082 0.078920 0.120813 0.865291 0.056703 0.498032 0.901627 0.522993
```

openFrameworks提供了ofMesh类来加载ply文件。ofMesh类是一个三维网格类，用于存储顶点和面信息。ofMesh类提供了load方法来加载ply文件。load方法的参数是ply文件的路径。
```c++
ofMesh mesh;
mesh.load("mesh.ply");
```
## 1.2. 透视投影  
透视投影是一种常见的投影方式，它可以将3D场景映射到2D屏幕上。透视投影可以将远处的物体缩小，而近处的物体放大，从而产生近大远小的效果。
glm::perspective函数用于创建透视投影矩阵。该函数接受四个参数：
1. fovy：视角，单位为弧度。
2. aspect：宽高比。
3. near：近平面距离。
4. far：远平面距离。
5. 返回值：透视投影矩阵。
```c++
glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
```

## 1.3. ofApp.cpp
```c++
void ofApp::setup()
{

	ofDisableArbTex();
	ofEnableDepthTest();
	torusMesh.load("torus.ply");
	torusShader.load("torus.vert", "torus.frag");
	
}
//--------------------------------------------------------------

void ofApp::draw()
{
	using namespace glm;
	cameraData.position=vec3(0, 0, 1);
	cameraData.fov=radians(90.f);
	float aspect=1024.0f/768.0f;

	mat4 model=glm::rotate(1.0f,vec3(1,1,1))*scale(vec3(0.5,0.5,0.5));
	mat4 view=glm::inverse(glm::translate(cameraData.position));
	mat4 proj=glm::perspective(cameraData.fov,aspect,0.01f,100.0f);
	mat4 mvp=proj*view*model;

	torusShader.begin();
	torusShader.setUniformMatrix4f("mvp", mvp);
	torusMesh.draw();
	torusShader.end();
}
```
## 1.4. torus.vert
```c++
#version 410

layout (location = 0) in vec3 pos;
layout (location = 3) in vec2 uv;

uniform mat4 mvp;
out vec2 fragUV;

void main()
{
	gl_Position =  vec4( pos, 1.0);
	gl_Position = mvp * gl_Position;
	fragUV = vec2(uv.x, 1.0-uv.y);
}
```

## 1.5. torus.frag
```c++
#version 410

in vec2 fragUV;

out vec4 outCol;

void main()
{
	outCol = vec4(fragUV, 0.0, 1.0);

}
```
**重点**
1. c++中mvp矩阵的计算
2. 顶点着色器中gl_Position的计算，以及mvp矩阵的传递，从而得到该顶点在屏幕上的位置。
3. 片元着色器中采用fragUV作为片元的颜色。
