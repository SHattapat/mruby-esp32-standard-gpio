#include <stdio.h>

#include <mruby.h>
#include <mruby/value.h>

#include "driver/gpio.h"
#include "driver/dac.h"
#include "driver/adc.h"

#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//GPIO // ADC
#define GPIO_MODE_DEF_PULLUP (BIT3)
#define GPIO_MODE_DEF_PULLDOWN (BIT3)
#define GPIO_MODE_INPUT_PULLUP ((GPIO_MODE_INPUT)|(GPIO_MODE_DEF_PULLUP))
#define GPIO_MODE_INPUT_PULLDOWN ((GPIO_MODE_INPUT)|(GPIO_MODE_DEF_PULLDOWN))
#define GPIO_MODE_OUTPUT (GPIO_MODE_DEF_OUTPUT) 
#define GPIO_MODE_OUTPUT_OD ((GPIO_MODE_DEF_OUTPUT)|(GPIO_MODE_DEF_OD))
#define GPIO_MODE_INPUT_OUTPUT_OD ((GPIO_MODE_DEF_INPUT)|(GPIO_MODE_DEF_OUTPUT)|(GPIO_MODE_DEF_OD))
#define GPIO_MODE_INPUT_OUTPUT ((GPIO_MODE_DEF_INPUT)|(GPIO_MODE_DEF_OUTPUT))

// PWM
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE_1               LEDC_HIGH_SPEED_MODE
#define LEDC_MODE_2              LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (18) // Define the output GPIO
//#define LEDC_CHANNEL            LEDC_CHANNEL_7
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (819) // Set duty to 50%. ((2 ** 13) - 1) * 50% = 4095
#define LEDC_FREQUENCY          (50) // Frequency in Hertz. Set frequency at 50 Hz

