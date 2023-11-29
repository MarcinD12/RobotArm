#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>
#include "hardware/adc.h"
#include "hardware/gpio.h"

#define dupa 3;

const int motor1btnA=15;
const int motor1btnB=14;
const int ShoulderUpBtn=13;
const int ShoulderDownBtn=12;
const int BaseCwBtn=11;
const int BaseCcwBtn=10;

const int motor1pwmA=16;
const int motor1pwmB=17;
const int motor2pwmA=18;
const int motor2pwmB=19;
const int motor3pwmA=20;
const int motor3pwmB=21;

const int basemax=2200;
const int basemin=160;
const int shouldermin=3200;
const int shouldermax=2700;   

void PWMMotor(uint slice,uint channel,int analogRead);
void ToMax(uint slice,uint channel);
void ToMin(uint slice,uint channel);
void Blink();
void ADCread(int *encoders);
void ReadGPIO(int readgpio[]);
void SetPWM(short pin,short dir);

int motorpinout[6]={motor1btnA, motor1btnB,ShoulderUpBtn,ShoulderDownBtn,BaseCwBtn,BaseCcwBtn};
int gpioreadpins[6];

short PWMLevels[6]={0};
uint PWMSlices[6];
uint PWMChannels[6];
int main() {
    printf("Running:");
    int encoders[3]={0};
    //adc setup
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);

    //led setup
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    //pwm pins setup
    gpio_set_function(motor1pwmA, GPIO_FUNC_PWM);
    gpio_set_function(motor1pwmB, GPIO_FUNC_PWM);
    gpio_set_function(motor2pwmA, GPIO_FUNC_PWM);
    gpio_set_function(motor2pwmB, GPIO_FUNC_PWM);
    gpio_set_function(motor3pwmA, GPIO_FUNC_PWM);
    gpio_set_function(motor3pwmB, GPIO_FUNC_PWM);
    stdio_init_all();
    
    //buttons setup
    gpio_set_dir(motor1btnA, GPIO_IN);
    gpio_set_dir(motor1btnB, GPIO_IN);
    gpio_set_dir(ShoulderUpBtn, GPIO_IN);
    gpio_set_dir(ShoulderDownBtn, GPIO_IN);
    gpio_set_dir(BaseCwBtn, GPIO_IN);
    gpio_set_dir(BaseCcwBtn, GPIO_IN);
    
    
    //Motor1 PWM setup
    uint SliceAMotor1 = pwm_gpio_to_slice_num(motor1pwmA);
    uint ChannelAMotor1 = pwm_gpio_to_channel(motor1pwmA);
    uint SliceBMotor1 = pwm_gpio_to_slice_num(motor1pwmB);
    uint ChannelBMotor1 = pwm_gpio_to_channel(motor1pwmB);
    pwm_set_wrap(SliceAMotor1, 16384);
    pwm_set_enabled(SliceAMotor1, true);
    pwm_set_wrap(SliceBMotor1, 16384);
    pwm_set_enabled(SliceBMotor1, true); 

    //Motor2 PWM setup
    uint SliceAMotor2 = pwm_gpio_to_slice_num(motor2pwmA);
    uint ChannelAMotor2 = pwm_gpio_to_channel(motor2pwmA);
    uint SliceBMotor2 = pwm_gpio_to_slice_num(motor2pwmB);
    uint ChannelBMotor2 = pwm_gpio_to_channel(motor2pwmB);
    pwm_set_wrap(SliceAMotor2, 16384);
    pwm_set_enabled(SliceAMotor2, true);
    pwm_set_wrap(SliceBMotor2, 16384);
    pwm_set_enabled(SliceBMotor2, true);

    //Motor3 PWM setup
    uint SliceAMotor3 = pwm_gpio_to_slice_num(motor3pwmA);
    uint ChannelAMotor3 = pwm_gpio_to_channel(motor3pwmA);
    uint SliceBMotor3 = pwm_gpio_to_slice_num(motor3pwmB);
    uint ChannelBMotor3 = pwm_gpio_to_channel(motor3pwmB);
    pwm_set_wrap(SliceAMotor3, 16384);
    pwm_set_enabled(SliceAMotor3, true);
    pwm_set_wrap(SliceBMotor3, 16384);
    pwm_set_enabled(SliceBMotor3, true); 
    PWMSlices[0]=SliceAMotor1;
    PWMSlices[1]=SliceBMotor1;
    PWMSlices[2]=SliceAMotor2;
    PWMSlices[3]=SliceBMotor2;
    PWMSlices[4]=SliceAMotor3;
    PWMSlices[5]=SliceBMotor3;

    PWMChannels[0]=ChannelAMotor1;
    PWMChannels[1]=ChannelBMotor1;
    PWMChannels[2]=ChannelAMotor2;
    PWMChannels[3]=ChannelBMotor2;
    PWMChannels[4]=ChannelAMotor3;
    PWMChannels[5]=ChannelBMotor3;


    //ToMax(SliceA, ChannelA);
    // PWMSlices[]={SliceAMotor1,SliceBMotor1,SliceAMotor2,SliceBMotor2,SliceAMotor3,SliceBMotor3};
    // uint PWMChannels[]={ChannelAMotor1,ChannelBMotor1,ChannelAMotor2,ChannelBMotor2,ChannelAMotor3,ChannelBMotor3};

    uint MotorALvl=3600;
    for(;;)
    {   
        uint mask=0;
        ReadGPIO(gpioreadpins);
        ADCread(encoders);
        printf("base: %d\n",encoders[0]);
        printf("shoulder: %d\n",encoders[1]);
        // printf("shoulder: %f\n",encoders[2]);
        //shoulder limits and increments
        if(gpioreadpins[2]==1 && encoders[1]>shouldermax){
            SetPWM(2,1);
        }
        else{
            SetPWM(2,0);
        }

        if(gpioreadpins[3]==1 && encoders[1]<shouldermin){
            SetPWM(3,1);
        }
        else{
            SetPWM(3,0);
        }
        //base limits and increment
        if(gpioreadpins[4]==1 && encoders[0]<basemax){
            SetPWM(4,1);
        }
        else{
            SetPWM(4,0);
        }

        if(gpioreadpins[5]==1 && encoders[0]>basemin){
            SetPWM(5,1);
        }
        else{
            SetPWM(5,0);
        }






        ////
        // for (int i=5; i >= 0; i--)
        // { 
        //     if(gpioreadpins[i]==1)
        //     {
        //         if (PWMLevels[i]>0 & PWMLevels[i]<=16384){
        //             PWMLevels[i]+=300;
        //         }
        //         else if(PWMLevels[i]+300>=16384)
        //             PWMLevels[i]=16384; 
        //         else{
        //             PWMLevels[i]=3600;
        //         }

        //         pwm_set_chan_level(PWMSlices[i],PWMChannels[i],PWMLevels[i]);
        //         mask=mask|(1u<<motorpinout[i]-1); //mask just for test
        //         printf("Channel: %d level: %d\n",PWMChannels[i],PWMLevels[i]);
        //     }
        //     else
        //     {
        //         PWMLevels[i]=(PWMLevels[i]-1000>=0)?PWMLevels[i]-=1000:0;           
        //         pwm_set_chan_level(PWMSlices[i],PWMChannels[i],PWMLevels[i]);
        //     }
        // }
        


        // int JoyPosition=ADCread();
        
        // if (JoyPosition>105)
        // {
        //     printf("%d    ",JoyPosition);
        //     power=(uint)3600+(12784*((JoyPosition-100)*0.01));    
        //     printf("%d\n",power);   
        //     pwm_set_chan_level(SliceA, ChannelA, power);
        //     sleep_ms(2);
        // }
        // else if(JoyPosition<95)
        // {
        //     power=(uint)3600+(12784-(12784*JoyPosition*0.01));   
        //     printf("%d\n",power);
        //     pwm_set_chan_level(SliceB, ChannelB, power);
        //     sleep_ms(2);
        // }
        // else
        // {
        //     pwm_set_chan_level(SliceB, ChannelB, 0);
        //     pwm_set_chan_level(SliceA, ChannelA, 0);
        // }
        sleep_ms(50);
        Blink();
    }
}
void SetPWM(short pin,short dir){
if (dir==1){
    if (PWMLevels[pin]>0 & PWMLevels[pin]<=16384){
        PWMLevels[pin]+=300;
    }
    else if(PWMLevels[pin]+300>=16384){
        PWMLevels[pin]=16384; 
    }
    else{
        PWMLevels[pin]=3600;
    }
    pwm_set_chan_level(PWMSlices[pin],PWMChannels[pin],PWMLevels[pin]);              
}
else{
    PWMLevels[pin]=(PWMLevels[pin]-1000>=0)?PWMLevels[pin]-=1000:0;           
    pwm_set_chan_level(PWMSlices[pin],PWMChannels[pin],PWMLevels[pin]);
}
}
void ReadGPIO(int* readgpio)
{
    readgpio[0]=gpio_get(motor1btnA);
    readgpio[1]=gpio_get(motor1btnB);
    readgpio[2]=gpio_get(ShoulderUpBtn);
    readgpio[3]=gpio_get(ShoulderDownBtn);
    readgpio[4]=gpio_get(BaseCwBtn);
    readgpio[5]=gpio_get(BaseCcwBtn);
}

//for testing motors
void ToMax(uint slice,uint channel){
    for (short i = 3600; i < 16384; i++)
    {
        pwm_set_chan_level(slice, channel, i); // 50%
        sleep_ms(2);
        printf("%d\n",i);       
    }
    ToMin(slice, channel);
}
void ToMin(uint slice,uint channel){
    for (short i = 16384; i > 3600; i--)
    {
        pwm_set_chan_level(slice, channel, i); // 50%
        sleep_ms(2);
        printf("%d\n",i);
    }
    ToMax(slice, channel);
}

void ADCread(int *encoders)
{
    adc_select_input(0);
    encoders[0]=adc_read();
    //printf("%.0f\n",encoders[0]);
    adc_select_input(1);
    encoders[1]= adc_read();
    adc_select_input(2);
    // encoders[2]= adc_read();
    // //printf("%f",RawShoulder);
}
void PWMMotor(uint slice,uint channel,int analogRead)
{
    if(analogRead>51)
    {
    pwm_set_chan_level(slice, channel, 16384*analogRead);
    }
    if(analogRead<49){

    }
    
}

void Blink()
{
    gpio_put(25, 1);
    sleep_ms(20);
    gpio_put(25, 0);
    sleep_ms(20);
}