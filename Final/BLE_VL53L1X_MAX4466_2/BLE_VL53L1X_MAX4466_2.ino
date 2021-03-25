/*--------------------BLE---------------------*/
#include <ArduinoBLE.h>
BLEService sensorsService("19B10010-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service


// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic distanceCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify);
BLEByteCharacteristic microphoneCharacteristic("19B10014-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify);


/*--------------------distance sensor---------------------*/
#include <SparkFun_VL53L1X.h> // sensor library
int timingBudget = 33;
SFEVL53L1X sensor;
float distance = 0;
int previousDistance = 0;
float distanceThreshold = 30;
long previousMillis = 0;

/*--------------------microphone sensor---------------------*/
// int sensorValue = 0;
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
unsigned long startMillis; // Start of sample window
unsigned int peakToPeak; // peak-to-peak level
unsigned int signalMax;
unsigned int signalMin;
float microphone = 0;
float microphoneThreshold = 2;
float previousMicrophone = 0;

//----------------------------------------------------------------
void setup() {
  // start I2C library:
  Wire.begin();
  // initialize serial and wait for serial monitor to be opened:
  Serial.begin(9600);
  while (!Serial);

  /*--------------------distance sensor---------------------*/

  // sensor.begin works the reverse of many libraries: when it succeeds, it returns 0:
  if (sensor.begin() != 0) {
    Serial.println("VL53L1X not connected, please check your wiring.");
    while (true);
  }
  // set distance to 1.3m max distance:
  sensor.setDistanceModeShort();
  // set distance to 4m max distance:
  //  sensor.setDistanceModeLong();

  // set sensor reading timing budget in ms:
  sensor.setTimingBudgetInMs(timingBudget);
  // Intermeasurement period must be > or = timing budget.
  timingBudget = sensor.getTimingBudgetInMs();
  sensor.setIntermeasurementPeriod(timingBudget);

  /*--------------------BLE---------------------*/

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("SENSORS");
  BLE.setAdvertisedService(sensorsService);

  // add the characteristic to the service
  sensorsService.addCharacteristic(distanceCharacteristic);
  sensorsService.addCharacteristic(microphoneCharacteristic);

  // add service:
  BLE.addService(sensorsService);

  // set the initial value for the characteristic:
  distanceCharacteristic.writeValue(0);
  microphoneCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE SENSORS Peripheral");
}

//----------------------------------------------------------------
void loop() {
  /*--------------------BLE---------------------*/
  // poll for BLE events
  BLE.poll();

  // wait for a BLE central
  BLEDevice central = BLE.central();

  // if a central is connected:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    //    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(LED_BUILTIN, HIGH);

    // while the central is still connected to peripheral:
    while (central.connected()) {

      /*-------------distance sensor--------------*/
      //initiate measurement:
      sensor.startRanging();
      // See if the sensor has a reading:
      while (!sensor.checkForDataReady());
      // get the distance in mm:
      distance = sensor.getDistance();
      // clear the sensor's interrupt and turn off ranging:
      sensor.clearInterrupt();
      sensor.stopRanging();
      /*-------------microphone-----------------*/
      microphone = analogRead(0);

      /*------------update every second-----------*/
      long currentMillis = millis();
      //      if (currentMillis - previousMillis >= 1000) {
      //        previousMillis = currentMillis;
      updateDistance();
      updateMicrophone();
      //
      //        Serial.println(distance);
      //        Serial.println(microphone);
      //      }
      /*-----------------------------------------*/

      // use the value to control the LED:
      if (distanceCharacteristic.written()) {
        if (distanceCharacteristic.value()) {   // any value other than 0
          Serial.println("Dis Threshold changed");
          distanceThreshold = distanceCharacteristic.value();
          Serial.println(distanceThreshold);
        }
      }

      if (microphoneCharacteristic.written()) {
        if (microphoneCharacteristic.value()) {   // any value other than 0
          Serial.println("Mic Threshold changed");
          microphoneThreshold = microphoneCharacteristic.value();
          Serial.println(microphoneThreshold);
        }
      }
    }

    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    //    Serial.println(central.address());
  }
}

void updateDistance() {
  if (distance != previousDistance && distance != 0 && distance < 100 && abs(distance - previousDistance) > distanceThreshold) {
    //    Serial.println("distance" + distance);
    distanceCharacteristic.writeValue(distance);
    previousDistance = distance;
    Serial.println(distance);
  }
}

void updateMicrophone() {
  //  sensorValue = sensorValue - 200;

  startMillis = millis(); // Start of sample window
  peakToPeak = 0; // peak-to-peak level
  signalMax = 0;
  signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(0);
    if (sample < 1024) // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample; // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample; // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude
  microphone = ((peakToPeak * 5.0) / 1024) * 10; // convert to volts

  if (microphone != previousMicrophone && abs(microphone - previousMicrophone) > microphoneThreshold) {
    microphoneCharacteristic.writeValue(microphone);
    previousMicrophone = microphone;
    Serial.println(microphone);

  }
}
