#include "ofMain.h"
#include "ofApp.h"

int main(){
    ofSetupOpenGL(1080, 1920, OF_WINDOW); // Set window size to 1080x1920 for TikTok-style portrait mode

    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
    return 0;
}

#ifdef TARGET_ANDROID
void ofAndroidApplicationInit()
{
    //application scope init
}

void ofAndroidActivityInit()
{
    //activity scope init
    main();
}
#endif
