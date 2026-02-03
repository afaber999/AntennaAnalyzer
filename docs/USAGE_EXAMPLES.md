# Usage Examples

## Example 1: Basic Antenna Testing

### Testing a Dipole Antenna

1. Connect the antenna analyzer to your dipole antenna
2. Power on the device
3. Use UP/DOWN buttons to tune to your target frequency (e.g., 7.100 MHz for 40m band)
4. Read the SWR value on the display
5. Adjust antenna length if SWR > 2.0:1

**Expected Results:**
- Well-tuned dipole should show SWR < 1.5:1 at resonance
- SWR will increase as you move away from resonant frequency

## Example 2: Finding Antenna Resonance

### Using Sweep Mode

1. Connect your antenna
2. Press SELECT button to start automatic sweep
3. Wait ~30 seconds for sweep to complete
4. Display will show the frequency with lowest SWR
5. Note this frequency - it's your antenna's resonant frequency

**Interpreting Results:**
```
Sweep Complete!
Best: 1.15 @ 7.150 MHz
```
This means:
- Antenna resonates at 7.150 MHz
- SWR at resonance is 1.15:1 (excellent)
- If target frequency is different, adjust antenna length

## Example 3: Antenna Length Adjustment

### For Lower Frequency (Make Antenna Longer)
If resonance is too high (e.g., you want 7.100 MHz but get 7.200 MHz):
1. Add length to antenna elements
2. Rule of thumb: 1% length change ≈ 1% frequency change
3. In this example: Need ~1.4% lower frequency, add ~1.4% length
4. Re-sweep to verify

### For Higher Frequency (Make Antenna Shorter)
If resonance is too low:
1. Remove length from antenna elements
2. Cut conservatively - you can always remove more
3. Re-sweep to verify

## Example 4: Multi-Band Antenna Testing

### Testing Each Band

For a multi-band antenna (e.g., 40m/20m/10m):

1. **40m Band** (7.0-7.3 MHz):
   - Tune to 7.100 MHz
   - Check SWR
   - Expected: < 2.0:1

2. **20m Band** (14.0-14.35 MHz):
   - Tune to 14.200 MHz
   - Check SWR
   - Expected: < 2.0:1

3. **10m Band** (28.0-29.7 MHz):
   - Tune to 28.500 MHz
   - Check SWR
   - Expected: < 2.0:1

## Example 5: Troubleshooting High SWR

### If SWR is Always High (>3.0:1)

**Possible Causes:**
1. **Poor connections**: Check coax connections, PL-259 connectors
2. **Damaged coax**: Test with known good coax cable
3. **Wrong antenna impedance**: Verify antenna design is 50Ω
4. **Ground issues**: Improve ground system for vertical antennas
5. **Nearby objects**: Remove metal objects near antenna

**Diagnostic Steps:**
```
Test Sequence:
1. Connect 50Ω dummy load → Should read SWR ≈ 1.0
2. If OK, problem is antenna or feedline
3. If high, check analyzer calibration
```

## Example 6: Serial Monitor Output

Connect USB cable and open serial monitor at 115200 baud:

```
ESP32 HF Antenna Analyzer
Initialization complete
Freq: 7.000 MHz, FWD: 2048, REF: 512, SWR: 1.50
Freq: 7.000 MHz, FWD: 2048, REF: 512, SWR: 1.50
```

During sweep:
```
Starting frequency sweep...
1.000 MHz: SWR = 4.50
1.100 MHz: SWR = 4.20
...
7.100 MHz: SWR = 1.15
7.200 MHz: SWR = 1.25
...
Sweep complete!
Best SWR: 1.15 at 7.100 MHz
```

## Example 7: Band-Specific Testing

### 80 Meters (3.5-4.0 MHz)
```
Typical sweep for 80m dipole:
3.500 MHz: SWR = 2.50
3.600 MHz: SWR = 2.00
3.700 MHz: SWR = 1.50
3.750 MHz: SWR = 1.20  ← Resonance
3.800 MHz: SWR = 1.50
3.900 MHz: SWR = 2.00
4.000 MHz: SWR = 2.50
```

### 40 Meters (7.0-7.3 MHz)
```
Typical sweep for 40m dipole:
7.000 MHz: SWR = 2.00
7.100 MHz: SWR = 1.30
7.150 MHz: SWR = 1.10  ← Resonance
7.200 MHz: SWR = 1.30
7.300 MHz: SWR = 2.00
```

### 20 Meters (14.0-14.35 MHz)
```
Typical sweep for 20m dipole:
14.000 MHz: SWR = 1.80
14.100 MHz: SWR = 1.40
14.200 MHz: SWR = 1.15  ← Resonance
14.300 MHz: SWR = 1.50
14.350 MHz: SWR = 1.80
```

## Example 8: Understanding SWR Graphs

### Good Antenna Response
```
SWR
 3|
  |
 2|    *           *
  |   * *         * *
 1|  *   *       *   *
  |_*_____*_____*_____*____ Frequency
    7.0   7.1   7.2   7.3 MHz
```
Sharp, well-defined minimum indicates good antenna design.

### Broad Antenna Response
```
SWR
 3|
  |
 2|  ***************
  | *               *
 1|*                 *
  |_____________________ Frequency
    7.0   7.1   7.2   7.3 MHz
```
Broad response good for covering entire band, but may sacrifice minimum SWR.

## Tips and Best Practices

1. **Always test with dummy load first** - Verifies analyzer is working correctly
2. **Use quality coax** - Poor coax can mask antenna problems
3. **Document your results** - Keep log of measurements for future reference
4. **Test in final location** - Nearby objects affect antenna performance
5. **Allow DDS to stabilize** - Wait a moment after frequency change
6. **Calibrate regularly** - Use 50Ω load to verify accuracy
7. **Compare with other tools** - Cross-check with SWR meter during transmit

## Common Antenna Types and Expected SWR

| Antenna Type | Band | Expected SWR at Resonance |
|--------------|------|---------------------------|
| Half-wave Dipole | Any | 1.1 - 1.5:1 |
| Vertical with Radials | Any | 1.2 - 2.0:1 |
| Inverted V | Any | 1.2 - 1.8:1 |
| Loop Antenna | Any | 1.2 - 2.0:1 |
| End-Fed Half-Wave | Any | 1.5 - 2.5:1 (with matching) |
| Yagi Beam | Any | 1.1 - 1.3:1 |

## Safety Reminders

- ⚠️ Never connect to powered transmitter
- ⚠️ Use low power levels only
- ⚠️ Disconnect before transmitting
- ⚠️ Keep away from high voltage
- ⚠️ Use in dry conditions only
