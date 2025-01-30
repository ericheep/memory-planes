#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4,1);

    int smallWidth = 608;
    int smallHeight = 328;
    int headerHeight = 32;
    int offset = 67;
    int rightSide = 1100;
    
    // create small window
    if (DEV_MODE) {
        settings.setSize(smallWidth, smallHeight);
    } else {
        settings.setSize(1920, 1200);
    }
    settings.setPosition(glm::vec2(0, 0));
    shared_ptr<ofAppBaseWindow> previewWindow = ofCreateWindow(settings);
    
    if (DEV_MODE) {
        // create surface 1 window, development settings
        settings.setSize(smallWidth, smallHeight);
        settings.setPosition(ofVec2f(rightSide, offset));
    } else {
        // create surface 1 window, fullscreen settings
        settings.setSize(1920, PANASONIC_HEIGHT);
        settings.setPosition(glm::vec2(1920, 0));
        settings.windowMode = OF_FULLSCREEN;
    }
    
    settings.shareContextWith = previewWindow;
    shared_ptr<ofAppBaseWindow> fresnelWindow = ofCreateWindow(settings);
    
    if (DEV_MODE) {
        // create surface 1 window, development settings
        settings.setSize(smallWidth, smallHeight);
        settings.setPosition(ofVec2f(rightSide, smallHeight + headerHeight + offset));
    } else {
        // create surface 1 window, fullscreen settings
        settings.setSize(1920, BENQ_HEIGHT);
        settings.setPosition(glm::vec2(3840, 0));
        settings.windowMode = OF_FULLSCREEN;
    }
    
    settings.shareContextWith = previewWindow;
    shared_ptr<ofAppBaseWindow> leftWindow = ofCreateWindow(settings);
    
    if (DEV_MODE) {
        // create surface 2 window, development settings
        settings.setSize(smallWidth, smallHeight);
        settings.setPosition(ofVec2f(rightSide, (smallHeight + headerHeight) * 2.0 + offset));
    } else {
        // create surface 2 window, fullscreen settings
        settings.setSize(1920, BENQ_HEIGHT);
        settings.setPosition(glm::vec2(5760, 0));
        settings.windowMode = OF_FULLSCREEN;
    }
    
    settings.shareContextWith = previewWindow;
    shared_ptr<ofAppBaseWindow> rightWindow = ofCreateWindow(settings);
    
    // uncomment next line to share main's OpenGL resources with gui
    // settings.shareContextWith = mainWindow;
    
    shared_ptr<ofApp> mainApp(new ofApp);
    
    // ofAddListener(previewWindow->events().draw, mainApp.get(), &ofApp::draw);
    ofAddListener(fresnelWindow->events().draw, mainApp.get(), &ofApp::drawFresnelWindow);
    ofAddListener(leftWindow->events().draw, mainApp.get(), &ofApp::drawLeftWindow);
    ofAddListener(rightWindow->events().draw, mainApp.get(), &ofApp::drawRightWindow);
    ofRunApp(previewWindow, mainApp);
    ofRunMainLoop();
}
