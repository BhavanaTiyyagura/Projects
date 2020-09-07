
#include "SysTick.h"

#define LIGHT                   (*((volatile unsigned long *)0x400050FC))
#define GPIO_PORTB_OUT          (*((volatile unsigned long *)0x400050FC)) 
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
#define GPIO_PORTE_IN           (*((volatile unsigned long *)0x4002400C)) 
#define SENSOR                  (*((volatile unsigned long *)0x4002400C))

#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOE      0x00000010  
#define SYSCTL_RCGC2_GPIOB      0x00000002 


// Linked data structure
struct State {
  unsigned long Out; 
  unsigned long Time;  
  unsigned long Next[4];}; 
typedef const struct State STyp;
#define goN   0
#define waitN 1
#define goE   2
#define waitE 3
STyp FSM[4]={
 {0x21,3000,{goN,waitN,goN,waitN}},  {0x22, 500,{goE,goE,goE,goE}}, {0x0C,3000,{goE,goE,waitE,waitE}}, {0x14, 500,{goN,goN,goN,goN}}};

unsigned long S;  // index to the current state 
unsigned long Input; 
int main(void){ volatile unsigned long delay;

  SysTick_Init();   
  SYSCTL_RCGC2_R |= 0x12;      
  delay = SYSCTL_RCGC2_R;      
  GPIO_PORTE_AMSEL_R &= ~0x03; //disable analog function
  GPIO_PORTE_PCTL_R &= ~0x000000FF; //enable regular GPIO
  GPIO_PORTE_DIR_R &= ~0x03;   //inputs on PE1-0
  GPIO_PORTE_AFSEL_R &= ~0x03; //regular function on PE1-0
  GPIO_PORTE_DEN_R |= 0x03;    //enable digital on PE1-0
  GPIO_PORTB_AMSEL_R &= ~0x3F; //disable analog function on PB5-0
  GPIO_PORTB_PCTL_R &= ~0x00FFFFFF; //enable regular GPIO
  GPIO_PORTB_DIR_R |= 0x3F;    //outputs on PB5-0
  GPIO_PORTB_AFSEL_R &= ~0x3F; //regular function on PB5-0
  GPIO_PORTB_DEN_R |= 0x3F;    //enable digital on PB5-0
  S = goN;  
  while(1){
    LIGHT = FSM[S].Out;  // set lights
    SysTick_Wait10ms(FSM[S].Time);
    Input = SENSOR;     // read sensors
    S = FSM[S].Next[Input];  
  }
}

