#include <Arduino.h>
#include "config.h"

const u16_t LDR_threshold = 3500;
const u32_t debounceDelay = 50;

u16_t adcPeriod = 100;
u32_t lastAdcReadTime = 0;

u16_t freq = 5000;
u16_t dutyCycle = 128;

u16_t alarmLightDelay = 500;
u32_t lastAlarmLightTime = 0;
u8_t alarmLightState = LOW;

u8_t button_emerg_state = LOW;
u8_t last_button_emerg_reading = LOW;
u8_t stable_button_emerg_state = LOW;
u32_t last_button_emerg_change_time = 0;

// u8_t button_crash_state = LOW;
// u8_t last_button_crash_reading = LOW;
// u8_t stable_button_crash_state = LOW;
// u8_t last_button_crash_change_time = 0;

u32_t adc_on_timer = 0;
u32_t adc_off_timer = 0;
u16_t adc_delay = 1000;

void setup()
{
  pinMode(LDR, INPUT);
  pinMode(LED_W, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(BUTTON_EMERGENCY, INPUT);
  // pinMode(BUTTON_CRASH, INPUT);
  pinMode(LED_PWM, OUTPUT);
  analogWriteFrequency(freq);
  Serial.begin(115200);
  Serial.println("Alarm - setuped\n");
}

void loop()
{
  const u32_t now = millis();
  const u8_t readingEmerg = digitalRead(BUTTON_EMERGENCY);
  // const u8_t readingCrash = digitalRead(BUTTON_CRASH);

  if (readingEmerg != last_button_emerg_reading)
  {
    last_button_emerg_change_time = now;
    last_button_emerg_reading = readingEmerg;
  }
  if (now - last_button_emerg_change_time >= debounceDelay && readingEmerg != stable_button_emerg_state)
  {
    stable_button_emerg_state = readingEmerg;
    if (stable_button_emerg_state == HIGH)
    {
      button_emerg_state = !button_emerg_state;
    }
  }
  if (button_emerg_state)
  {
    if (now - lastAlarmLightTime >= alarmLightDelay && button_emerg_state)
    {
      lastAlarmLightTime = now;
      alarmLightState = !alarmLightState;
      digitalWrite(LED_Y, alarmLightState ? HIGH : LOW);
    }
  }
  else
  {
    lastAlarmLightTime = 0;
    alarmLightState = LOW;
    digitalWrite(LED_Y, LOW);
  }

  if (now - lastAdcReadTime >= adcPeriod)
  {
    lastAdcReadTime = now;
    u16_t adc_value = analogRead(LDR);
    u16_t limitedAdcValue = constrain(adc_value, 2000, 4000);
    dutyCycle = map(limitedAdcValue, 2000, 4000, 5, 255);
    analogWrite(LED_PWM, dutyCycle);

    if (adc_value < LDR_threshold)
    {
      adc_off_timer = 0;
      adc_on_timer = (adc_on_timer == 0) ? now : adc_on_timer;
      if (adc_value < LDR_threshold && now - adc_on_timer >= adc_delay)
      {
        digitalWrite(LED_W, HIGH);
      }
    }
    else
    {
      adc_on_timer = 0;
      adc_off_timer = (adc_off_timer == 0) ? now : adc_off_timer;
      if (adc_value >= LDR_threshold && now - adc_off_timer >= adc_delay)
      {
        digitalWrite(LED_W, LOW);
      }
    }

    Serial.printf("ADC value: %d   Duty cycle: %d  Alarm is enabled: %s\n", adc_value, dutyCycle, button_emerg_state ? "YES" : "NO");
  }
}
