#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd (rs, en, d4, d5, d6, d7);

int laserOutput = 9;
int resistorInputA = A4;
int resistorInputB = A5;
int piezoOutput = 10;
int flashOutput = 9;
int potInput = A2;

int distance = 0.2; //meters, adjust to real distance
unsigned long time1 = 0;
unsigned long time2 = 0;

bool triggered = false;
bool measuring = false;

float measuredSpeed = 0;
float maxSpeed = 0;
float lastMaxSpeed = -1;

int threshold = 500; //adjust this

void setup() {
    pinMode(laserOutput, OUTPUT);
    pinMode(piezoOutput, OUTPUT);
    pinMode(flashOutput, OUTPUT);
    pinMode(resistorInputA, INPUT);
    pinMode(resistorInputB, INPUT);

    digitalWrite(laserOutput, HIGH);

    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.print("Hello World!");
}

void displaySpeed() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Speed: ");
    
    lcd.setCursor(0, 1);
    lcd.print(measuredSpeed);
    lcd.print(" m/s");
}

void checkSpeedLimit() {
    if (measuredSpeed > maxSpeed) {
        for (int i = 0; i < 5; i++) {
            digitalWrite(flashOutput, HIGH);
            tone(piezoOutput, 1000);
            delay(200);
            
            digitalWrite(flashOutput, LOW);
            noTone(piezoOutput);
            delay(200); 
        }
    }
}

void displayMaxSpeed() {
    if(abs(lastMaxSpeed - maxSpeed) > 1) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Max Speed: ");
        
        lcd.setCursor(0, 1);
        lcd.print(maxSpeed);
        lcd.print(" m/s");
        delay(1000);
        
        lastMaxSpeed = maxSpeed;
    }
}

void loop() {
    int resValA = analogRead(resistorInputA);
    int resValB = analogRead(resistorInputB);
    int potVal = analogRead(potInput);
    maxSpeed = map(potVal, 0, 1023, 1, 50);
    
    Serial.println(resValA);

    if (!triggered && resValA > threshold) {
        time1 = micros();
        triggered = true;
        measuring = true;
    }
    
    if (measuring && resValB > threshold) {
        time2 = micros();
        float timeSec = (time2 - time1) / 1000000.0;
        measuredSpeed = distance/timeSec;
        
        measuring = false;
        triggered = false;

        displaySpeed();
        checkSpeedLimit();
    }
    
}
