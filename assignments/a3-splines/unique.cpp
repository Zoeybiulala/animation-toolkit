#include "curve_editor.h"
#include <string>
#include <iostream>
#include <algorithm>
#include "atkui/framework.h"
#include "spline.h"
using namespace glm;

CurveEditor::CurveEditor() : 
  atkui::Framework(atkui::Orthographic), 
  mSpline(),
  mRadius(10.0),
  mSelected(-1),
  mButtonState(0),
  mMode(ADD), 
  mShowControlPoints(false) {

  mSpline.addInterpolationType("Linear", &mLinear);
  mSpline.addInterpolationType("Hermite", &mHermite);
  mSpline.addInterpolationType("Catmull-Rom", &mCatmullRom);

  mHermite.setClamped(true);
  mHermite.setClampedDirection(vec3(100,0,0));
}

CurveEditor::~CurveEditor() {
}
vec3 col = vec3(0,0,1);
bool calcDistance(glm::vec3 p1, glm::vec3 p2) {
   float a = (p1[0]-p2[0]) * (p1[0]-p2[0]) + (p1[1]-p2[1]) * (p1[1]-p2[1]) + (p1[2]-p2[2]) * (p1[2]-p2[2]);
   return a<500.0f;
}

void CurveEditor::setup() {
  listControls();
  keyCol = glm::vec3(0,0,1);
  ctrlCol =glm::vec3(1,1,0);
}

void CurveEditor::scene() {
    drawState();

    if(mSpline.getInterpolationType() == "Linear" ){

      mSpline.computeControlPoints();
      int nC = mSpline.getNumControlPoints();
      setColor(keyCol);
      for(int j=0; j<nC; j++) {
        drawSphere(mSpline.getControlPoint(j),10.0f);
      }
      for(int i=0;i<nC-1;i++) {
        drawLine(mSpline.getKey(i),mSpline.getKey(i+1));
      }

    } 
    if(mSpline.getInterpolationType() == "Catmull-Rom") {
      mSpline.computeControlPoints();

      int nK = mSpline.getNumKeys();
      for(int i=0;i<nK;i++) {
        drawSphere(mSpline.getKey(i),10.0f);
      }

      //draw yellow line
      if(mShowControlPoints == true) {
        int nC = mSpline.getNumControlPoints();
        setColor(ctrlCol);
        for(int j=0; j<nC; j++) {
          drawSphere(mSpline.getControlPoint(j),10.0f);
        }
        setColor(ctrlCol);
        for(int j=0; j<nC-1; j++) {
          drawLine(mSpline.getControlPoint(j),mSpline.getControlPoint(j+1));
          
        }
      }
      //draw blue line
      if(nK > 2){
         if(!calcDistance(mSpline.getKey(0),mSpline.getKey(nK-1))){
            setColor(col);
         } else {
            setColor(glm::vec3(0,0,1));
         }
      }
      
      for (int t=0; t < 100 * (nK-1); t++){
         drawLine(mSpline.getValue(t/100.0f),mSpline.getValue(t/100.0f+0.01));
      }
    }

    if(mSpline.getInterpolationType() == "Hermite") {
      //calculate  + draw the control points
      int nC = mSpline.getNumControlPoints();
      if(mShowControlPoints == true) {
        for(int j=0; j<nC; j++) {
          if(j % 2 == 0) {
            setColor(keyCol);
            drawSphere(mSpline.getControlPoint(j),10.0f);
            setColor(ctrlCol);
            drawSphere(mSpline.getControlPoint(j)+mSpline.getControlPoint(j+1),10.0f);
          } 
        }
        for(int k =0; k<nC-1; k++) {
          if(k % 2 == 0) {
            drawLine(mSpline.getControlPoint(k),mSpline.getControlPoint(k)+mSpline.getControlPoint(k+1));
          } 
        }
      } else {
        setColor(keyCol);
        int nK = mSpline.getNumKeys();
        setColor(keyCol);
        for(int i=0;i<nK; i++) {
          drawSphere(mSpline.getKey(i),10.0f);
        } 
      }
      setColor(keyCol);
      int nK = mSpline.getNumKeys();
      for(int t=0; t< 100 * (nK-1); t++ ){
        drawLine(mSpline.getValue(t/100.0f),mSpline.getValue(t/100.0f+0.01));
      }
    }
  
}
//when deleting, this method kinda messed up
void CurveEditor::addPoint(const vec3& p) {
  //std::cout << "Add key: " << p << std::endl;
  mSpline.appendKey(mSpline.getNumKeys(), p);
  mSpline.computeControlPoints();
}

void CurveEditor::deletePoint(int key) {
  if (mShowControlPoints) return;
  if(key == -1) return; //avoid abort 6 when there is no key to delete
  mSpline.deleteKey(key);
  mSpline.computeControlPoints();
}

