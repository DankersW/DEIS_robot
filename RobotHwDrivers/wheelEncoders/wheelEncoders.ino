#include <RedBot.h>

RedBotEncoder encoder = RedBotEncoder(A2, 10);  // initializes encoder on pins A2 and 10
int buttonPin = 12;
int countsPerRev = 192;   // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev

void setup()
{
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("left    right");
  Serial.println("================");
}

void loop(void)
{
  // wait for a button press to clear registers.
  if (digitalRead(buttonPin) == LOW)
  {
    encoder.clearEnc(BOTH);  // Reset the counters.
  }

  // print out to Serial Monitor the left and right encoder counts.
  Serial.print("Left: ");
  Serial.print(encoder.getTicks(LEFT));
  Serial.print("  Right: ");
  Serial.println(encoder.getTicks(RIGHT));
}

