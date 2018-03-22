#include <SparkFunMPU9250-DMP.h>

#define SerialPort SerialUSB

MPU9250_DMP imu;

#include <WS2811.h>
#define LED_DDR  DDRB
#define LED_PORT PORTB
#define LED_PIN0 6
#define LED_PIN1 5
#define LED_PIN2 4

//DEFINE_WS2811_OUT_1_FN(ws2811_send_01, LED_PORT, LED_PIN0)
DEFINE_WS2811_OUT_3_COMMON_FN(ws2811_send3, LED_PORT, LED_PIN0, LED_PIN1, LED_PIN2)

void setup() {
  SerialPort.begin(115200);

  // Call imu.begin() to verify communication with and
  // initialize the MPU-9250 to it's default values.
  // Most functions return an error code - INV_SUCCESS (0)
  // indicates the IMU was present and successfully set up
  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      SerialPort.println("Unable to communicate with MPU-9250");
      SerialPort.println("Check connections, and try again.");
      SerialPort.println();
      delay(5000);
    }
  }

  // Use setSensors to turn on or off MPU-9250 sensors.
  // Any of the following defines can be combined:
  // INV_XYZ_GYRO, INV_XYZ_ACCEL, INV_XYZ_COMPASS,
  // INV_X_GYRO, INV_Y_GYRO, or INV_Z_GYRO
  // Enable all sensors:
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);

  // Use setGyroFSR() and setAccelFSR() to configure the
  // gyroscope and accelerometer full scale ranges.
  // Gyro options are +/- 250, 500, 1000, or 2000 dps
  imu.setGyroFSR(2000); // Set gyro to 2000 dps
  // Accel options are +/- 2, 4, 8, or 16 g
  imu.setAccelFSR(2); // Set accel to +/-2g
  // Note: the MPU-9250's magnetometer FSR is set at 
  // +/- 4912 uT (micro-tesla's)

  // setLPF() can be used to set the digital low-pass filter
  // of the accelerometer and gyroscope.
  // Can be any of the following: 188, 98, 42, 20, 10, 5
  // (values are in Hz).
  imu.setLPF(5); // Set LPF corner frequency to 5Hz

  // The sample rate of the accel/gyro can be set using
  // setSampleRate. Acceptable values range from 4Hz to 1kHz
  imu.setSampleRate(10); // Set sample rate to 10Hz

  // Likewise, the compass (magnetometer) sample rate can be
  // set using the setCompassSampleRate() function.
  // This value can range between: 1-100Hz
  imu.setCompassSampleRate(10); // Set mag rate to 10Hz

  
  LED_DDR |= 1 << LED_PIN0;
  LED_DDR |= 1 << LED_PIN1;
  LED_DDR |= 1 << LED_PIN2;
  LED_PORT &= ~(1 << LED_PIN0);
  LED_PORT &= ~(1 << LED_PIN1);
  LED_PORT &= ~(1 << LED_PIN2);

}
static int led_cnt=90;
void prog_set_leds1(RGB_t *rgb)
{
  static char rinc=1;
  static char ginc=1;
  static char binc=1;
for (uint8_t i = 0; i < led_cnt; i ++) {
    rgb[i].r += rinc;
    rgb[i].g += ginc;
    rgb[i].b += binc;
    if(rgb[i].r>88) rinc = -1 ;
    if(rgb[i].g >60) ginc=-1;
    if(rgb[i].b >130) binc=-1;
    if(rgb[i].r==0) rinc = 1 ;
    if(rgb[i].g==0) ginc=1;
    if(rgb[i].b==0) binc=1;
  }  
}

void prog_set_leds(RGB_t *rgb)
{
for (uint8_t i = 0; i < led_cnt; i ++) {
  if ( imu.dataReady() )
  {
    // Call update() to update the imu objects sensor data.
    // You can specify which sensors to update by combining
    // UPDATE_ACCEL, UPDATE_GYRO, UPDATE_COMPASS, and/or
    // UPDATE_TEMPERATURE.
    // (The update function defaults to accel, gyro, compass,
    //  so you don't have to specify these values.)
    imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
    //printIMUData();
  }
  SerialPort.println("Accel: " + String(imu.ax) + ", " +
              String(imu.ay) + ", " + String(imu.az) + " ");

    if (i<30)
    {
      rgb[i].r = 128-imu.ax>>9;
      rgb[i].g = 128-imu.ay>>9;
      rgb[i].b = 128-imu.az>>9;
    } else if (i<60)
    {
      rgb[i].r = 128-imu.mx>>9;
      rgb[i].g = 128-imu.my>>9;
      rgb[i].b = 128-imu.mz>>9;
    }else
    {
      rgb[i].r = 128-imu.gx>>8;
      rgb[i].g = 128-imu.gy>>9;
      rgb[i].b = 128-imu.gz>>3;
    }
  }  
}

void printIMUData(void)
{  
  // After calling update() the ax, ay, az, gx, gy, gz, mx,
  // my, mz, time, and/or temerature class variables are all
  // updated. Access them by placing the object. in front:

  // Use the calcAccel, calcGyro, and calcMag functions to
  // convert the raw sensor readings (signed 16-bit values)
  // to their respective units.
  float accelX = imu.calcAccel(imu.ax);
  float accelY = imu.calcAccel(imu.ay);
  float accelZ = imu.calcAccel(imu.az);
  float gyroX = imu.calcGyro(imu.gx);
  float gyroY = imu.calcGyro(imu.gy);
  float gyroZ = imu.calcGyro(imu.gz);
  float magX = imu.calcMag(imu.mx);
  float magY = imu.calcMag(imu.my);
  float magZ = imu.calcMag(imu.mz);
  
  SerialPort.println("Accel: " + String(accelX) + ", " +
              String(accelY) + ", " + String(accelZ) + " g");
  SerialPort.println("Gyro: " + String(gyroX) + ", " +
              String(gyroY) + ", " + String(gyroZ) + " dps");
  SerialPort.println("Mag: " + String(magX) + ", " +
              String(magY) + ", " + String(magZ) + " uT");
  SerialPort.println("Time: " + String(imu.time) + " ms");
  SerialPort.println();
}

  
  
  RGB_t ledsrgb[128];

void loop() {
  prog_set_leds(ledsrgb);
  //ws2811_send_01(ledsrgb, led_cnt);
  ws2811_send3(ledsrgb + 0, ledsrgb + 30, ledsrgb + 60, 30);
}
