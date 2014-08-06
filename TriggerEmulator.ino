/*
Trigger emulator
Send signals to emulate SPS supercyle and a trigger
 */

int beginOfSpill=9;
int endOfSpill=10;
int trigger=11;

long unsigned int nPulsesInSpill=0;
long unsigned int beginOfSpillTime=0;
int status = 0;
int freq=0;
unsigned long nTriggers=0;
const unsigned int spillLength=5000; //spill length in milliseconds
const unsigned int interSpillLength=1000; //interspill length in milliseconds
const bool enableSpillStructure=true;

// the setup routine runs once when you press reset:
void setup() {                
  Serial.begin(9600);
  //Wait for serial
  while (!Serial)
  {
    ;
  }
//  Serial.setTimeout(10000);
  Serial.println("Trigger Emulator V0.1"); 
  Serial.println("Type a trigger frequency in Hz (integer value 1-2000) or 0 to stop pulsing");  
  // initialize the digital pin as an output.
  pinMode(trigger, OUTPUT);  
  digitalWrite(trigger, HIGH);  
  pinMode(beginOfSpill, OUTPUT);  
  digitalWrite(beginOfSpill, HIGH);  
  pinMode(endOfSpill, OUTPUT);  
  digitalWrite(endOfSpill, HIGH);  

}

// the loop routine runs over and over again forever:
void loop() {
  
  if (Serial.available() > 0) {
    //Serial.println(" waiting for input");
    // sentence:
    int val = Serial.parseInt(); 
    val=constrain(val,0,2000); //constrain value up to 500Hz
    freq=val;
    if (val>0)
    {
      Serial.print("Frequency has been set to ");
      Serial.print(freq,DEC);
      Serial.println(" Hz. Resetting pulser");
      nPulsesInSpill=0;
    }
    else if (val==0)
    {
       Serial.println("Stopping pulser");
       nPulsesInSpill=0;
    } 
  }

  if (freq<=0)
    return;
  
  
  //Begin Of Spill  
  if (enableSpillStructure)
    {
      if (nPulsesInSpill == 0)
      {
        digitalWrite(beginOfSpill, LOW);
        delayMicroseconds(10);
        digitalWrite(beginOfSpill, HIGH);
        delay(500); //0.5s delay between begin of spill and actual begin
        beginOfSpillTime=millis();
      }
    }
  
  // In Spill triggers  
  digitalWrite(trigger, LOW);   // turn the LED on (HIGH is the voltage level)
  delayMicroseconds(10);               // send a pulse of 100usec
  digitalWrite(trigger, HIGH);    // turn the LED off by making the voltage LOW
  if (freq>=100)
  {
    delayMicroseconds((1000000/freq)-125);
  }
  else
  {
    delay(1000/freq);
  }
  
  nPulsesInSpill++;
  if ((nPulsesInSpill%500)==0)
  {
    Serial.print("#pulses sent in this spill: "); Serial.println(nPulsesInSpill,DEC); 
  }
  
  //End of Spill
  if (enableSpillStructure)
    {
      if ((millis()-beginOfSpillTime)>spillLength)
      {
        //send endOfSpill
        digitalWrite(endOfSpill, LOW);
        delayMicroseconds(10);
        digitalWrite(endOfSpill, HIGH);
        Serial.print("Spill finished. #pulses sent in this spill: "); Serial.println(nPulsesInSpill,DEC);
        nPulsesInSpill=0;
        delay(interSpillLength);
      }  
    }
}





