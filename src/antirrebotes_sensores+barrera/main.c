#include "main.h"


int8 time;
int8 trainsInside;
int1 anySensorChangeDetected;
sensor sensors[NUM_OF_SENSORS];


#int_RB
void  RB_isr(void) 
{   

   
   
   int i;
   
   int a;
   int b;
   for(i=0; i<NUM_OF_SENSORS;i++){
   
       
      sensors[i].last=a=sensors[i].current;
      sensors[i].current=b=input(sensors[i].pin_in);      
      
      //if(sensors[i].current != sensors[i].last){ //No va bien el compilador
      if(a != b){
      //Ha cambiado el sensor
      if(i==BEFORE_BARRIER_SENSOR || i==AFTER_BARRIER_SENSOR){
         anySensorChangeDetected=1;
      }
                        
            if(sensors[i].current == 1){             
            //Se ha activado el sensor
            
               if (sensors[i].status == IDLE){
                  
                  if(i==BEFORE_BARRIER_SENSOR){
                     trainIn();
                  }
               
                  output_high(sensors[i].pin_out);  
                  interruptionPulse();
               }
               sensors[i].status = ACTIVE;
               
            }else{
               //Se ha desactivado el sensor               
               sensors[i].debounceTime=time;
               sensors[i].status=DEBOUNCING;
            }            
      }
   }
}


#int_TIMER1
void  TIMER1_isr(void) 
{

   
   time++;
   
   int i;
   for(i=0; i<NUM_OF_SENSORS;i++){
      if(sensors[i].status==DEBOUNCING){
         if((time-sensors[i].debounceTime)>=DEBOUNCE_TIME){
            output_low(sensors[i].pin_out);
            
            if(i==AFTER_BARRIER_SENSOR){
               trainOut();
            }
            
            sensors[i].status=IDLE;
            interruptionPulse();
         }
      }
   }
   

}

#inline
void interruptionPulse(void){
   output_high(INT_PIN);
   output_low(INT_PIN);
}

#inline
void closeBarrier(void){
   set_pwm1_duty(PWM_CLOSE);
}

#inline
void openBarrier(void){
   set_pwm1_duty(PWM_OPEN);
}

#inline
void trainIn(void){
   closeBarrier();
   trainsInside++;                  
}

#inline
void trainOut(void){
  
   if(trainsInside>0){
      trainsInside--;
   }
   if(trainsInside==0){
      openBarrier();
   }                        
}





void main()
{

   setup_oscillator(OSC_500KHZ|OSC_INTRC|OSC_31250|OSC_PLL_OFF);
   setup_adc_ports(NO_ANALOGS|VSS_VDD);
   setup_adc(ADC_CLOCK_DIV_2);
   setup_spi(SPI_SS_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);
   setup_timer_2(T2_DIV_BY_16,155,1);
   setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
   setup_ccp1(CCP_PWM);
   set_pwm1_duty(PWM_CLOSE);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   
   time=0;
   trainsInside=0;
   anySensorChangeDetected=0;
   
   sensors[0].pin_in=PIN_B4;
   sensors[0].pin_out=PIN_A0;
   
   sensors[1].pin_in=PIN_B5;
   sensors[1].pin_out=PIN_A1;
   
   sensors[2].pin_in=PIN_B6;
   sensors[2].pin_out=PIN_A2;
   
   sensors[3].pin_in=PIN_B7;
   sensors[3].pin_out=PIN_A3;
   
   int8 i;
   for(i=0; i<NUM_OF_SENSORS;i++){
      sensors[i].last=0;
      sensors[i].current=0;
      sensors[i].status=IDLE;
      input(sensors[i].pin_in);
      output_low(sensors[i].pin_out);
   }
   
   
   
   
   
   
   enable_interrupts(INT_TIMER1);
   enable_interrupts(INT_RB);
   enable_interrupts(GLOBAL);
   
   delay_ms(STARTUP_SAFE_TIME);
   if(!anySensorChangeDetected){
      openBarrier();
   }

   
   while(1){
   
  
   
      
   }

}
