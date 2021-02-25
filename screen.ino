#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define CLOCK 11
#define DATA 12

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;

#define WIDTH 240
#define HEIGHT 320
#define ROWS 3
#define COLUMNS 2

#define TEXT_SIZE 3

double colors[5] = {BLUE, GREEN, MAGENTA, YELLOW, CYAN};
char texts[6][13] = {"Neutral", "Sad", "Angry", "Happy", "X", "Toggle Scene"};

bool currentClock = LOW;

void setup(void) {
  Serial.begin(9600);
  
  tft.reset();
  
  uint16_t identifier = tft.readID();

  tft.begin(identifier);

  tft.fillScreen(BLACK);

  tft.setTextColor(WHITE);
  for(int rowIndex = 0; rowIndex < ROWS; rowIndex++){
    for(int columnIndex = 0; columnIndex < COLUMNS; columnIndex++){
      tft.drawRect(WIDTH/COLUMNS * columnIndex , HEIGHT/ROWS * rowIndex, WIDTH/COLUMNS, HEIGHT/ROWS, colors[rand() % 5]);
      int textLen = strlen(texts[(rowIndex * COLUMNS) + columnIndex]);
      tft.setCursor(WIDTH/COLUMNS * (columnIndex + 0.5) - (textLen * TEXT_SIZE), HEIGHT/ROWS * (rowIndex + 0.5));
      tft.println(texts[(rowIndex * COLUMNS) + columnIndex]);
    }
  }

  currentcolor = BLUE;
 
  pinMode(13, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
}

#define MINPRESSURE 100
#define MAXPRESSURE 1000

void loop()
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {

    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    p.x /= (WIDTH/COLUMNS);
    p.y /= (HEIGHT/ROWS);
    tft.setCursor(0,0);
    tft.setTextColor(BLACK, WHITE);
    tft.print((p.y * COLUMNS) + p.x);
    sendClicked((p.y * COLUMNS) + p.x);
    delay(1000);
  }
}

void sendClicked(int data) {
  for(int index = 0; index < 8; index++){
    digitalWrite(DATA, data >> index & 1);
    digitalWrite(CLOCK, currentClock);
    currentClock = !currentClock;
    delay(10);
  }
}
