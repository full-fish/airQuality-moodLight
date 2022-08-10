// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
// connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
// a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h> // HTTP 클라이언트
#include <Adafruit_NeoPixel.h>

long firstPixelHue;
int pixelHue;
int frist_Setting=0;
int led_Speed=10;
int rainbow_Out=0;
int reset = D7;
int bright=255;
int test;
int count = 0;
int onoff = 0;
int manu = 1;
char auth[] = "H3uKzh9xGzs_pUAj5w1WxiyLYRRvxNHM";
char ssid[] = "51111";
char pass[] = "05110511";
int red_Color, green_Color, blue_Color;

extern volatile unsigned long timer0_millis;

unsigned long previous_time = 0;
unsigned long interval_send = 600000;

#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN D6
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 1
// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
// NEO_KHZ800 800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
// NEO_KHZ400 400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
// NEO_GRB Pixels are wired for GRB bitstream (most NeoPixel products)
// NEO_RGB Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
// NEO_RGBW Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

// setup() function -- runs once at startup --------------------------------


String sido = "전북"; // 서울, 부산, 대구, 인천, 광주, 대전, 울산, 경기, 강원, 충북, 충남, 전북, 전남, 경북, 경남, 제주, 세종 중 입력
String key = "Kdog0mCPR32iOlu95dxiKlrGEdUhpQxGK4IJfEBnkxEw%2BL3EuME1U7dD7JXRh%2BYHqMZhHZFhB0%2BZp08lOYzoAg%3D%3D";
String url = "http://openapi.airkorea.or.kr/openapi/services/rest/ArpltnInforInqireSvc/getCtprvnMesureSidoLIst?sidoName=" + sido + "&searchCondition=HOUR&pageNo=1&numOfRows=200&ServiceKey=" + key;

float so2, co, o3, no2, pm10, pm25 = 0; // 대기오염정보 데이터값
int score = 8; // 대기오염점수 0-최고 7-최악
int so2_Score, co_Score, o3_Score, no2_Score, pm10_Score, pm25_Score = 8;

void setup() {
digitalWrite(reset, HIGH);
pinMode(reset, OUTPUT);
pinMode(D4, OUTPUT);
digitalWrite(D4, HIGH);
Blynk.begin(auth, ssid, pass);
// These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
// Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
clock_prescale_set(clock_div_1);
#endif
// END of Trinket-specific code.
Serial.begin(115200);
strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
strip.show(); // Turn OFF all pixels ASAP
//strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

BLYNK_WRITE(0)
{
test = param.asInt();
if (test == 0)
digitalWrite(D4, LOW);
if (test == 1)
digitalWrite(D4, HIGH);
}

BLYNK_WRITE(1)
{
onoff = param.asInt();
if(onoff==0){
rainbow_Out=1;}
else if(onoff==1){
rainbow_Out=0;}
Blynk.virtualWrite(3, led_Speed);
Blynk.virtualWrite(1, onoff);
}

BLYNK_WRITE(2) {
switch (param.asInt()) {
case 1: {
manu = 1;
break;
}

case 2: {
manu = 2;
break;
}

case 3: {
manu = 3;
break;
}

case 4: {
manu = 4;
break;
}

case 5: {
manu = 5;
break;
}
}
Serial.print("manu : ");
Serial.println(manu);
}

BLYNK_WRITE(3)
{
led_Speed = param.asInt();
}

BLYNK_WRITE(4)
{
score = param.asInt();
}

BLYNK_WRITE(5)
{
bright=param.asInt();
strip.setBrightness(bright);
}

BLYNK_WRITE(11)
{
red_Color = param.asInt();
}

BLYNK_WRITE(12)
{
green_Color = param.asInt();
}

BLYNK_WRITE(13)
{
blue_Color = param.asInt();
}

// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
Blynk.run();
if(frist_Setting==0){
Blynk.virtualWrite(1, onoff);
Blynk.virtualWrite(3, led_Speed);
Blynk.virtualWrite(4, score);
Blynk.virtualWrite(5, bright);
Blynk.virtualWrite(11, red_Color);
Blynk.virtualWrite(12, green_Color);
Blynk.virtualWrite(13, blue_Color);
Blynk.virtualWrite(20, score);
Blynk.virtualWrite(21, so2_Score);
Blynk.virtualWrite(22, co_Score);
Blynk.virtualWrite(23, o3_Score);
Blynk.virtualWrite(24, no2_Score);
Blynk.virtualWrite(25, pm10_Score);
Blynk.virtualWrite(26, pm25_Score);
frist_Setting=1;
}

if (onoff == 0) {
Blynk.virtualWrite(11, 0);
Blynk.virtualWrite(12, 0);
Blynk.virtualWrite(13, 0);
red_Color = 0;
green_Color = 0;
blue_Color = 0;
}

unsigned long current_time = millis();

if(current_time - previous_time > interval_send||count<1){
Serial.println("전송실행");
count+=1;
previous_time = current_time;

if (WiFi.status() == WL_CONNECTED) // 와이파이가 접속되어 있는 경우
{
WiFiClient client; // 와이파이 클라이언트 객체
HTTPClient http; // HTTP 클라이언트 객체
if (http.begin(client, url)) { // HTTP
// 서버에 연결하고 HTTP 헤더 전송
int httpCode = http.GET();
// httpCode 가 음수라면 에러
if (httpCode > 0) { // 에러가 없는 경우
if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
String payload = http.getString(); // 받은 XML 데이터를 String에 저장
int cityIndex = payload.indexOf("전주시");
if(getNumber(payload, "<pm10Value>", cityIndex)>0&&getNumber(payload, "<pm25Value>", cityIndex)>0){
so2 = getNumber(payload, "<so2Value>", cityIndex);
co = getNumber(payload, "<coValue>", cityIndex);
o3 = getNumber(payload, "<o3Value>", cityIndex);
no2 = getNumber(payload, "<no2Value>", cityIndex);
pm10 = getNumber(payload, "<pm10Value>", cityIndex);
pm25 = getNumber(payload, "<pm25Value>", cityIndex);
}

}
} else {
Serial.printf("[HTTP] GET... 실패, 에러코드: %s\n", http.errorToString(httpCode).c_str());
}

http.end();
} else {
Serial.printf("[HTTP] 접속 불가\n");
}

}
score = getScore(); // score 변수에 대기오염점수 저장
so2_Score = getso2S();
co_Score = getcoS();
o3_Score = geto3S();
no2_Score = getno2S();
pm10_Score = getpm10S();
pm25_Score = getpm25S();
Blynk.virtualWrite(20, score);
Blynk.virtualWrite(21, so2_Score);
Blynk.virtualWrite(22, co_Score);
Blynk.virtualWrite(23, o3_Score);
Blynk.virtualWrite(24, no2_Score);
Blynk.virtualWrite(25, pm10_Score);
Blynk.virtualWrite(26, pm25_Score);
}

