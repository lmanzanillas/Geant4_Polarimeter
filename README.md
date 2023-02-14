# Polarimeter



## Getting started

Geant4 simulation project developed at SOLEIL to study the effect of polarization on the scattering of photons

## How to use it

- It's highly recommended to use it in the SUMO cluster of SOLEIL 
- If you want to use it in your local computer you first need to install the geant4 shoftware
- In SUMO you just need to connect to the cluster and then charge the required modules: 

```
# Modules:
module load physics/geant4/4.11
module load tools/gcc/9.2.0

# Environment variables for Geant4 and ROOT.
export GEANT4=/usr/local/applications/physics/geant4/4.11/
export ROOT6=/usr/local/applications/tools/root6/
export BOOST=/usr/local/applications/tools/boost/1.78

# Geant4 and ROOT environment variables are loaded.
source $GEANT4/bin/geant4.sh
source $ROOT6/bin/thisroot.sh


# ROOT library.
export LD_LIBRARY_PATH=$BOOST/lib:$LD_LIBRARY_PATH:$ROOT6/lib:$GEANT4/lib64:/usr/lib64/:/usr/lib

```

You can just add the previous commnads in a setup_polarimeter.sh file and just use 
```
source setup_polarimeter.sh

```
when you start a new session in SUMO 

## Installing the sofwtware 

If is the first time that you are using it, you can just clone it by doing: 

```
git clone git@gitlab.synchrotron-soleil.fr:detecteurs/polarimeter.git

```
The go to the polarimeter folder 

```
cd polarimeter/build 
cmake ../
make 

```
That should install the polarimter software. Then you can use a macro (for example test.mac) to change the settings as energy of photons, degree of polarization or angle of polarization 

```
#just in case, it shoud be true by default
/process/em/fluo true
/process/em/auger true
/process/em/augerCascade true
/process/em/pixe true

#/run/numberOfThreads 4
#/PoSOS/det/setOutputDirectory /nfs/tegile/work/experiences/detecteurs/manzanillas/LEAPS_INNOV/Geant4_output/
/PoSOS/det/setSetupName run_1
#select output format, options are: csv root hdf5
/PoSOS/det/setDataType csv
#Choise of source type: 0 gamma,1 Fe-55,2 Cs-137; 1 Bi-207; 2 Sr-90; 3 e-

/run/initialize
/PoSOS/gun/sourceType 0
/PoSOS/gun/sourceDiameter 0.1 mm
/PoSOS/gun/sourceEnergy 7.0  keV
/PoSOS/gun/sourceGammaPolarizationAngle 90
/PoSOS/gun/sourceGammaPolarizationDegree 0.9

/run/beamOn 9000000


```

- Then you can run it from your build file 


```
./PoSOS -m test.mac

```

This will create a .csv file or other format depending on what was your choice where it's stored the hits information position of interaction, number of event, energy ...
You can use the code of your choice to analyze the data  

