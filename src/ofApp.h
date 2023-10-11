#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Emitter.h"
#include "Shape.h"


class Player: public Sprite{
public:
    void distanceToHead(){
        glm::vec3 current=pos;
        while(inside(current)){
            current.y--;
        }
        posToHead=pos.y-current.y-1;
    }
    float energy=0;
    float posToHead=0;
};

class Shooter: public Emitter{
public:
    void spawnSprite();
    void moveSprite(Sprite*);
    void update();
    int spawnCount=1;
    float spriteSize;
    float spritePower=1;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void gameStart();
        void gameStop();
        void resetDefaults(int level);
    
        bool bHide;
        ofxFloatSlider energy;
        ofxFloatSlider playerSpeed;
        ofxFloatSlider playerRotation;
        ofxFloatSlider playerSize;
    
        ofxFloatSlider rate;
        ofxIntSlider count;
        ofxFloatSlider life;
        ofxFloatSlider fireSpeed;
        ofxLabel screenSize;
        ofxFloatSlider fireSize;
            
        ofxPanel gui;
    
    
        Player player;
        Shooter shooter;
    
        ofImage bg;
        bool bgLoaded;
        
        ofImage playerImg;
        bool playerLoaded;
    
        ofImage fireImg;
        bool fireLoaded;
    
        map<int, bool> keymap;
    
        bool gameStarted;
        
        enum states{
            Landing,
            Started,
            Finish
        };
    
        enum levels{
            Level1,
            Level2,
            Level3
        };
        
        ofTrueTypeFont font;
        ofTrueTypeFont smallFont;
    
        float gameStartTime;
        float gameEndTime;
        
        states state;
        levels level;
};
