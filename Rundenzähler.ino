/*
    Stabocar - Rundenzähler und Zeitmesser mit Arduino pro mini,
    Lichtschranken und OLED-Display
 
    FP 2020
*/

// https://github.com/greiman/SSD1306Ascii/blob/master/examples/HelloWorldWire/HelloWorldWire.ino

#include <Wire.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>

unsigned long starta, startb, ttimea, ttimeb;
unsigned limit = 600;
unsigned timea, timeb, besta, bestb;
unsigned rundea, rundeb;
unsigned statea, olda, stateb, oldb;
int i, disp;

char buffer[18];

// lightbeams
char  pina = 6;
char  pinb = 7;
//  pushbutton
char  pushb = 4; //Reset
char  pushc = 3; //Start

#define I2C_ADDRESS 0x3C

// noname I2C Display
SSD1306AsciiWire oled;

void setup() {
  Wire.begin();
  pinMode (pina, INPUT);
  pinMode (pinb, INPUT);
  pinMode (pushb, INPUT);
  pinMode (pushc, INPUT);

  oled.begin(&SH1106_128x64, I2C_ADDRESS);
  oled.setFont(Adafruit5x7);
  
  oled.clear();
  oled.set2X();
  oled.setCursor (0, 1);
  oled.print("Herrmann");
  delay (500);
  oled.setCursor (0, 3);
  oled.print("    Track");
  delay (1000);
  oled.set1X();
  oled.setCursor (0, 6);
  oled.print("     Kupferzell");
  delay (3000);


  oled.clear();
  oled.set2X();
  oled.setCursor (0, 1);
  oled.print("  Welcome");
  delay (1500);
//  oled.setCursor (0, 3);
// oled.print("   Track");
//  delay (1000);
  oled.set1X();
  oled.setCursor (0, 6);
  oled.print("    !! Have fun !!");
  delay (3000);

// small animation <img draggable="false" role="img" class="emoji" alt="🙂" src="https://s0.wp.com/wp-content/mu-plugins/wpcom-smileys/twemoji/2/svg/1f642.svg">
//  oled.clear();
//  oled.set2X();
//  for (i = 10; i > -1; i--) {
//    sprintf (buffer, "   %d", i);
//    oled.setCursor (0, 3);
//    oled.print(buffer);
//    delay (1000);

//  }
cowntdown();

// main display now
  oled.clear();
  oled.set1X();

  starta = millis();
  startb = millis();

  update_loops();
}


void loop() {

  if (digitalRead (pushb) == HIGH) {
    delay (20);
// wait for release
    while (!digitalRead (pushb)) {};

    cowntdown();


    starta = millis();
    startb = millis();
    rundea = 0;
    rundeb = 0;
    besta = 0;
    bestb = 0;
    oled.clear();
    update_loops();
  }

  statea = digitalRead (pina);
  if (statea != olda) {
// start counting when line goes up
    if (statea) {
// debouncing: time between changes must be > 1s
      if ((millis() - starta) > 1000) {
  // display last time
        timea = (unsigned) (millis() - starta);
        if (!besta && rundea) {
          besta = timea; // first round
        }
        else if (timea < besta) {
          besta = timea; // best time
        }
        rundea++;
      }
      starta = millis(); //start new counter
      update_loops();
      update_times();
    }
  }
olda = statea;

// the same procedure for track b
stateb = digitalRead (pinb);
if (stateb != oldb) {
//start counting when line goes up
if (stateb) {
// debouncing: time between changes must be > 1s
if ((millis() - startb) > 1000) {
// display last time
timeb = (unsigned) (millis() - startb);
if (!bestb && rundeb) {
bestb = timeb; // first round
}
else if (timeb < bestb) {
bestb = timeb; // best time
}
rundeb++;
}
startb = millis(); //start new counter
update_loops();
update_times();
}
}
oldb = stateb;
delay (10);
}

void convert(unsigned t) {
unsigned s, ms;
s = t / 1000;
ms = (t % 1000) / 10; // only 2 digits
sprintf (buffer, "%2d.%02d s", s, ms);
}

void update_loops() {
char c;
oled.set2X();
if (statea) {
c= '!'; // Auto gemessen
} else {
c= ' '; // kein Echo
}
sprintf (buffer, "%4d%c", rundea,c);
oled.setCursor (0, 1); oled.print (buffer);
if (stateb) {
c= '!'; // Auto gemessen
} else {
c= ' '; // kein Echo
}  
sprintf (buffer, "%4d%c", rundeb,c);
oled.setCursor (64, 1); oled.print (buffer);
oled.set1X();
}

void update_times() {
oled.setCursor (0, 4);
convert (timea); oled.print (buffer);
oled.setCursor (70, 4);
convert (timeb); oled.print (buffer);

oled.setCursor (0, 6);
convert (besta); oled.print (buffer);
oled.setCursor (70, 6);
convert (bestb); oled.print (buffer);
}

void cowntdown() {

oled.clear();
  oled.set2X();
  oled.setCursor (0, 2);
  oled.print("  Ready ?");
  delay (1000);
  oled.set1X();
  oled.setCursor (0, 7);
  oled.print("     Push Button!");
  delay (4000);

  while (!digitalRead (pushc)== HIGH) {};


  oled.clear();
  oled.set2X();
  oled.setCursor (0, 3);
  oled.print("    10");
  delay (1000);
  oled.clear();
  oled.set2X();
  oled.setCursor (0, 3);
  oled.print("     9");
  delay (1000);
  oled.clear();
  oled.set2X();
  oled.setCursor (0, 3);
  oled.print("     8");
  delay (1000);
  oled.clear();
  oled.set2X();
  oled.setCursor (0, 3);
  oled.print("     7");
  delay (1000);
  oled.clear();
  oled.set2X();
  oled.setCursor (0, 3);
  oled.print("     6");
  delay (1000);
  oled.clear();
  oled.set2X();
  oled.setCursor (0, 3);
  oled.print("     5");
  delay (1000);
  oled.clear();
  oled.set2X();
  oled.setCursor (0, 3);
  oled.print("     4");
  delay (1000);
  oled.clear();
  oled.set2X();
  oled.setCursor (0, 3);
  oled.print("     3");
  delay (1000);
  oled.clear();
  oled.set2X();
  oled.setCursor (0, 3);
  oled.print("     2");
  delay (1000);
  oled.clear();
  oled.set2X();
  oled.setCursor (0, 3);
  oled.print("     1");
  delay (1000);
  oled.clear();
  oled.setCursor (0, 3);
  oled.print("   Go!!!");
  delay (100);

}