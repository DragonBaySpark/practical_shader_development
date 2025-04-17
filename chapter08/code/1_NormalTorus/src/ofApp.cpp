#include "ofApp.h"


glm::mat4 buildMatrix(glm::vec3 trans, float rot, glm::vec3 scale)
{
	using glm::mat4;
	mat4 translation = glm::translate(trans);
	mat4 rotation = glm::rotate(rot, glm::vec3(0.0, 0.0, 1.0));
	mat4 scaler = glm::scale(scale);
	return translation * rotation * scaler;
}

glm::mat4 buildViewMatrix(CameraData cam)
{
	using namespace glm;
	return inverse(buildMatrix(cam.position, cam.rotation, vec3(1, 1, 1)));
}

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

	//mat4 model=glm::rotate(1.0f,vec3(1,1,1))*scale(vec3(0.5,0.5,0.5));
	mat4 model = glm::rotate(glm::radians(30.0f), vec3(0,1, 0)) * scale(vec3(0.5, 0.5, 0.5));
	//mat4 model = glm::mat4(1.0f)*scale(vec3(0.5, 0.5, 0.5));
	mat4 view=glm::inverse(glm::translate(cameraData.position));
	mat4 proj=glm::perspective(cameraData.fov,aspect,0.01f,100.0f);
	mat4 mvp=proj*view*model;

	torusShader.begin();
	torusShader.setUniformMatrix4f("mvp", mvp);
	torusMesh.draw();
	torusShader.end();
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
