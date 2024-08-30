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

## 🤹‍♂️ **Controlling the program flow**
### **1. Visualization**
The visualization is controlled by the **vis.mac** macro file located inside the macrofiles directory. The basis set up consists of:
<pre>
   /run/initialize                                  # Initialize the visual executive
   /vis/open OGL                                    # Visualization driver
   /vis/viewer/set/viewpointVector 2.0 0.0 0.0      # The view pointer 
   /vis/viewer/set/lightsVector -1 0 0
   /vis/drawVolume                                  # Draw the volume
   /vis/viewer/set/autoRefresh true                 # auto refresh
   /vis/scene/add/trajectories smooth               # trajectory type
   #/vis/scene/add/scale 1 m                        # If you want to see the scale
   #/vis/scene/add/axes                             # Draw (X, Y, Z) axes
   /vis/scene/add/eventID                           # Add event ID
   #/vis/reviewKeptEvents                           # review the evevnt
   /vis/scene/endOfEventAction accumulate           # Accumulate in case of mulitple beam at once
   /vis/viewer/flush
</pre>

### **2. Run in batch mode**
The application can be run in batch mode while the run is controlled by the **G4SIM.cnt** file. The basic control flow of the **.cnt** file includes:
<pre>
 Task                    G4SIM                                           # Task name
 InputPath               /path/to/inputfile/dir/                         # Input path
 OutputPath              /path/to/outputfile/dir/                        # Output Path
 PrimaryEnergy           0.100   0.100                                   # Energy range (MeV) of primary beam
 PrimaryParticle         photon                                          # Primary particle name used as beam
 MacroPath               /macro/files/path                               # Path containing `.mac' files
 NBeam                   50000                                           # Number of beams in one run
 GraphicalMode           0                                               # [ `1' for True | `0' for False]
</pre>

**Note 1:** The `PrimaryEnergy` gives the option to choose the energy range which generates random energy based on a power law distribution. If the values are kept the same, it essentially generates particle beam of constant energy. The basis unit of input energy is given in MeV. 

**Note 2:** The primary particle definitions are added in the `g4PrimaryGenerator.cc` class. The available primary particles are: 

**`electron/e-`, `positron/e+`, `muon+/mu+`, `muon-/mu-`, `photon/gamma`, `proton`, `alpha`, `Li6`, `Li7`, `Be9`, `B10`, `B11`, `C12`, `C13`, `N14`, `N15`, `O16`, `O17`, `O18`, `F19`, `Ne20`, `Ne21`, `Ne22`, `Na23`, `Mg24`, `Mg25`, `Mg26`, `Al27`, `Si28`, `Si29`, `Si30`.**



## Class Descriptions

### Initialization and Main Classes

- **`g4main.cc`**
  - The entry point of the Geant4 application. It configures the main simulation loop, initializes the detector, physics, and user interfaces, and starts the event loop.
   
- **`g4ActionInitialize.cc`**
  - Handles the initialization of user actions within the Geant4 simulation. This class is responsible for setting up the sequence of actions like event handling, run management, and stepping actions.

### Control and Task Management
- **`g4ControlFileHandler.cc`**
  - Manages control files that configure the simulation. This class is used to parse and apply settings from external files, making it easier to modify simulation parameters.

- **`g4TaskManager.cc`**
  - Handles the distribution and management of computational tasks. This class is crucial for optimizing the simulation's performance, particularly in parallel computing environments.
  
### Detector and Material Management
- **`g4DetectorConstruction.cc`**
  - Defines the geometry and materials of the detector setup. This class is crucial for specifying the physical world in which particles interact.

- **`g4UserMaterial.cc`**
  - Manages custom user-defined materials. This class allows for the creation and management of materials that are not predefined in Geant4 or adding specific properties.

### Physics and Primary Generator
- **`g4PhysicsList.cc`**
  - Specifies the physics processes and models used in the simulation. This class is responsible for defining how particles interact with materials and fields.

- **`g4PrimaryGenerator.cc`**
  - Handles the generation of primary particles at the start of each event. This class defines the type, energy, and initial position of the particles to be simulated.

### Event and Run Management
- **`g4EventAction.cc`**
  - Manages actions to be taken at the beginning and end of each event. This class is essential for gathering event-level data and performing event-specific tasks.

- **`g4RunAction.cc`**
  - Defines actions to be taken at the beginning and end of each simulation run. This class is used for setting up run-specific parameters and collecting overall statistics.

### Stepping and Sensitive Detectors
- **`g4SteppingAction.cc`**
  - Handles actions to be taken at each simulation step. This class is where detailed particle tracking and interaction logging occur.

- **`g4SensitiveDetector.cc`**
  - Manages sensitive detectors, which record the interactions of particles as they pass through detector components. This class is key to data collection during the simulation.

