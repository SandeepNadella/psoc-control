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


void raiseEyebrow() {
    
    RightEyebrow_WriteCompare1(500);
    RightEyebrow_WriteCompare2(1600);
    CyDelay(900);
    RightEyebrow_WriteCompare1(500);
    RightEyebrow_WriteCompare2(1600);
    CyDelay(900);
    
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    RightEyebrow_Start();
    int c =0;
    for(;;)
    {
        raiseEyebrow();
        c++;
        if(c==5){
            break;
        }
    }
}


/* [] END OF FILE */
