#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;
WiFiClient espClient;
PubSubClient client(espClient);

const char* password = "12121213";
const char* ssid = "Yurdaer";
const char* mqtt_server = "m23.cloudmqtt.com";
const int mqtt_port =   10941;
const char* mqtt_user = "Weareble";
const char* mqtt_password = "Weareble";
char* inTopic = "Project/#";
char* outTopic = "Project";
char* helloMsg = "Weareble online";
String msg = "Project/Fall";
const String httpUrl = "http://192.168.1.1";
const int httpPort = 8888;



// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;
// Select SDA and SCL pins for I2C communication
const uint8_t scl = D6;
const uint8_t sda = D7;
int indx = 0;
int LED = D0;
const int arrSize = 30;
double AX[arrSize];
double AY[arrSize];
double AZ[arrSize];
double GX[arrSize];
double GY[arrSize];
double GZ[arrSize];
double Ax, Ay, Az, Gx, Gy, Gz;
int AxD, AyD, AzD, GxD, GyD, GzD;
// sensitivity scale factor respective to full scale setting provided in datasheet
const uint16_t AccelScaleFactor = 10;
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

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  Wire.begin(sda, scl);
  MPU6050_Init();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.connect("ESP8266Client2", mqtt_user, mqtt_password);
  client.setCallback(callback);
  client.subscribe(inTopic);
}

bool sen_req(String httpMsg) {
  HTTPClient http;
  http.begin(httpUrl, httpPort , "");

  int httpCode =  http.POST("FALL");
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
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
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED, HIGH);
}

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

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    digitalWrite(LED, LOW);

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
  digitalWrite(LED, HIGH);

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
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
}

int Calculate_Diff(double arr[]) {
  double minumum = 0, maximum = 0;

  for (int i = 0; i < arrSize ; i++) {
    if (arr[i] < minumum) {
      minumum = arr[i];
    }
    else if (arr[i] > maximum) {
      maximum = arr[i];
    }
  }
  //  Serial.print("Maximum");   Serial.println(maximum);
  // Serial.print("Minimum");   Serial.println(minumum);
  return (maximum - minumum);
}


void loop() {
  Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
  //divide each with their sensitivity scale factor
  Ax = (double)AccelX / AccelScaleFactor;
  Ay = (double)AccelY / AccelScaleFactor;
  Az = (double)AccelZ / AccelScaleFactor;
  Gx = (double)GyroX / GyroScaleFactor;
  Gy = (double)GyroY / GyroScaleFactor;
  Gz = (double)GyroZ / GyroScaleFactor;
  AX[indx] = Ax;
  AY[indx] = Ay;
  AZ[indx] = Az;
  GX[indx] = Gx;
  GY[indx] = Gy;
  GZ[indx] = Gz;


  /*
    Serial.print("Ax: "); Serial.print(Ax);
    Serial.print(" Ay: "); Serial.print(Ay);
    Serial.print(" Az: "); Serial.print(Az);
    Serial.print(" T: "); Serial.print(T);
    Serial.print(" Gx: "); Serial.print(Gx);
    Serial.print(" Gy: "); Serial.print(Gy);
    Serial.print(" Gz: "); Serial.println(Gz);
    Serial.print(Ax);
    Serial.print(" ");
    Serial.print(Ay);
    Serial.print(" ");
    Serial.print(Az);
    Serial.print(" ");
    Serial.print(Gx);
    Serial.print(" ");
    Serial.print(Gy);
    Serial.print(" ");
    Serial.println(Gz);
  */

  indx++;
  if (indx == arrSize) {
    AxD = Calculate_Diff(AX);
    AyD = Calculate_Diff(AY);
    AzD = Calculate_Diff(AZ);
    GxD = Calculate_Diff(GX);
    GyD = Calculate_Diff(GY);
    GzD = Calculate_Diff(GZ);
    if ((AxD >= 5000) || (AyD >= 5000) || (AzD >= 5000)) {
      digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
      client.subscribe(msg.c_str());
    }
    else {
      digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level)
    }

    Serial.print("Delta Ax : "); Serial.println(AxD);
    Serial.print("Delta Ay : "); Serial.println(AyD);
    Serial.print("Delta Az : "); Serial.println(AzD);
    Serial.print("Delta Gx : "); Serial.println(GxD);
    Serial.print("Delta Gy : "); Serial.println(GxD);
    Serial.print("Delta Gz : "); Serial.println(GxD);
    Serial.println("-----------------------------------------------");
    indx = 0;
    AxD = 0;
    AyD = 0;
    AzD = 0;
    GxD = 0;
    GyD = 0;
    GzD = 0;
  }
  delay(100);
}
