#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

    public:
        
        void setup();
        void update();
        void draw();
        
        void keyPressed(int key);
        void keyReleased(int key);
        void windowResized(int w, int h);

        void pause();
        void stop();
        void resume();
        void reloadTextures();

        bool backPressed();
        void okPressed();
        void cancelPressed();

        // Added variables
        ofMesh dodecahedron;
        float rotationAngle;
        float verticalRotation; // For up/down rotation

        // Lighting
        ofLight keyLight;

        // Mouse interaction variables
        bool isDragging;
        glm::vec2 lastMousePosition;

        // Mouse event handlers
        void mousePressed(int x, int y, int button);
        void mouseDragged(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        
        // Dynamic color update method
        void updateColors(std::vector<ofColor> colors);

        // Camera
        ofEasyCam cam;

        // Dynamic Coloring Variables
        std::vector<ofColor> colorSets;
        ofColor backgroundColor;
        ofColor edgeColor;
        ofColor faceColor;
};