static mrb_value
mrb_esp32_pwm(mrb_state *mrb, mrb_value self) {
  mrb_value pin,duty,chan,freq;
  mrb_get_args(mrb, "oooo",&chan,&pin,&freq, &duty);
  /*
  if (!mrb_fixnum_p(pin) || !mrb_fixnum_p(dir)) {
    return mrb_nil_value();
  }
  */
  

  char buf[100], *dir_str;
  ledc_channel_t LEDC_CHANNEL;
  if( mrb_fixnum(chan) == 0 )
  {  
    dir_str = "0";
    ledc_channel_t LEDC_CHANNEL = LEDC_CHANNEL_0;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE_1, // LEDC_HIGH_SPEED_MODE
        .timer_num        = LEDC_TIMER, // LEDC_TIMER_0
        .duty_resolution  = LEDC_DUTY_RES, // LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
        .freq_hz          = mrb_fixnum(freq),  // Set output frequency 
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE_1, // LEDC_HIGH_SPEED_MODE
        .channel        = LEDC_CHANNEL, // LEDC_CHANNEL
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = mrb_fixnum(pin),
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Set duty to 50%
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE_1, LEDC_CHANNEL, mrb_fixnum(duty)));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE_1, LEDC_CHANNEL));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("chanel = %d pin = %d freq = %d duty = %d\n",mrb_fixnum(chan),mrb_fixnum(pin),mrb_fixnum(freq),mrb_fixnum(duty));
  }
  else if( mrb_fixnum(chan) == 1 )
  {
    dir_str = "1";
    ledc_channel_t LEDC_CHANNEL = LEDC_CHANNEL_1;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer1 = {
        .speed_mode       = LEDC_MODE_1, // LEDC_LOW_SPEED_MODE
        .timer_num        = LEDC_TIMER, // LEDC_TIMER_0
        .duty_resolution  = LEDC_DUTY_RES, // LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
        .freq_hz          = mrb_fixnum(freq),  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer1));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE_1, // LEDC_LOW_SPEED_MODE
        .channel        = LEDC_CHANNEL, // LEDC_CHANNEL
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = mrb_fixnum(pin),
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Set duty to 50%
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE_1, LEDC_CHANNEL, mrb_fixnum(duty)));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE_1, LEDC_CHANNEL));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("chanel = %d pin = %d freq = %d duty = %d\n",mrb_fixnum(chan),mrb_fixnum(pin),mrb_fixnum(freq),mrb_fixnum(duty));
  }
  else if( mrb_fixnum(chan) == 2 )
  {
    dir_str = "2";
    ledc_channel_t LEDC_CHANNEL = LEDC_CHANNEL_2;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE_1, // LEDC_LOW_SPEED_MODE
        .timer_num        = LEDC_TIMER, // LEDC_TIMER_0
        .duty_resolution  = LEDC_DUTY_RES, // LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
        .freq_hz          = mrb_fixnum(freq),  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE_1, // LEDC_LOW_SPEED_MODE
        .channel        = LEDC_CHANNEL, // LEDC_CHANNEL
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = mrb_fixnum(pin),
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Set duty to 50%
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE_1, LEDC_CHANNEL, mrb_fixnum(duty)));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE_1, LEDC_CHANNEL));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("chanel = %d pin = %d freq = %d duty = %d\n",mrb_fixnum(chan),mrb_fixnum(pin),mrb_fixnum(freq),mrb_fixnum(duty));
  }
  else if( mrb_fixnum(chan) == 3 )
  {
    dir_str = "3";
    ledc_channel_t LEDC_CHANNEL = LEDC_CHANNEL_3;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE_1, // LEDC_LOW_SPEED_MODE
        .timer_num        = LEDC_TIMER, // LEDC_TIMER_0
        .duty_resolution  = LEDC_DUTY_RES, // LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
        .freq_hz          = mrb_fixnum(freq),  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE_1, // LEDC_LOW_SPEED_MODE
        .channel        = LEDC_CHANNEL, // LEDC_CHANNEL
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = mrb_fixnum(pin),
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Set duty to 50%
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE_1, LEDC_CHANNEL, mrb_fixnum(duty)));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE_1, LEDC_CHANNEL));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("chanel = %d pin = %d freq = %d duty = %d\n",mrb_fixnum(chan),mrb_fixnum(pin),mrb_fixnum(freq),mrb_fixnum(duty));
  }
  else if( mrb_fixnum(chan) == 4 )
  {
    dir_str = "4";
    ledc_channel_t LEDC_CHANNEL = LEDC_CHANNEL_4;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE_2, // LEDC_LOW_SPEED_MODE
        .timer_num        = LEDC_TIMER, // LEDC_TIMER_0
        .duty_resolution  = LEDC_DUTY_RES, // LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
        .freq_hz          = mrb_fixnum(freq),  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE_2, // LEDC_LOW_SPEED_MODE
        .channel        = LEDC_CHANNEL, // LEDC_CHANNEL
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = mrb_fixnum(pin),
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Set duty to 50%
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE_2, LEDC_CHANNEL, mrb_fixnum(duty)));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE_2, LEDC_CHANNEL));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("chanel = %d pin = %d freq = %d duty = %d\n",mrb_fixnum(chan),mrb_fixnum(pin),mrb_fixnum(freq),mrb_fixnum(duty));
  }
  else if( mrb_fixnum(chan) == 5 )
  {
    dir_str = "5";
    ledc_channel_t LEDC_CHANNEL = LEDC_CHANNEL_5;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE_2, // LEDC_LOW_SPEED_MODE
        .timer_num        = LEDC_TIMER, // LEDC_TIMER_0
        .duty_resolution  = LEDC_DUTY_RES, // LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
        .freq_hz          = mrb_fixnum(freq),  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE_2, // LEDC_LOW_SPEED_MODE
        .channel        = LEDC_CHANNEL, // LEDC_CHANNEL
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = mrb_fixnum(pin),
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Set duty to 50%
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE_2, LEDC_CHANNEL, mrb_fixnum(duty)));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE_2, LEDC_CHANNEL));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("chanel = %d pin = %d freq = %d duty = %d\n",mrb_fixnum(chan),mrb_fixnum(pin),mrb_fixnum(freq),mrb_fixnum(duty));
  }
  else if( mrb_fixnum(chan) == 6 )
  {
    dir_str = "6";
    ledc_channel_t LEDC_CHANNEL = LEDC_CHANNEL_6;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE_2, // LEDC_LOW_SPEED_MODE
        .timer_num        = LEDC_TIMER, // LEDC_TIMER_0
        .duty_resolution  = LEDC_DUTY_RES, // LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
        .freq_hz          = mrb_fixnum(freq),  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE_2, // LEDC_LOW_SPEED_MODE
        .channel        = LEDC_CHANNEL, // LEDC_CHANNEL
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = mrb_fixnum(pin),
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Set duty to 50%
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE_2, LEDC_CHANNEL, mrb_fixnum(duty)));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE_2, LEDC_CHANNEL));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("chanel = %d pin = %d freq = %d duty = %d\n",mrb_fixnum(chan),mrb_fixnum(pin),mrb_fixnum(freq),mrb_fixnum(duty));
  }
  else if( mrb_fixnum(chan) == 7 )
  {
    dir_str = "7";
    ledc_channel_t LEDC_CHANNEL = LEDC_CHANNEL_7;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE_2, // LEDC_LOW_SPEED_MODE
        .timer_num        = LEDC_TIMER, // LEDC_TIMER_0
        .duty_resolution  = LEDC_DUTY_RES, // LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
        .freq_hz          = mrb_fixnum(freq),  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE_2, // LEDC_LOW_SPEED_MODE
        .channel        = LEDC_CHANNEL, // LEDC_CHANNEL
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = mrb_fixnum(pin),
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Set duty to 50%
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE_2, LEDC_CHANNEL, mrb_fixnum(duty)));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE_2, LEDC_CHANNEL));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("chanel = %d pin = %d freq = %d duty = %d\n",mrb_fixnum(chan),mrb_fixnum(pin),mrb_fixnum(freq),mrb_fixnum(duty));
  }
  
  else {  // input
    printf("Out of Cannel !!! \n");
  }
  
  
  
  /*
  if( mrb_fixnum(dir) == 1 ){  // output
    dir_str = "output";
  }
  else if(mrb_fixnum(dir) ==2 )
  {
    dir_str = "MIN";
  }
  
  else {  // input
    dir_str = "input";
  }
  
  printf("set %s to port %d \n", dir_str, mrb_fixnum(pin)); 
  */
  


  return self;
}

