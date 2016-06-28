# little script to generate scanning patters based on some given boundary conditions
# usage:
#  - Laser system number (1: upstream / 2: downstream)
#  - start positons
#  - scanning step size (horizontal / vertical)
#  - number of steps (horizontal / vertical)


from numpy.polynomial import polynomial as pol
import numpy as np

def polar_abs_deg2steps(deg):
    ''' converts degrees to absolute stepsize, only use this for the starting positions.
        calibration was performed with mirror approaching from the homing position to the '''

    if deg > 180 or deg < 0:
        raise ValueError("polar angle is out of boundaries [0, 180], value was " + str(deg))
    steps_deg = 0.000178683
    return int(1 / steps_deg * (deg - offsets_deg[0]) + offsets_steps[0])

def polar_deg2steps(deg):
    steps_deg = 0.000178683
    return int(deg / steps_deg)

def azimu_abs_deg2steps(deg):
    steps_deg = 0.00008064
    return int(1 / steps_deg * (deg - offsets_deg[1]) + offsets_steps[1])

def azimu_deg2steps(deg):
    steps_deg = 0.00008064
    return int(deg / steps_deg)

def power2steps(power):
    range = power_range[0] - power_range[1]
    steps = range * power
    return int(steps + power_range[1])

id = 2
StartingPosition = 0
ScanningSteps = [0, 0]      # polar, azimuthal
ScanningStepsize = [1,2]    # horizontal, vertical

offsets_deg = []            # horizontal, vertical calibration in deg
offsets_steps = []          # offsets in microsteps
power_range = []           # power calibration in steps (first: maximum, second: minimum)
power_pol_coeff = []        # coefficients of the power to angle polynomial fit

# load settings according for the selected system
if id == 1:
    offsets_steps = [0, 0]
    offsets_deg = [0., 0.]
    power_range = [12500, 8000]
    power_pol_coeff = [9.42032760E-01, 3.64403908E-03, -1.14119451E-04, 3.96052294E-07]
elif id == 2:
    offsets_steps = [174000, 970000]
    offsets_deg = [28.8218, -4.62600708]
    power_range = [12500, 8000]
    power_pol_coeff = [9.42032760E-01, 3.64403908E-03, -1.14119451E-04, 3.96052294E-07]
else:
    raise ValueError("Laser ID unknown")



# define starting points and ranges:
polar_start = 27.0
polar_end = 130

azimu_start = -5.0
azimu_end = 48

n_polar_steps = 23

polar_range = np.linspace(polar_start, polar_end, n_polar_steps)
polar_start_steps = polar_abs_deg2steps(polar_start)
polar_range_steps = [polar_deg2steps(x) for x in np.diff(polar_range)]

azimu_start_steps = azimu_abs_deg2steps(azimu_start)
azimu_range_steps = azimu_deg2steps(azimu_end - azimu_start)
azimu_range = azimu_end - azimu_start

# power calculation
power_coeff = pol.polyval(polar_range, power_pol_coeff)
power_steps =[power2steps(steps) for steps in power_coeff]

# scanning speeds (microsteps / s)
polar_drive_speed = 15000
azimu_drive_speed = 15000
polar_scan_speed = 8000
azimu_scan_speed = 3000

# shhoting frequencies
shot_freq = 4


# file handling
config = 1
outfile = "scan_pattern-" + str(config) + ".csv"
file = open("../scans/" + outfile, "w+")
file.write("#N      HRM,     VRM,       HS,    VS,   S,  SC,    AT, AP\n")
# for file fomratting see: services/positions.py

instructions = []
# go to first positions
instructions.append([0, azimu_start_steps, polar_start_steps, azimu_drive_speed, polar_drive_speed, 0, 0, power_steps[0], 0])

for line in range(1, n_polar_steps+1):
    # do a horizontal scan:
    instructions.append([2*line-1, (-1)**line*azimu_range_steps, 0, azimu_scan_speed, polar_scan_speed, shot_freq, 0, power_steps[line-2], 0])
    # move a bit
    instructions.append([2*line, 0, polar_range_steps[line-2], azimu_scan_speed, polar_scan_speed, 0, 0, power_steps[line-2], 0])

for i in instructions:
    file.write(', '.join(str(e) for e in i) + "\n")


