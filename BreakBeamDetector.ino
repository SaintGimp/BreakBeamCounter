#include <Bounce.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include <TinyWireM.h>

const int sensorPin = 3;
const int displayButtonPin = 4;
const boolean displayOnDemand = true;

Bounce sensor = Bounce(sensorPin, 500);
Bounce displayButton = Bounce(displayButtonPin, 50);
Adafruit_7segment display = Adafruit_7segment();
int counter = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting . . .");
  
  display.begin(0x70);
  if (displayOnDemand)
  {
    display.clear();
  }
  else
  {
    display.print(counter);
  }
  display.writeDisplay();

  pinMode(sensorPin, INPUT);     
  pinMode(displayButtonPin, INPUT_PULLUP);     
}

void loop()
{
  sensor.update();
  
  if (sensor.risingEdge())
  {
    counter++;
    
    if (!displayOnDemand)
    {
      display.print(counter);
      display.writeDisplay();
    }      
  }
  
  if (displayOnDemand && displayButton.update())
  {
    if (displayButton.read() == LOW)
    {
      display.print(counter);
      display.writeDisplay();
    }
    else
    {
      display.clear();
      display.writeDisplay();
    }
      
  }
}
