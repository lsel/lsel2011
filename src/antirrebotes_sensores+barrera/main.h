#include <18F2550.h>
#device adc=8

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES INTRC_IO                 //Internal RC Osc, no CLKOUT
#FUSES NOPROTECT                //Code not protected from reading
#FUSES BROWNOUT                 //Reset when brownout detected
#FUSES BORV28                   //Brownout reset at 2.8V
#FUSES PUT                      //Power Up Timer
#FUSES NOCPD                    //No EE protection
#FUSES STVREN                   //Stack full/underflow will cause reset
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOWRT                    //Program memory not write protected
#FUSES NOWRTD                   //Data EEPROM not write protected
#FUSES NOIESO                   //Internal External Switch Over mode disabled
#FUSES NOFCMEN                  //Fail-safe clock monitor disabled
#FUSES NOPBADEN                 //PORTB pins are configured as digital I/O on RESET
#FUSES NOWRTC                   //configuration not registers write protected
#FUSES NOWRTB                   //Boot block not write protected
#FUSES NOEBTR                   //Memory not protected from table reads
#FUSES NOEBTRB                  //Boot block not protected from table reads
#FUSES NOCPB                    //No Boot Block code protection
#FUSES NOMCLR                   //Master Clear pin used for I/O
#FUSES NOLPT1OSC                //Timer1 configured for higher power operation
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#FUSES PLL1                     //No PLL PreScaler
#FUSES CPUDIV1                  //No System Clock Postscaler
#FUSES NOUSBDIV                 //USB clock source comes from primary oscillator
#FUSES NOVREGEN                 //USB voltage regulator disabled

#use delay(clock=500000)

#define NUM_OF_SENSORS  4
#define DEBOUNCE_TIME 3 // Contado cada 524ms. 3x524ms=1572ms

#define IDLE 0
#define ACTIVE 1 
#define DEBOUNCING 2

#define INT_PIN PIN_B0

#define BEFORE_BARRIER_SENSOR 0
#define AFTER_BARRIER_SENSOR 1

#define PWM_CLOSE 0x000E
#define PWM_OPEN 0x0007

#define STARTUP_SAFE_TIME 10000

typedef struct{
  int1 current;
  int1 last;
  int16 pin_in; //From sensor
  int16 pin_out; //Debounced to Antares
  int8 status;
  int8 debounceTime;
}sensor;



#inline void interruptionPulse(void);

#inline void closeBarrier(void);
#inline void openBarrier(void);

#inline void trainIn(void);
#inline void trainOut(void);
