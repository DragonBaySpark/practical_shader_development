#include "ofApp.h"

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
void buildMesh(ofMesh &mesh, float w, float h, glm::vec3 pos)
{
	float verts[] = {-w / 2 + pos.x, -h / 2 + pos.y, pos.z,
					 -w / 2 + pos.x, h / 2 + pos.y, pos.z,
					 w / 2 + pos.x, h / 2 + pos.y, pos.z,
					 w / 2 + pos.x, -h / 2 + pos.y, pos.z};

	float uvs[] = {0, 0, 0, 1, 1, 1, 1, 0};

	for (int i = 0; i < 4; ++i)
	{
		int idx = i * 3;
		int uvIdx = i * 2;

		mesh.addVertex(glm::vec3(verts[idx], verts[idx + 1], verts[idx + 2]));
		mesh.addTexCoord(glm::vec2(uvs[uvIdx], uvs[uvIdx + 1]));
	}

	ofIndexType indices[6] = {0, 1, 2, 2, 3, 0};
	mesh.addIndices(indices, 6);
}

void ofApp::setup()
{

	ofDisableArbTex();
	buildMesh(charMesh, 0.25, 0.5, glm::vec3(0.0, -0.2, 0.0));
	alienImg.load("alien.png");
	buildMesh(forestMesh, 2.0, 2.0, glm::vec3(0.0, 0.0, 0.5));
	forestImg.load("forest.png");
	charShader.load("passthrough.vert", "alphaTest.frag");
	buildMesh(cloudMesh, 0.5, 0.24, glm::vec3(-0.55, 0.0, 0));
	cloudImg.load("cloud.png");
	cloudShader.load("cloud.vert", "cloud.frag");

	buildMesh(sunMesh,2.0, 2.0, glm::vec3(00, 0.0, 0));
	sunImg.load("sun.png");
	sunShader.load("sun.vert", "sun.frag");
}
//--------------------------------------------------------------

void ofApp::draw()
{

	ofEnableDepthTest();
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
//--------------------------------------------------------------
void ofApp::update()
{
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
