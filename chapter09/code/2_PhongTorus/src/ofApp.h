#pragma once

#include "ofMain.h"
struct CameraData {
	glm::vec3 position;
	float rotation;
	float fov;
};
struct DirectionalLightData {
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
};

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
	
		glm::vec3 getLightDirection(const  DirectionalLightData & lightData);

		ofMesh torusMesh;
		ofShader torusShader;

		CameraData cameraData;
		DirectionalLightData directionalLightData; // 平行光 
		

};