if (onoff == 1) {
// Fill along the length of the strip in various colors...
if (manu == 2) {
colorWipe(strip.Color(red_Color, green_Color, blue_Color), led_Speed); // Red
}
else if (manu == 3) {
Serial.println("무지개실행");
rainbow(led_Speed);
Blynk.virtualWrite(11, 0);
Blynk.virtualWrite(12, 0);
Blynk.virtualWrite(13, 0);
delay(10);
}
else if (manu == 4) {
setLEDColor(score); // 점수에 따라 LED 색상 출력
delay(1000);
}

else if (manu == 5) {
digitalWrite(reset, LOW);
}
}
else if (onoff == 0)
delay(1000);
/////////////////-/-/-/-/-/-/-/-------------------------------------

/*Serial.print("current_time : ");
Serial.print(current_time);
Serial.print(", ");
Serial.print("so2 : ");
Serial.print(so2);
Serial.print(", ");
Serial.print("co : ");
Serial.print(co);
Serial.print(", ");
Serial.print("o3 : ");
Serial.print(o3);
Serial.print(", ");
Serial.print("no2 : ");
Serial.print(no2);
Serial.print(", ");
Serial.print("pm10 : ");
Serial.print(pm10);
Serial.print(", ");
Serial.print("pm25 : ");
Serial.print(pm25);
Serial.print(", ");
Serial.print("onoff : ");
Serial.print(onoff);
Serial.print(", ");
Serial.print("manu : ");
Serial.print(manu);
Serial.print(", ");
Serial.print("score : ");
Serial.print(score); // 시리얼로 출력
Serial.print(", ");
Serial.print("count : ");
Serial.print(count);
Serial.print(", ");
Serial.print("led_Speed : ");
Serial.println(led_Speed);*/
delay(100);
}

float getNumber(String str, String tag, int from) {
int f = str.indexOf(tag, from) + tag.length(); // 테그의 위치와 테그의 문자 길이의 합
int t = str.indexOf("<", f); // 다음 테스시작위치
String s = str.substring(f, t); // 테그 사이의 숫자를 문자열에 저장
return s.toFloat(); // 문자를 소수로 변환 후 반환
}

