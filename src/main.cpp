#include <Arduino.h>

#define LDR 2
#define LED_W 16
#define LED_Y 17
#define LED_PWM 18
#define BUTTON 21

u16_t adcPeriod = 50;
u32_t lastAdcReadTime = 0;
u16_t freq = 5000;
u16_t dutyCycle = 128;
u16_t alarmLightDelay = 500;
u32_t lastAlarmLightTime = 0;
u8_t alarmLightState = LOW;
u8_t button_state = LOW;

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
    u32_t value = analogReadMilliVolts(LDR);

    u16_t limitedAdcValue = constrain(adc_value, 2000, 4000);
    dutyCycle = map(limitedAdcValue, 2000, 4000, 5, 255);
    analogWrite(LED_PWM, dutyCycle);

    if (adc_value < 3500)
    {
      digitalWrite(LED_W, HIGH);
    }
    else
    {
      digitalWrite(LED_W, LOW);
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
