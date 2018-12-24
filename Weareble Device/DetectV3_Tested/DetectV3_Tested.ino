#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
ESP8266WiFiMulti WiFiMulti;
WiFiClient espClient;
PubSubClient client(espClient);
const char* password = "yurdaer1";
const char* ssid = "Yurdaer";
//const char* password = "206264D2480";
//const char* ssid = "Tele2Internet-9EB85";
//IPAddress ip(192, 168, 20, 6);
//IPAddress gateway(192, 168, 20, 1);
//IPAddress subnet(255, 255, 255, 0);
const char* mqtt_server = "m23.cloudmqtt.com";
const int mqtt_port =   10941;
const char* mqtt_user = "Weareble";
const char* mqtt_password = "Weareble";
char* inTopic = "Project/Weareble";
char* outTopic = "sensor";
char* helloMsg = "Fall detected";
String msg = "Project/Fall";
const String httpUrl = "http://192.168.20.2:8080/getReady";
const int httpPort = 8080;
// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;
// Select SDA and SCL pins for I2C communication
const uint8_t scl = 5;
const uint8_t sda = 4;
int timeIndex = 0;
int indx = 0;
int wifi = 0;
int alarm = 15;
boolean isNew = true;
const int arrSize = 60;
double AX[arrSize];
double AY[arrSize];
double AZ[arrSize];
double GX[arrSize];
double GY[arrSize];
double GZ[arrSize];
double Ax, Ay, Az, Gx, Gy, Gz, Ax1, Ay1, Az1, AxD, AyD, AzD, GxD, GyD, GzD;
// sensitivity scale factor respective to full scale setting provided in datasheet
const uint16_t AccelScaleFactor = 128;
const uint16_t GyroScaleFactor = 10;
// MPU6050 few configuration register addresses
const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;
int16_t AccelX, AccelY, AccelZ, GyroX, GyroY, GyroZ;
boolean wifiConnected = false;
Ticker checkWifi;



void CheckWifiStatus() {
  wifiConnected = false;
}

bool sen_req(String httpMsg) {
  HTTPClient http;
  //http.begin(httpUrl, httpPort , "/getReady");
  http.begin(httpUrl);
  // http.addHeader("Content-Type", "text/plain");  //Specify content-type header
  int httpCode =  http.POST("getReady");
  if (httpCode > 0) {
    Serial.println(httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(http.errorToString(httpCode));
      Serial.println(payload);
      Serial.println("Http kod is ok");
    }
  } else {
    String errorCode = "[HTTP] POST... failed, error: " + http.errorToString(httpCode);
    Serial.println(errorCode);
  }
  http.end();
  return httpCode;

}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFiMulti.addAP(ssid, password);
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    digitalWrite(wifi, HIGH);
    delay(500);
    digitalWrite(wifi, LOW);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(wifi, HIGH);
  wifiConnected = true;
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    digitalWrite(wifi, LOW);
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, helloMsg);
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  digitalWrite(wifi, HIGH);
}


int Calculate_Diff(double arr[]) {
  int minumum = 0, maximum = 0;
  for (int i = 0; i < arrSize ; i++) {
    if (arr[i] < minumum) {
      minumum = arr[i];
    }
    else if (arr[i] >= maximum) {
      maximum = arr[i];
    }
  }
  return (maximum - minumum);
}

void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}

// read all 14 register
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (uint8_t)14);
  AccelX = (((int16_t)Wire.read() << 8) | Wire.read());
  AccelY = (((int16_t)Wire.read() << 8) | Wire.read());
  AccelZ = (((int16_t)Wire.read() << 8) | Wire.read());
  GyroX = (((int16_t)Wire.read() << 8) | Wire.read());
  GyroY = (((int16_t)Wire.read() << 8) | Wire.read());
  GyroZ = (((int16_t)Wire.read() << 8) | Wire.read());
}

//configure MPU6050
void MPU6050_Init() {
  delay(150);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x03);// set +/- 16g full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
}
/*
  void callback(char* topic, byte* payload, unsigned int length) {
  String str_message = String((char*)payload).substring(0, length);
  str_message.toUpperCase();
  int delimiter = str_message.indexOf('#');
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println();
  Serial.println(str_message);
  }
*/

void setup() {
  Wire.begin(sda, scl);
  Serial.begin(9600);
  delay(1000);
  pinMode(wifi, OUTPUT);
  pinMode(alarm, OUTPUT);
  digitalWrite(wifi, HIGH);
  delay(1000);
  digitalWrite(wifi, LOW);
  delay(1000);
  digitalWrite(alarm, HIGH);
  delay(1000);
  digitalWrite(alarm, LOW);
  MPU6050_Init();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.connect("ESP8266Client2", mqtt_user, mqtt_password);
  checkWifi.attach(10, CheckWifiStatus);
  // client.setCallback(callback);
  //  client.subscribe(inTopic);
}



void loop() {
  if (!wifiConnected) {
    if (WiFi.status() != WL_CONNECTED) {
      setup_wifi();
    }
    else {
      wifiConnected = true;
    }
  }
  delay(50);
  Ax1 = Ax;
  Ay1 = Ay;
  Az1 = Az;
  Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
  Ax = (double) AccelX / AccelScaleFactor;
  Ay = (double) AccelY / AccelScaleFactor;
  Az = (double) AccelZ / AccelScaleFactor;
  Gx = (double)GyroX / GyroScaleFactor;
  Gy = (double)GyroY / GyroScaleFactor;
  Gz = (double)GyroZ / GyroScaleFactor;
  if (isNew) {
    isNew = false;
  }
  else {
    if ((Ax - Ax1 >= 60) || (Ax - Ax1 <= -60) || (Ay - Ay1 >= 60) || (Ay - Ay1 <= -60) || (Az - Az1 >= 60) || (Az - Az1 <= -60)) {
      isNew = true;
      digitalWrite(alarm, HIGH);
      while (indx < 60) {
        AX[indx] = Ax;
        AY[indx] = Ay;
        AZ[indx] = Az;
        GX[indx] = Gx;
        GY[indx] = Gy;
        GZ[indx] = Gz;
        indx++;
        delay(50);
      }
      digitalWrite(alarm, LOW);
      indx = 0;
      AxD = Calculate_Diff(AX);
      Serial.print("AxD: "); Serial.println(AxD);
      AyD = Calculate_Diff(AY);
      Serial.print("AyD: "); Serial.println(AyD);
      AzD = Calculate_Diff(AZ);
      Serial.print("AzD: "); Serial.println(AzD);
      // if ((AxD >= 200) || (AyD >= 200) || (AzD >= 200)) {
      if (AxD + AyD + AzD >= 300) {
        sen_req("Test");
        client.publish(outTopic, helloMsg);
        for (int i = 0; i <= 5; i++) {
          digitalWrite(alarm, HIGH);   // turn the LED on (HIGH is the voltage level)
          delay(500);
          digitalWrite(alarm, LOW);   // turn the LED on (HIGH is the voltage level)
          delay(500);
        }
      }
    }
  }
  client.loop();
}
