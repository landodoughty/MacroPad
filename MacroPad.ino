#include <Keyboard.h>;
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

void checkForPress(int x, int y, char key1, char key2 = '\0', char key3 = '\0');//prototype function for optional arguments

//ctrl and shift keys THESE NEED TO BE CHANGED BASED ON YOUR DESIRED OPERATING SYSTEM
char ctrlKey = KEY_LEFT_CTRL;
char shiftKey = KEY_LEFT_SHIFT;
char altKey = KEY_LEFT_ALT;

//define neopixels
int neoPixelPin = 14;
Adafruit_NeoPixel pixels(20, neoPixelPin, NEO_GRB + NEO_KHZ800);

//The following are the collumn and row numbers for writing and reading the keys
int c1 = 12;
int c2 = 10;
int c3 = 1;
int c4 = 0;
int r1 = 17;
int r2 = 3;
int r3 = 4;
int r4 = 5;
int r5 = 6;

//arrays for pins and holding values
int cols[] = {c1, c2, c3, c4};
int rows[] = {r1, r2, r3, r4, r5};
int vals[4][5];
int prev[4][5];


int ledMode = 2; // mode 0 is constant color which can be switched, mode 1 is rgb, mode 2 is by layer
int ledValSelector = 0; //this changes between solid colors
long ledModeSwitchCooldown = 0;//these two cooldowns makes sure it doesnt switch through led modes too quickly
long ledValSelectorCooldown = 0;
int r = 255;//rgb values to cycle
int g = 0;
int b = 0;
int rgbToChange = 2;//helper variable for rgb cycle
long prevLedChange = 0;//helper variable for slowing rgb cycle

int layer = 1; //currently: 0->num pad 1->links and macros
int numPadLeds[20] = {0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0};
int partStudioLeds[20][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
  {0, 0, 255}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0},
  {0, 0, 255}, {255, 200, 0}, {255, 200, 0}, {0, 255, 0},
  {255, 60, 0}, {255, 60, 0}, {0, 255, 255}, {0, 255, 0},
  {0, 0, 0}, {255, 0, 255}, {255, 0, 255}, {0, 0, 0}
};


//settings
long keypressCooldown = 200;//use this variable to set limit on how fast you can change RGB modes
long ledChangeDelay = 5;//use this variable to change how fast RGB cycle goes
int ledVals[9][3] = {{255, 255, 255}, {255, 0, 0}, {255, 60, 0}, {255, 200, 0}, {0, 255, 0}, {0, 255, 255}, {0, 0, 255}, {255, 0, 255}, {0, 0, 0}};
//^ these are the solid colors you can cycle through. If you want to change the number of colors make sure to change the number in the initialization of ledVals and the modulus number in the detection for led color change block of code in loop


// the setup function runs once when you press reset or power the board
void setup() {
  //set collumns to output and rows to input pulled low
  pinMode(c1, OUTPUT);
  pinMode(c2, OUTPUT);
  pinMode(c3, OUTPUT);
  pinMode(c4, OUTPUT);
  pinMode(r1, INPUT_PULLDOWN);
  pinMode(r2, INPUT_PULLDOWN);
  pinMode(r3, INPUT_PULLDOWN);
  pinMode(r4, INPUT_PULLDOWN);
  pinMode(r5, INPUT_PULLDOWN);

  Serial.begin(9600);
  Keyboard.begin();
  pixels.begin();
  //doing this will disable all the columns.
  digitalWrite(c1, LOW);
  digitalWrite(c2, LOW);
  digitalWrite(c3, LOW);
  digitalWrite(c4, LOW);

}

void checkForPress(int x, int y, char key1, char key2 , char key3) {
  if (vals[x][y] == 1 && prev[x][y] == 0) {
    Keyboard.press(key3);
    Keyboard.press(key2);
    Keyboard.press(key1);
  } else if (vals[x][y] == 0 && prev[x][y] == 1) {
    Keyboard.release(key1);
    Keyboard.release(key2);
    Keyboard.release(key3);
  }
  prev[x][y] = vals[x][y];
}
void checkForPressToLink(int x, int y, String link) {
  if (vals[x][y] == 1 && prev[x][y] == 0) {
    Keyboard.press(ctrlKey);
    Keyboard.press('t');
    delay(2);
    Keyboard.release(ctrlKey);
    Keyboard.release('t');
    delay(2);
    Keyboard.print(link);
    Keyboard.press('\n');
    Keyboard.release('\n');
  }
  prev[x][y] = vals[x][y];
}


