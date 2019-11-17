/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"


void initializeComponenets() {
    //RightEyebrow_Start();
    //LeftEyebrow_Start();
    EyeBallVertical_Start();
    EyelidGroup_Start();
    BaseJaw_Start();
    LeftEyebrow_Start();
    EyeBallHorizontal_Start();
    Neck_Start();
    Lip_Start();
}

void resetAllServoPositions() {
    RightEyebrow_WriteCompare1(200);
    LeftEyebrow_WriteCompare1(200);
    EyeBallVertical_WriteCompare1(200);
    EyelidGroup_WriteCompare1(200);
    BaseJaw_WriteCompare1(200);
    LeftEyebrow_WriteCompare1(200);
    EyeBallHorizontal_WriteCompare1(200);
    Neck_WriteCompare1(200);
    Lip_WriteCompare1(200);
    
    RightEyebrow_WriteCompare2(200);
    LeftEyebrow_WriteCompare2(200);
    EyeBallVertical_WriteCompare2(200);
    EyelidGroup_WriteCompare2(200);
    BaseJaw_WriteCompare2(200);
    LeftEyebrow_WriteCompare2(200);
    EyeBallHorizontal_WriteCompare2(200);
    Neck_WriteCompare2(200);
    Lip_WriteCompare2(200);
}

void resetLeftEyebrow() {
    LeftEyebrow_WriteCompare1(1000);
    LeftEyebrow_WriteCompare2(1000);
}

void sadLeftEyebrow() {
    LeftEyebrow_WriteCompare1(1600);
    LeftEyebrow_WriteCompare2(1600);
}

void raisedLeftEyebrow() {
    LeftEyebrow_WriteCompare1(200);
    LeftEyebrow_WriteCompare2(200);
}

void clearLeftEyebrow() {
    LeftEyebrow_WriteCompare1(0);
    LeftEyebrow_WriteCompare2(0);
}

void resetRightEyebrow() {
    RightEyebrow_WriteCompare1(1000);
    RightEyebrow_WriteCompare2(1000);
}

void raisedRightEyebrow() {
    RightEyebrow_WriteCompare1(1600);
    RightEyebrow_WriteCompare2(1600);
}

void sadRightEyebrow() {
    RightEyebrow_WriteCompare1(200);
    RightEyebrow_WriteCompare2(200);
}

void clearRightEyebrow() {
    RightEyebrow_WriteCompare1(0);
    RightEyebrow_WriteCompare2(0);
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    // Initialize the Components
    initializeComponenets();
    resetAllServoPositions();
    CyDelay(5000);
    
    // Actions
    while(1) {
        sadLeftEyebrow();
        sadRightEyebrow();
        CyDelay(3000);
        raisedLeftEyebrow();
        raisedRightEyebrow();
        CyDelay(3000);
    }
    
    
}


/* [] END OF FILE */
