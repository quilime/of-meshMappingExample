#pragma once

#include "ofMain.h"
#include "ofxCameraSaveLoad.h"

class ofApp : public ofBaseApp{
public:
  
  void setup();
  void update();
  void draw();
  
  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);
  
  void loadScene();
  void saveScene();
  
  bool editMode,
       camMouse,
       mouseDragging,
       showHelp;
  
  int normalSmoothAmt;
  
  ofVec3f nearestVertex;
  int nearestIndex;
  
  ofMesh mesh;
  ofMesh sceneMesh;
  ofMaterial material;
  
  ofSpherePrimitive sphere;
  
  ofEasyCam cam;
  ofLight sun;
  ofLight moon;
};
