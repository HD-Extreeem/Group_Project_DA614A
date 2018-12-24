#include <Wire.h>

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
