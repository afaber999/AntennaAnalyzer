# Frequently Asked Questions (FAQ)

## General Questions

### Q: What is an antenna analyzer used for?
**A:** An antenna analyzer measures how well an antenna is matched to your transmission line. It helps you:
- Find the antenna's resonant frequency
- Measure SWR (Standing Wave Ratio)
- Determine if antenna adjustments are needed
- Verify antenna performance across a frequency range

### Q: What frequency range does this analyzer cover?
**A:** This analyzer covers 1-30 MHz (the HF amateur radio bands), including:
- 160m, 80m, 60m, 40m, 30m, 20m, 17m, 15m, 12m, and 10m bands
- Can be modified in software for different ranges

### Q: How accurate is this analyzer?
**A:** Typical accuracy is ±10% with proper calibration. Accuracy depends on:
- Quality of RF detector circuit
- Calibration with known loads
- Frequency being measured
- Quality of connections

### Q: Can I use this while transmitting?
**A:** **NO!** This analyzer generates a very low-power test signal and should **never** be connected to a transmitter that is active. Always disconnect before transmitting.

## Hardware Questions

### Q: What ESP32 board should I use?
**A:** Any standard ESP32 development board works, such as:
- ESP32-DevKitC
- ESP32-WROOM-32
- ESP32-WROVER
- NodeMCU-32S

### Q: Where can I buy an AD9850 module?
**A:** AD9850 DDS modules are available from:
- eBay (search "AD9850 DDS module")
- Amazon
- AliExpress
- Electronic component distributors

Cost: Typically $5-15 USD

### Q: Can I use AD9851 instead of AD9850?
**A:** Yes, but you'll need to modify the code slightly as AD9851 has a different clock frequency (180 MHz vs 125 MHz). Change the `DDS_CLOCK_FREQ` constant.

### Q: What OLED display should I use?
**A:** Use a 128x64 I2C OLED display with SSD1306 controller:
- 0.96" or 1.3" size works well
- Must be I2C interface (not SPI)
- 3.3V or 5V compatible
- I2C address typically 0x3C or 0x3D

### Q: Do I need to build the RF detector circuit?
**A:** Yes, the RF detector circuit is essential for measuring forward and reflected power. See the CIRCUIT.md document for schematics.

### Q: What is a directional coupler and do I need one?
**A:** A directional coupler separates forward and reflected RF power. Options:
- Simple resistive bridge (easiest, less accurate)
- Transformer-based coupler (better performance)
- Commercial directional coupler (best, but expensive)

See CIRCUIT.md for DIY designs.

## Software Questions

### Q: Can I use Arduino IDE instead of PlatformIO?
**A:** Yes, but you'll need to:
1. Install ESP32 board support
2. Manually install required libraries (Adafruit SSD1306, GFX, BusIO)
3. Open src/main.cpp as a .ino file

### Q: What libraries are required?
**A:** 
- Adafruit SSD1306 (v2.5.7 or later)
- Adafruit GFX Library (v1.11.3 or later)
- Adafruit BusIO (v1.14.1 or later)

All are available through Arduino Library Manager or PlatformIO.

### Q: How do I change the frequency range?
**A:** Edit these constants in src/main.cpp:
```cpp
#define MIN_FREQ 1000000UL      // Minimum frequency
#define MAX_FREQ 30000000UL     // Maximum frequency
#define FREQ_STEP 100000UL      // Step size
```

### Q: Can I change the sweep step size?
**A:** Yes, modify the `sweepStep` variable or `FREQ_STEP` constant for different resolution. Smaller steps = more accurate but slower sweep.

## Usage Questions

### Q: What is a good SWR value?
**A:**
- **1.0:1** - Perfect (theoretical ideal)
- **1.5:1 or less** - Excellent
- **2.0:1 or less** - Good (acceptable for most uses)
- **3.0:1** - Poor (adjustment recommended)
- **Above 3.0:1** - Needs attention

