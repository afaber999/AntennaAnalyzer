# ESP32 HF Antenna Analyzer

An ESP32-based HF (High Frequency) Antenna Analyzer for measuring antenna characteristics in the 1-30 MHz range.

## Features

- **Frequency Range**: 1 MHz to 30 MHz (HF band)
- **SWR Measurement**: Real-time Standing Wave Ratio measurement
- **Frequency Sweep**: Automatic sweep to find antenna resonance
- **OLED Display**: 128x64 OLED display for easy reading
- **User Controls**: Three buttons for frequency tuning and sweep control
- **Serial Output**: Detailed measurements via USB serial connection

## Hardware Requirements

### Main Components
- **ESP32 Development Board** (ESP32-DevKitC or similar)
- **AD9850 DDS Module** - Direct Digital Synthesis signal generator (125 MHz clock)
- **RF Detector Circuit** - For measuring forward and reflected power
- **SSD1306 OLED Display** - 128x64 I2C display
- **Push Buttons** - 3 tactile buttons for user interface

### RF Detector Circuit
The RF detector uses diode detectors to convert RF power to DC voltage:
- Schottky diodes (1N5711 or BAT85) for forward and reflected power detection
- Voltage dividers to scale signals to ESP32 ADC range (0-3.3V)
- RF directional coupler or bridge to separate forward/reflected power

### Pin Connections

#### AD9850 DDS Module
| AD9850 Pin | ESP32 GPIO | Description |
|------------|------------|-------------|
| W_CLK      | GPIO 18    | Word Load Clock |
| FQ_UD      | GPIO 19    | Frequency Update |
| DATA       | GPIO 23    | Serial Data |
| RESET      | GPIO 5     | Reset |
| VCC        | 3.3V/5V    | Power Supply |
| GND        | GND        | Ground |

#### RF Detector
| Signal | ESP32 GPIO | Description |
|--------|------------|-------------|
| Forward Power  | GPIO 34 (ADC1_CH6) | Forward power detection |
| Reflected Power | GPIO 35 (ADC1_CH7) | Reflected power detection |

#### OLED Display (I2C)
| OLED Pin | ESP32 GPIO | Description |
|----------|------------|-------------|
| SDA      | GPIO 21    | I2C Data |
| SCL      | GPIO 22    | I2C Clock |
| VCC      | 3.3V       | Power Supply |
| GND      | GND        | Ground |

#### Control Buttons
| Button | ESP32 GPIO | Function |
|--------|------------|----------|
| UP     | GPIO 32    | Increase frequency |
| DOWN   | GPIO 33    | Decrease frequency |
| SELECT | GPIO 25    | Start frequency sweep |

All buttons use internal pull-up resistors (active LOW).

## Software Setup

### Prerequisites
- [PlatformIO](https://platformio.org/) - Recommended development environment
- OR [Arduino IDE](https://www.arduino.cc/en/software) with ESP32 board support

### Installation

1. Clone this repository:
```bash
git clone https://github.com/afaber999/AntennaAnalyzer.git
cd AntennaAnalyzer
```

2. Using PlatformIO:
```bash
pio lib install
pio run --target upload
```

3. Using Arduino IDE:
   - Install ESP32 board support via Board Manager
   - Install required libraries via Library Manager:
     - Adafruit SSD1306
     - Adafruit GFX Library
     - Adafruit BusIO
   - Open `src/main.cpp` and upload to your ESP32

### Library Dependencies
- **Adafruit SSD1306** (v2.5.7+) - OLED display driver
- **Adafruit GFX Library** (v1.11.3+) - Graphics primitives
- **Adafruit BusIO** (v1.14.1+) - I2C communication

## Usage

### Basic Operation

1. **Power On**: The analyzer starts at 7.0 MHz by default
2. **View SWR**: The display shows current frequency and SWR measurement
3. **Tune Frequency**:
   - Press **UP** button to increase frequency by 100 kHz
   - Press **DOWN** button to decrease frequency by 100 kHz
4. **Frequency Sweep**:
   - Press **SELECT** button to start automatic sweep from 1-30 MHz
   - The analyzer will find and display the frequency with best (lowest) SWR
   - Results are shown on display and serial monitor

### Serial Monitor

Connect via USB serial at 115200 baud to see detailed measurements:
- Real-time frequency, forward/reflected power readings, and SWR
- Sweep progress and results
- Diagnostic information

Example output:
```
Freq: 7.000 MHz, FWD: 2048, REF: 512, SWR: 1.50
Freq: 7.100 MHz, FWD: 2048, REF: 410, SWR: 1.40
Starting frequency sweep...
7.000 MHz: SWR = 1.50
7.100 MHz: SWR = 1.40
...
Sweep complete!
Best SWR: 1.05 at 7.150 MHz
```

## Understanding SWR

**Standing Wave Ratio (SWR)** indicates how well an antenna is matched to the transmission line:
- **SWR 1.0:1** - Perfect match (ideal, rarely achieved)
- **SWR 1.5:1 or less** - Excellent match
- **SWR 2.0:1 or less** - Acceptable for most applications
- **SWR 3.0:1 or higher** - Poor match, antenna adjustment recommended

## Calibration

For accurate measurements, calibration is recommended:

1. **Open Circuit**: Disconnect antenna, measure (should show high SWR)
2. **Short Circuit**: Connect 50Ω load, measure (should show SWR near 1.0)
3. **Adjust RF detector resistor values** if needed to scale readings properly

## Troubleshooting

### No Display
- Check I2C connections (SDA/SCL)
- Verify display I2C address (default 0x3C)
- Check power supply to display

### No Signal Generation
- Verify AD9850 connections
- Check AD9850 power supply
- Measure AD9850 output with oscilloscope

### SWR Always Shows 999
- Check RF detector connections
- Verify detector circuit is receiving RF signal
- Check ADC pin connections

### Inaccurate Readings
- Calibrate with known 50Ω load
- Check RF detector diodes (should be Schottky type)
- Verify voltage divider ratios in detector circuit

## Theory of Operation

### Signal Generation
The AD9850 DDS generates a precise, tunable RF signal from 1-30 MHz using Direct Digital Synthesis. The ESP32 programs the DDS via serial interface to set the desired frequency.

### Power Measurement
A directional coupler or RF bridge separates forward and reflected power. Diode detectors convert RF power to DC voltage, which the ESP32 ADC measures.

### SWR Calculation
SWR is calculated from the reflection coefficient (Γ):
```
Γ = Vreflected / Vforward
SWR = (1 + Γ) / (1 - Γ)
```

## Safety Notes

⚠️ **Important Safety Information**:
- Use appropriate RF shielding
- Keep RF power levels low during testing
- Never connect to live transmitter
- Use in well-ventilated area
- Follow local RF exposure guidelines

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Acknowledgments

- Based on common HF antenna analyzer designs
- Uses Adafruit libraries for display support
- ESP32 platform by Espressif Systems

## References

- [AD9850 Datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/AD9850.pdf)
- [SWR Measurement Theory](https://en.wikipedia.org/wiki/Standing_wave_ratio)
- [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)

## Version History

- **v1.0** - Initial release with basic SWR measurement and frequency sweep
