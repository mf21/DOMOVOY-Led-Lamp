const int pinButton = 5;
const int pinLed = 3;  // external led strip lights

int pinLedState = LOW;
int pinButtonState;
int lastButtonState = HIGH;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

bool isButtonPressed = false; 
unsigned long holdDelay = 2000;
unsigned long holdLastTime = 0; 

bool timerIsActivated = false;
unsigned long timerDelay = 30 * 60000; // power off time
unsigned long timerTime = 0;

void setup() {

  pinMode(pinButton, INPUT);
  pinMode(pinLed, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(pinLed, pinLedState);
  digitalWrite(LED_BUILTIN, pinLedState);  

  Serial.begin(9600);
  Serial.println("Initialization done");
}

void ledSignal() {

  Serial.println("Timer activated");
  timerIsActivated = true;
  timerTime = millis();

  digitalWrite(pinLed, HIGH);
  delay(1000);
  digitalWrite(pinLed, LOW);
  delay(400);
  digitalWrite(pinLed, HIGH);
  delay(1000);
  digitalWrite(pinLed, LOW);
  delay(400);
  digitalWrite(pinLed, HIGH);
  delay(1000);
  digitalWrite(pinLed, LOW);
  delay(400);
  digitalWrite(pinLed, HIGH);
  pinLedState = HIGH;
  
}

void btnLoop() {
//  Serial.println("btnLoop");

  if (!isButtonPressed) { 
    isButtonPressed = true;
    holdLastTime = millis();
    Serial.println("btnLoop activated");
  }

  if (isButtonPressed) {
    if (millis() - holdLastTime > holdDelay) {
      holdLastTime = 0;
      isButtonPressed = false;
      ledSignal();
    }
  }
  
}

void timerCheck() {

  Serial.println("Timer check");

  if (millis() - timerTime > timerDelay) {
    Serial.println("Timer completed");
    timerTime = 0;
    timerIsActivated = false;
    pinLedState = LOW;
  }
}

void loop() {
  
  int reading = digitalRead(pinButton);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if ((pinButtonState == LOW) and isButtonPressed) {
      btnLoop();
    }

    if (reading != pinButtonState) {
      pinButtonState = reading;     

      if (pinButtonState == LOW) {
        Serial.println("Button pressed");
        btnLoop();
      } else {
        if (isButtonPressed) {
          holdLastTime = 0;
          isButtonPressed = false;
          pinLedState = !pinLedState;
          Serial.print("Led state: ");
          Serial.println(pinLedState);
        } 
      }
    
    }
 
  }

  lastButtonState = reading;
//===========================================================

  if (timerIsActivated) timerCheck();
  digitalWrite(pinLed, pinLedState);

}