static mrb_value
mrb_esp32_gpio_pin_mode(mrb_state *mrb, mrb_value self) {
  mrb_value pin, dir;
  mrb_get_args(mrb, "oo", &pin, &dir);
  
  if (!mrb_fixnum_p(pin) || !mrb_fixnum_p(dir)) {
    return mrb_nil_value();
  }
  
  printf("SET PIN %d \n",mrb_fixnum(pin));
  gpio_pad_select_gpio(mrb_fixnum(pin)); // EDIT 
  gpio_set_direction(mrb_fixnum(pin), mrb_fixnum(dir) & ~GPIO_MODE_DEF_PULLUP);

  if (mrb_fixnum(dir) & GPIO_MODE_DEF_PULLUP) {
    gpio_set_pull_mode(mrb_fixnum(pin), GPIO_PULLUP_ONLY);
  }
  

  return self;
}

static mrb_value
mrb_esp32_gpio_digital_write(mrb_state *mrb, mrb_value self) {
  mrb_value pin, level;

  mrb_get_args(mrb, "oo", &pin, &level);

  if (!mrb_fixnum_p(pin) || !mrb_fixnum_p(level)) {
    return mrb_nil_value();
  }
  
  printf("PIN %d ",mrb_fixnum(pin));
  printf("Level %d \n",mrb_fixnum(level));

  gpio_set_level(mrb_fixnum(pin), mrb_fixnum(level));

  return self;
}

