#include <xc.h>
#include "keypad.h"
#include "timer.h"

/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */
//4x3 keypad will require 4 ODC output pins and 3 input pins with a pull up resister (external)
#define TRIS_0 TRISEbits.TRISE0     //row 1 output
#define TRIS_1 TRISDbits.TRISD10     //row 2 output
#define TRIS_2 TRISCbits.TRISC1     //row 3 output    
#define TRIS_3 TRISCbits.TRISC3     //row 4 output
#define TRIS_4 TRISAbits.TRISA6     //col 1 input    
#define TRIS_5 TRISAbits.TRISA7     //col 2 input 
#define TRIS_6 TRISGbits.TRISG1     //col 3 input 
#define ODC_0 ODCEbits.ODCE0        //row 1 ODC       
#define ODC_1 ODCDbits.ODCD10        //row 2 ODC 
#define ODC_2 ODCCbits.ODCC1        //row 3 ODC       
#define ODC_3 ODCCbits.ODCC3        //row 4 ODC 
#define LAT_0 LATEbits.LATE0        //row 1 write           
#define LAT_1 LATDbits.LATD10        //row 2 write
#define LAT_2 LATCbits.LATC1        //row 3 write      
#define LAT_3 LATCbits.LATC3        //row 4 write
#define PORT_0 PORTAbits.RA6        //read col1 input
#define PORT_1 PORTAbits.RA7        //read col2 input
#define PORT_2 PORTGbits.RG1        //read col3 input

#define INPUT 1 
#define OUTPUT 0
#define ENABLE 1

void initKeypad(void)
{
    TRIS_0 = OUTPUT;TRIS_1 = OUTPUT;TRIS_2 = OUTPUT;TRIS_3 = OUTPUT;
    TRIS_4 = INPUT;TRIS_5 = INPUT;TRIS_6 = INPUT;
    ODC_0 = ENABLE;ODC_1 = ENABLE;ODC_2 = ENABLE;ODC_3 = ENABLE;
    LAT_0 = 1;LAT_1 = 1;LAT_2 = 1;LAT_3 = 1;
    
    CNCONAbits.ON = 1;CNCONGbits.ON = 1;
    IEC1bits.CNAIE = 1;IEC1bits.CNGIE = 1;
    
    CNENAbits.CNIEA6 = 1;CNENAbits.CNIEA7 = 1;CNENGbits.CNIEG1 = 1;
    
    IPC8bits.CNIP = 7;
    IPC8bits.CNIS = 2;
    
    IFS1bits.CNAIF = 0;
    IFS1bits.CNGIF = 0;
    
    /*overall CN and pin specific interrupts 
     * CNCONGbits.ON = 1;      //CN for G 
    CNENGbits.CNIEG13 = 1;  //CN on pin
    IEC1bits.CNGIE = 1;     //overall CN
    CNPUGbits.CNPUG13 = 1;  //PUR
    IPC8bits.CNIP = 7;
    IPC8bits.CNIS = 2;
    IFS1bits.CNGIF = 0;

     */
}

/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed.
 */
//ODC 1 is enable. Turn off/on to scan
char scanKeypad(void){
    char key = -1;
    int found = 0;
    
    LAT_0 = 1;LAT_1 = 0;LAT_2 = 0;LAT_3 = 0;
    if(PORT_0 == 0){key = 1;found++;}
    if(PORT_1 == 0){key = 2;found++;}
    if(PORT_2 == 0){key = 3;found++;}
    
    LAT_0 = 0;LAT_1 = 1;LAT_2 = 0;LAT_3 = 0;
    if(PORT_0 == 0){key = 4;found++;}
    if(PORT_1 == 0){key = 5;found++;}
    if(PORT_2 == 0){key = 6;found++;}
    
    LAT_0 = 0;LAT_1 = 0;LAT_2 = 1;LAT_3 = 0;
    if(PORT_0 == 0){key = 7;found++;}
    if(PORT_1 == 0){key = 8;found++;}
    if(PORT_2 == 0){key = 9;found++;}
    
    LAT_0 = 0;LAT_1 = 0;LAT_2 = 0;LAT_3 = 1;
    if(PORT_0 == 0){key = '*';found++;}
    if(PORT_1 == 0){key = 0;found++;}
    if(PORT_2 == 0){key = '#';found++;}
    
    if(found>1)
        key = -1;
    LAT_0 = 1;LAT_1 = 1;LAT_2 = 1;LAT_3 = 1;
    return key;
}