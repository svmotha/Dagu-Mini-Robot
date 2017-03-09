# DAGU Mini Driver Project | Motion
Moving the robot is essential to the success of the project. Because of the limited storage capacity of the controller **(10512kb)**, multiple motion functions are not an advisable design approach. To maximise the optimisation of code storage, a single **goSomewhere( )** function has been designed.

## goSomewhere( )
When called, the function takes in five inputs, namely:

1. **dirBinaryLeft** - A two state integer variale (boolean) that can be a 0 or 1. It dictates the direction the left motor rotates. 0 being forward and 1 being backwards.
2. **dirBinaryRight** - A two state integer variale (boolean) that can be a 0 or 1. It dictates the direction the right motor rotates. 0 being forward and 1 being backwards.
3. **leftSpeed** - An integer variable between 0 and 255. It dictates the speed the left motor rotates.
4. **rightSpeed** - An integer variable between 0 and 255. It dictates the speed the right motor rotates.
5. **delayTime** - An integer variable of any magnitude, that determines how long the process must run. The time is measured in miliseconds.

#### Left and Right Motor Speeds

###### Variable Motor Speed Alignment vs Side Positioned Distance Sensor Alignment 
The use of varying motor speeds is done to account for mechanical differences in each motor. Ideally running the motors at the same speed would result in perfectly aligned motion. This ideal case is however impractival, as the motors may have a varying number of reasons as to why they may not run at the same speed. These may range from manufacturing process inmperfections, to calibration issues, and even faulty wiring.

An alternative system is one where the motors both run at the same speed and are incrementally aligned through the use of the side distance sensor. In this instance, the motion would be limited to incremental delay times, to ensure gradual infrared distance sensor feedback within each iteration. This solution also renders the need of two varying motor speeds null. By relying on a sensor positioned perpendicular to the direction of motion, it is possible to maintain the warehouse robot's general motion within a preset range or tolerance. This range then allows the robot to make modifications, i.e. turning (left or right) to ensure it maintains a relatively straight, forward or backward motion.

###### Maximum Motor Speed Limiting
Whether or not the alignment is done through the varying of the respective motor speeds or the side positioned distance sensor, it is pertinent that the input motor speed be limited to the rated 255. This is done within the goSomewhere( ) function using the **if** conditional code below:

```
if (motorSpeed > 255)                          // limit maximum motor speed value 
  {
    motorSpeed = 255;
  }
```

The code ensures that the input motorSpeed variable is limited to a value of 255.
