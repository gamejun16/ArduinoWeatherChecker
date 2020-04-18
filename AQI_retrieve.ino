#include <SoftwareSerial.h>

#define TIMEOUT 10000 // mS

String json;
bool res;
int tryno;

String data0; long int regionNum; // 지역 정보 (숫자). 1835329=대구 , 1844308=하양 etc.
String data1; int iconNum;  // 날씨 정보 (아이콘)
String data2; float temp;   // 온도 정보 (섭씨)
String data3; int humi;     // 습도 정보 (퍼센트)
String data4;
String data5;

SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {
          Serial.write("시작");
  Serial.begin(9600);
  mySerial.begin(9600);
  connectWiFi();
 // acquireAQI();
}

void loop() {
  
}

boolean SendCommand(String cmd, String ack){
  mySerial.println(cmd); // Send "AT+" command to module
  
  
  if (!echoFind_withJSON(ack, TIMEOUT)) // timed out waiting for ack string
    return true; // ack blank
    else return false; //  ack found
    
}
 
boolean echoFind_withJSON(String keyword, int timeout){
  json = "";
  byte current_char = 0;
  byte keyword_length = keyword.length();
  long deadline = millis() + timeout;
  bool onmatch = false;
  bool jsonflag = false;
  while(millis() < deadline){
    if (mySerial.available()){
      char ch = mySerial.read();
      if(jsonflag)
        Serial.write(ch);
      if(ch == '{') jsonflag = true;
      if(jsonflag == true) json += ch;
      if (ch == keyword[current_char]) {
        if (++current_char == keyword_length) return true;
        onmatch = true;
      } else {
        onmatch = false;
        current_char = 0;
      }
    }
  }
  return false; // Timed out
}

int connectWiFi(void) {  
  tryno = 1;
  while(tryno<5) {
    Serial.print(F("\n와이파이를 찾는 중입니다 ... (연결 시도: "));
    Serial.print(tryno);
    Serial.println(F(")"));
    
    // <SET>  wifi ssid and password
    res = SendCommand(F("AT+CWJAP=\"jin\",\"01053127068\""),F("OK"));
    if(res == false) break;
    else tryno++;
  }
  if(tryno == 5) return -1;
  Serial.println(F("\n와이파이가 연결되었습니다.\n"));
}



