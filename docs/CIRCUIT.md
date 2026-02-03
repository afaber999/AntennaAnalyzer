# Circuit Diagrams and Schematics

## RF Detector Circuit

### Forward Power Detector

```
RF Input (from DDS) 
    |
    +----[Directional Coupler]---- To Antenna
    |
    +---- Coupled Output
              |
              +----[1N5711 Diode]---+----[10kΩ]---- GND
                                    |
                                    +---- To ESP32 GPIO 34
                                    |
                                    +----[0.1µF Cap]---- GND
```

### Reflected Power Detector

```
Antenna Input
    |
    +----[Directional Coupler]---- To DDS
    |
    +---- Coupled Output
              |
              +----[1N5711 Diode]---+----[10kΩ]---- GND
                                    |
                                    +---- To ESP32 GPIO 35
                                    |
                                    +----[0.1µF Cap]---- GND
```

## Component Details

### Diode Detector
- **Diode**: 1N5711, BAT85, or similar Schottky diode
- **Load Resistor**: 10kΩ
- **Filter Capacitor**: 0.1µF ceramic
- **Coupling Capacitor**: 100pF ceramic (at RF input)

### Directional Coupler Options

#### Option 1: Simple Resistive Bridge
```
                    50Ω
DDS Output ----+----/\/\/\----+---- Antenna
               |               |
              [R]             [R]
               |               |
           To FWD Det      To REF Det
```
Where R = 470Ω to 1kΩ

#### Option 2: Transformer-Based Coupler
Use a small toroid (FT37-43 or similar) to create a directional coupler:
- Primary: 10-15 turns trifilar wound
- Provides better directivity (-20 dB to -30 dB coupling)

### Voltage Divider (if needed)
If detector voltage exceeds 3.3V, add voltage divider:
```
From Detector ----[10kΩ]----+---- To ESP32 ADC
                             |
                           [10kΩ]
                             |
                            GND
```
This provides 50% attenuation (2:1 divider).

## Power Supply

### ESP32 Board
- **Input**: 5V via USB or VIN pin
- **Onboard regulator**: Provides 3.3V to ESP32 and peripherals

### AD9850 Module
- **Power**: 5V recommended (some modules work with 3.3V)
- **Current**: ~150 mA typical
- **Decoupling**: 100µF electrolytic + 0.1µF ceramic near module

### OLED Display
- **Power**: 3.3V
- **Current**: ~20 mA typical
- **Decoupling**: 0.1µF ceramic capacitor

## PCB Layout Recommendations

1. **RF Section**:
   - Keep RF traces short and direct
   - Use ground plane for RF return
   - 50Ω impedance for RF traces (if using PCB)

2. **Digital Section**:
   - Separate from RF section
   - Use ground plane
   - Add bypass capacitors near IC power pins

3. **Power Distribution**:
   - Wide traces for power distribution
   - Decoupling capacitors at each module

4. **Grounding**:
   - Single point ground or star ground preferred
   - Keep digital and analog grounds separate if possible
   - Join at single point near power supply

## Testing and Calibration

### Test Equipment Needed
- 50Ω dummy load (known good)
- Short circuit (direct connection)
- Open circuit (no connection)
- Optional: Vector Network Analyzer (VNA) for comparison

### Calibration Procedure

1. **Connect 50Ω Load**:
   - Should read SWR close to 1.0:1 across all frequencies
   - If not, adjust detector circuit values

2. **Connect Short Circuit**:
   - Should read very high SWR (>10:1)
   - Confirms reflected power detection works

3. **Connect Open Circuit**:
   - Should read very high SWR (>10:1)
   - Confirms system can detect mismatch

4. **Adjust Software Calibration**:
   - Modify voltage scaling factors in code if needed
   - Adjust gamma calculation if necessary

## Bill of Materials (BOM)

### Electronic Components

| Component | Quantity | Description | Example Part Number |
|-----------|----------|-------------|-------------------|
| ESP32 DevKit | 1 | ESP32 development board | ESP32-DevKitC |
| AD9850 Module | 1 | DDS signal generator | AD9850 DDS Module |
| OLED Display | 1 | 128x64 I2C OLED | SSD1306 0.96" |
| Schottky Diodes | 2 | RF detector diodes | 1N5711, BAT85 |
| Resistors 10kΩ | 2 | Detector load resistors | 1/4W metal film |
| Resistors 470Ω-1kΩ | 2 | Bridge resistors | 1/4W metal film |
| Capacitors 0.1µF | 4 | Ceramic bypass caps | 50V ceramic |
| Capacitors 100pF | 2 | RF coupling caps | 50V ceramic |
| Capacitor 100µF | 1 | Power supply filter | 16V electrolytic |
| Push Buttons | 3 | Tactile switches | 6mm tactile |
| Connectors | 2+ | SMA or BNC connectors | For RF I/O |

### Mechanical Components

| Component | Quantity | Description |
|-----------|----------|-------------|
| Enclosure | 1 | Plastic project box |
| Standoffs | 4+ | M3 PCB standoffs |
| Screws | 8+ | M3 screws |
| Wire | - | 22 AWG hookup wire |
| RG58 Cable | 1 | 50Ω coax cable for connections |

### Optional Components

| Component | Description |
|-----------|-------------|
| BNC Connectors | Alternative to SMA |
| Heat Sinks | For AD9850 if gets hot |
| Ferrite Beads | For additional filtering |
| Toroid Core | For transformer-based coupler (FT37-43) |

## Assembly Notes

1. **Start with Power Supply**:
   - Verify 3.3V and 5V rails before connecting components
   - Check for shorts before applying power

2. **Build and Test Sections Separately**:
   - Test AD9850 signal generation first
   - Test OLED display with simple test sketch
   - Build and test RF detector circuit last

3. **RF Safety**:
   - Work with low power levels initially
   - Shield RF sections to prevent interference
   - Use proper RF connectors and cables

4. **Final Assembly**:
   - Mount all components in enclosure
   - Label controls clearly
   - Ensure good ventilation for AD9850

## Performance Specifications

### Typical Performance
- **Frequency Range**: 1-30 MHz
- **Frequency Resolution**: 1 Hz (AD9850 capability)
- **Frequency Step Size**: 100 kHz (adjustable in software)
- **SWR Range**: 1.0:1 to 10:1 (higher values displayed as "---")
- **Measurement Accuracy**: ±10% typical (with calibration)
- **Sweep Time**: ~30 seconds for full 1-30 MHz range (100 kHz steps)
- **Input Impedance**: 50Ω
- **Maximum Input Power**: <1W (depends on detector design)

### Limitations
- Accuracy depends on detector circuit quality
- Lower accuracy at band edges (1 MHz, 30 MHz)
- Requires calibration with known loads
- Not suitable for transmitter tuning under full power

## Next Steps

After building the hardware, follow these steps:

1. **Initial Testing**:
   - Connect power and verify ESP32 boots
   - Check OLED display shows startup message
   - Test buttons for responsiveness

2. **DDS Signal Verification**:
   - Connect oscilloscope or frequency counter to AD9850 output
   - Verify signal generation at various frequencies
   - Check signal quality and stability

3. **Detector Calibration**:
   - Connect 50Ω dummy load
   - Adjust detector circuit for proper voltage levels
   - Verify SWR reading near 1.0:1

4. **Real-World Testing**:
   - Connect to antenna under test
   - Perform frequency sweeps
   - Compare results with known antenna specifications

5. **Future Enhancements**:
   - Add Smith chart display
   - Implement impedance measurement (R + jX)
   - Add data logging to SD card
   - Bluetooth connectivity for remote monitoring
   - Battery power option for portable operation
