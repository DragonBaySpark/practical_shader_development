#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	//窗口大小为 768x768 ，单位为像素，与纹理的宽高比一致，避免图像拉伸
	settings.setSize(300, 300); 
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	settings.setGLVersion(4, 1);// OPENGL 4.1
	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();

}
