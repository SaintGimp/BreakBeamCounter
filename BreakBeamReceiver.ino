#include <Debounce.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include <TinyWireM.h>

const int sensorPin = 3;
const int displayButtonPin = 4;
const boolean displayOnDemand = true;
const int noiseFilterTime = 100;
const int delayBetweenTriggers = 750;

StableDebounce sensor = StableDebounce();
StableDebounce displayButton = StableDebounce();
Adafruit_7segment display = Adafruit_7segment();
unsigned long lastBeamResetTime = 0;

int breakCounter = 0;

void setup()
{
  display.begin(0x70);
  if (displayOnDemand)
  {
    display.clear();
  }
  else
  {
    display.print(breakCounter);
  }
  display.writeDisplay();

  pinMode(sensorPin, INPUT);
  sensor.attach(sensorPin);
  sensor.interval(noiseFilterTime);
  
  pinMode(displayButtonPin, INPUT_PULLUP);     
  displayButton.attach(displayButtonPin);
  displayButton.interval(50);
}

void loop()
{
  if (sensor.update())
  {
    if (sensor.read() == HIGH && millis() - lastBeamResetTime > delayBetweenTriggers)
    {
      breakCounter++;
      
      if (!displayOnDemand)
      {
        display.print(breakCounter);
        display.writeDisplay();
      }
    }
    else if (sensor.read() == LOW)
    {
      lastBeamResetTime = millis();
    }      
  }
  
  if (displayOnDemand && displayButton.update())
  {
    if (displayButton.read() == LOW)
    {
      display.print(breakCounter);
      display.writeDisplay();
    }
    else
    {
      display.clear();
      display.writeDisplay();
    }
  }
}