void CurveEditor::drawState() {
  int th = 25;
  switch (mMode) {
    case ADD: drawText("Mode: ADD", 5, th); break;
    case EDIT: drawText("Mode: EDIT", 5, th); break;
    case REMOVE: drawText("Mode: DELETE", 5, th); break;
  }
  th += renderer.textHeight() + 5; 
  drawText("Type: "+mSpline.getInterpolationType(), 5, th); 
}

void CurveEditor::listControls() {

  std::cout << "Key controls:" << std::endl;
  std::cout << "0: Linear" << std::endl;
  std::cout << "1: Catmull-Rom" << std::endl;
  std::cout << "2: Hermite" << std::endl;
  std::cout << "c: Toggle control points" << std::endl;
  std::cout << "h: clamp hermite on/off" << std::endl;
  std::cout << "a: add mode" << std::endl;
  std::cout << "e: edit mode" << std::endl;
  std::cout << "d: delete mode" << std::endl;
  std::cout << "q: change color" << std::endl; //support change color !
  std::cout << "SPACEBAR: clear" << std::endl;
}

void CurveEditor::mouseMotion(int pX, int pY, int dx, int dy) {
  if (mMode != EDIT) return;
  if (mSelected == -1) return;
  
  if (mButtonState == GLFW_MOUSE_BUTTON_LEFT) {
    vec3 p = vec3(pX, height() - pY, 0);
    if (mShowControlPoints) {
      if (mSpline.getInterpolationType() == "Hermite" && mSelected % 2 == 1) {
        mSpline.editControlPoint(mSelected, p-mSpline.getControlPoint(mSelected-1));
      }
      if(mSpline.getInterpolationType() == "Catmull-Rom"){
        mSpline.editControlPoint(mSelected, p);
      }
      
    } else mSpline.editKey(mSelected, p);
  }
}

int CurveEditor::pickPoint(int x, int y) {
  vec3 tmp = vec3(x, y, 0);

  int num = mSpline.getNumKeys();
  if (mShowControlPoints) {
    num = mSpline.getNumControlPoints();
  }

  // check data points -- needs to be done before ctrl points
  for (int i = 0; mSelected == -1 && i < num; i++) {
    vec3 pt;
    if (mShowControlPoints) {
      pt = mSpline.getControlPoint(i);
      if (mSpline.getInterpolationType() == "Hermite" && i % 2 == 1) {
        pt = pt + mSpline.getControlPoint(i-1);
      }
    } else {
      pt = mSpline.getKey(i);
    }

    if (length(tmp - pt) < mRadius) {
      return i;
    }
  }

  return -1;
}

void CurveEditor::mouseUp(int pButton, int state) {
  if (mButtonState == GLFW_MOUSE_BUTTON_LEFT) {
    mSelected = -1;
  }
}

void CurveEditor::mouseDown(int pButton, int state) {
  mButtonState = pButton;
  glm::vec2 p = mousePosition();
  int pX = (int) p.x;
  int pY = (int) p.y;

  if (mButtonState == GLFW_MOUSE_BUTTON_LEFT) {
    if (mMode == ADD) {
      vec3 tmp(p, 0);
      // guard against adding multiple copies of the same point
      // (multiple mouse events may be triggered for the same point)
      bool add = true;
      if (mSpline.getNumKeys() > 0) {
        vec3 lastKey = mSpline.getKey(mSpline.getNumKeys() - 1);
        float d = length(lastKey-tmp);
        if (d < 10) add = false;
      }

      if (add) {
        addPoint(vec3(pX, height()-pY, 0));
      }

    } else if (mMode == REMOVE) {
      mSelected = pickPoint(pX, height() - pY);
      deletePoint(mSelected);

    } else if (mMode == EDIT) {
      mSelected = pickPoint(pX, height() - pY);
      std::cout << "SELECTED: " << mSelected << std::endl;
      mouseMotion(pX, pY, 0, 0);
    }
  }

}

void CurveEditor::keyUp(int pKey, int mods) {
  if (pKey == 'p') {
    for (int i = 0; i < mSpline.getNumKeys(); i++) {
      std::cout << mSpline.getKey(i) << std::endl;
    }
  } else if (pKey == '1') {
    mSpline.setInterpolationType("Linear");

  } else if (pKey == '2') {
    mSpline.setInterpolationType("Catmull-Rom");

  } else if (pKey == '3') {
    mSpline.setInterpolationType("Hermite");

  } else if (pKey == 'A') {
    mMode = ADD;

  } else if (pKey == 'D') {
    mMode = REMOVE;

  } else if (pKey == 'E') {
    mMode = EDIT;

  } else if (pKey == 'C') {
    mShowControlPoints = !mShowControlPoints;

  } else if(pKey == 'Q') {
     col = vec3(agl::random(),agl::random(),agl::random());

  } else if (pKey == GLFW_KEY_SPACE) {
    mSpline.clear();

  } else if (pKey == 'H') {
    mHermite.setClamped(!mHermite.isClamped());
    mSpline.computeControlPoints();
  } 
}

int main(int argc, char** argv) {
   CurveEditor viewer;
	viewer.run();
}

