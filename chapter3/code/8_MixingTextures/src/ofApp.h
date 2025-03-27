#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

private:
		// 定义一个ofMesh对象，用于存储和处理三维网格数据
		ofMesh mesh;
		// 定义一个ofShader对象，用于加载和使用着色器程序
		ofShader shader;
		// 定义一个ofImage对象，用于加载和显示图像
		ofImage checkerImage;
		ofImage parrotImage;
		
};
