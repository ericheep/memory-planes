#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4,1);

    int smallWidth = 608;
    int smallHeight = 280;
    int headerHeight = 32;
    int offset = 67;
    int rightSide = 1100;
    
    if (DEV_MODE) {
        // create surface 1 window, development settings
        settings.setSize(smallWidth, smallHeight);
        settings.setPosition(ofVec2f(rightSide, headerHeight));
    } else {
        // create surface 1 window, fullscreen settings
        settings.setSize(1920, PANASONIC_HEIGHT);
        settings.setPosition(glm::vec2(3840, 0));
        settings.windowMode = OF_FULLSCREEN;
    }
    
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

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
    
    settings.shareContextWith = mainWindow;
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

    settings.shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> rightWindow = ofCreateWindow(settings);
    
    shared_ptr<ofApp> mainApp(new ofApp);
    
    ofAddListener(leftWindow->events().draw, mainApp.get(), &ofApp::drawLeftWindow);
    ofAddListener(rightWindow->events().draw, mainApp.get(), &ofApp::drawRightWindow);
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
}
