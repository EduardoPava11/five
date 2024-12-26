#include "ofApp.h"
#include <glm/gtx/string_cast.hpp> // Optional: For debugging purposes

//--------------------------------------------------------------
// Helper function to calculate normals for an ofMesh
void calculateNormals(ofMesh &mesh) {
    std::vector<glm::vec3> normals(mesh.getVertices().size(), glm::vec3(0.0f, 0.0f, 0.0f));

    for (size_t i = 0; i < mesh.getNumIndices(); i += 3) {
        if (i + 2 >= mesh.getNumIndices()) {
            ofLogError() << "Incomplete triangle at indices: " << i;
            continue;
        }

        int index0 = mesh.getIndex(i);
        int index1 = mesh.getIndex(i + 1);
        int index2 = mesh.getIndex(i + 2);
        
        if(index0 >= mesh.getVertices().size() || 
           index1 >= mesh.getVertices().size() || 
           index2 >= mesh.getVertices().size()) {
            ofLogError() << "Index out of bounds: " << index0 << ", " << index1 << ", " << index2;
            continue;
        }

        glm::vec3 v0 = mesh.getVertex(index0);
        glm::vec3 v1 = mesh.getVertex(index1);
        glm::vec3 v2 = mesh.getVertex(index2);

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

        normals[index0] += faceNormal;
        normals[index1] += faceNormal;
        normals[index2] += faceNormal;
    }

    for (auto &normal : normals) {
        normal = glm::normalize(normal);
    }

    mesh.clearNormals();
    mesh.addNormals(normals);
}

//--------------------------------------------------------------
// Helper function to scale mesh vertices
void scaleMesh(ofMesh &mesh, float scaleFactor) {
    for (auto &vertex : mesh.getVertices()) {
        vertex *= scaleFactor;
    }
}

//--------------------------------------------------------------
// Correct Dodecahedron Vertices and Faces
std::vector<glm::vec3> dodecahedronVertices = {
    {0, 0.618f, 1.618f},   // Vertex 1
    {0, -0.618f, 1.618f},  // Vertex 2
    {0, -0.618f, -1.618f}, // Vertex 3
    {0, 0.618f, -1.618f},  // Vertex 4
    {1.618f, 0, 0.618f},   // Vertex 5
    {-1.618f, 0, 0.618f},  // Vertex 6
    {-1.618f, 0, -0.618f}, // Vertex 7
    {1.618f, 0, -0.618f},  // Vertex 8
    {0.618f, 1.618f, 0},   // Vertex 9
    {-0.618f, 1.618f, 0},  // Vertex 10
    {-0.618f, -1.618f, 0}, // Vertex 11
    {0.618f, -1.618f, 0},  // Vertex 12
    {1, 1, 1},              // Vertex 13
    {-1, 1, 1},             // Vertex 14
    {-1, -1, 1},            // Vertex 15
    {1, -1, 1},             // Vertex 16
    {1, -1, -1},            // Vertex 17
    {1, 1, -1},             // Vertex 18
    {-1, 1, -1},            // Vertex 19
    {-1, -1, -1},           // Vertex 20
};

std::vector<std::vector<int>> dodecahedronFaces = {
    {0, 1, 15, 4, 12},   // Face 1
    {0, 12, 8, 9, 13},   // Face 2
    {0, 13, 5, 14, 1},   // Face 3
    {1, 14, 10, 11, 15}, // Face 4
    {2, 3, 17, 7, 16},   // Face 5
    {2, 16, 11, 10, 19}, // Face 6
    {2, 19, 6, 18, 3},   // Face 7
    {18, 9, 8, 17, 3},   // Face 8
    {15, 11, 16, 7, 4},  // Face 9
    {4, 7, 17, 8, 12},   // Face 10
    {13, 9, 18, 6, 5},   // Face 11
    {5, 6, 19, 10, 14},  // Face 12
};

