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

glm::vec3 ofApp::getLightDirection(const  DirectionalLightData & lightData) {
	using namespace glm;
	return normalize(lightData.direction*-1.0f); // 注意，这里的方向是反的，因为我们的光源是指向场景的
}

void ofApp::setup()
{

	ofDisableArbTex();
	ofEnableDepthTest();
	torusMesh.load("torus.ply");
	torusShader.load("mesh.vert", "diffuse.frag");

	directionalLightData.direction = glm::vec3(1.0f, -1.0f, 0.0f); 
	directionalLightData.color = glm::vec3(1.0f, 1.0f, 1.0f); // 白色
	directionalLightData.intensity = 1.0f; // 强度		
}
//--------------------------------------------------------------

void ofApp::draw()
{
	using namespace glm;
	cameraData.position=vec3(0, 0.0f, 1.0f);
	cameraData.fov=radians(90.f);
	float aspect=1024.0f/768.0f;

	//mat4 model=glm::rotate(1.0f,vec3(1,1,1))*scale(vec3(0.5,0.5,0.5));
	mat4 model = glm::rotate(glm::radians(-45.0f), vec3(1,0, 0)) * scale(vec3(0.5, 0.5, 0.5));
	//mat4 model = glm::mat4(1.0f)*scale(vec3(0.5, 0.5, 0.5));
	mat4 view=glm::inverse(glm::translate(cameraData.position))*rotate(glm::radians(-10.0f), vec3(1,0, 0));
	mat4 proj=glm::perspective(cameraData.fov,aspect,0.01f,100.0f);
	mat4 mvp=proj*view*model;

	vec3 lightDirection = getLightDirection(directionalLightData); // 获取光源方向


	mat3 normalMatrix = glm::transpose(glm::inverse(mat3(model)));
	torusShader.begin();
	torusShader.setUniformMatrix4f("mvp", mvp);
	torusShader.setUniformMatrix3f("normalMatrix", normalMatrix);

	torusShader.setUniform3f("lightDir", lightDirection);
	torusShader.setUniform3f("lightCol", directionalLightData.color*directionalLightData.intensity);

	torusShader.setUniform3f("meshCol", vec3(1,0,0));
	torusShader.setUniform3f("meshSpecCol", vec3(1,1,1));
	torusShader.setUniformMatrix4f("setUniformMatrix4f", model); // 模型矩阵
	torusShader.setUniform3f("rimCol", vec3(0,1,0)); //轮廓颜色
	torusShader.setUniform3f("cameraPos",cameraData.position); //相机位置

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