void updateLEDs() {
  //led mode 0: constant color for all keys
  if (ledMode == 0) {
    pixels.clear();
    for (int j = 0; j < 20; j++) {
      pixels.setPixelColor(j, pixels.Color(ledVals[ledValSelector][0], ledVals[ledValSelector][1], ledVals[ledValSelector][2]));
    }
  }
  //led mode 1: rgb
  else if (ledMode == 1 && (prevLedChange + ledChangeDelay < millis()) ) {
    prevLedChange = millis();
    pixels.clear();

    switch (rgbToChange) {
      case 1:
        r++;
        if (r == 255) rgbToChange = -3;
        break;
      case 2:
        g++;
        if (g == 255) rgbToChange = -1;
        break;
      case 3:
        b++;
        if (b == 255) rgbToChange = -2;
        break;
      case -1:
        r--;
        if (r == 0) rgbToChange = 3;
        break;
      case -2:
        g--;
        if (g == 0) rgbToChange = 1;
        break;
      case -3:
        b--;
        if (b == 0) rgbToChange = 2;
        break;
    }

    for (int j = 0; j < 20; j++) {
      pixels.setPixelColor(j, pixels.Color(r, g, b));
    }

  //led mode 2 changes based on current layer:
  } else if (ledMode == 2) {
    pixels.clear();
    if (layer == 0 ) {
      for (int j = 0; j < 20; j++) {
        pixels.setPixelColor(j, pixels.Color(ledVals[ledValSelector][0]*numPadLeds[j], ledVals[ledValSelector][1]*numPadLeds[j], ledVals[ledValSelector][2]*numPadLeds[j]));
      }
    } else if (layer == 1) {
      for (int j = 0; j < 20; j++) {
        pixels.setPixelColor(j, pixels.Color(ledVals[ledValSelector][0], ledVals[ledValSelector][1], ledVals[ledValSelector][2]));
      }

    } else {
      for (int j = 0; j < 20; j++) {
        pixels.setPixelColor(j, pixels.Color(partStudioLeds[j][0], partStudioLeds[j][1], partStudioLeds[j][2]));
      }

    }
  }

  pixels.show();

}