//--------------------------------------------------------------
// Helper Function to Add Faces to the Mesh
void addDodecahedronFaces(ofMesh &mesh, const std::vector<std::vector<int>> &faces) {
    for (const auto &face : faces) {
        if (face.size() == 5) { // Ensure each face is a pentagon
            // Triangulate pentagon into three triangles
            mesh.addIndex(face[0]);
            mesh.addIndex(face[1]);
            mesh.addIndex(face[2]);

            mesh.addIndex(face[0]);
            mesh.addIndex(face[2]);
            mesh.addIndex(face[3]);

            mesh.addIndex(face[0]);
            mesh.addIndex(face[3]);
            mesh.addIndex(face[4]);
        } else {
            ofLogError() << "Face does not have 5 vertices: " << face.size();
        }
    }
}

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofEnableDepthTest();
    ofBackground(backgroundColor); // Initialize with the background color

    // Initialize colors
    colorSets = {
        ofColor::yellow, // Background
        ofColor::blue,   // Edges
        ofColor::red     // Faces
    };

    backgroundColor = colorSets[0];
    edgeColor = colorSets[1];
    faceColor = colorSets[2];

    // Initialize rotation angles
    rotationAngle = 0.0f;
    verticalRotation = 0.0f;

    // Initialize mouse interaction variables
    isDragging = false;
    lastMousePosition = glm::vec2(0, 0);

    // Setup lighting
    ofEnableLighting();
    keyLight.setup();
    keyLight.setPosition(300, 300, 300);
    keyLight.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
    keyLight.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    keyLight.enable();

    // Add vertices to the mesh
    dodecahedron.addVertices(dodecahedronVertices);

    // Add faces (indices) to the mesh
    addDodecahedronFaces(dodecahedron, dodecahedronFaces);

    // Set rendering mode
    dodecahedron.setMode(OF_PRIMITIVE_TRIANGLES);

    // Calculate normals
    calculateNormals(dodecahedron);

    // Scale for visibility
    scaleMesh(dodecahedron, 100.0f);

    // Log mesh details
    ofLogNotice() << "Total Vertices: " << dodecahedron.getNumVertices(); // Should be 20
    ofLogNotice() << "Indices: " << dodecahedron.getNumIndices();         // Should be 108
}

//--------------------------------------------------------------
void ofApp::update() {
    // Update rotation angles
    rotationAngle += 0.5f;        // Rotate 0.5 degrees per frame
    verticalRotation += 0.3f;      // Rotate vertically at a different rate

    // Cycle colors every 3 seconds
    float time = ofGetElapsedTimef(); // Elapsed time in seconds
    int colorIndex = static_cast<int>(time / 3) % colorSets.size();

    // Update colors dynamically
    backgroundColor = colorSets[colorIndex];
    edgeColor = colorSets[(colorIndex + 1) % colorSets.size()];
    faceColor = colorSets[(colorIndex + 2) % colorSets.size()];
}

//--------------------------------------------------------------
void ofApp::draw() {
    // Set background color
    ofBackground(backgroundColor);

    ofEnableDepthTest();
    ofEnableLighting();
    cam.begin();

    // Apply rotations
    ofRotateDeg(rotationAngle, 1, 1, 0);           // Rotate around X and Y axes
    ofRotateDeg(verticalRotation, 0, 1, 0);        // Rotate around Y axis for vertical movement

    // Draw faces
    ofSetColor(faceColor);
    dodecahedron.draw();

    // Draw edges
    ofSetColor(edgeColor);
    ofSetLineWidth(2);
    dodecahedron.drawWireframe();

    cam.end();
    ofDisableDepthTest();
    ofDisableLighting();

    // Save every 60 frames (~1 second at 60 FPS)
    static int frameCount = 0;
    frameCount++;
    if(frameCount % 60 == 0){
        ofSaveFrame();
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    isDragging = true;
    lastMousePosition = glm::vec2(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(isDragging){
        glm::vec2 currentPosition(x, y);
        glm::vec2 delta = currentPosition - lastMousePosition;
        rotationAngle += delta.x * 0.5f;        // Horizontal rotation based on mouse X movement
        verticalRotation += delta.y * 0.5f;      // Vertical rotation based on mouse Y movement
        lastMousePosition = currentPosition;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    isDragging = false;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){ 

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
    float targetWidth = 1080;
    float targetHeight = 1920;
    float aspectRatio = targetWidth / targetHeight;

    int newWidth = ofGetWidth();
    int newHeight = newWidth / aspectRatio;
    ofSetWindowShape(newWidth, newHeight);
}

//--------------------------------------------------------------
void ofApp::pause(){

}

//--------------------------------------------------------------
void ofApp::stop(){

}

//--------------------------------------------------------------
void ofApp::resume(){

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
    return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){

}

//--------------------------------------------------------------
void ofApp::cancelPressed(){

}

//--------------------------------------------------------------
void ofApp::updateColors(std::vector<ofColor> colors) {
    for (size_t i = 0; i < colors.size() && i < dodecahedron.getNumColors(); ++i) {
        dodecahedron.setColor(i, colors[i]);
    }
}
