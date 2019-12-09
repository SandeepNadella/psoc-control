/* ========================================
 * Project: Social Robot
 * Course: CSE 564 - Software Design
 * Team: 4
 * University: Arizona State University
 * Authors: Sandeep Nadella, Sumanth Paranjape, Arvind Ramesh
 * Initial Version Date: 11/16/2019
 * Version: 0.1
 * ========================================
*/

#include <project.h>

void raiseRightEyebrow(int value) {
    Right_Eyebrow_WriteCompare1(value);
    Right_Eyebrow_WriteCompare2(value);
}

void raiseLeftEyebrow(int value) {
    Left_Eyebrow_WriteCompare1(value);
    Left_Eyebrow_WriteCompare2(value);
}

void moveEyesLeft(int value){
   Right_Eye_WriteCompare1(value);
   Left_Eye_WriteCompare1(value);
}

void moveEyesRight(int value){
   Right_Eye_WriteCompare2(value);
   Left_Eye_WriteCompare2(value);
}

void moveEyes(){
    moveEyesLeft(1000);
    moveEyesRight(1000);
}

void moveNeck(){
    //Moves neck in up direction
    Neck_WriteCompare1(700);
    Neck_WriteCompare2(700);
    CyDelay(2000);
    //Moves neck down
    Neck_WriteCompare1(1000);
    Neck_WriteCompare2(1000);
}

void rotateHead(){
    //Rotates Base
    Base_Jaw_WriteCompare1(700);
    CyDelay(3000);
    Base_Jaw_WriteCompare1(1000);
}

void tiltNeck(){
    Neck_WriteCompare1(700);
    CyDelay(1000);
    Neck_WriteCompare1(1000);
    CyDelay(1000);
    Neck_WriteCompare2(700);
    CyDelay(1000);
    Neck_WriteCompare2(1000);
}

void moveJaw(){
    Jaw_WriteCompare1(200);
    CyDelay(1000);
    Jaw_WriteCompare1(1000);
    CyDelay(1000);
}

void moveRightLip(int value){
    Lip_WriteCompare1(value);
}

void moveLeftLip(int value){
    Lip_WriteCompare2(value);
}

void initializeAllComponenets() {
    Right_Eyebrow_Start();
    Left_Eyebrow_Start();
    Right_Eye_Start();
    Left_Eye_Start();
    Neck_Start();
    Base_Jaw_Start();
    Lip_Start();
    Jaw_Start();
}
void stopAllComponents() {
    Right_Eyebrow_Stop();
    Left_Eyebrow_Stop();
    Right_Eye_Stop();
    Left_Eye_Stop();
    Neck_Stop();
    Base_Jaw_Stop();
    Lip_Stop();
    Jaw_Stop();
}
int main(void)
{
    CyGlobalIntEnable;
    initializeAllComponenets();
    /* Does a sequence of raisedEyeBrows, moveEyes, move lips,
           move neck, tilt neck and rotate head repeatedly*/
    for(;;){
        raiseRightEyebrow(500);
        raiseLeftEyebrow(500);
        CyDelay(1500);
        moveEyesLeft(200);
        moveEyesRight(200);
        CyDelay(1500);
        moveRightLip(1000);
        moveLeftLip(1000);
        CyDelay(1500);
        
        raiseRightEyebrow(200);
        raiseLeftEyebrow(200);
        CyDelay(1500);
        moveEyesLeft(500);
        moveEyesRight(500);
        CyDelay(1500);
        moveRightLip(200);
        moveLeftLip(200);
        CyDelay(1500); 
        
        moveNeck();
        tiltNeck();
        rotateHead();
        CyDelay(1500); 
        moveJaw();
    }
    
    stopAllComponents();
}
