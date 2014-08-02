/*
 (c) Gabriel L. Dunne, 2014
*/
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  
  // of stuff
	ofSetVerticalSync(true);
  ofSetFrameRate(60);
  ofEnableDepthTest();
  ofSetFullscreen(true);

  
  // settings
  showHelp = false;
  editMode = true;
  camMouse = false;
  mouseDragging = false;
  nearestIndex = 0;
  normalSmoothAmt = 20;
  

  // set up material
  // shininess is a value between 0 - 128, 128 being the most shiny //
	material.setShininess( 120 );
	material.setSpecularColor(ofColor(255, 255, 255));
  material.setAmbientColor(ofColor(0, 0, 0));
  material.setShininess(10.0f);

  
  // load mesh
  mesh.load("landscape.ply");
  
  // test sphere
  sphere.setRadius(20);
  
  // set up lights
  ofSetGlobalAmbientColor(ofColor(0, 0, 0));
  ofSetSmoothLighting(true);
  
  sun.setup();
  sun.setDirectional();
  sun.setDiffuseColor( ofFloatColor(0.7f, 0.7f, 0.7f) );
  sun.setSpecularColor( ofFloatColor(0.3f, 0.3f, 0.3f) );
  
  moon.setup();
  moon.setDirectional();
  moon.setDiffuseColor( ofFloatColor(0.0f, 0.0f, 0.15f) );
  moon.setSpecularColor( ofFloatColor(0.0f, 0.0f, 0.08f) );
  
  // load camera settings
  ofxLoadCamera(cam, "cameraSettings");
  
  // start w/ mouse input disabled
  cam.disableMouseInput();
//  cam.setFov(5);
}

//--------------------------------------------------------------
void ofApp::update(){
  
  float d = 400.0f; // distance from center
  float speed = 13; // speed multiplier

  // move sun around
  ofPoint sp;
  sp.set(d, 0, 0);
  sp.rotate( ofGetElapsedTimef() * speed , ofVec3f(0, 0, 1));
  sun.setPosition(sp);
  sun.lookAt(ofVec3f(0, 0, 0));
  
  // move moon around
  ofPoint mp;
  mp.set(d, 0, 0);
  mp.rotate( ofGetElapsedTimef() * speed + 180 , ofVec3f(0, 0, 1));
  moon.setPosition(mp);
  moon.lookAt(ofVec3f(0, 0, 0));
}

//--------------------------------------------------------------
void ofApp::draw(){
  
  if (editMode) {
    // if editing, show background gradient
    ofBackgroundGradient(ofColor(80), ofColor(5));
  } else {
    // black bg
    ofBackground(ofColor(0));
  }
  
  // begin camera
	cam.begin();
  
  // enable lighting
  ofEnableLighting();
  sun.enable();
  moon.enable();
  
  // start material
  material.begin();
  ofSetColor(255);
  ofFill();
  
  // test sphere
  //sphere.setPosition(0, 60, 0);
  //sphere.draw();
  
  // draw mesh faces
  if (!editMode) {
    sceneMesh.draw();
  }
 
  // end material
  material.end();

  // end lighting
  ofDisableLighting();
  
  // draw light position
  if (editMode) {
    // draw grey lines to lights
    ofSetColor(ofColor::gray);
    ofSetLineWidth(1);
    ofLine(sun.getPosition(),  ofVec3f(0));
    ofLine(moon.getPosition(), ofVec3f(0));
    // draw lights
    ofFill();
    ofSetColor(sun.getDiffuseColor());
    sun.draw();
    ofSetColor(moon.getDiffuseColor());
    moon.draw();
  }
  
  // draw verts and wireframe when editing
  if (editMode) {
    // draw wireframe
    ofSetColor(ofColor::white);
    glLineWidth(1);
    
    //  NOTE :
    // drawWireframe may draw incorrectly when compiled for ARM
    // openframeworks bug?
    mesh.drawWireframe();
    
    // draw verts
    ofSetColor(ofColor::white);
    glPointSize(4);
    mesh.drawVertices();
  }
  
  // end camera
  cam.end();

  // vert selection
  if (editMode) {
    // create vec2 of the mouse for reference
    ofVec2f mouse(mouseX, mouseY);
    // if not dragging the mouse, find the nearest vert
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
    
    // draw a line from the nearest vertex to the mouse position
    ofSetColor(ofColor::gray);
    ofSetLineWidth(1);
    ofLine(nearestVertex, mouse);
    
    // draw a cirle around the nearest vertex
    ofNoFill();
    ofSetColor(ofColor::magenta);
    ofSetLineWidth(2);
    if (mouseDragging) {
      ofCircle(mouse, 4);
    } else {
      ofCircle(nearestVertex, 4);
    }
    ofSetLineWidth(1);
    
    // edit position of nearest vert with mousedrag
    if (!camMouse && mouseDragging) {
      mesh.setVertex(nearestIndex, cam.screenToWorld(ofVec3f(mouse.x, mouse.y, nearestVertex.z)));
    }
    
    // draw a label with the nearest vertex index
    ofDrawBitmapStringHighlight(ofToString(nearestIndex), mouse + ofVec2f(10, -10));
  }
  
  // draw info text
  if (showHelp) {
    stringstream ss;
    ss << "Framerate " << ofToString(ofGetFrameRate(),0) << "\n";
    ss << "  f : Toggle Fullscreen" << "\n";
    ss << "  h : Toggle Help Text" << "\n";
    ss << "  c : Toggle Camera Control : " << (camMouse == true ? "ON" : "OFF") << "\n";
    ss << "TAB : Toggle Mesh Edit : " << (editMode == true ? "ON" : "OFF") << "\n";
    ss << "  s : Save Scene" << "\n";
    ss << "  l : Load Scene";
    ofSetColor(ofColor::white);
    ofDrawBitmapStringHighlight(ss.str().c_str(), ofVec3f(10, 20), ofColor(45), ofColor(255));
  }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  
	switch(key) {
      
    // toggle fullscreen
		case 'f':
			ofToggleFullscreen();
			break;

    // toggle camera control
    case 'c':
      camMouse = !camMouse;
      camMouse ? cam.enableMouseInput() : cam.disableMouseInput();
			break;
      
    // saves the camera and mesh settings
    case 's':
      ofxSaveCamera(cam, "cameraSettings");
      mesh.save("mesh-tweaked.ply");
      break;

    // loads camera and mesh settings
    case 'l':
      ofxLoadCamera(cam, "cameraSettings");
      mesh.load("mesh-tweaked.ply");
      camMouse = false;
      editMode = false;
      break;
   
    // show help
    case 'h':
      showHelp = !showHelp;
      break;
      
    // toggle 'TAB' to edit verts
    case OF_KEY_TAB:
      editMode = !editMode;
      if (!editMode) {
        sceneMesh.clear();
        sceneMesh.append(mesh);
        sceneMesh.smoothNormals( normalSmoothAmt );
      }
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