int getScore() {
int s = -1;
if (pm10 >= 151 || pm25 >= 76 || o3 >= 0.38 || no2 >= 1.1 || co >= 32 || so2 > 0.6) // 최악
s = 7;
else if (pm10 >= 101 || pm25 >= 51 || o3 >= 0.15 || no2 >= 0.2 || co >= 15 || so2 > 0.15) // 매우 나쁨
s = 6;
else if (pm10 >= 76 || pm25 >= 38 || o3 >= 0.12 || no2 >= 0.13 || co >= 12 || so2 > 0.1) // 상당히 나쁨
s = 5;
else if (pm10 >= 51 || pm25 >= 26 || o3 >= 0.09 || no2 >= 0.06 || co >= 9 || so2 > 0.05) // 나쁨
s = 4;
else if (pm10 >= 41 || pm25 >= 21 || o3 >= 0.06 || no2 >= 0.05 || co >= 5.5 || so2 > 0.04) // 보통
s = 3;
else if (pm10 >= 31 || pm25 >= 16 || o3 >= 0.03 || no2 >= 0.03 || co >= 2 || so2 > 0.02) // 양호
s = 2;
else if (pm10 >= 16 || pm25 >= 9 || o3 >= 0.02 || no2 >= 0.02 || co >= 1 || so2 > 0.01) // 좋음
s = 1;
else // 최고
s = 0;
return s;
}

int getso2S() {
int so2s = -1;
if (so2 >= 0.6)
so2s = 7;
else if (so2 >= 0.15)
so2s = 6;
else if (so2 >= 0.1)
so2s = 5;
else if (so2 >= 0.05)
so2s = 4;
else if (so2 >= 0.04)
so2s = 3;
else if (so2 >= 0.02)
so2s = 2;
else if (so2 >= 0.01)
so2s = 1;
else
so2s = 0;
return so2s;
}

int getcoS() {
int coS = -1;
if (co >= 32)
coS = 7;
else if (co >= 15)
coS = 6;
else if (co >= 12)
coS = 5;
else if (co >= 9)
coS = 4;
else if (co >= 5.5)
coS = 3;
else if (co >= 2)
coS = 2;
else if (co >= 1)
coS = 1;
else
coS = 0;
return coS;
}

int getno2S() {
int no2s = -1;
if (no2 >= 1.1)
no2s = 7;
else if (no2 >= 0.2)
no2s = 6;
else if (no2 >= 0.13)
no2s = 5;
else if (no2 >= 0.06)
no2s = 4;
else if (no2 >= 0.05)
no2s = 3;
else if (no2 >= 0.03)
no2s = 2;
else if (no2 >= 0.02)
no2s = 1;
else
no2s = 0;
return no2s;
}

int geto3S() {
int o3s = -1;
if (o3 >= 0.38)
o3s = 7;
else if (o3 >= 0.15)
o3s = 6;
else if (o3 >= 0.12)
o3s = 5;
else if (o3 >= 0.09)
o3s = 4;
else if (o3 >= 0.06)
o3s = 3;
else if (o3 >= 0.03)
o3s = 2;
else if (o3 >= 0.02)
o3s = 1;
else
o3s = 0;
return o3s;
}

int getpm25S() {
int pm25s = -1;
if (pm25 >= 76)
pm25s = 7;
else if (pm25 >= 51)
pm25s = 6;
else if (pm25 >= 38)
pm25s = 5;
else if (pm25 >= 26)
pm25s = 4;
else if (pm25 >= 21)
pm25s = 3;
else if (pm25 >= 16)
pm25s = 2;
else if (pm25 >= 9)
pm25s = 1;
else
pm25s = 0;
return pm25s;
}

int getpm10S() {
int pm10s = -1;
if (pm10 >= 151)
pm10s = 7;
else if (pm10 >= 101)
pm10s = 6;
else if (pm10 >= 76)
pm10s = 5;
else if (pm10 >= 51)
pm10s = 4;
else if (pm10 >= 41)
pm10s = 3;
else if (pm10 >= 31)
pm10s = 2;
else if (pm10 >= 16)
pm10s = 1;
else
pm10s = 0;
return pm10s;
}

