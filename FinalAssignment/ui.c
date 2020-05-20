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
#include "uart0.h"

void ui_task(void *pvParameters)
{

    while(1){
        INT8U key = 0;
        INT16U TEMP = 0;
        key = get_keyboard();

        gfprintf(COM2, "%c%cCASH:1 CARD:2   ", 0x1B, 0x80);



        if(key=='1'){
            gfprintf(COM2, "%c%cINSERT CASH", 0x1B, 0xA8);

            //digiswitch.c, driver() returns value of the digiswitch, which is the cash input
            TEMP=driver() * 10;

            gfprintf(COM2, "%c%c%4uDKK inserted", 0x1B, 0xA8,TEMP);
            vTaskDelay(2000 / portTICK_RATE_MS);

            //gas_select() returns the price of the gas you chose
            //cash_gas_pump() receives 2 parameter(price of gas you chose, the input amount of cash) and pumps gas when sw2 is pushed, and halted when sw1 is pushed
            cash_gas_pump(gas_select(), TEMP);

            gfprintf(COM1, "%c%cCASH", 0x1B, 0xA8);
            gfprintf(COM1, "%c%c%4uDKK inserted", 0x1B, 0xA8,TEMP);
        }



        else if(key=='2'){
           //read_card() reads 12 digit from keyboard(8 for card, 4 for pin) and add the end digit of the card and pin.
           //If the sum is odd, the function returns 0, if even, returns 1
           if(!read_card()){
               gfprintf(COM2, "%c%cCARD REJECTED", 0x1B, 0xA8);
               vTaskDelay(2000 / portTICK_RATE_MS);
               gfprintf(COM2, "%c%c                ", 0x1B, 0xA8);
           }
           else{
               //same function as cash_gas_pump(), but only 1 parameter since there is no cash input
               gas_pump(gas_select());
               gfprintf(COM1, "%c%cCARD", 0x1B, 0xA8);
               gfprintf(COM1, "%c%cGAS PRICE:%4u", 0x1B, 0xA8,(gas_select()/100)));
           }
        }


    }
};

