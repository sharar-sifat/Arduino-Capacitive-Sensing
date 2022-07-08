/*
Senses proximity of a hand using capacitive touch and uses it to control the pitch on a buzzer.

See capacitive_touch and blocking_tones.


Hardware: Connect a 20-100 MOhm resistor between TxPin (8) and RxPin (9).
          Connect an alligator clip between RxPin and a piece of aluminum foil.
          Place a piece of paper over the tinfoil (optional).
          Connect a piezo from buzzerPin (10) to ground.
          For volume control, connect it through a potentiometer.

Notes for piano exercise:
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'D', 'E', 'F', 'G', 'A', 'B'};
  int frequencies[] = {262, 294, 330, 349, 392, 440, 494, 262*2, 294*2, 330*2, 349*2, 392*2, 440*2, 494*2};


*/


// Set which pins are used here
byte TxPin = 8;
byte RxPin = 9;
byte buzzerPin = 10;

// This is used to keep track of a low-pass filtered frequency
float filtered_frequency = 1000.0;

// The bitmask is used to access the PINB register
byte bitmask=1 << (RxPin-8);

// This runs once when the board starts up
void setup()
{
  // Setup
  pinMode(TxPin, OUTPUT);
  pinMode(RxPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  // Drive the Tx pin low to start
  digitalWrite(TxPin, LOW);
}

// This is called indefinitely
void loop()
{
  float frequency;
  long i=0;
  int note;
  
  // Drive the TxPin high to send the signal
  digitalWrite(TxPin, HIGH);
  
  // Count how long it takes to get to RxPin
  //while( (digitalRead(RxPin) == LOW) && (i<2000)) i++;   // this is the slow way
  while( ((PINB & bitmask) == 0) && (i<2000)) i++;   // this is the faster way

  
  // Finished sending the signal. Drive the pin low for the next cycle
  digitalWrite(TxPin, LOW);

  // Discharge the stray capacitor
  pinMode(RxPin, OUTPUT);
  digitalWrite(RxPin, LOW);
  pinMode(RxPin, INPUT);
  
  // Write out the result over the USB cable
  //Serial.println(i);

  // Clip the result to a fixed range
  //if (i<21) i=21;
  //if (i>400) i=400;
  
  // Calculate the frequency and filter it
  frequency = (i-20)*20.0;
  // The next line is a lowpass digital filter (an IIR filter: Infinite Impulse Response)
  filtered_frequency=filtered_frequency*0.9+frequency*0.1;
  Serial.println(filtered_frequency);
  // Play the tone
  if(filtered_frequency > 5800 and filtered_frequency<7100)
  {
    //making sure the buzzer makes noise only when my hand is close to it (not touching or not far away)
    
    //each note should be 185hz in width ((4400-3880)/7)
    //setting each range of filtered_frequency to a specific note using a combination of if else statements
    if (filtered_frequency>5800 and filtered_frequency<(5800+185))
    {
      note=262;
      playTone(note,50);
      
    }
    else if (filtered_frequency>(5800+185) and filtered_frequency<(5800+185*2))
    {
      note=294;
      playTone(note,50);
      
    }

    else if (filtered_frequency>(5800+185*2) and filtered_frequency<(5800+185*3))
    {
      note=330;
      playTone(note,50);
      
    }

    else if (filtered_frequency>(5800+185*3) and filtered_frequency<(5800+185*4))
    {
      note=349;
      playTone(note,50);
      
    }

    else if (filtered_frequency>(5800+185*4) and filtered_frequency<(5800+185*5))
    {
      note=392;
      playTone(note,50);
      
    }

    else if (filtered_frequency>(5800+185*5) and filtered_frequency<(5800+185*6))
    {
      note=440;
      playTone(note,50);
      
    }

    else if (filtered_frequency>(5800+185*6) and filtered_frequency<(7100))
    {
      note=492;
      playTone(note,50);
      
    }









    
  }
  
}


void playTone(float frequency, int duration)
{
  // This function plays a tone at the requested frequency for the requested duration.
  // frequency: frequency of the tone
  // duration: how long the tone should last in milliseconds
  
  int pulse = 500000.0/frequency;
  long i;
  
  // Keep looping until the note is finished playing
  for (i = 0; i < duration * 1000L; i += pulse*2 )
  {
    // Make the pulse: HIGH, wait, LOW, wait:
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(pulse);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(pulse);
  }
}
