# Contributing to ESP32 HF Antenna Analyzer

Thank you for your interest in contributing! This document provides guidelines for contributing to the project.

## How to Contribute

### Reporting Issues

If you find a bug or have a suggestion:

1. Check if the issue already exists in the issue tracker
2. If not, create a new issue with:
   - Clear description of the problem or suggestion
   - Steps to reproduce (for bugs)
   - Expected vs actual behavior
   - Hardware configuration (ESP32 board, AD9850 module, etc.)
   - Software version and environment

### Submitting Changes

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Test your changes thoroughly
5. Commit with clear messages (`git commit -m 'Add amazing feature'`)
6. Push to your fork (`git push origin feature/amazing-feature`)
7. Open a Pull Request

### Code Style

- Follow existing code formatting
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and reasonably sized
- Use `#define` constants instead of magic numbers

### Testing

Before submitting:

1. **Build Test**: Ensure code compiles without errors
2. **Hardware Test**: Test on actual ESP32 hardware if possible
3. **Functionality Test**: Verify your changes work as intended
4. **Regression Test**: Ensure existing features still work

## Development Setup

### Prerequisites

- PlatformIO Core or PlatformIO IDE
- ESP32 development board
- USB cable for programming
- (Optional) Hardware components for full testing

### Building

```bash
# Clone your fork
git clone https://github.com/YOUR_USERNAME/AntennaAnalyzer.git
cd AntennaAnalyzer

# Build the project
pio run

# Upload to ESP32
pio run --target upload

# Monitor serial output
pio device monitor
```

## Areas for Contribution

### High Priority

- [ ] Improved SWR calculation algorithms
- [ ] Calibration routine implementation
- [ ] Save/load settings to EEPROM
- [ ] Additional display modes (Smith chart)
- [ ] Better RF detection circuits

### Medium Priority

- [ ] Data logging to SD card
- [ ] Bluetooth/WiFi connectivity
- [ ] Web interface for remote control
- [ ] Battery power management
- [ ] Different display sizes support

### Documentation

- [ ] Additional usage examples
- [ ] Video tutorials
- [ ] Circuit board layouts (PCB designs)
- [ ] Calibration procedures
- [ ] Troubleshooting guides

### Testing

- [ ] Unit tests for calculations
- [ ] Hardware test procedures
- [ ] Comparison with commercial analyzers
- [ ] Different antenna types documentation

## Code Review Process

1. All submissions require review
2. Maintainers will provide feedback
3. Address review comments
4. Once approved, changes will be merged

## Communication

- **Issues**: For bugs and feature requests
- **Pull Requests**: For submitting code changes
- **Discussions**: For questions and general discussion

## Recognition

Contributors will be acknowledged in the project documentation and release notes.

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

## Questions?

Feel free to open an issue with the "question" label if you need help getting started.

Thank you for contributing to make this project better!
