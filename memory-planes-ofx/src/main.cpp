#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4,1);

    int smallWidth = 800;
    int smallHeight = 600;
    
    if (DEV_MODE) {
        settings.setSize(smallWidth, smallHeight);
        settings.windowMode = OF_WINDOW;
    } else {
        //settings.setSize(1920, 1200);
        settings.setPosition(ofVec2f(1920, 0));
        settings.windowMode = OF_FULLSCREEN;
    }
    
    auto window = ofCreateWindow(settings);
    
    ofRunApp(window, make_shared<ofApp>());
    ofRunMainLoop();
}
