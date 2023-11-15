#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>
#include "hardware/adc.h"

void ToMax(uint slice,uint channel);
void ToMin(uint slice,uint channel);
void Blink();
int ADCread();
void PWMMotor(uint slice,uint channel,int analogRead);
int main() {
    printf("Running:");
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_set_function(16, GPIO_FUNC_PWM);
    gpio_set_function(17, GPIO_FUNC_PWM);
    stdio_init_all();
    
    
    
    uint SliceA = pwm_gpio_to_slice_num(16);
    uint ChannelA = pwm_gpio_to_channel(16);
    uint SliceB = pwm_gpio_to_slice_num(17);
    uint ChannelB = pwm_gpio_to_channel(17);
    pwm_set_wrap(SliceA, 16384);
    //pwm_set_chan_level(SliceA, ChannelA, 8192); // 50%
    pwm_set_enabled(SliceA, true); // Aktywuj PWM
    pwm_set_wrap(SliceB, 16384);
    //pwm_set_chan_level(SliceB, ChannelB, 8192); // 50%
    pwm_set_enabled(SliceB, true); // Aktywuj PWM
    //ToMax(SliceA, ChannelA);
    
    uint power;
    for(;;)
    {
        
        int JoyPosition=ADCread();
        
        if (JoyPosition>103)
        {
            printf("%d    ",JoyPosition);
            power=(uint)3600+(12784*((JoyPosition-100)*0.01));    
            printf("%d\n",power);   
            pwm_set_chan_level(SliceA, ChannelA, power);
            sleep_ms(2);
        }
        else if(JoyPosition<97)
        {
            power=(uint)3600+(12784-(12784*JoyPosition*0.01));   
            printf("%d\n",power);
            pwm_set_chan_level(SliceB, ChannelB, power);
            sleep_ms(2);
        }
        else
        {
            pwm_set_chan_level(SliceB, ChannelB, 0);
            pwm_set_chan_level(SliceA, ChannelA, 0);
        }
        Blink();
    }
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