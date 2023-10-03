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
// spawnSprite - we override this function in the Emitter class to spawn our
// "custom" Agent sprite.
//
void Shooter::spawnSprite() {
    Sprite sprite;
    if (haveChildImage) sprite.setImage(childImage);
    sprite.velocity = velocity;
    sprite.lifespan = lifespan;
    sprite.pos = pos;
    sprite.birthtime = ofGetElapsedTimeMillis();
    sys->add(sprite);
}
// moveSprite - we override this function in the Emitter class to implment
// "following" motion towards the target
//
void Shooter::moveSprite(Sprite* sprite) {
    // Get a pointer to the app so we can access application
    // data such as the target sprite.
    //
    ofApp* theApp = (ofApp*)ofGetAppPtr();
    // rotate sprite to point towards target
    // - find vector "v" from sprite to target
    // - set rotation of sprite to align with v
    //
    glm::vec3 v = glm::normalize(theApp->player.pos - sprite->pos);
    glm::vec3 u = glm::vec3(0, -1, 0);
    float a = glm::orientedAngle(u, v, glm::vec3(0, 0, 1));
    sprite->rot = glm::degrees(a);
    // Calculate new velocity vector
    // with same speed (magnitude) as the old one but in direction of "v"
    //
    sprite->velocity = glm::length(sprite->velocity) * v;
    // Now move the sprite in the normal way (along velocity vector)
    //
    Emitter::moveSprite(sprite);
}

//--------------------------------------------------------------
void ofApp::setup(){
//    bgLoaded = bg.load("images/spaceBackground.png");
    playerLoaded = playerImg.load("images/ship.png");
    if (playerLoaded) player.setImage(playerImg);
    
    fireLoaded = fireImg.load("images/fire.png");
    
    gui.setup();
    gui.add(energy.setup("Energy", 100, 10, 100));
    gui.add(playerSpeed.setup("Speed", 10, 1, 100));
    gui.add(playerRotation.setup("Rotation",10,1,100));
    gui.add(playerSize.setup("Player Size", .1, .05, 1.0));
    
    gui.add(rate.setup("Fire rate", 1, 1, 10));
    gui.add(life.setup("life", 5, .1, 10));
    gui.add(velocity.setup("velocity", ofVec3f(0, -200, 0), ofVec3f(-1000, -1000,-1000), ofVec3f(1000, 1000, 1000)));
    gui.add(fireSize.setup("Fire size", .5, .1, 1.0));
    gui.add(rotationSpeed.setup("Rotation Speed (deg/Frame)", 0, 0, 10));

    bHide = false;
    
    player.distanceToHead();
    player.pos=glm::vec3(ofGetWindowWidth()/2,ofGetWindowHeight()/2,0);
    player.energy=energy;
    
    ofSetBackgroundColor(ofColor::black);

    shooter.pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0,
    0);
    shooter.drawable = false;
    if (fireLoaded) shooter.setChildImage(fireImg);
    shooter.start();
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
    
    shooter.setRate(rate);
    shooter.setLifespan(life * 1000); // convert to milliseconds
    shooter.setVelocity(ofVec3f(velocity->x, velocity->y, velocity->z));
    shooter.update();
    for (int i = 0; i < shooter.sys->sprites.size(); i++) {
    // Get values from sliders and update sprites dynamically
    //
    float sc = fireSize;
    float rs = rotationSpeed;
    shooter.sys->sprites[i].scale = glm::vec3(sc, sc, sc);
    shooter.sys->sprites[i].rotationSpeed = rs;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (bgLoaded) bg.draw(0, 0, ofGetWidth(), ofGetHeight());
    player.draw();
    shooter.draw();
    
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
