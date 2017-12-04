// include SPI, MP3, SD and KEYPAD libraries
#include <Keypad.h>
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

int ledPin = 2; // choose the pin for the LED; output that switches on the LED 
int inPin = 18;   // choose the input pin (for a pushbutton); acts like a switch, goes to PIN 18, when you push the pushbutton, it receives electricity it triggers other actions 
int val = 0;     // variable for reading the pin status; not linked to any PIN 

// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin (how to set up the music makershield) 

// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin


Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
  

String trackNames[3] = { //create an array of strings named trackNames composed of 4 instances track001.mp3 => 0 ...
  "track000.mp3", // track000 needs to start at 000 because an array starts at 0, the names of the track reflect the keypad, not what is in the SDcard.
  "track001.mp3",
  "track002.mp3"
};


// KEYPAD INTERFACE
const byte ROWS = 4; // defining the keypad
const byte COLS = 3; 

char hexaKeys[ROWS][COLS] = { //definiing global pattern of keypad 
  {'1', '2', '3'}, 
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {17, 10, 8, 5}; 
byte colPins[COLS] = {14, 15, 16}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); //define rows and cols of keypad, when '1' is dialed, you will receive an input at the same time from both pins.

void setup(){
  Serial.begin(9600);
  //Serial.println("TrackNames[0] : " + trackNames[0]);

  //button
  pinMode(ledPin, OUTPUT);  // declare LED as output
  pinMode(inPin, INPUT);    // declare pushbutton as input

  //SD INIT


  Serial.println("Adafruit VS1053 Simple Test");

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // list files
  printDirectory(SD.open("/"), 0);
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20);

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

  
}
  
void loop(){

    val = digitalRead(inPin);  // read input value
    if (val == HIGH) {         // check if the input is HIGH (button released)
      digitalWrite(ledPin, HIGH);  // turn LED on 
      char customKey = customKeypad.getKey(); //get the key dialed on the keypad

      if(customKey){ //when a key is dialed
          
          String t = ""; //create an empty string named t
          t+=customKey; //put customKey inside this string named t
          int trackNumber = t.toInt(); // PARSER CUSTOM KEY EN INT transform the string into an int
          String track = ""; //create an empty string named track
          musicPlayer.stopPlaying(); //stop playing
          track = trackNames[trackNumber]; //put here the track matching the key dialed
          /*   
          Serial.println(customKey); //print the key dialed
          Serial.println("PLAYING " + track); // print the name of the track to be played
         */
          musicPlayer.startPlayingFile(track.c_str()); //start playing the track

          while (musicPlayer.playingMusic) //makes the player play the right track 
            {
             digitalWrite(ledPin, HIGH);
             delay(2000);
             digitalWrite(ledPin, LOW);
             delay(1000);
            }

           if (musicPlayer.stopped()){ 
            musicPlayer.playFullFile("track011.mp3"); //use playFull file instead of startPlayingFile because startPlayingFile is overlapping multiple files at the same time. 
            musicPlayer.playFullFile("track022.mp3");
      }
      }
    } else {
      digitalWrite(ledPin, HIGH);  // turn LED on 
      musicPlayer.stopPlaying(); //stop playing
      musicPlayer.playFullFile("track666.mp3"); //start playing the track named on SDcard, the first loop of instructions.  
      if (musicPlayer.stopped()){
      musicPlayer.playFullFile("track011.mp3"); //use playFull file instead of startPlayingFile because startPlayingFile is overlapping multiple files at the same time. 
      musicPlayer.playFullFile("track022.mp3"); // second instructions 
      }
       }

//      while (musicPlayer.playingMusic) //commented out because you cannot stop a track while it's playing.
//        {
//         digitalWrite(ledPin, HIGH);
//         delay(2000);
//         digitalWrite(ledPin, LOW);
//         delay(1000);
//        }
    }
    
/*

    val = digitalRead(inPin);  // read input value
    if (val == HIGH) {         // check if the input is HIGH (button released)
      digitalWrite(ledPin, LOW);  // turn LED OFF
    } else {
      digitalWrite(ledPin, HIGH);  // turn LED ON

      char customKey = customKeypad.getKey(); //get the key dialed on the keypad

       if(customKey){ //when a key is dialed
          
          String t = ""; //create an empty string named t
          t+=customKey; //put customKey inside this string named t
          int trackNumber = t.toInt(); // PARSER CUSTOM KEY EN INT transform the string into an int
        
          String track = ""; //create an empty string named track
      
          musicPlayer.stopPlaying(); //stop playing
            
          track = trackNames[trackNumber]; //put here the track matching the key dialed
            
          Serial.println(customKey); //print the key dialed
          Serial.println("PLAYING " + track); // print the name of the track to be played
      
          musicPlayer.startPlayingFile(track.c_str()); //start playing the track


  }
    }*/