static mrb_value
mrb_esp32_gpio_analog_write(mrb_state *mrb, mrb_value self) {
  mrb_value ch, vol;

  mrb_get_args(mrb, "oo", &ch, &vol);

  if (!mrb_fixnum_p(ch) || !mrb_fixnum_p(vol)) {
    return mrb_nil_value();
  }

  dac_output_enable(mrb_fixnum(ch));

  dac_output_voltage(mrb_fixnum(ch), mrb_fixnum(vol));

  return self;
}

static mrb_value
mrb_esp32_gpio_digital_read(mrb_state *mrb, mrb_value self) {
  mrb_value pin;

  mrb_get_args(mrb, "o", &pin);

  if (!mrb_fixnum_p(pin)) {
    return mrb_nil_value();
  }
  //gpio_set_level(mrb_fixnum(pin), 1);
  //printf("PIN %d ",mrb_fixnum(pin));
  //printf(" input level is %d \n",gpio_get_level(mrb_fixnum(pin)));

  //gpio_set_level(GPIO_NUM_18, 1);
  
  //printf(" input level is %d \n",gpio_get_level(mrb_fixnum(pin)));

  return mrb_fixnum_value(gpio_get_level(mrb_fixnum(pin)));
}

static mrb_value
mrb_esp32_gpio_analog_read(mrb_state *mrb, mrb_value self) {
  mrb_value ch;

  mrb_get_args(mrb, "o", &ch);

  if (!mrb_fixnum_p(ch)) {
    return mrb_nil_value();
  }

  adc1_config_channel_atten(mrb_fixnum(ch), ADC_ATTEN_DB_11);

  return mrb_fixnum_value(adc1_get_raw(mrb_fixnum(ch)));
}

