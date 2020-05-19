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
#include "read_card.h"
#include "gas_select.h"
#include "digiswitch.h"
#include "button.h"
#include "gas_pump.h"

void ui_task(void *pvParameters)
{

    while(1){
        INT8U key = 0;
        INT16U TEMP = 0;
        key = get_keyboard();

        gfprintf(COM2, "%c%cCASH:1 CARD:2   ", 0x1B, 0x80);



        if(key=='1'){
            gfprintf(COM2, "%c%cINSERT CASH", 0x1B, 0xA8);
            TEMP=driver() * 10;
            gfprintf(COM2, "%c%c%4uDKK inserted", 0x1B, 0xA8,TEMP);
            vTaskDelay(2000 / portTICK_RATE_MS);
            cash_gas_pump(gas_select(), TEMP);
        }



        else if(key=='2'){
           if(!read_card()){
               gfprintf(COM2, "%c%cCARD REJECTED", 0x1B, 0xA8);
               vTaskDelay(2000 / portTICK_RATE_MS);
               gfprintf(COM2, "%c%c                ", 0x1B, 0xA8);
           }
           else{
               gas_pump(gas_select());
           }
        }


    }
};

