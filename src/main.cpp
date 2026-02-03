/**
 * ESP32 HF Antenna Analyzer
 * 
 * Hardware connections:
 * - AD9850 DDS Module:
 *   - W_CLK (Word Load Clock) -> GPIO 18
 *   - FQ_UD (Frequency Update) -> GPIO 19
 *   - DATA -> GPIO 23
 *   - RESET -> GPIO 5
 * 
 * - RF Detector (voltage divider + diode detector):
 *   - Forward Power -> GPIO 34 (ADC1_CH6)
 *   - Reflected Power -> GPIO 35 (ADC1_CH7)
 * 
 * - OLED Display (I2C):
 *   - SDA -> GPIO 21
 *   - SCL -> GPIO 22
 * 
 * - Control Buttons:
 *   - UP -> GPIO 32
 *   - DOWN -> GPIO 33
 *   - SELECT -> GPIO 25
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// AD9850 DDS pins
#define W_CLK_PIN 18
#define FQ_UD_PIN 19
#define DATA_PIN 23
#define RESET_PIN 5

// ADC pins for RF detection
#define FWD_POWER_PIN 34
#define REF_POWER_PIN 35

// Button pins
#define BTN_UP 32
#define BTN_DOWN 33
#define BTN_SELECT 25

// Frequency settings
#define MIN_FREQ 1000000UL      // 1 MHz
#define MAX_FREQ 30000000UL     // 30 MHz
#define FREQ_STEP 100000UL      // 100 kHz

// AD9850 DDS constants
#define DDS_TUNING_WORD_MAX 4294967296ULL  // 2^32 for frequency calculation
#define DDS_CLOCK_FREQ 125000000UL         // 125 MHz reference clock

// UI constants
#define BUTTON_DEBOUNCE_MS 200  // Button debounce delay in milliseconds

// Current state
unsigned long currentFreq = 7000000UL; // Start at 7 MHz
bool sweepMode = false;
unsigned long sweepStart = 1000000UL;
unsigned long sweepEnd = 30000000UL;
unsigned long sweepStep = 100000UL;

// SWR data
float currentSWR = 1.0;
float minSWR = 999.0;
unsigned long minSWRFreq = 0;

// Function prototypes
void initAD9850();
void setFrequency(unsigned long freq);
void sendBit(uint8_t bit);
void updateDisplay();
void measureSWR();
float calculateSWR(int fwdReading, int refReading);
void performSweep();
void handleButtons();

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 HF Antenna Analyzer");
  
  // Initialize AD9850 pins
  pinMode(W_CLK_PIN, OUTPUT);
  pinMode(FQ_UD_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);
  
  // Initialize button pins
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  
  // Initialize ADC
  analogReadResolution(12); // 12-bit ADC resolution
  analogSetAttenuation(ADC_11db); // Full scale voltage ~3.3V
  
  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    // Continue anyway, just no display
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("ESP32 HF Antenna"));
  display.println(F("Analyzer v1.0"));
  display.display();
  delay(2000);
  
  // Initialize AD9850
  initAD9850();
  
  // Set initial frequency
  setFrequency(currentFreq);
  
  Serial.println("Initialization complete");
}

void loop() {
  handleButtons();
  
  if (sweepMode) {
    performSweep();
    sweepMode = false; // Complete sweep
  } else {
    measureSWR();
    updateDisplay();
    delay(100);
  }
}

void initAD9850() {
  // Reset AD9850
  digitalWrite(RESET_PIN, HIGH);
  delay(1);
  digitalWrite(RESET_PIN, LOW);
  
  // Initialize with frequency 0
  digitalWrite(W_CLK_PIN, LOW);
  digitalWrite(FQ_UD_PIN, LOW);
  
  // Send 40 bits of zeros to initialize
  for (int i = 0; i < 40; i++) {
    sendBit(0);
  }
  
  // Pulse FQ_UD to update
  digitalWrite(FQ_UD_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(FQ_UD_PIN, LOW);
}

void setFrequency(unsigned long freq) {
  // Calculate frequency word for AD9850
  // freq_word = (target_freq * 2^32) / ref_clock
  unsigned long freqWord = ((unsigned long long)freq * DDS_TUNING_WORD_MAX) / DDS_CLOCK_FREQ;
  
  // Send 32 bits of frequency word
  for (int i = 0; i < 32; i++) {
    sendBit(freqWord & 0x01);
    freqWord >>= 1;
  }
  
  // Send 8 control bits (all zeros for default operation)
  for (int i = 0; i < 8; i++) {
    sendBit(0);
  }
  
  // Pulse FQ_UD to update frequency
  digitalWrite(FQ_UD_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(FQ_UD_PIN, LOW);
}

void sendBit(uint8_t bit) {
  digitalWrite(DATA_PIN, bit);
  
  // Pulse W_CLK
  digitalWrite(W_CLK_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(W_CLK_PIN, LOW);
}

void measureSWR() {
  // Read forward and reflected power
  int fwdReading = analogRead(FWD_POWER_PIN);
  int refReading = analogRead(REF_POWER_PIN);
  
  // Calculate SWR
  currentSWR = calculateSWR(fwdReading, refReading);
  
  // Print to serial
  Serial.print("Freq: ");
  Serial.print(currentFreq / 1000000.0, 3);
  Serial.print(" MHz, FWD: ");
  Serial.print(fwdReading);
  Serial.print(", REF: ");
  Serial.print(refReading);
  Serial.print(", SWR: ");
  Serial.println(currentSWR, 2);
}

float calculateSWR(int fwdReading, int refReading) {
  // Convert ADC readings to voltages
  float fwdVoltage = fwdReading * (3.3 / 4095.0);
  float refVoltage = refReading * (3.3 / 4095.0);
  
  // Avoid division by zero
  if (fwdVoltage < 0.01) {
    return 999.0; // No signal
  }
  
  // Calculate reflection coefficient
  float gamma = refVoltage / fwdVoltage;
  
  // Limit gamma to valid range
  if (gamma >= 1.0) {
    return 999.0;
  }
  
  // Calculate SWR = (1 + gamma) / (1 - gamma)
  float swr = (1.0 + gamma) / (1.0 - gamma);
  
  // Limit SWR to reasonable range
  if (swr < 1.0) swr = 1.0;
  if (swr > 999.0) swr = 999.0;
  
  return swr;
}

void performSweep() {
  minSWR = 999.0;
  minSWRFreq = 0;
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Sweeping..."));
  display.display();
  
  Serial.println("Starting frequency sweep...");
  
  for (unsigned long freq = sweepStart; freq <= sweepEnd; freq += sweepStep) {
    setFrequency(freq);
    delay(10); // Allow frequency to settle
    
    // Measure SWR at this frequency
    int fwdReading = analogRead(FWD_POWER_PIN);
    int refReading = analogRead(REF_POWER_PIN);
    float swr = calculateSWR(fwdReading, refReading);
    
    // Track minimum SWR
    if (swr < minSWR) {
      minSWR = swr;
      minSWRFreq = freq;
    }
    
    Serial.print(freq / 1000000.0, 3);
    Serial.print(" MHz: SWR = ");
    Serial.println(swr, 2);
  }
  
  // Return to original frequency or best frequency
  currentFreq = minSWRFreq;
  setFrequency(currentFreq);
  
  Serial.println("Sweep complete!");
  Serial.print("Best SWR: ");
  Serial.print(minSWR, 2);
  Serial.print(" at ");
  Serial.print(minSWRFreq / 1000000.0, 3);
  Serial.println(" MHz");
  
  // Show results
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Sweep Complete!"));
  display.print(F("Best: "));
  display.print(minSWR, 2);
  display.print(F(" @ "));
  display.print(minSWRFreq / 1000000.0, 3);
  display.println(F(" MHz"));
  display.display();
  delay(3000);
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  
  // Display frequency
  display.print(F("Freq: "));
  display.print(currentFreq / 1000000.0, 3);
  display.println(F(" MHz"));
  
  // Display SWR
  display.println();
  display.setTextSize(2);
  display.print(F("SWR: "));
  if (currentSWR < 99.0) {
    display.println(currentSWR, 2);
  } else {
    display.println(F("---"));
  }
  
  // Display instructions
  display.setTextSize(1);
  display.println();
  display.println(F("UP/DN: Tune"));
  display.println(F("SEL: Sweep"));
  
  display.display();
}

void handleButtons() {
  static unsigned long lastButtonPress = 0;
  unsigned long now = millis();
  
  // Debounce delay
  if (now - lastButtonPress < BUTTON_DEBOUNCE_MS) {
    return;
  }
  
  // UP button - increase frequency
  if (digitalRead(BTN_UP) == LOW) {
    currentFreq += FREQ_STEP;
    if (currentFreq > MAX_FREQ) {
      currentFreq = MAX_FREQ;
    }
    setFrequency(currentFreq);
    lastButtonPress = now;
    Serial.print("Frequency: ");
    Serial.print(currentFreq / 1000000.0, 3);
    Serial.println(" MHz");
  }
  
  // DOWN button - decrease frequency
  if (digitalRead(BTN_DOWN) == LOW) {
    if (currentFreq > MIN_FREQ + FREQ_STEP) {
      currentFreq -= FREQ_STEP;
    } else {
      currentFreq = MIN_FREQ;
    }
    setFrequency(currentFreq);
    lastButtonPress = now;
    Serial.print("Frequency: ");
    Serial.print(currentFreq / 1000000.0, 3);
    Serial.println(" MHz");
  }
  
  // SELECT button - start sweep
  if (digitalRead(BTN_SELECT) == LOW) {
    sweepMode = true;
    lastButtonPress = now;
    Serial.println("Starting sweep mode...");
  }
}
