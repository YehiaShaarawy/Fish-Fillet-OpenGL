//
//  main.cpp
//  Fish-Fillet-Project
//
//  Created by Yehia Sharawy on 21/12/2023.
//

//Libraries
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION  //remove anu OPENGL Deprications
#include <GLUT/glut.h>
#else
#include<GL/glut.h>
#endif
#include <math.h>
#include <cstring>
#include<stdlib.h>
#include <ctime>
#include <vector>

//Global Variables
int physicalWidth = 1200;
int physicalHeight = 600;
int logicalWidth = 1200;
int logicalHeight = 600;
int centerX = logicalWidth/2;
int centerY = logicalHeight/2;
int mouseX=centerX, mouseY=centerY;

bool isGameOver = false;
int score = 0;
int gameTime = 10;
int F2Pressed = 0;
int yellow =0;
bool allYellowFishGone = false;

//waves animation
bool sqUp=true;
int sqDelta=0;
int maxY=15;


void textDisplay(char *str,int x,int y) {
    glRasterPos2d(x,y);
    for (int i=0;i< strlen(str);i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[i]);
    glFlush();
}

void scoreDisplay(int posx, int posy, int score)
{
    glRasterPos2d(posx,posy);
    if (score < 0) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, '-');
            score = -score;// change score positive for display
        }
    int numDigits = (score == 0) ? 1 : 0;
        int temp = score;
        while (temp != 0) {
            temp /= 10;
            ++numDigits;
        }
        char scoreStr[numDigits + 1]; // +1 for null terminator
        sprintf(scoreStr, "%d", score);

        for (int i = 0; i < numDigits; ++i) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, scoreStr[i]);
        }
        glFlush();
    glFlush();
}

void gameOver(char *str,int x,int y) {
    glRasterPos2d(x,y);
    for (int i=0;i< strlen(str);i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str[i]);
    glFlush();
}

void Timer(int value) {
    glutTimerFunc(10, Timer, value);
    glutPostRedisplay();
}

void DrawWaves(float cx, float cy, float r, int num_segments){
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);//get the current angle
        float x = r * cosf(theta-44.92);//calculate the x component
        float y = r * sinf(theta-44.92);//calculate the y component
        glVertex2f(x + cx, y + cy + sqDelta);//output vertex
    }
    glEnd();
}

class Fish{
public:
    bool fishPlayer, playerDetected;
    bool isRedFish;
    int redFish=0, yellowFish=0;
    float positionX, positionY;
    float direction;
public:
    Fish(bool isfishPlayer = false){
        fishPlayer = isfishPlayer;
        if(fishPlayer == true){
            positionX = 600;
            positionY = 300;
            playerDetected = true;
        }else{
            isRedFish = rand() % 2 == 0;
            positionX= static_cast<float>(rand() % (physicalWidth-100));
            positionY = static_cast<float>(rand() % (physicalHeight-100));
        }
        direction = (rand() % 2)* 1.4f - 0.7f;
    }
    
    void drawFish(){
        if(isRedFish){
            glColor3f(1, 0, 0);
            redFish++;
        }else{
            glColor3f(1, 1, 0);
            yellowFish++;
            yellow++;
        }
        if(playerDetected){glColor3f(0, 0, 1);}
        drawFish(positionX, positionY, direction);
    }
    void drawFish(float positionX, float positionY, float direction) {
        // Tail
        glBegin(GL_TRIANGLES);
        glVertex2f(direction < 0 ? positionX - 15 : positionX + 15, positionY + 15);
        glVertex2f(direction < 0 ? positionX + 20 : positionX - 20, positionY + 5);
        glVertex2f(direction < 0 ? positionX + 20 : positionX - 20, positionY + 25);
        glEnd();
        // Body
        glBegin(GL_TRIANGLES);
        glVertex2f(positionX, positionY - 10);
        glVertex2f(direction < 0 ? positionX - 30 : positionX + 30, positionY + 15);
        glVertex2f(positionX, positionY + 40);
        glEnd();
    }
    void moveFish(float directionX= 0.0f,float directionY= 0.0f){
        if(!fishPlayer){
            positionX += direction;
            if(positionX > physicalWidth){
                positionX -= physicalWidth;
                positionY = static_cast<float>(rand() % (physicalHeight-100));
            }
            if(positionX < 0){
                positionX += physicalWidth;
                positionY = static_cast<float>(rand() % (physicalHeight-100));
            }
        }else{// fish player mouse position
            positionX = directionX;
            positionY = 650 - directionY;
        }
    }
};

std::vector<Fish> arrayOfFish; //create an array that will be filled with fish objects
Fish fishPlayer(true); //create a fish player

