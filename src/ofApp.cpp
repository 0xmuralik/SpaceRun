#include "ofApp.h"


void ofApp::gameStart(){
    if(state!=Started){
        state=Started;
        gameStarted=true;
        player.energy=energy;
        player.pos=glm::vec3(ofGetWindowWidth()/2,ofGetWindowHeight()/2,0);
        player.rot=0;
        shooter.start();
        gameStartTime=ofGetElapsedTimeMillis();
        gameEndTime=0;
    }
}

void ofApp::gameStop(){
    if(state==Finish) return;
    state=Finish;
    gameStarted=false;
    shooter.stop();
    gameEndTime=ofGetElapsedTimeMillis();
}

void Shooter::spawnSprite() {
    Sprite sprite;
    if (haveChildImage) sprite.setImage(childImage);
    sprite.velocity = velocity;
    sprite.lifespan = lifespan;
    sprite.pos = glm::vec3(ofRandom(ofGetWindowWidth()),ofRandom(ofGetWindowHeight()),0);
    sprite.birthtime = ofGetElapsedTimeMillis();
    sys->add(sprite);
}

void Shooter::moveSprite(Sprite* sprite) {
    ofApp* theApp = (ofApp*)ofGetAppPtr();
    glm::vec3 v = glm::normalize(theApp->player.pos - sprite->pos);
    glm::vec3 u = sprite->heading();
    float a = glm::orientedAngle(u, v, glm::vec3(0, 0, 1));

    // a/100 to damp the instantaneous rotation of fire sprites
    sprite->velocity=glm::rotate(sprite->velocity, a/100, glm::vec3(0,0,1));
    sprite->rot += glm::degrees(a/100);
    
    Emitter::moveSprite(sprite);
}

void Shooter::update(){
    if (!started) return;
    ofApp* theApp = (ofApp*)ofGetAppPtr();
    float time = ofGetElapsedTimeMillis();
    if ((time - lastSpawned) > (1000.0 / rate)) {
        for(int i=0;i<spawnCount;i++){
            spawnSprite();
        }
        lastSpawned = time;
    }

    if (sys->sprites.size() == 0) return;
    vector<Sprite>::iterator s = sys->sprites.begin();
    vector<Sprite>::iterator tmp;

    while (s != sys->sprites.end()) {
        if (s->lifespan != -1 && s->age() > s->lifespan) {
            tmp = sys->sprites.erase(s);
            s = tmp;
        }
        else if(theApp->player.inside(s->pos)){
            tmp = sys->sprites.erase(s);
            s = tmp;
            theApp->player.energy-=spritePower;
        }
        else {
            s->scale = glm::vec3(spriteSize, spriteSize, spriteSize);
            s++;
        }
    }

    for (int i = 0; i < sys->sprites.size(); i++) {
        moveSprite(&sys->sprites[i]);
    }
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(ofColor::black);
    font.load("fonts/pixeboy-font/Pixeboy-z8XGD.ttf", 20);
    bgLoaded = bg.load("images/space-background.png");
    playerLoaded = playerImg.load("images/ship.png");
    if (playerLoaded) player.setImage(playerImg);
    player.distanceToHead();
    
    fireLoaded = fireImg.load("images/fire.png");
    if (fireLoaded) shooter.setChildImage(fireImg);
    
    gui.setup();
    gui.add(energy.setup("Energy", 10, 1, 100));
    gui.add(playerSpeed.setup("Speed", 3, 1, 10));
    gui.add(playerRotation.setup("Rotation",1,1,2));
    gui.add(playerSize.setup("Player Size", .5, .1, 1.0));
    
    gui.add(rate.setup("Fire rate", .5, 1, 10));
    gui.add(count.setup("Spawn Count",1,1,10));
    gui.add(life.setup("life", 5, .1, 10));
    gui.add(fireSpeed.setup("Fire Speed", 2,1,10));
    gui.add(fireSize.setup("Fire size", .3, .1, 1.0));
    
    bHide = false;
    
    player.pos=glm::vec3(ofGetWindowWidth()/2,ofGetWindowHeight()/2,0);
    player.energy=energy;
    
    shooter.pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
    shooter.drawable = false;
    
    state=Landing;
}

//--------------------------------------------------------------
void ofApp::update(){
    if(player.energy<=0){
        gameStop();
    }
    
    float scale=playerSize;
    player.scale = glm::vec3(scale,scale,scale);
    
    shooter.setRate(rate);
    shooter.setLifespan(life * 1000); // convert to milliseconds
    shooter.setVelocity(ofVec3f(0, -1, 0)*fireSpeed*100);
    shooter.spawnCount=count;
    shooter.spriteSize=fireSize;
    shooter.update();
    
    if (!gameStarted) return;
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
void ofApp::draw(){
    if (bgLoaded) bg.draw(0, 0, ofGetWidth(), ofGetHeight());
    int time=(ofGetElapsedTimeMillis()-gameStartTime)/1000;
    if(state!=Started){
        time=0;
    }
    if(state==Finish){
        time=(gameEndTime-gameStartTime)/1000;
        string score="Score:\t"+std::to_string(time);
        string message="Press Space to restart game";
        font.drawString(score,ofGetWindowWidth()/2-font.stringWidth(score)/2,(ofGetWindowHeight()*3/4)-font.stringHeight(message)-10);
        font.drawString(message,ofGetWindowWidth()/2-font.stringWidth(message)/2,ofGetWindowHeight()*3/4);
    }
    
    string topRight="Energy:\t"+std::to_string((int)player.energy)+"\t\tTime:\t"+std::to_string(time)+"\t\tFrame rate:\t"+std::to_string((int)ofGetFrameRate());
    font.drawString(topRight, ofGetWindowWidth()-font.stringWidth(topRight)-10, font.stringHeight("Energy: 100")+10);
    
    if(state==Landing){
        string message = "Press Space to start game";
        font.drawString(message,ofGetWindowWidth()/2-font.stringWidth(message)/2,ofGetWindowHeight()*3/4);
        
        player.pos=glm::vec3(ofGetWindowWidth()/2,ofGetWindowHeight()/2,0);
    }
    
    if (!bHide) {
        gui.draw();
    }
    
    player.draw();
    shooter.draw();
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
