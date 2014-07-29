#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  
  editMode = false;
  camMouse = true;
  mouseDragging = false;
  nearestIndex = 0;
  
  
  // set vertical sync to true
	ofSetVerticalSync(true);
  
  // load mesh
	mesh.load("landscape-squ.ply");
  //mesh.smoothNormals( 15 );
//  mesh.usingIndices();
  
  sphere.setRadius( 20 );
  
  // set up lights
  ofSetGlobalAmbientColor(ofColor(0,0,0));
  ofSetSmoothLighting(true);
  light.setPointLight();
  light.setDiffuseColor( ofFloatColor(0.6f, 0.6f, 0.6f) );
  light.setSpecularColor( ofFloatColor(1.0f, 1.0f, 1.0f));
  light.setAttenuation(0.5, 0, 0);
//  setAttenuation(float constant=1.f, float linear=0.f, float quadratic=0.f))
  
  
  // set up material
  // shininess is a value between 0 - 128, 128 being the most shiny //
	material.setShininess( 120 );
  // the light highlight of the material //
	material.setSpecularColor(ofColor(255, 255, 255, 255));
  material.setAmbientColor(ofColor(0,0,0));
  
  ofSetSphereResolution(24);
  
  
  // load camera setting
  ofxLoadCamera(cam, "cameraSettings");
  
}

//--------------------------------------------------------------
void ofApp::update(){
  
  // move light around
  //light.setPosition((ofGetWidth() * .5) + cos(ofGetElapsedTimef() * .5)*(ofGetWidth() * .3), ofGetHeight()/2, 0);
  //light.setPosition(100, 100, 0);
  light.setPosition(cos(ofGetElapsedTimef()) * 100.0,
                         40,
                         sin(ofGetElapsedTimef()) * 100.0);
  
}

//--------------------------------------------------------------
void ofApp::draw(){


  ofEnableDepthTest();
  
  if (editMode) {
    // background gradient
    ofBackgroundGradient(ofColor(64), ofColor(0));
  } else {
    ofBackground(ofColor(0));
  }
  
  // set main color to white
  ofSetColor(255);

  // begin camera
	cam.begin();
  
  // enable lighting
  ofEnableLighting();
  light.enable();
  
  // start material
  material.begin();
  ofFill();
  ofSetColor(255);
  
  
  // test sphere
  sphere.setPosition(0, 60, 0);
  sphere.draw();

  
  // draw mesh faces
  mesh.draw();
  
  // end material
  material.end();
  

  // draw light position
  if (editMode) {
    ofFill();
    ofSetColor(light.getDiffuseColor());
    light.draw();
  }
  
  
  // end lighting
  ofDisableLighting();
  light.disable();
  
  
  // draw verts
  if (editMode) {
    // set color to white
    ofSetColor(ofColor::white);
    // set point size to 2
    glPointSize(2);
    // draw verts
    mesh.drawVertices();
  }
  

  
  // end camera
  cam.end();

  //
  // vert selection
  //
  if (editMode) {
    
    ofVec2f mouse(mouseX, mouseY);
    
    cout << mesh.getNumVertices() << endl;
    
    // if not mousedragging, find the nearest vert
    if (!mouseDragging) {
      // loop through all verticies in mesh and find the nearest vert position and index
      int n = mesh.getNumVertices();
      float nearestDistance = 0;
      for(int i = 0; i < n; i++) {
        ofVec3f cur = cam.worldToScreen(mesh.getVertex(i));
        float distance = cur.distance(mouse);
        if(i == 0 || distance < nearestDistance) {
          nearestDistance = distance;
          nearestVertex = cur;
          nearestIndex = i;
        }
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
    
    // move vertex with mouse while dragging
    if (!camMouse && mouseDragging) {
      
      ofVec3f stow = cam.screenToWorld( ofVec3f(mouse.x, mouse.y, nearestVertex.z) );
      
      mesh.setVertex(nearestIndex, stow);
      
    }
    
    // draw some text with then nearest vertex index
    ofVec2f offset(10, -10);
    ofDrawBitmapStringHighlight(ofToString(nearestIndex), mouse + offset);
  }
  
  
  
  // draw info text
  stringstream ss;
  ss << "Framerate: " << ofToString(ofGetFrameRate(),0) << "\n";
  ss << "f : Toggle Fullscreen" << "\n";
  ss << "c : Toggle Camera Control" << (camMouse == true ? " ON" : " OFF") << "\n";
  ss << "TAB : Toggle Mesh Edit Mode" << (editMode == true ? " ON" : " OFF") << "\n";
  ofSetColor(ofColor::white);
  ofDrawBitmapString(ss.str().c_str(), 20, 20);
  
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  
	switch(key) {
      
    // toggle 'f' for fullscreen
		case 'f':
			ofToggleFullscreen();
			break;

    // toggle 'c' for camera control
    case 'c':
      camMouse = !camMouse;
      cam.getMouseInputEnabled() ? cam.disableMouseInput() : cam.enableMouseInput();
			break;
      
    case 's':
      ofxSaveCamera(cam, "cameraSettings");
      break;
      
    // toggle 'TAB' to edit verts
    case OF_KEY_TAB:
      editMode = !editMode;
      break;
      
	}
  
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
  
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
  
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
  mouseDragging = true;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
  
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
  mouseDragging = false;
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