void init2D(float r, float g, float b){// initializes the background color & add 20 fish in an array of fish
    glClearColor(r,g,b,0);
    for (int i = 0; i < 20; ++i) {
        arrayOfFish.push_back(Fish());
    }
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,logicalWidth,0.0,logicalHeight);
}

void passiveMouse (int x,int y){ //moves the player fish based on the mouse position (x,y)
    fishPlayer.moveFish(x,y);
}

void timerrand(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timerrand, 0);  // 60 FPS
}

void timer(int value) { //while the game is running, we decrease the time by one until 0, afterwards det isGameOver to true
    if (!isGameOver) {
        if (gameTime > 0) {
            --gameTime;
        } else {
            isGameOver = true;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(1000, timer, 0);
}

void specialKeyboard(int key, int x, int y) { //when F2 is pressed, we reset the time, score, array of Fish, and isGameover
    if (key == GLUT_KEY_F2) {
        isGameOver = false;
        gameTime = 10;
        score = 0;
        F2Pressed = 1;
        arrayOfFish.clear();
        for (int i = 0; i < 20; ++i) { //refill the array with 20 fish
            arrayOfFish.push_back(Fish());
        }
    }
    glutPostRedisplay();
}

void waves(){
    //Background SEA
    glBegin(GL_POLYGON);
        glVertex2f(0,0);
        glVertex2f(0,500);
        glVertex2f(1200,500);
        glVertex2f(1200,0);
    glEnd();

    for(int i=0;i<=1200;i+=60){
        DrawWaves(i, 500, 40, 100);
    }
    if (abs(sqDelta)>maxY) sqUp=!sqUp ;
    if (sqUp) sqDelta+=1; else sqDelta-=1;
}

bool checkCollision(Fish& fishPlayer, Fish& restOfFish){
    const float fishSize = 15.0f;
    return abs(fishPlayer.positionX - restOfFish.positionX) < fishSize + fishSize && //checks the distance of the 2 center of the 2 fish is less than the sum of radii fishsize. If this condition is true, it means the fish overlap in the horizontal direction.
           abs(fishPlayer.positionY - restOfFish.positionY) < fishSize + fishSize; //vertical
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.03,0.87,1);
    
    //Waves
    waves();
    if(isGameOver == false){
        //ScoreBoard
        glColor3f (1.0, 1.0, 1.0);
        textDisplay("Score:",5,570);
        glColor3f (1.0, 1.0, 1.0);
        scoreDisplay(45,570,score);
        textDisplay("Time:",1100,570);
        scoreDisplay(1160,570,gameTime);

        fishPlayer.drawFish();
        
        for(int i=0;i< arrayOfFish.size();++i){
            arrayOfFish[i].moveFish();
            if (!arrayOfFish[i].fishPlayer && checkCollision(fishPlayer, arrayOfFish[i])){
                if (arrayOfFish[i].redFish > 0) {
                    score--;
                } else if (arrayOfFish[i].yellowFish > 0) {
                    score++;
                }
                arrayOfFish.erase(arrayOfFish.begin()+i); //Removes the fish at index i from the fishPool.
            }else{
                arrayOfFish[i].drawFish();
            }
        }
        allYellowFishGone = true;
        for (const auto& fish : arrayOfFish) {// Check if all yellowfish are gone
            if (!fish.fishPlayer && fish.yellowFish > 0) {
                allYellowFishGone = false;
                break;
            }
        }
    } else {
        if (allYellowFishGone) {
            glColor3f(1.0, 1.0, 0.0); // Green color for win message
            gameOver("YOU WIN!", 500, 450);
            gameOver("Max Score: ",500,260);
            glColor3f (1.0, 1.0, 0.0);
            scoreDisplay(620,260,score);
            gameOver("Play Again by pressing -> F2",450,160);
            textDisplay("Created By Yehia Sharawy & Abdelrahman Sharaf",900,10);
        } else {
            glColor3f(1.0, 0.0, 0.0);
            gameOver("YOU FAILED!", 500, 450);
            gameOver("Max Score: ",500,260);
            glColor3f (1.0, 0.0, 0.0);
            scoreDisplay(620,260,score);
            gameOver("Play Again by pressing -> F2",450,160);
            textDisplay("Created By Yehia Sharawy & Abdelrahman Sharaf",900,10);
        }
    }
    glutSwapBuffers();
    glFlush();
}


int main(int argc, char *argv[])
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(physicalWidth,physicalHeight);
    glutInitWindowPosition(250,200);
    glutCreateWindow("Fishh"); //Title of the window.
    init2D(0.07,0.01,0.75); //Background Color
    glutPassiveMotionFunc(passiveMouse);
    glutSpecialFunc(specialKeyboard);
    glutTimerFunc(0, timerrand, 0);
    glutTimerFunc(0, timer, 0);
    glutDisplayFunc(display);
    glutMainLoop();
    Timer(0);
}
