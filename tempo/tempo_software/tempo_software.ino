


unsigned int counter=0;
unsigned long temp[10];
unsigned long wait =0;

volatile unsigned char state = LOW;
bool data_effect=true;
const int max_heartpulse_duty=2500;//you can change it follow your system's request.2000 meams 2 seconds. 
//System return error if the duty overtrip 2 second.



//set output pins for go indicators
const int pinGo = 11;
const int buzzerOutPin = 4;
const int beatOutPin = 6;
const int rateLowerPin = 9;
const int rateUpperPin = 10;
//set input pins for heartbeat count
const int pinCount = 2;

//
int goState = LOW;
int startState = LOW;
int beat = 0;
int lowerLevel = 50;
int upperLevel = 110;


//Set limit
int goBegin = 40;
int goEnd = 12;

int plyrCount = 0;

void setup()
{
  
  pinMode(pinGo, OUTPUT);
  pinMode(buzzerOutPin, OUTPUT);
  pinMode(beatOutPin, OUTPUT);
  pinMode(rateLowerPin, OUTPUT);
  pinMode(rateUpperPin, OUTPUT);
  
//  Serial.begin(9600);
  
  blinkOutPins(500, 6000);

  digitalWrite(pinGo, goState);
  array_init();
  attachInterrupt(0, interrupt, RISING);//set interrupt 0,digital port 2
  //attachInterrupt(1, interruptB, RISING);//set interrupt 1,digital port 3

}
  


void loop()
{

  if (beat == 1){
    beat =0;
    digitalWrite(beatOutPin, HIGH);

    if (goState == HIGH){

      buzz(buzzerOutPin, 3000, 100);    
    }else{
      buzz(buzzerOutPin, 100, 80);      
    }


  
    digitalWrite(beatOutPin, LOW);
  } 
  
  delay(5);
  wait = calcBeatWait();
  
  if(wait>max_heartpulse_duty){
    counter=0;
//    Serial.println(wait);
    array_init();
    analogWrite(rateLowerPin, 0);
    analogWrite(rateUpperPin, 0);
    if (goState == HIGH){
      switchMode();//if it buggers up, you lose your go. Thems the breaks!
    }
  }
  
}

void blinkOutPins(int blinkSpeed, int length)
{
  int buzzFreq;
  for(int i =0; i < length; i += blinkSpeed){
    
    digitalWrite(pinGo, startState);

    if((i % 2) == 0){
      buzzFreq = (i + 1) * 100;
      buzz(buzzerOutPin, buzzFreq, blinkSpeed);
    }else{
    
      delay(blinkSpeed);      
    }
      
    startState = !startState;
    
    
    

    
  }
  
  
}

void interrupt()
{
  beat = 1;
  runPlyrCounter();
  takeHeartRate();
}


void runPlyrCounter()
{ 
  
  plyrCount += 1;
//  Serial.print ("Player ");
//  Serial.print (" count is ");
//  Serial.println( plyrCount);
  
  if(goState == LOW){
      if (plyrCount >= goBegin){
         switchMode();    
      }

  }else{
      if (plyrCount >= goEnd){
       
        switchMode();        
      }    
  }
  
}

void switchMode()
{

  goState = !goState;
  digitalWrite(pinGo, goState);
  plyrCount = 0;    
  
}


void buzz(int targetPin, long frequency, long length) {
  long delayValue = 1000000/frequency/2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length/ 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
 for (long i=0; i < numCycles; i++){ // for the calculated length of time...
    digitalWrite(targetPin,HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin,LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait againf or the calculated delay value
  }
}


void takeHeartRate()
{
 
 temp[counter]=millis();
 unsigned long sub;

 sub = calcBeatLength();
 
 
  if (counter==9){
    calcRate();
    counter=0;
    //calcRate in here
  }else{
   counter++; 
  }
  

  
}

unsigned long calcBeatLength()
{
    
   if(counter == 0){
     return temp[counter]- temp[9];
   }else{
     return temp[counter]-temp[counter-1];
   }
   
}

unsigned long calcBeatWait()
{
  unsigned long now =   millis();
   if(counter == 0){
     return now - temp[9];
   }else{
     return now - temp[counter-1];
   }
  
}

void calcRate()
{
 
      int heart_rate;
      heart_rate=600000/(temp[9]-temp[0]);//60*20*1000/20_total_time 
//      Serial.print("Heart_rate_is:\t");
//      Serial.println(heart_rate);      
      
      int lowerRate = makeInRange((heart_rate-lowerLevel) * 4.25);
      int upperRate = makeInRange((heart_rate-upperLevel) * 2.55);
      analogWrite(rateLowerPin, lowerRate);
      analogWrite(rateUpperPin, upperRate);
      
      
}  


int makeInRange(int val)
//ensures value is between 0 and 255.
{

  if (val < 0){
     return 0; 
  }else if(val > 255){
     return 255;
  }else{
   return val; 
  }
  
}

void array_init()
{
  for(unsigned char i=0;i!=9;++i){
    temp[i]=0;
  }
  temp[9]=millis();
}