void
mrb_mruby_standard_gpio_gem_init(mrb_state* mrb)
{
  struct RClass *esp32, *standard, *constants;

  esp32 = mrb_define_module(mrb, "ESP32");

  standard = mrb_define_module_under(mrb, esp32, "STANDARD"); // Module
  mrb_define_module_function(mrb, standard, "pwm", mrb_esp32_pwm, MRB_ARGS_REQ(3));
  mrb_define_module_function(mrb, standard, "pinMode", mrb_esp32_gpio_pin_mode, MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, standard, "digitalWrite", mrb_esp32_gpio_digital_write, MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, standard, "digitalRead", mrb_esp32_gpio_digital_read, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, standard, "analogWrite", mrb_esp32_gpio_analog_write, MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, standard, "analogRead", mrb_esp32_gpio_analog_read, MRB_ARGS_REQ(1));
  
  adc1_config_width(ADC_WIDTH_BIT_12);//EDIT ADC_WIDTH_MAX ,ADC_BITWIDTH_12

  constants = mrb_define_module_under(mrb, standard, "Constants"); // Module

#define define_const(SYM) \
  do { \
    mrb_define_const(mrb, constants, #SYM, mrb_fixnum_value(SYM)); \
  } while (0)

  define_const(GPIO_NUM_0);
  define_const(GPIO_NUM_1);
  define_const(GPIO_NUM_2);
  define_const(GPIO_NUM_3);
  define_const(GPIO_NUM_4);
  define_const(GPIO_NUM_5);
  define_const(GPIO_NUM_6);
  define_const(GPIO_NUM_7);
  define_const(GPIO_NUM_8);
  define_const(GPIO_NUM_9);
  define_const(GPIO_NUM_10);
  define_const(GPIO_NUM_11);
  define_const(GPIO_NUM_12);
  define_const(GPIO_NUM_13);
  define_const(GPIO_NUM_14);
  define_const(GPIO_NUM_15);
  define_const(GPIO_NUM_16);
  define_const(GPIO_NUM_17);
  define_const(GPIO_NUM_18);
  define_const(GPIO_NUM_19);

  define_const(GPIO_NUM_21);
  define_const(GPIO_NUM_22);
  define_const(GPIO_NUM_23);

  define_const(GPIO_NUM_25);
  define_const(GPIO_NUM_26);
  define_const(GPIO_NUM_27);

  define_const(GPIO_NUM_32);
  define_const(GPIO_NUM_33);
  define_const(GPIO_NUM_34);
  define_const(GPIO_NUM_35);
  define_const(GPIO_NUM_36);
  define_const(GPIO_NUM_37);
  define_const(GPIO_NUM_38);
  define_const(GPIO_NUM_39);
  define_const(GPIO_NUM_MAX);

  define_const(DAC_CHANNEL_1);
  define_const(DAC_CHANNEL_2);

  define_const(ADC1_CHANNEL_0);
  define_const(ADC1_CHANNEL_1);
  define_const(ADC1_CHANNEL_2);
  define_const(ADC1_CHANNEL_3);
  define_const(ADC1_CHANNEL_4);
  define_const(ADC1_CHANNEL_5);
  define_const(ADC1_CHANNEL_6);
  define_const(ADC1_CHANNEL_7);
  define_const(ADC1_CHANNEL_MAX);

  mrb_define_const(mrb, constants, "LOW", mrb_fixnum_value(0));
  mrb_define_const(mrb, constants, "HIGH", mrb_fixnum_value(1));
  mrb_define_const(mrb, constants, "MINPUT", mrb_fixnum_value(2));


  mrb_define_const(mrb, constants, "INPUT",          mrb_fixnum_value(GPIO_MODE_INPUT));
  mrb_define_const(mrb, constants, "INPUT_OUTPUT",   mrb_fixnum_value(GPIO_MODE_INPUT_OUTPUT));
  mrb_define_const(mrb, constants, "OUTPUT",         mrb_fixnum_value(GPIO_MODE_OUTPUT));
  mrb_define_const(mrb, constants, "INPUT_PULLUP",   mrb_fixnum_value(GPIO_MODE_INPUT_PULLUP));
  mrb_define_const(mrb, constants, "INPUT_PULLDOWN", mrb_fixnum_value(GPIO_MODE_INPUT_PULLDOWN));

  //New param mode
  mrb_define_const(mrb, constants, "IN",          mrb_fixnum_value(GPIO_MODE_INPUT));
  mrb_define_const(mrb, constants, "OUT",         mrb_fixnum_value(GPIO_MODE_OUTPUT));
  mrb_define_const(mrb, constants, "PULL_UP",   mrb_fixnum_value(GPIO_MODE_INPUT_PULLUP));
  mrb_define_const(mrb, constants, "PULL_DOWN", mrb_fixnum_value(GPIO_MODE_INPUT_PULLDOWN));
  // HIGH_Z..
  // OPEN_DRAIN..

  // PWM
  mrb_define_const(mrb, constants, "CH0", mrb_fixnum_value(0));
  mrb_define_const(mrb, constants, "CH1", mrb_fixnum_value(1));
  mrb_define_const(mrb, constants, "CH2", mrb_fixnum_value(2));
  mrb_define_const(mrb, constants, "CH3", mrb_fixnum_value(3));
  mrb_define_const(mrb, constants, "CH4", mrb_fixnum_value(4));
  mrb_define_const(mrb, constants, "CH5", mrb_fixnum_value(5));
  mrb_define_const(mrb, constants, "CH6", mrb_fixnum_value(6));
  mrb_define_const(mrb, constants, "CH7", mrb_fixnum_value(7));
 
    
}

void
mrb_mruby_standard_gpio_gem_final(mrb_state* mrb)
{
}
