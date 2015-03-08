#include <Debounce.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include <TinyWireM.h>

const int sensorPin = 3;
const int displayButtonPin = 4;
const int outputPin = 1;
const int noiseFilterTime = 7;
const int delayBetweenTriggers = 750;

ThresholdDebounce sensor = ThresholdDebounce();
StableDebounce displayButton = StableDebounce();
Adafruit_7segment display = Adafruit_7segment();
boolean displayOnDemand = true;
unsigned long lastBeamResetTime = 0;
unsigned long lastButtonPressTime = 0;

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
  sensor.threshold(0.20);
  
  pinMode(displayButtonPin, INPUT_PULLUP);     
  displayButton.attach(displayButtonPin);
  displayButton.interval(50);
  
  pinMode(outputPin, OUTPUT);
}

void loop()
{
  if (sensor.update())
  {
    if (sensor.read() == HIGH && millis() - lastBeamResetTime > delayBetweenTriggers)
    {
      breakCounter++;
      
      digitalWrite(outputPin, HIGH);
      delay(10);
      digitalWrite(outputPin, LOW);
      
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
  
  if (displayButton.update())
  {
    if (displayButton.read() == LOW && displayOnDemand)
    {
      display.print(breakCounter);
      display.writeDisplay();
      lastButtonPressTime = millis();
    }
    else if (displayButton.read() == HIGH)
    {
      if (millis() > lastButtonPressTime + 6000)
      {
        displayOnDemand = !displayOnDemand;
      }
      
      if (displayOnDemand)
      {
        display.clear();
        display.writeDisplay();
      }
    }
  }
}

