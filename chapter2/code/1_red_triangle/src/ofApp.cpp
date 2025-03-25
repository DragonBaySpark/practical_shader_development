#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	//mesh.addVertex(glm::vec3(0, 0, 0.0));
	//mesh.addVertex(glm::vec3(0, 768.0f, 0.0));
	//mesh.addVertex(glm::vec3(1024, 768.0f, 0.0));
	
	mesh.addVertex(glm::vec3(-1.0f, 1.0f, 0.0f));
	mesh.addVertex(glm::vec3(-1.0f, -1.0f, 0.0f));
	mesh.addVertex(glm::vec3(1.0f, -1.0f, 0.0f));

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
    

    
    ofLoadIdentityMatrix();

    // 开始使用着色器，准备进行渲染
    shader.begin();
    // 绘制之前设置好的网格
    mesh.draw();
    // 结束使用着色器，完成渲染
    shader.end();
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
