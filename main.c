#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>
#include "hardware/adc.h"
#include "hardware/gpio.h"

#define dupa 3;

const int motor1btnA=15;
const int motor1btnB=14;
const int motor2btnA=13;
const int motor2btnB=12;
const int motor3btnA=11;
const int motor3btnB=10;

const int motor1pwmA=16;
const int motor1pwmB=17;
const int motor2pwmA=18;
const int motor2pwmB=19;
const int motor3pwmA=20;
const int motor3pwmB=21;



void PWMMotor(uint slice,uint channel,int analogRead);
void ToMax(uint slice,uint channel);
void ToMin(uint slice,uint channel);
void Blink();
int ADCread();
void ReadGPIO(int readgpio[]);

int motorpinout[6]={motor1btnA, motor1btnB,motor2btnA,motor2btnB,motor3btnA,motor3btnB};
int gpioreadpins[6];

int main() {
    printf("Running:");
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_set_function(motor1pwmA, GPIO_FUNC_PWM);
    gpio_set_function(motor1pwmB, GPIO_FUNC_PWM);
    gpio_set_function(motor2pwmA, GPIO_FUNC_PWM);
    gpio_set_function(motor2pwmB, GPIO_FUNC_PWM);
    gpio_set_function(motor3pwmA, GPIO_FUNC_PWM);
    gpio_set_function(motor3pwmB, GPIO_FUNC_PWM);
    stdio_init_all();

    gpio_set_dir(motor1btnA, GPIO_IN);
    gpio_set_dir(motor1btnB, GPIO_IN);
    gpio_set_dir(motor2btnA, GPIO_IN);
    gpio_set_dir(motor2btnB, GPIO_IN);
    gpio_set_dir(motor3btnA, GPIO_IN);
    gpio_set_dir(motor3btnB, GPIO_IN);
    
    
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


    //ToMax(SliceA, ChannelA);
    uint PWMSlices[]={SliceAMotor1,SliceBMotor1,SliceAMotor2,SliceBMotor2,SliceAMotor3,SliceBMotor3};
    uint PWMChannels[]={ChannelAMotor1,ChannelBMotor1,ChannelAMotor2,ChannelBMotor2,ChannelAMotor3,ChannelBMotor3};
    short PWMLevels[6]={0};
    uint MotorALvl=3600;
    for(;;)
    {   
        uint mask=0;
        ReadGPIO(gpioreadpins);
        for (int i=5; i >= 0; i--)
        { 
            if(gpioreadpins[i]==1)
            {
                if (PWMLevels[i]>0 & PWMLevels[i]<=16384){
                    PWMLevels[i]+=300;
                }
                else if(PWMLevels[i]+300>=16384)
                    PWMLevels[i]=16384; 
                else{
                    PWMLevels[i]=3600;
                }

                pwm_set_chan_level(PWMSlices[i],PWMChannels[i],PWMLevels[i]);
                mask=mask|(1u<<motorpinout[i]-1); //mask just for test
                printf("Channel: %d level: %d\n",PWMChannels[i],PWMLevels[i]);
            }
            else
            {
                PWMLevels[i]=(PWMLevels[i]-1000>=0)?PWMLevels[i]-=1000:0;           
                pwm_set_chan_level(PWMSlices[i],PWMChannels[i],PWMLevels[i]);
            }
        }
        // printf("mask: ");
        // printf("%d\n",mask);
        
        sleep_ms(10);


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
        Blink();
    }
}

void ReadGPIO(int* readgpio)
{
    readgpio[0]=gpio_get(motor1btnA);
    readgpio[1]=gpio_get(motor1btnB);
    readgpio[2]=gpio_get(motor2btnA);
    readgpio[3]=gpio_get(motor2btnB);
    readgpio[4]=gpio_get(motor3btnA);
    readgpio[5]=gpio_get(motor3btnB);
}

void ToMax(uint slice,uint channel){
    for (short i = 3600; i < 16384; i++)
    {
        pwm_set_chan_level(slice, channel, i); // 50%
        sleep_ms(2);
        printf("aeebe");
        printf("%d\n",i);       
    }
    ToMin(slice, channel);
}
void ToMin(uint slice,uint channel){
    for (short i = 16384; i > 3600; i--)
    {
        pwm_set_chan_level(slice, channel, i); // 50%
        sleep_ms(2);
        printf("aeee");
        printf("%d\n",i);
    }
    ToMax(slice, channel);
}

int ADCread()
{
    adc_select_input(0);
    double RawBase = adc_read()*0.0488;//
    //printf("%.0f\n",0-(-RawBase));
    adc_select_input(0);
    double RawShoulder= adc_read();
    return (int)RawBase;
    //printf("%f",RawShoulder);


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