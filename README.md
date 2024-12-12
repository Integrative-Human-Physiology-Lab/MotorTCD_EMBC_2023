# MOTORTCD

The motorTCD project enables the Transcranial doppler probe to be moved with the help of terminal commands. It enables the correct positioning of probe once again even if the signal is lost due to movements or slight jerk.
The project runs on Arduino Uno.

# **GETTING STARTED**
**1. Order the Off-The-Shelf Components:**

  Order the components as listed in the [Bill of Material](https://github.com/Integrative-Human-Physiology-Lab/MotorTCD_EMBC_2023/blob/main/Documentation/BOM.xlsx) for the project.


**2. Print the 3D-printable files:**

Print the STL file as shown in the [Print_Files](https://github.com/Integrative-Human-Physiology-Lab/MotorTCD_EMBC_2023/tree/main/Print_Files).

**3. Start the Assembly:**
The [Assembly Guide](https://github.com/Integrative-Human-Physiology-Lab/MotorTCD_EMBC_2023/blob/main/Documentation/assembly%20guide.pdf) shows you step by step process to assemble and wire the entire project with pictures. The entire assembly process is break down into following steps:
1. Assembly of 3D-printed components
2. Assembly of power components
3. Assembly of motor components
4. Assembly of Arduino with terminal and joystick shield
5. Wiring of all components
6. Final check to ensure connection

This step requires you to have all the components as listed in the Bill of Materials for our project.

**4. Upload the code:**
Connect the Arduino Uno to your computer and upload the Arduino Code.

# **Positioning reference: **

Whenever a signal is locked in. It is always advised that the harness for the TCD probe be placed in the center of the clamp boundaries. Repeated Experimentation has shown that due to the weight of the system, when the subject (individual being experimented on) is made to move in any direction, the TCD probe might be displaced by a very amount in the direction opposite to the moving direction of the subject. As this change in position is very miniscule, putting the probe in the center allows for the position to be adjusted in any direction. 

It is also advised that the person who operates the joystick familiarize themselves with the speed and movement of the clamp for better and more efficient reading. 