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
const char* password = "12345678";
const char* ssid = "Yurdaer";
//const char* password = "46931304";
//const char* ssid = "Tele2Internet-E6371";
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
// Select SDA and SCL pins for I2C communication
const uint8_t scl = 5;
const uint8_t sda = 4;
int timeIndex = 0;
int wifi = 15;
int alarm = 0;
boolean isNew = true;
// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;
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
const double oldAccMin = -3276;
const double oldAccMax = 2303;
const double oldGyroMin = -2771;
const double oldGyroMax = 3276;
const double newMinimum = 0;
const double newMaximum = 200;
int16_t dataArray[180];
double dataNormArray[180];
double finalArray[180];
int16_t Ax, Ay, Az, Gx, Gy, Gz, Ax1, Ay1, Az1, AxD, AyD, AzD, GxD, GyD, GzD;
const uint16_t AccelScaleFactor = 10; // sensitivity scale factor
const uint16_t GyroScaleFactor = 10;
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
void MovingAverage() {
  for (int16_t i = 0; i < 30; i++) {
    if (i == 0) {
      dataNormArray[i] = dataArray[i];
      dataNormArray[1] = dataArray[1];
      dataNormArray[2] = dataArray[2];
      dataNormArray[3] = dataArray[3];
      dataNormArray[4] = dataArray[4];
      dataNormArray[5] = dataArray[5];
    }
    if (i == 1) {
      dataNormArray[6] = (dataArray[6] + dataArray[0]) / 2;
      dataNormArray[7] = (dataArray[7] + dataArray[1]) / 2;
      dataNormArray[8] = (dataArray[8] + dataArray[2]) / 2;
      dataNormArray[9] = (dataArray[9] + dataArray[3]) / 2;
      dataNormArray[10] = (dataArray[10] + dataArray[4]) / 2;
      dataNormArray[11] = (dataArray[11] + dataArray[5]) / 2;
    }
    if (i == 2) {
      dataNormArray[12] = (dataArray[12] + dataArray[6] + dataArray[0]) / 3;
      dataNormArray[13] = (dataArray[13] + dataArray[7] + dataArray[1]) / 3;
      dataNormArray[14] = (dataArray[14] + dataArray[8] + dataArray[2]) / 3;
      dataNormArray[15] = (dataArray[15] + dataArray[9] + dataArray[3]) / 3;
      dataNormArray[16] = (dataArray[16] + dataArray[10] + dataArray[4]) / 3;
      dataNormArray[17] = (dataArray[17] + dataArray[11] + dataArray[5]) / 3;
    }
    if (i == 3) {
      dataNormArray[18] = (dataArray[18] + dataArray[12] + dataArray[6] + dataArray[0]) / 4;
      dataNormArray[19] = (dataArray[19] + dataArray[13] + dataArray[7] + dataArray[1]) / 4;
      dataNormArray[20] = (dataArray[20] + dataArray[14] + dataArray[8] + dataArray[2]) / 4;
      dataNormArray[21] = (dataArray[21] + dataArray[15] + dataArray[9] + dataArray[3]) / 4;
      dataNormArray[22] = (dataArray[22] + dataArray[16] + dataArray[10] + dataArray[4]) / 4;
      dataNormArray[23] = (dataArray[23] + dataArray[17] + dataArray[11] + dataArray[5]) / 4;
    }
    if (i >= 4) {
      dataNormArray[6 * (i)] = (dataArray[6 * (i)] + dataArray[6 * (i - 1)] + dataArray[6 * (i - 2)] + dataArray[6 * (i - 3)] + dataArray[6 * (i - 4)]) / 5;
      dataNormArray[6 * (i) + 1] = (dataArray[6 * (i) + 1] + dataArray[6 * (i - 1) + 1] + dataArray[6 * (i - 2) + 1] + dataArray[6 * (i - 3) + 1] + dataArray[6 * (i - 4) + 1]) / 5;
      dataNormArray[6 * (i) + 2] = (dataArray[6 * (i) + 2] + dataArray[6 * (i - 1) + 2] + dataArray[6 * (i - 2) + 2] + dataArray[6 * (i - 3) + 2] + dataArray[6 * (i - 4) + 2]) / 5;
      dataNormArray[6 * (i) + 3] = (dataArray[6 * (i) + 3] + dataArray[6 * (i - 1) + 3] + dataArray[6 * (i - 2) + 3] + dataArray[6 * (i - 3) + 3] + dataArray[6 * (i - 4) + 3]) / 5;
      dataNormArray[6 * (i) + 4] = (dataArray[6 * (i) + 4] + dataArray[6 * (i - 1) + 4] + dataArray[6 * (i - 2) + 4] + dataArray[6 * (i - 3) + 4] + dataArray[6 * (i - 4) + 4]) / 5;
      dataNormArray[6 * (i) + 5] = (dataArray[6 * (i) + 5] + dataArray[6 * (i - 1) + 5] + dataArray[6 * (i - 2) + 5] + dataArray[6 * (i - 3) + 5] + dataArray[6 * (i - 4) + 5]) / 5;
    }
  }
}

