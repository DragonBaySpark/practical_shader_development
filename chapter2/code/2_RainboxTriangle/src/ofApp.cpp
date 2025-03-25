#include "ofApp.h"

//--------------------------------------------------------------
/**
 * @brief 设置应用程序的初始状态。
 *
 * 此函数用于初始化应用程序的各种元素，包括创建一个三角形网格并加载着色器。
 * 它添加三角形的顶点和颜色，然后加载顶点着色器和片段着色器。
 */
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
