# 🎯 **Geant4 Simulation for a Radiation Monitorn Using The [GRAPES-3](https://www.tifr.res.in/grapes3/) Scintillator Detectors**

## 🌟 **Overview**

Welcome to the **Radiation Monitor Simulation** project! This application leverages the power of **Geant4** to simulate a radiation monitor using cylindrical plastic scintillator detectors, fabricated at the **GRAPES-3** facility. These detectors are equipped with **Kuraray Y11** wavelength-shifting (WLS) fibers, meticulously arranged on the surface to capture optical photons generated within the scintillators.

## ✨ **Features**

- 🛠 **GRAPES-3 Scintillator Detectors**: Fabricated at the GRAPES-3 facility.
- 🔍 **Wavelength-Shifting Fibers**: Utilizes Kuraray Y11 WLS fibers to effectively capture and transport optical photons.
- 🌐 **Optical Photon Tracking**: Tracks the generation, propagation, and detection of optical photons with high precision.

## 📋 **Dependencies**

- **[Geant4](https://geant4.web.cern.ch/)**: version 11.0. Plese follow the [link](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/index.html) for geant4 installation guide.
- **[CLHEP](https://proj-clhep.web.cern.ch/proj-clhep/)**: version 2.4.5
- **[ROOT](https://root.cern/)**: version 6.30
- **gcc**
- **cmake**: versio 3.22 

## 🛠 **Getting started with the appliction**
Please follow the follwoing steps to start the Geant4 Application
1. **Clone the repository**
   ```bash
    git clone https://github.com/Dipti-ASTROPART/Geant4_Application.git
2. **Building the application**
   ```bash
   cd Geant4_Application
   mkdir build
   cd build
   cmake ..
   make -j8
3. **Execution**
   ```bash
   ./g4sim ../G4SIM.cnt
