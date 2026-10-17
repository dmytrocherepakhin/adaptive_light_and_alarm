#include <Arduino.h>

#define LDR 2
#define LED_W 16
#define LED_Y 17
#define LED_PWM 18
#define BUTTON 21

const u16_t LDR_threshold = 3500;

u16_t adcPeriod = 100;
u32_t lastAdcReadTime = 0;
u16_t freq = 5000;
u16_t dutyCycle = 128;
u16_t alarmLightDelay = 500;
u32_t lastAlarmLightTime = 0;
u8_t alarmLightState = LOW;
u8_t button_state = LOW;
u32_t adc_on_timer = 0;
u32_t adc_off_timer = 0;
u16_t adc_delay = 1000;

void setup()
{
  pinMode(LDR, INPUT);
  pinMode(LED_W, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(LED_PWM, OUTPUT);
  analogWriteFrequency(freq);
  Serial.begin(115200);
  Serial.println("Alarm - setuped\n");
}

void loop()
{
  if (millis() - lastAdcReadTime >= adcPeriod)
  {
    lastAdcReadTime = millis();
    u16_t adc_value = analogRead(LDR);
    u16_t limitedAdcValue = constrain(adc_value, 2000, 4000);
    dutyCycle = map(limitedAdcValue, 2000, 4000, 5, 255);
    analogWrite(LED_PWM, dutyCycle);

    if (adc_value < LDR_threshold)
    {
      adc_off_timer = 0;
      adc_on_timer = (adc_on_timer == 0) ? millis() : adc_on_timer;
      if (adc_value < LDR_threshold && millis() - adc_on_timer >= adc_delay)
      {
        digitalWrite(LED_W, HIGH);
      }
    }
    else
    {
      adc_on_timer = 0;
      adc_off_timer = (adc_off_timer == 0) ? millis() : adc_off_timer;
      if (adc_value >= LDR_threshold && millis() - adc_off_timer >= adc_delay)
      {
        digitalWrite(LED_W, LOW);
      }
    }

    u8_t new_button_state = digitalRead(BUTTON);
    if (new_button_state || button_state)
    {
      button_state = HIGH;
      Serial.printf("Button pressed\n");
      if (millis() - lastAlarmLightTime >= alarmLightDelay)
      {
        lastAlarmLightTime = millis();
        alarmLightState = !alarmLightState;
        digitalWrite(LED_Y, alarmLightState ? HIGH : LOW);
      }
    }

    Serial.printf("ADC value: %d   Duty cycle: %d  Button is pressed: %s\n", adc_value, dutyCycle, button_state ? "YES" : "NO");
  }
}
