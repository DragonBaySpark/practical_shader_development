# 1. 第一个光照模型

## 1.1. 镜面光照 
镜面光照（Specular Lighting）是计算机图形学中用于模拟物体表面反射高光的一种光照模型。当光线照射到光滑表面时，会以特定角度反射出去。如果观察者恰好处于反射光的路径上，就会看到明亮的高光区域，这就是镜面光照所模拟的效果。
镜面光照的计算通常依赖于多个因素，包括光源的方向、观察者的方向、物体表面的法线方向以及物体表面的光泽度。光泽度越高，高光区域就越小且越亮；反之，光泽度越低，高光区域就越大且越暗淡。
在实际应用中，镜面光照常与环境光、漫反射光照结合使用，以创建更真实的光照效果。常见的镜面光照模型有 Phong 模型和 Blinn - Phong 模型等。

![](https://easyimage.elyt.cn/i/2025/04/12/5722831975018928490-2.webp)  
3个不同示例的镜面光照效果
请注意，左侧的立方体与漫射光照的立方体没有任何不同，这是因为没有光线以适当的角度撞击立方体，然后反射到游戏摄像机中，所以我们在网格上看不到任何镜面反射光。

如果我们恰好从光线反射的方向观看一个高光物体，最终会看到一个非常明亮的镜面高光，当你的视角与与光线发射 的方向不太平行时，该高光就会消失。这与漫反射光照不同，因为漫反射光照不会因视角方向的不同而消失。

![](https://easyimage.elyt.cn/i/2025/04/12/5722833655819147634-2.webp)  
左侧球体仅使用漫反射光照 ，中心球体使用了镜面光照，右侧球体使用了镜面光照和漫反射光照。  

## 1.2. 镜面着色器  
![](https://easyimage.elyt.cn/i/2025/04/13/5723070937251714508-2.webp)  

我们首要的是计算光线在片元上的反射方向
```c++

vec3 reflectDir = reflect(-lightDir, norm);
```
传入的第一个参数是入射光向量，第二个参数是当前片元的法线向量。之所以取反，是因为我们在c++中向着色器传入的光线取反了。
下图是我们传入的入射光向量 。
![](https://easyimage.elyt.cn/i/2025/04/12/5722730637354340567-2.webp)  

而实际 reflect 第一个参数是下图这样的入射光向量  
![](https://easyimage.elyt.cn/i/2025/04/12/5722728321322586333-2.webp)    

```c++
vec3 toCam=(normalize(cameraPos-fragWorldPos));

```
我们计算反射方向后，就可以计算出其与toCam之间的点积，然后将这个点积值 提高N次幂，以便控制镜面反射高光的光泽度。指数越高，高光区域就越小且越亮；反之，指数越低，高光区域就越大且越暗淡。
```c++
float spec = pow(max(dot(toCam, reflectDir), 0.0), 32);
```

### 1.2.1. diffuse.frag
```c++
#version 410


uniform vec3 lightDir; // light directio
uniform vec3 lightCol; // light color
uniform vec3 meshCol;	// mesh color
uniform vec3 meshSpecCol; // mesh specular color
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
	vec3 reflectDir=normalize(reflect(-lightDir,normal));
	float specAmt=max(0.0,dot(reflectDir,toCam));
	specAmt=pow(specAmt,16.0f);
	outCol = vec4(lightCol*  meshSpecCol*specAmt , 1.0);
	

}
```


## 1.3. 组合漫反射和镜面反射光照  
![](https://easyimage.elyt.cn/i/2025/04/13/5723070199180041639-2.webp)   
关键代码：
得到漫反射光照
```c++
	float diffuse = max(0.0, dot(normal, lightDir));
	vec3 diffuseCol=meshCol*lightCol*diffuse;
```
得到镜面反射光照
```c++
	vec3 toCam=(normalize(cameraPos-fragWorldPos));
	vec3 reflectDir=normalize(reflect(-lightDir,normal));
	float specAmt=max(0.0,dot(reflectDir,toCam));
	specAmt=pow(specAmt,16.0f);
	vec3 specCol=meshSpecCol*lightCol*specAmt;
```
将漫反射和镜面反射光照相加
```c++
	outCol = vec4(diffuseCol+specCol, 1.0);
```

## 1.4. 环境光照  
环境光照（Ambient Lighting）是一种简单的光照模型，它模拟了光线被其他物体吸收的情况。这种光照通常用于增加场景的层次感，使场景看起来更加真实。
核心代码：
```c++
	vec3 ambient=meshCol*ambientCol;
	outCol = vec4(ambient+diffuseCol+specCol, 1.0);
```

这是使用了与背景颜色相同的环境光颜色(0.5,0.5,0.5)的效果。
![](https://easyimage.elyt.cn/i/2025/04/12/5722868161213111141-2.webp)  	

# 2. Phong光照模型
Phong光照模型是一种经典的光照模型，用于模拟真实世界中的光照效果，它将光照分为环境光（Ambient）、漫反射光（Diffuse）和镜面反射光（Specular）三个部分。最终的光照颜色是这三部分光照颜色的总和。下面详细介绍各部分的数学公式及整体公式。

### 2.0.1. 环境光（Ambient）
环境光模拟了场景中全局的、均匀的光照，它不依赖于光源的位置和物体的朝向。环境光的计算公式如下：
$$
I_{ambient} = k_a \times I_a
$$
其中：

- $I_{ambient}$ 是环境光的颜色强度。
- $k_a$ 是物体的环境光反射系数，取值范围通常在 $[0, 1]$ 之间，它表示物体对环境光的反射能力。
- $I_a$ 是环境光的颜色强度。
### 2.0.2. 漫反射光（Diffuse）
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
### 2.0.3. 镜面反射光（Specular）
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
### 2.0.4. Phong光照模型整体公式
最终的光照颜色 $I_{total}$ 是环境光、漫反射光和镜面反射光的总和：
$$
I_{total} = I_{ambient} + I_{diffuse} + I_{specular}
$$
将前面的公式代入可得：
$$
I_{total} = k_a \times I_a + k_d \times I_l \times \max(0, \mathbf{N} \cdot \mathbf{L}) + k_s \times I_l \times (\max(0, \mathbf{R} \cdot \mathbf{V}))^n
$$






## 2.1. 低光泽指数的镜面高光问题
低光泽指数的镜面高光问题是指，当光泽度较低时，会产生怪异的镜面反射效果。比如下图，光泽指数设置为0.5时，光照 似乎有一个锐利的轮廓。
![](https://easyimage.elyt.cn/i/2025/04/13/5723270787243187452-2.webp)  

## 2.2. Blinn-Phong光照模型
Blinn-Phong光照模型是Phong光照模型的改进版本，它通过将反射光线的计算从表面法线方向转移到从光源方向到观察者方向的中间向量上来解决低光泽指数的镜面高光问题。

### 2.2.1. 计算效率更高
- Phong模型的计算瓶颈 ：在Phong光照模型中，计算镜面反射光时需要先计算反射光线向量 $\mathbf{R}$，其计算公式为 $\mathbf{R} = 2(\mathbf{N} \cdot \mathbf{L})\mathbf{N} - \mathbf{L}$，其中 $\mathbf{N}$ 是法线向量，$\mathbf{L}$ 是从物体表面指向光源的向量。之后还要计算反射光线向量 $\mathbf{R}$ 与从物体表面指向观察者的向量 $\mathbf{V}$ 的点积 $(\max(0, \mathbf{R} \cdot \mathbf{V}))^n$。计算反射光线向量 $\mathbf{R}$ 涉及多次向量运算，在实时渲染中，当场景中有大量物体或复杂光照时，会带来较大的计算开销。
- Blin-Phong的优化 ：Blin-Phong模型引入了半程向量 $\mathbf{H}$，它是光源方向向量 $\mathbf{L}$ 和观察方向向量 $\mathbf{V}$ 的中间向量，计算公式为 $\mathbf{H} = \frac{\mathbf{L} + \mathbf{V}}{\vert\mathbf{L} + \mathbf{V}\vert}$。然后直接计算半程向量 $\mathbf{H}$ 与法线向量 $\mathbf{N}$ 的点积 $(\max(0, \mathbf{N} \cdot \mathbf{H}))^n$ 来得到镜面反射效果。相比Phong模型，Blin-Phong模型避免了反射光线向量 $\mathbf{R}$ 的计算，减少了向量运算次数，从而提高了计算效率。
### 2.2.2. 视觉效果更符合预期
- Phong模型的视觉缺陷 ：在Phong模型中，当观察者与光源的夹角较大时，镜面反射高光区域可能会变得非常小甚至消失，导致视觉效果不够自然。
- Blin-Phong的改进效果 ：Blin-Phong模型使用半程向量来计算镜面反射，在很多情况下能产生更平滑、更明显的高光效果。半程向量的引入使得高光区域在不同观察角度下变化更加自然，即使观察者与光源夹角较大时，也能保持较好的视觉效果，更符合人眼对光照的感知。
### 2.2.3. 实时渲染的需求
在游戏开发、虚拟现实等实时渲染场景中，需要在保证视觉效果的前提下尽可能提高渲染效率。Blin-Phong光照模型在计算效率和视觉效果之间找到了较好的平衡，能够在有限的硬件资源下实现高质量的光照渲染，因此被广泛应用于实时渲染领域

### 2.2.4. 片段着色器示例
```c++
#version 410

uniform vec3 lightDir; // 光源方向
uniform vec3 lightCol; // 光源颜色
uniform vec3 meshCol;  // 物体颜色
uniform vec3 cameraPos; // 相机位置

// 反射系数
uniform float ka; // 环境光反射系数
uniform float kd; // 漫反射系数
uniform float ks; // 镜面反射系数
uniform float shininess; // 高光指数

// 环境光颜色
uniform vec3 ambientLight;

in vec2 fragUV;
in vec3 fragNormal;
in vec3 fragWorldPos;

out vec4 outCol;

void main()
{
    // 归一化法线向量
    vec3 normal = normalize(fragNormal);
    // 归一化光源方向向量
    vec3 L = normalize(lightDir);
    // 计算观察方向向量
    vec3 V = normalize(cameraPos - fragWorldPos);
    // 计算半程向量
    vec3 H = normalize(L + V);

    // 环境光计算
    vec3 ambient = ka * ambientLight;

    // 漫反射计算
    float diff = max(dot(normal, L), 0.0);
    vec3 diffuse = kd * lightCol * diff;

    // 镜面反射计算
    float spec = pow(max(dot(normal, H), 0.0), shininess);
    vec3 specular = ks * lightCol * spec;

    // 最终颜色计算
    outCol = vec4((ambient + diffuse + specular) * meshCol, 1.0);
}

```	
![](https://easyimage.elyt.cn/i/2025/04/13/5723274061753096204-2.webp)  
为了得到相同大小的镜面反射高光，需要将Blinn-Phong模型中的光泽指数（Shininess）设置为Phong模型中的2-4倍。上图是光泽指数为2.0。由此看到更平滑了。

## 使用纹理控制光照   
本节我们采用两个纹理贴图，一个shied_diffuse.png是漫反射贴图，一个shied_spec.png是镜面反射贴图。

下图为漫反射贴图  
![](https://easyimage.elyt.cn/i/2025/04/14/5723608050611262452-2.webp)  

下图为镜面反射贴图

![](https://easyimage.elyt.cn/i/2025/04/14/5723608209445361362-2.webp)  

对于每一个片元，我们从shied_diffuse.png中采样出一个颜色作为meshCol，经过与光源的运算，得到最终的漫反射颜色。
对于每一个片元，我们从shied_spece.png中采样出一个颜色作为specCol，经过与光源的运算，得到最终的高光颜色。  
运行结果如下：  
![](https://easyimage.elyt.cn/i/2025/04/14/5723614884025868074-2.webp)  

### ofApp.cpp
```c++
void ofApp::draw()
{
	//其它代码

	shieldShader.setUniformTexture("diffuseTex", diffuseImage.getTexture(), 0); // 纹理
	shieldShader.setUniformTexture("specularTex", specImage.getTexture(), 1); // 纹理

	shieldMesh.draw();
	shieldShader.end();
}
```

### 2.2.5. diffuse.frag
```c++
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


in vec2 fragUV;
in vec3 fragNormal;
out vec4 outCol;
in vec3 fragWorldPos; //片元的世界坐标

void main()
{
	//outCol = vec4(fragUV, 0.0, 1.0);
	vec3 normal=normalize(fragNormal);

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
重点代码：
```c++
	vec3 meshCol=texture(diffuseTex,fragUV).rgb;
	vec3 meshSpecCol=texture(specularTex,fragUV).rgb;
```  
使用纹理贴图，从diffuseTex中采样出漫反射颜色，从specularTex中采样出高光颜色。