### Q: How do I calibrate the analyzer?
**A:** 
1. Connect a 50Ω dummy load (known good)
2. Check SWR reading at various frequencies
3. Should read close to 1.0:1 across the range
4. If not, adjust detector circuit values
5. See CIRCUIT.md for calibration details

### Q: The display shows "SWR: ---", what does that mean?
**A:** This indicates:
- No RF signal detected
- Forward power too low
- Possible hardware issue
- Check connections and AD9850 operation

### Q: How long does a sweep take?
**A:** With default settings (100 kHz steps, 1-30 MHz):
- Full sweep: ~30 seconds
- Faster with larger step size
- Slower with smaller step size

### Q: Can I save sweep results?
**A:** Not in the current version. Future enhancements could add:
- SD card logging
- Bluetooth data transfer
- WiFi upload to computer

## Troubleshooting

### Q: Display doesn't work
**A:** Check:
- I2C connections (SDA to GPIO 21, SCL to GPIO 22)
- Display power (3.3V)
- I2C address (try 0x3C or 0x3D)
- Serial monitor for error messages

### Q: No signal from AD9850
**A:** Check:
- AD9850 power supply
- Serial connections (W_CLK, FQ_UD, DATA, RESET)
- Test with oscilloscope or frequency counter
- Verify AD9850 module is working (some come DOA)

### Q: SWR readings seem incorrect
**A:** 
- Calibrate with 50Ω dummy load
- Check RF detector circuit
- Verify diode orientation
- Check ADC connections
- Ensure good RF connections

### Q: Buttons don't respond
**A:** Check:
- Button connections to correct GPIO pins
- Buttons are wired active-LOW (to ground)
- Internal pull-ups are enabled (default in code)
- Debounce delay (200ms) - wait between presses

### Q: Readings are unstable/jumping around
**A:** 
- Add more filtering capacitors
- Improve grounding
- Shield RF section
- Check for loose connections
- Move away from interference sources

## Advanced Questions

### Q: Can I add more features?
**A:** Yes! See CONTRIBUTING.md for ideas:
- Smith chart display
- Impedance measurement (R + jX)
- Data logging
- Web interface
- Battery operation

### Q: Can I use a different display?
**A:** Yes, you can adapt the code for:
- Larger OLEDs (128x128)
- TFT color displays
- E-paper displays
- Character LCDs

You'll need to modify the display initialization and drawing code.

### Q: Is the source code open source?
**A:** Yes, licensed under MIT License. You're free to:
- Use for personal or commercial purposes
- Modify and distribute
- Include in other projects
- See LICENSE file for details

### Q: Can I build a PCB for this?
**A:** Absolutely! Consider:
- Separate analog (RF) and digital sections
- Good grounding practices
- RF shielding if needed
- Share your design with the community!

### Q: What about VHF/UHF frequencies?
**A:** The AD9850 can generate up to 40 MHz, but:
- Accuracy decreases at higher frequencies
- Detector circuit needs modification
- Consider AD9851 or AD9854 for better HF/VHF coverage
- VHF/UHF requires different detector design

### Q: Can I measure impedance (R + jX)?
**A:** Current version measures SWR only. To measure impedance:
- Need phase detection circuit
- More complex RF bridge design
- Additional ADC channels
- See CONTRIBUTING.md for this enhancement

### Q: How can I improve accuracy?
**A:** 
1. Use higher quality RF detector diodes (1N5711)
2. Calibrate with precision loads
3. Use shorter, better coax cables
4. Add temperature compensation
5. Use precision voltage reference for ADC

## Getting Help

### Q: Where can I get support?
**A:** 
- Check this FAQ first
- Read README.md and CIRCUIT.md
- Search existing GitHub issues
- Open a new issue with details of your problem

### Q: How can I contribute?
**A:** See CONTRIBUTING.md for:
- Reporting bugs
- Suggesting features
- Submitting code improvements
- Improving documentation

### Q: Is there a user community?
**A:** Check the GitHub repository:
- Discussions tab for questions
- Issues for bugs and features
- Wiki for additional documentation

---

**Didn't find your question?** Open an issue with the "question" label on GitHub!