void NormalizaData() {
  for (int16_t i = 0; i < 30; i++) {
    finalArray[6 * (i)] = ((dataNormArray[6 * (i)] - oldAccMin) / (oldAccMax - oldAccMin)) * (newMaximum - newMinimum) + newMinimum;
    finalArray[6 * (i) + 1] = ((dataNormArray[6 * (i) + 1] - oldAccMin) / (oldAccMax - oldAccMin)) * (newMaximum - newMinimum) + newMinimum;
    finalArray[6 * (i) + 2] = ((dataNormArray[6 * (i) + 2] - oldAccMin) / (oldAccMax - oldAccMin)) * (newMaximum - newMinimum) + newMinimum;
    finalArray[6 * (i) + 3] = ((dataNormArray[6 * (i) + 3] - oldGyroMin) / (oldGyroMax - oldGyroMin)) * (newMaximum - newMinimum) + newMinimum;
    finalArray[6 * (i) + 4] = ((dataNormArray[6 * (i) + 4] - oldGyroMin) / (oldGyroMax - oldGyroMin)) * (newMaximum - newMinimum) + newMinimum;
    finalArray[6 * (i) + 5] = ((dataNormArray[6 * (i) + 5] - oldGyroMin) / (oldGyroMax - oldGyroMin)) * (newMaximum - newMinimum) + newMinimum;
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin(sda, scl);
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
  client.publish(outTopic, "ready");
  Serial.println("Setup ready");
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
  delay(200);
  Ax1 = Ax;
  Ay1 = Ay;
  Az1 = Az;
  Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
  Ax =  AccelX / AccelScaleFactor;
  Ay =  AccelY / AccelScaleFactor;
  Az =  AccelZ / AccelScaleFactor;
  Gx = GyroX / GyroScaleFactor;
  Gy = GyroY / GyroScaleFactor;
  Gz = GyroZ / GyroScaleFactor;
  if (isNew) {
    isNew = false;
  }
  else {
    if ((Ax - Ax1 >= 650) || (Ax - Ax1 <= -650) || (Ay - Ay1 >= 650) || (Ay - Ay1 <= -650) || (Az - Az1 >= 650) || (Az - Az1 <= -650)) {
      isNew = true;

      for (int i = 0; i < 30; i++) {
        Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
        Ax =  AccelX / AccelScaleFactor;
        Ay =  AccelY / AccelScaleFactor;
        Az =  AccelZ / AccelScaleFactor;
        Gx = GyroX / GyroScaleFactor;
        Gy = GyroY / GyroScaleFactor;
        Gz = GyroZ / GyroScaleFactor;
        dataArray[(i * 6)] = Ax;
        dataArray[(i * 6) + 1] = Ay;
        dataArray[(i * 6) + 2] = Az;
        dataArray[(i * 6) + 3] = Gx;
        dataArray[(i * 6) + 4] = Gy;
        dataArray[(i * 6) + 5] = Gz;
        delay(100);
      }
      MovingAverage();
      NormalizaData();
      int a = (int)finalArray[9];
      int b = (int)finalArray[146];
      Serial.println(a);
      Serial.println(b);
      if ((a <= 84) && (b <= 121)) {
        digitalWrite(alarm, HIGH);
        Serial.println("FALL");
        sen_req("Test");
        client.publish(outTopic, helloMsg);
        for (int i = 0; i <= 10; i++) {
          digitalWrite(alarm, HIGH);   // turn the LED on (HIGH is the voltage level)
          delay(500);
          digitalWrite(alarm, LOW);   // turn the LED on (HIGH is the voltage level)
          delay(500);
        }
      }

      digitalWrite(alarm, LOW);
    }
  }
  client.loop();
}
