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


//--------------------------------------------------------------
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
//--------------------------------------------------------------
void ofApp::update(){

}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
