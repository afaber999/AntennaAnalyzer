import numpy as np


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
