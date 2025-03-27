#include "ofApp.h"

//--------------------------------------------------------------

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
    shader.load("vertex.vert","fragment.frag");
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
    // 设置图像纹理的环绕模式为GL_REPEAT，这意味着当纹理坐标超出[0, 1]范围时，纹理会重复显示
    // 第一个参数GL_REPEAT用于设置纹理的水平环绕模式
    // 第二个参数GL_REPEAT用于设置纹理的垂直环绕模式
    image.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
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
    shader.setUniform1f("time", ofGetElapsedTimef()); // Pass time as uniform dat
    mesh.draw();
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
