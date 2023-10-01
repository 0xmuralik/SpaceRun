#include "ofApp.h"


void ofApp::gameStart(){
    player.energy=energy;
    gameStarted=true;
}

void ofApp::gameStop(){
    gameStarted=false;
    player.pos=player.pos=glm::vec3(ofGetWindowWidth()/2,ofGetWindowHeight()/2,0);
    player.rot=0;
}
//--------------------------------------------------------------
void ofApp::setup(){
//    bgLoaded = bg.load("images/spaceBackground.png");
    playerLoaded = playerImg.load("images/ship.png");
    if (playerLoaded) player.setImage(playerImg);
    
    gui.setup();
    gui.add(energy.setup("energy", 100, 10, 100));
    gui.add(playerSpeed.setup("speed", 10, 1, 100));
    gui.add(playerRotation.setup("rotation",10,1,100));
    gui.add(playerSize.setup("Size", .1, .05, 1.0));

    bHide = false;
    
    player.distanceToHead();
    player.pos=glm::vec3(ofGetWindowWidth()/2,ofGetWindowHeight()/2,0);
    player.energy=energy;
}

//--------------------------------------------------------------
void ofApp::update(){
    float scale=playerSize;
    player.scale = glm::vec3(scale,scale,scale);
    
//    bound player within the screen
    if(player.pos.x+player.posToHead*scale>ofGetWindowWidth()){
        player.pos.x=ofGetWindowWidth()-player.posToHead*scale;
    }
    if(player.pos.y+player.posToHead*scale>ofGetWindowHeight()){
        player.pos.y=ofGetWindowHeight()-player.posToHead*scale;
    }
    if(player.pos.x<player.posToHead*scale){
        player.pos.x=player.posToHead*scale;
    }
    if(player.pos.y<player.posToHead*scale){
        player.pos.y=player.posToHead*scale;
    }

    if(player.energy<=0){
        gameStop();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (bgLoaded) bg.draw(0, 0, ofGetWidth(), ofGetHeight());
    player.draw();
    
    if (!bHide) {
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'F':
        case 'f':
            ofToggleFullscreen();
            break;
        case 'H':
        case 'h':
            bHide = !bHide;
            break;
        case 'Q':
        case 'q':
            gameStop();
            break;
        case ' ':
            gameStart();
            break;
    }
    
    if(!gameStarted){
        return;
    }
    
    keymap[key] = true;
    if(keymap[OF_KEY_UP]){
        player.pos+=(float)playerSpeed*player.heading();
    }
    if(keymap[OF_KEY_DOWN]){
        player.pos-=(float)playerSpeed*player.heading();
    }
    if(keymap[OF_KEY_RIGHT]){
        player.rot+=(float)playerRotation;
    }
    if(keymap[OF_KEY_LEFT]){
        player.rot-=(float)playerRotation;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    keymap[key] = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
