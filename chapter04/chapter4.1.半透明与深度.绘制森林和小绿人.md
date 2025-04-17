

# 1. 半透明与深度  
颜色通常由红（Red）、绿（Green）、蓝（Blue）三个通道组成，也就是 RGB 颜色模型。而 Alpha 通道则是在此基础上添加的第四个通道，形成了 RGBA 颜色模型。Alpha 通道的值一般处于 0.0 到 1.0 这个范围，其中：  

- 0.0 表示完全透明，即该颜色不可见。  
- 1.0 表示完全不透明，也就是正常显示颜色  

**Alpha 通道的应用场景**  
- 半透明效果 ：借助 Alpha 通道，能够实现物体的半透明效果，例如玻璃、烟雾等。  
- 遮罩效果 ：可以利用 Alpha 通道来创建遮罩，只显示部分区域的内容。  
- 淡入淡出效果 ：通过动态改变 Alpha 通道的值，能够实现物体的淡入淡出效果。  

## 1.1. 绘制森林和小人    

运行结果  

![](https://easyimage.elyt.cn/i/2025/03/28/5717463080833653493-2.webp)  

### 1.1.1. 绘制小人  
运行结果   
![](https://easyimage.elyt.cn/i/2025/03/28/5717388719271448846-2.webp)  

由于后续会有很多需要绘制矩形窗格，所以这里先定义一个函数来构建矩形网格。  

### 1.1.2. buildMesh

```cpp
/**
 * @brief 构建一个矩形网格
 * 
 * 该函数创建一个带有纹理坐标的矩形网格。矩形以指定位置为中心点，
 * 按给定的宽度和高度构建。网格包含4个顶点和6个索引(2个三角形)。
 * 
 * @param mesh 要构建的网格对象引用
 * @param w 矩形的宽度
 * @param h 矩形的高度  
 * @param pos 矩形中心点的位置
 */
void buildMesh(ofMesh& mesh, float w, float h, glm::vec3 pos)
{
	float verts[] = { -w/2 + pos.x, -h/2 + pos.y,  pos.z,
		-w/2 + pos.x,  h/2 + pos.y,  pos.z,
		w/2 + pos.x,  h/2 + pos.y,  pos.z,
		w/2 + pos.x, -h/2 + pos.y,  pos.z };

	float uvs[] = { 0,0, 0,1, 1,1, 1,0 };

	for (int i = 0; i < 4; ++i) {
		int idx = i * 3;
		int uvIdx = i * 2;

		mesh.addVertex(glm::vec3(verts[idx], verts[idx + 1], verts[idx + 2]));
		mesh.addTexCoord(glm::vec2(uvs[uvIdx], uvs[uvIdx + 1]));
	}

	ofIndexType indices[6] = { 0,1,2,2,3,0 };
	mesh.addIndices(indices, 6);
}
```
接下来，我们就可以绘制小绿人了。代码和之前章节没有什么区别，只是在绘制时，我们需要设置 Alpha 通道的值。
```cpp
void ofApp::setup()
{
	ofDisableArbTex();
	buildMesh(charMesh, 0.25, 0.5, glm::vec3(0.0, 0.15, 0.0));
	alienImg.load("alien.png");
	charShader.load("passthrough.vert", "alphaTest.frag");

}
//--------------------------------------------------------------
void ofApp::draw(){
	charShader.begin();
	charShader.setUniformTexture("greenMan", alienImg, 0);
	charMesh.draw();
	charShader.end();
}
```

### 1.1.3. passthrough.vert

```cpp
#version 410

layout (location = 0) in vec3 pos;
layout (location = 3) in vec2 uv;

out vec2 fragUV;

void main()
{
	gl_Position =  vec4( pos, 1.0);
	fragUV = vec2(uv.x, 1.0-uv.y);
}
```
### 1.1.4. alphaTest.frag

```cpp
#version 410
uniform sampler2D greenMan;
in vec2 fragUV;

out vec4 outCol;

void main()
{
	outCol = texture(greenMan, fragUV);
	if (outCol.a < 1.0) discard;
}
```

## 1.2. alpha 测试和丢弃
在前面的例子中有
```cpp
if (outCol.a < 1.0) discard;
```
这行代码，它的作用是判断当前像素的 Alpha 通道的值是否小于 1.0，如果小于 1.0，则将该像素丢弃，不进行绘制。
discard 是一个关键字，用于告诉 OpenGL 丢弃当前像素，终止当前流程，该片元永远不会前进到管线中的下一个阶段。

## 1.3. 使用深度测试构建场景  
![](https://easyimage.elyt.cn/i/2025/03/28/5717463080833653493-2.webp)

### 1.3.1. ofApp.cpp
```cpp
void ofApp::setup()
{
	ofDisableArbTex();
	// 构建角色网格
	buildMesh(charMesh, 0.25, 0.5, glm::vec3(0.0, -0.2, 0.0));
	alienImg.load("alien.png");
	// 构建森林网格
	buildMesh(forestMesh, 2.0, 2.0, glm::vec3(0.0, 0.0, 0.5));
	forestImg.load("forest.png");
	charShader.load("passthrough.vert", "alphaTest.frag");

	// 启用深度测试
	ofEnableDepthTest();
}
//--------------------------------------------------------------
void ofApp::draw()
{
	charShader.begin();
	if(false)
	{ //未启用深度测试
		//需要先绘制森林，否则角色会被遮挡
		charShader.setUniformTexture("tex", forestImg, 0);
		forestMesh.draw();
		charShader.setUniformTexture("tex", alienImg, 0);
		charMesh.draw();
		
	}
	else
	{
		//启用深度测试
		forestMesh.draw();
		charShader.setUniformTexture("tex", alienImg, 0);
		charMesh.draw();
		charShader.setUniformTexture("tex", forestImg, 0);
		forestMesh.draw();

	}
	charShader.end();
}

``` 
**在未加深度缓冲区时需要先绘制森林，否则角色会被遮挡**
**在加了深度缓冲区后，角色和森林的绘制顺序不影响最终效果**