#include <Arduino.h>
#include "config.h"
#include "Button.h"
#include "Ldr.h"
#include "Led.h"

u32_t lastAlarmLightTime = 0;
u32_t lastCrashLightTime = 0;
u8_t alarmLightState = false;
u8_t crashLightState = false;

bool automaticWhiteState = false;
const u16_t ldrHysteresis = 100;

Button emergencyButton;
Button crashButton;
Ldr ldr;
Led whiteLed;
Led yellowLed;
Led pwmLed;

void setup()
{
  emergencyButton.init(BUTTON_EMERGENCY);
  crashButton.init(BUTTON_CRASH);
  ldr.init(LDR);
  whiteLed.init(LED_W);
  yellowLed.init(LED_Y);
  pwmLed.init(LED_PWM);

  analogWriteFrequency(ANALOG_FREQUENCY);
  Serial.begin(115200);
  Serial.println("Alarm - setuped\n");
}

void loop()
{
  const u32_t now = millis();

  emergencyButton.poll(now);
  crashButton.poll(now);
  ldr.poll(now);

  bool readingEmerg = emergencyButton.get_state();
  bool readingCrash = crashButton.get_state();
  u32_t adcValue = ldr.get_value();

  if (adcValue < LDR_THRESHOLD - ldrHysteresis)
  {
    automaticWhiteState = true;
  }
  else if (adcValue > LDR_THRESHOLD + ldrHysteresis)
  {
    automaticWhiteState = false;
  }

  u16_t limitedAdcValue = constrain(adcValue, 2000, 4000);
  u8_t dutyCycle = map(limitedAdcValue, 2000, 4000, 5, 255);

  pwmLed.set_duty(dutyCycle);

  if (readingCrash)
  {
    if (now - lastCrashLightTime >= CRASH_LIGHT_DELAY)
    {
      lastCrashLightTime = now;
      whiteLed.set_state(crashLightState ? HIGH : LOW);
      yellowLed.set_state(crashLightState ? HIGH : LOW);
      crashLightState = !crashLightState;
    }
  }
  else
  {
    lastCrashLightTime = 0;
    crashLightState = false;

    whiteLed.set_state(automaticWhiteState);

    if (readingEmerg)
    {
      if (now - lastAlarmLightTime >= ALARM_LIGHT_DELAY)
      {
        lastAlarmLightTime = now;
        yellowLed.set_state(alarmLightState ? HIGH : LOW);
        alarmLightState = !alarmLightState;
      }
    }
    else
    {
      lastAlarmLightTime = 0;
      yellowLed.set_state(LOW);
      alarmLightState = false;
    }
  }

  Serial.printf("ADC value: %d   Duty cycle: %d  Alarm is enabled: %s Is crashed: %s\n",
                adcValue, dutyCycle, readingEmerg ? "YES" : "NO", readingCrash ? "YES" : "NO");
}
