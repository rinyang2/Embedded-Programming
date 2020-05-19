#include "FreeRTOS.h"
#include "glob_def.h"
#include "emp_type.h"
#include "tm4c123gh6pm.h"
#include "string.h"
#include "file.h"
#include "lcd.h"
#include "key.h"
#include "ui.h"
#include "scale.h"
#include "semphr.h"
#include "adc.h"
#include "digiswitch.h"
#include "read_card.h"


void ui_task(void *pvParameters)
{
    INT8U temp =0;

    while(1){
        INT8U key = 0;
        INT8U TEM = 0;
        key = get_keyboard();

        gfprintf(COM2, "%c%cCASH:1 CARD:2", 0x1B, 0x80);




        if(key=='1'){
            gfprintf(COM2, "%c%cINSERT CASH", 0x1B, 0xA8);
            TEM=driver() * 10;
            gfprintf(COM2, "%c%c%4uDKK inserted", 0x1B, 0xA8,TEM);
        }








        else if(key=='2'){
            read_card();
           // read_pin();
        }


    }
    gfprintf(COM2, "%c%cENDOFLOOP", 0x1B, 0xA8);
};

