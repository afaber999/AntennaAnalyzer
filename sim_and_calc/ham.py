import numpy as np

def hamlibversion():
    """Return the version of hamlib."""
    return "0.1.4"

def VpeakToVrms(Vpeak):
    """Convert peak voltage to RMS voltage."""
    return Vpeak / np.sqrt(2)


def VrmsToDbm(Vrms, R=50):
    """Convert RMS voltage to dBm."""
    P = (Vrms**2) / R  # Power in watts
    P_mW = P * 1000  # Power in milliwatts
    return 10 * np.log10(P_mW)


def VpeakToDbm(Vpeak, R=50):
    """Convert peak voltage to dBm."""
    Vrms = VpeakToVrms(Vpeak)
    return VrmsToDbm(Vrms, R)


def DbmToVrms(Dbm, R=50):
    """Convert dBm to RMS voltage."""
    P_mW = 10 ** (Dbm / 10)  # Power in milliwatts
    P = P_mW / 1000  # Power in watts
    Vrms = np.sqrt(P * R)
    return Vrms


def DbmToVpeak(Dbm, R=50):
    """Convert dBm to peak voltage."""
    Vrms = DbmToVrms(Dbm, R)
    return Vrms * np.sqrt(2)


def DbmToVpeak(Dbm, R=50):
    """Convert dBm to peak voltage."""
    Vrms = DbmToVrms(Dbm, R)
    return Vrms * np.sqrt(2)


def DbmToWatts(Dbm):
    """Convert dBm to watts."""
    P_mW = 10 ** (Dbm / 10)  # Power in milliwatts
    return P_mW / 1000  # Power in watts


def VfwdVrevToReflectionCoeff(Vfwd, Vrev):
    """Calculate the reflection coefficient from forward and reverse voltages."""
    return Vrev / Vfwd


def ReflectionCoeffToSWR(Gamma):
    """Calculate SWR from reflection coefficient."""
    return (1 + abs(Gamma)) / (1 - abs(Gamma))


def SWRToReflectionCoeff(SWR):
    """Calculate reflection coefficient from SWR."""
    return (SWR - 1) / (SWR + 1)


def ReflectionCoeffToReturnLoss(Gamma):
    """Calculate return loss from reflection coefficient."""
    return -20 * np.log10(abs(Gamma))


def ReturnLossToReflectionCoeff(RL):
    """Calculate reflection coefficient from return loss."""
    return 10 ** (-RL / 20)


def SWRToReturnLoss(SWR):
    """Calculate return loss from SWR."""
    Gamma = SWRToReflectionCoeff(SWR)
    return ReflectionCoeffToReturnLoss(Gamma)


def ReturnLossToSWR(RL):
    """Calculate SWR from return loss."""
    Gamma = ReturnLossToReflectionCoeff(RL)
    return ReflectionCoeffToSWR(Gamma)


def ZLZ0ToReflection(ZL, Z0=50):
    """Calculate reflection coefficient from load impedance and characteristic impedance."""
    return (ZL - Z0) / (ZL + Z0)


def blackman_harris_window(N):
    """
    Generate a Blackman-Harris window of N samples.

    Parameters:
    -----------
    N : int
        Number of samples in the window

    Returns:
    --------
    window : numpy.ndarray
        Blackman-Harris window coefficients
    """
    # Blackman-Harris 4-term window coefficients
    a0 = 0.35875
    a1 = 0.48829
    a2 = 0.14128
    a3 = 0.01168

    n = np.arange(N)
    window = (
        a0
        - a1 * np.cos(2 * np.pi * n / (N - 1))
        + a2 * np.cos(4 * np.pi * n / (N - 1))
        - a3 * np.cos(6 * np.pi * n / (N - 1))
    )

    return window


def gamma_to_impedance(Gamma, Z0=50):
    """Convert reflection coefficient to impedance."""
    return Z0 * (1 + Gamma) / (1 - Gamma)

def impedance_to_gamma(ZL, Z0=50):
    """Convert impedance to reflection coefficient."""
    return (ZL - Z0) / (ZL + Z0)


def load_s1p(fname):
    """Load S-parameter data from an .s1p file."""
    freqs = []
    s11 = []
    with open(fname, 'r') as f:
        for line in f:
            if line.startswith('#') or line.startswith('!'):
                continue  # Skip comments and headers
            parts = line.split()
            if len(parts) >= 3:
                freqs.append(float(parts[0]))
                s11.append(complex(float(parts[1]), float(parts[2])))
    return np.array(freqs), np.array(s11)

def load_calset(fname):
    """Load calibration set from a file."""

    # exmples lines
    # Calibration data for NanoVNA-Saver
    # ! PCB calset with short 10cm coax
    # # Hz ShortR ShortI OpenR OpenI LoadR LoadI
    # 50000 -0.088676736 -7.9408e-05 0.08546744 0.000189059 0.000216443 -3.0423e-05
    # 99504 -0.088719856 0.000209607 0.085457616 -0.000114871 0.000214054 2.535e-06
    calset = []
    with open(fname, 'r') as f:
        for line in f:
            if line.startswith('#') or line.startswith('!'):
                continue  # Skip comments and headers
            parts = line.split()
            if len(parts) >= 7:
                entry = {
                    'freq': float(parts[0]),
                    'short': complex(float(parts[1]), float(parts[2])),
                    'open': complex(float(parts[3]), float(parts[4])),
                    'load': complex(float(parts[5]), float(parts[6]))
                }
                calset.append(entry)
    return calset


def calc_calibration(gamma_open, gamma_short, gamma_load):
    # From LOAD measurement (ra = 0):
    # ra = (rm - Ed) / (Er_calc + Es_calc * (rm - Ed_calc))
    # 0 = (rm - Ed)
    # => Ed = rm
    Ed = gamma_load

    # From SHORT and OPEN measurements:
    # Correct formula: Es = (Γm_short + Γm_open - 2*Ed) / (Γm_open - Γm_short)
    Es = (gamma_short + gamma_open - 2*Ed) / (gamma_open - gamma_short)

    # Correct formula: Er = -(Γm_short - Ed) * (1 + Es)
    # Or equivalently: Er = (Γm_open - Ed) * (1 - Es)
    Er = -(gamma_short - Ed) * (1 + Es)

    return Ed, Es, Er


def calibrate_set(calset):
    calibration_results = {}
    for cal in calset:
        freq = cal['freq']
        gamma_open = cal['open']
        gamma_short = cal['short']
        gamma_load = cal['load']
        Ed, Es, Er = calc_calibration(gamma_open, gamma_short, gamma_load)
        calibration_results[freq] = {
            'Ed': Ed,
            'Es': Es,
            'Er': Er
        }
    return calibration_results


# Apply calibration correction
def apply_calibration(gamma, Ed, Es, Er):
    """Apply OSL calibration to correct measurement"""
    return (gamma - Ed) / (Er + Es * (gamma - Ed))