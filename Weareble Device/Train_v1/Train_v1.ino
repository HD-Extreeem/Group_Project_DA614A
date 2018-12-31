#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Arduino.h>

// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;
// Select SDA and SCL pins for I2C communication
const uint8_t scl = 5;
const uint8_t sda = 4;
int timeIndex = 0;
int indx = 0;
int alarm = 15;
boolean isNew = true;
const int arrSize = 60;

int16_t Ax, Ay, Az, Gx, Gy, Gz, Ax1, Ay1, Az1, AxD, AyD, AzD, GxD, GyD, GzD;
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


void setup() {
  Wire.begin(sda, scl);
  Serial.begin(9600);
  delay(500);
  pinMode(alarm, OUTPUT);
  delay(500);
  delay(500);
  digitalWrite(alarm, HIGH);
  delay(500);
  digitalWrite(alarm, LOW);
  MPU6050_Init();
}



void loop() {
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
      String str = "";
      digitalWrite(alarm, HIGH);
      for (int i = 0; i < 30; i++) {
        Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
        Ax =  AccelX / AccelScaleFactor;
        Ay =  AccelY / AccelScaleFactor;
        Az =  AccelZ / AccelScaleFactor;
        Gx = GyroX / GyroScaleFactor;
        Gy = GyroY / GyroScaleFactor;
        Gz = GyroZ / GyroScaleFactor;
        str = "h," + String(Ax) + "," + String(Ay) + "," + String(Az) + "," + String(Gx) + "," + String(Gy) + "," + String(Gz) + ",";
        Serial.println(str);
        delay(100);
        str = "";
      }
      for (int i = 0; i <= 9; i++) {
        delay(1000);
      }
      digitalWrite(alarm, LOW);

    }
  }
}
