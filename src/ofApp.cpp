#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	mesh.load("mesh.ply");
}

//--------------------------------------------------------------
void ofApp::update(){
  
}

//--------------------------------------------------------------
void ofApp::draw(){
	
  // background gradient
  ofBackgroundGradient(ofColor(64), ofColor(0));
  
  // set main color to white
  ofSetColor(255);
  
  // begin camera
	cam.begin();
	
  // set color to gray
	ofSetColor(ofColor::gray);
	
  // draw mesh wireframe
  mesh.drawWireframe();
	
  // set point size to 2
	glPointSize(2);
  
  // set color to white
	ofSetColor(ofColor::white);
	
  // draw all verts
  mesh.drawVertices();
	
  // end camera
  cam.end();
	
  // loop through all verticies in mesh and find the nearest vert position and index
	int n = mesh.getNumVertices();
	float nearestDistance = 0;
	ofVec2f nearestVertex;
	int nearestIndex = 0;
	ofVec2f mouse(mouseX, mouseY);
	for(int i = 0; i < n; i++) {
		ofVec3f cur = cam.worldToScreen(mesh.getVertex(i));
		float distance = cur.distance(mouse);
		if(i == 0 || distance < nearestDistance) {
			nearestDistance = distance;
			nearestVertex = cur;
			nearestIndex = i;
		}
	}
	
  // set color to gray
	ofSetColor(ofColor::gray);
  
  // draw a line from the nearest vertex to the mouse position ( in screen space )
	ofLine(nearestVertex, mouse);
	
  // draw a cirle around the nearest vertex
	ofNoFill();
	ofSetColor(ofColor::yellow);
	ofSetLineWidth(2);
	ofCircle(nearestVertex, 4);
	ofSetLineWidth(1);
	
  // draw some text with then nearest vertex index
	ofVec2f offset(10, -10);
	ofDrawBitmapStringHighlight(ofToString(nearestIndex), mouse + offset);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  mesh.save("mesh.ply");
  
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
  
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
  
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
void ofApp::windowResized(int w, int h){
  
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
  
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
  
}