// the loop function runs over and over again forever
void loop() {
  updateLEDs();

  //loop through array of keys and read
  for (int i = 0; i < 4; i++) {
    digitalWrite(cols[i], HIGH);
    for (int j = 0; j < 5; j++) {
      vals[i][j] = digitalRead(rows[j]);
    }
    digitalWrite(cols[i], LOW);
  }


  //detection for led mode change
  if (vals[0][0] == 1 && vals[1][0] == 1 && vals[2][0] == 1) {
    vals[0][0] = 0;
    vals[1][0] = 0;
    vals[2][0] = 0;
    if (ledModeSwitchCooldown + keypressCooldown < millis()) {
      ledMode++;
      ledMode = ledMode % 3; // this number is the total number of mode options
      ledModeSwitchCooldown = millis();
    }
  }

  //detection for led color change
  if (vals[0][0] == 1 && vals[1][0] == 1 && vals[3][0] == 1) {
    vals[0][0] = 0;
    vals[1][0] = 0;
    vals[3][0] = 0;
    if (ledValSelectorCooldown + keypressCooldown < millis()) {
      ledValSelector++;
      ledValSelector = ledValSelector % 9; // this number is the total number of mode options
      ledValSelectorCooldown = millis();
    }
  }

  if (vals[0][0] == 1 && vals[1][0] == 1) {
    vals[0][0] = 0;
    vals[1][0] = 0;
  }
  
  //check for layer change
  if (vals[2][0] == 1 && prev[2][0] == 0) {
    layer += 1;
    layer = layer % 2;
  }
  prev[2][0] = vals[2][0];



  if (layer == 0) { //example of using layer 0 as a number pad
    //always active
    checkForPress(3, 0, '\b');
    checkForPress(0, 0, '\x1B');
    checkForPress(3, 4, '\n');
    checkForPress(0, 1, '7');
    checkForPress(1, 1, '8');
    checkForPress(2, 1, '9');
    checkForPress(0, 2, '4');
    checkForPress(1, 2, '5');
    checkForPress(2, 2, '6');
    checkForPress(0, 3, '1');
    checkForPress(1, 3, '2');
    checkForPress(2, 3, '3');
    checkForPress(1, 4, '0');
    checkForPress(2, 4, '.');

    if (vals[0][4] == 0) { //non shift
      checkForPress(3, 1, '(');
      checkForPress(3, 2, '*');
      checkForPress(3, 3, '+');
    } else { //shift
      checkForPress(3, 1, ')');
      checkForPress(3, 2, '/');
      checkForPress(3, 3, '-');
    }
  } else if (layer == 1) {//this is an example of using the bottom layer for opening links in chrome
    checkForPressToLink(0, 1, "https://chatgpt.com/?model=auto");
    checkForPressToLink(1, 1, "https://www.youtube.com/");
    checkForPressToLink(2, 1, "https://www.linkedin.com/feed/");
    checkForPressToLink(3, 1, "https://www.amazon.com/");
    checkForPressToLink(0, 2, "https://www.printables.com/model?period=year");
    checkForPressToLink(1, 2, "https://cad.onshape.com/documents?nodeId=0&resourceType=filter");
    checkForPressToLink(2, 2, "https://github.com/landodoughty");
    checkForPressToLink(3, 2, "https://landodoughty.github.io/index.html");
    checkForPressToLink(0, 3, "https://mail.google.com/mail/u/0/#inbox");
    checkForPressToLink(1, 3, "https://mail.google.com/mail/u/4/#inbox");
    checkForPressToLink(2, 3, "https://docs.google.com/document/u/4/?tgif=d");
    //    checkForPressToLink(3,3,"htt");
    checkForPressToLink(0, 4, "https://idp.rice.edu/idp/profile/cas/login?execution=e1ps://www.youtube.com/s1");
    checkForPressToLink(1, 4, "https://canvas.rice.edu/");
    checkForPress(2, 4, 'c', ctrlKey);
    checkForPress(3, 4, 'p', ctrlKey);//example of doing commands
  }

}


//onshape part studio shortcuts:
/*
   if (vals[0][4] == 0) { //non shift
      checkForPress(0, 1, 's', shiftKey); //sketch
      checkForPress(1, 1, 'e', shiftKey); //extrude
      checkForPress(2, 1, 'w', shiftKey); //revolve
      checkForPress(3, 1, 'f', shiftKey); //filet
      checkForPress(0, 2, 'n');//view normal
      checkForPress(1, 2, 'm', shiftKey, altKey); //mirror
      checkForPress(2, 2, 's', shiftKey, altKey); //shell
      checkForPress(3, 2, 'l', shiftKey, altKey); //linear pattern
      checkForPress(0, 3, 't', ctrlKey, altKey); //transform
      checkForPress(1, 3, 'm', ctrlKey, altKey); //move face
      checkForPress(2, 3, 'i');//derived/insert
      checkForPress(3, 3, 'h', ctrlKey, altKey); //helix
      checkForPress(1, 4, 'm');//mate
      checkForPress(2, 4, 'r', ctrlKey, altKey); //mate relation

    } else { //shift
      checkForPress(0, 1, 'p', shiftKey, altKey); //new plane
      checkForPress(1, 1, 'l', ctrlKey, altKey); //loft
      checkForPress(2, 1, 'w', ctrlKey, altKey); //sweep
      checkForPress(3, 1, 'c');//chamfer
      checkForPress(0, 2, 'p');//show/hide planes
      checkForPress(1, 2, 'b', ctrlKey, altKey); //boolean
      checkForPress(2, 2, 's', ctrlKey, altKey); //split
      checkForPress(3, 2, 'o');//circular pattern
      checkForPress(0, 3, 't', ctrlKey, altKey); //transform
      checkForPress(1, 3, 'f', ctrlKey, altKey); //replace face

      checkForPress(2, 3, 'v');//variable
      checkForPress(3, 3, 'p', ctrlKey, altKey); //curvePattern
      checkForPress(1, 4, 'r');//replicate
      checkForPress(2, 4, 'j');//hide/show mates
    }
*/
