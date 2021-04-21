
#define CURRENT_SENSOR_PIN A0 // Current sensor is connected to analog pin A0
#define RELAY_PIN 2 // Relay module is connected to digital pin 2
#define RELAY_LOGIC LOW //Set to LOW if your relay is activated by a ground signal. Set to HIGH if your relay is activated by a 5V signal. If your relay is active at bootup, and turns off when CURRENT_THRESHOLD is met, then try changing this setting to the opposite.  
#define CURRENT_THRESHOLD 520 // The analog value above which the relay shall be triggered
#define CURRENT_SAMPLE_PERIOD 500 // The number of milliseconds to sample the current reading
#define RELAY_STARTUP_DELAY 2000 // The number of milliseconds to delay before activating relay after tool has started
#define RELAY_SHUTOFF_DELAY 5000 // The number of milliseconds to delay shutting off the relay after the tool has been shutoff

int analogValue = 0; // Stores ADC values read in from the current sensor
unsigned long stopwatch = 0; // Used to keep track of elapsed time
boolean relayPinState = HIGH; // Stores the current state of the relay pin

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN,OUTPUT);
}

void loop() {
  analogValue = 0; // Initialize analogValue to a known state
  stopwatch = millis(); // Store a snapshot of the current time since the program started executing

  // Collect the max ADC value from the current sensor for the predetermined sample period
  while(millis() - stopwatch < CURRENT_SAMPLE_PERIOD) { 
    analogValue = max(analogValue,analogRead(CURRENT_SENSOR_PIN));
    Serial.println(analogValue);
  }

  // If the max ADC value from the current sensor exceeds the threshold, activate the relay
  if(analogValue > CURRENT_THRESHOLD) {
    delay(RELAY_STARTUP_DELAY);
    relayPinState = RELAY_LOGIC;
  }
  // Otherwise if the relay was just activated, wait for the predetermined shutoff delay and set the state to HIGH
  else {
    if(relayPinState == RELAY_LOGIC) {
      delay(RELAY_SHUTOFF_DELAY);
      relayPinState = !relayPinState;
    }
  }
  digitalWrite(RELAY_PIN,relayPinState); // Write the RELAY_PIN to the correct state
}
