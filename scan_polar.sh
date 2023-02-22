#!/bin/sh
x=0.
y=0.
for angle in 0 45 90
do
        echo $angle
	for job in {1..4..1}
	do
		for polarizationDegree in 0.5 0.6 0.7 0.8 0.9 1.0 
		do
			 FILE="run_polarized_at_${polarizationDegree}_angle_${angle}mm_job_${job}.mac"

			 /bin/cat <<EOM >$FILE
/process/em/fluo true
/process/em/auger true
/process/em/augerCascade true
/process/em/pixe true

/PoSOS/det/setOutputDirectory /nfs/tegile/work/experiences/detecteurs/manzanillas/Polarimetry/Geant4Output/
/PoSOS/det/setSetupName angle_${angle}_at_${polarizationDegree}0_pc_run_${job}
#select output format, options are: csv root hdf5
/PoSOS/det/setDataType csv
#Set the position of the collimator, a values between -37 and 37 mm (size of samples)
#Choise of source type: 0 gamma,1 Fe-55,2 Cs-137; 1 Bi-207; 2 Sr-90; 3 e-

/run/initialize
/PoSOS/gun/sourceType 0
/PoSOS/gun/sourcePositionX ${x} mm
/PoSOS/gun/sourcePositionY ${y} mm
/PoSOS/gun/sourceDiameter 0.1 mm
/PoSOS/gun/sourceEnergy 7.0  keV
#the default is linear polarization
#/PoSOS/gun/sourceGammaCircularPolarizationAngle ${angle}.
/PoSOS/gun/sourceGammaPolarizationAngle ${angle}.
/PoSOS/gun/sourceGammaPolarizationDegree ${polarizationDegree}

/run/beamOn 80000000

EOM
		FILEJOB="job_polarization_${polarizationDegree}_${angle}degrees_job_${job}.sh"

                /bin/cat <<EOM >$FILEJOB
#!/bin/bash
#SBATCH -n 1
#SBATCH --qos=parallel
#SBATCH --partition=sumo
#SBATCH --time=2:00:00
#SBATCH --cpus-per-task=2
#
source ~/environment_GeSOS.sh
./PoSOS -m $FILE  
exit 0
EOM
sbatch $FILEJOB
		done

	done
done

