void setLEDColor(int s) {
if (s == 0) { // 최고
red_Color = 0;
green_Color = 0;
blue_Color = 255;
colorWipe(strip.Color(red_Color, green_Color, blue_Color), 50);
Blynk.virtualWrite(11, red_Color);
Blynk.virtualWrite(12, green_Color);
Blynk.virtualWrite(13, blue_Color);
}
else if (s == 1) { // 좋음
red_Color = 0;
green_Color = 100;
blue_Color = 255;
colorWipe(strip.Color(red_Color, green_Color, blue_Color), 50);
Blynk.virtualWrite(11, red_Color);
Blynk.virtualWrite(12, green_Color);
Blynk.virtualWrite(13, blue_Color);
}
else if (s == 2) { // 양호
red_Color = 0;
green_Color = 255;
blue_Color = 255;
colorWipe(strip.Color(red_Color, green_Color, blue_Color), 50);
Blynk.virtualWrite(11, red_Color);
Blynk.virtualWrite(12, green_Color);
Blynk.virtualWrite(13, blue_Color);
}

else if (s == 3) { // 보통
red_Color = 0;
green_Color = 255;
blue_Color = 63;
colorWipe(strip.Color(red_Color, green_Color, blue_Color), 50);
Blynk.virtualWrite(11, red_Color);
Blynk.virtualWrite(12, green_Color);
Blynk.virtualWrite(13, blue_Color);
}

else if (s == 4) { // 나쁨
red_Color = 255;
green_Color = 127;
blue_Color = 0;
colorWipe(strip.Color(red_Color, green_Color, blue_Color), 50);
Blynk.virtualWrite(11, red_Color);
Blynk.virtualWrite(12, green_Color);
Blynk.virtualWrite(13, blue_Color);
}

else if (s == 5) { // 상당히 나쁨
red_Color = 255;
green_Color = 63;
blue_Color = 0;
colorWipe(strip.Color(red_Color, green_Color, blue_Color), 50);
Blynk.virtualWrite(11, red_Color);
Blynk.virtualWrite(12, green_Color);
Blynk.virtualWrite(13, blue_Color);
}

else if (s == 6) { // 매우 나쁨
red_Color = 255;
green_Color = 31;
blue_Color = 0;
colorWipe(strip.Color(red_Color, green_Color, blue_Color), 50);
Blynk.virtualWrite(11, red_Color);
Blynk.virtualWrite(12, green_Color);
Blynk.virtualWrite(13, blue_Color);
}
else if (s == 7) { // 최악
red_Color = 255;
green_Color = 0;
blue_Color = 0;
colorWipe(strip.Color(red_Color, green_Color, blue_Color), 50);
Blynk.virtualWrite(11, red_Color);
Blynk.virtualWrite(12, green_Color);
Blynk.virtualWrite(13, blue_Color);
}

else {
red_Color = 255;
green_Color = 255;
blue_Color = 255;
colorWipe(strip.Color(red_Color, green_Color, blue_Color), 50);
Blynk.virtualWrite(11, red_Color);
Blynk.virtualWrite(12, green_Color);
Blynk.virtualWrite(13, blue_Color);
}
}
// Some functions of our own for creating animated effects -----------------
// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
strip.setPixelColor(i, color); // Set pixel's color (in RAM)
strip.show(); // Update strip to match
delay(wait); // Pause for a moment
}
}
// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
for (int a = 0; a < 10; a++) { // Repeat 10 times...
for (int b = 0; b < 3; b++) { // 'b' counts from 0 to 2...
strip.clear(); // Set all pixels in RAM to 0 (off)
// 'c' counts up from 'b' to end of strip in steps of 3...
for (int c = b; c < strip.numPixels(); c += 3) {
strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
}
strip.show(); // Update strip with new contents
delay(wait); // Pause for a moment
}
}
}
// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
// Hue of first pixel runs 5 complete loops through the color wheel.
// Color wheel has a range of 65536 but it's OK if we roll over, so
// just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
// means we'll make 5*65536/256 = 1280 passes through this outer loop:
for (firstPixelHue = 0; firstPixelHue < 65536; firstPixelHue += 64) {
for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
// Offset pixel hue by an amount to make one full revolution of the
// color wheel (range of 65536) along the length of the strip
// (strip.numPixels() steps):
pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
// strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
// optionally add saturation and value (brightness) (each 0 to 255).
// Here we're using just the single-argument hue variant. The result
// is passed through strip.gamma32() to provide 'truer' colors
// before assigning to each pixel:
strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
if(rainbow_Out==1){
manu=1;
break;
} 
}
if(rainbow_Out==1){
manu=1;
break;
}
strip.show(); // Update strip with new contents
delay(wait); // Pause for a moment
}
}
/*
// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
int firstPixelHue = 0; // First pixel starts at red (hue 0)
for (int a = 0; a < 30; a++) { // Repeat 30 times...
for (int b = 0; b < 3; b++) { // 'b' counts from 0 to 2...
strip.clear(); // Set all pixels in RAM to 0 (off)
// 'c' counts up from 'b' to end of strip in increments of 3...
for (int c = b; c < strip.numPixels(); c += 3) {
// hue of pixel 'c' is offset by an amount to make one full
// revolution of the color wheel (range 65536) along the length
// of the strip (strip.numPixels() steps):
int hue = firstPixelHue + c * 65536L / strip.numPixels();
uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
}
strip.show(); // Update strip with new contents
delay(wait); // Pause for a moment
firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
}
}
}
*/