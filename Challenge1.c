//TCSS 437 Challenge 1: Travis Stinebaugh, Daniel Wood
//configure left touch sensor
#pragma config(Sensor, S1,     leftTouch,      sensorEV3_Touch)
//configure right touch sensor
#pragma config(Sensor, S2,     rightTouch,     sensorEV3_Touch)
//configure left motor
#pragma config(Motor,  motorA,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft)
//configure right motor
#pragma config(Motor,  motorB,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)

task main()
{
	//minimum value for random high speed motor power
	int highPowerMin = 50;
	//maximum value for random high speed motor power
	int highPowerMax = 80;
	//minimum value for random low speed motor power
	int lowPowerMin = 0;
	//maximum value for random low speed motor power
	int lowPowerMax = 10;
	//minimum value for biased random walk time interval
	int minWalkTime = 10000;
	//maximum value for biased random walk time interval
	int maxWalkTime = 30000;
	//number of times a sensor was triggered; used for detecting if stuck in corner.
	int loops = 0;

	//sets seed for the random number generator
	srand(nSysTime);

	//Intentional infinite loop. 
	while (true)
	{
		//Detect if either sensor has been triggered.		
		if(getTouchValue(S1) == 1 || getTouchValue(S2) == 1) {
			sleep(150);

			//Detect if both sensors have been triggered; front end collission. 
			if (getTouchValue(S1) == 1 && getTouchValue(S2) == 1)
			{
				//Play sound
				playSound(soundBeepBeep);
				//backup 
				motor[motorA] = -(highPowerMax);
				motor[motorB] = -(highPowerMax);
				sleep(500);

				//wait two seconds
				motor[motorA] = 0;
				motor[motorB] = 0;
				sleep(2000);

				//Randomly choose between turning right or left a random degree. 
				int power = (random(highPowerMax-highPowerMin) + highPowerMin); 
 				if(random(1) == 0){
 					motor[motorA] = power;
					motor[motorB] = -power;
				} else {
					motor[motorA] = -power;
					motor[motorB] = power;
				}
				sleep(250);
			}
			//check if sensor 1 (left) is triggered
			else if (getTouchValue(S1) == 1)
			{
				// If total number of bumps on both sensors over 2 within loops number of iterations, 
				// maybe we're stuck at a corner/wall and need to turn around. 
				if((getBumpedValue(S1) + getBumpedValue(S2)) > 2) {
					playSound(soundBeepBeep);
					motor[motorA] = -65;
					motor[motorB] = 65;
					sleep(500);
				} else {
					//turn a random amount and continue 
					motor[motorA] = -(random(highPowerMax-highPowerMin) + highPowerMin);
					motor[motorB] = -(random(lowPowerMax-lowPowerMin) + lowPowerMin);

					sleep(random(500) + 250);
				}
			}
			else if (getTouchValue(S2) == 1)
			{
				// If total number of bumps on both sensors over 2 within loops number of iterations, 
				// maybe we're stuck at a corner/wall and need to turn around. 
				if((getBumpedValue(S1) + getBumpedValue(S2)) > 2) {
					playSound(soundBeepBeep);
					motor[motorA] = 65;
					motor[motorB] = -65;
					sleep(500);
				} else {
					//turn a random amount and continue
					motor[motorA] = -(random(lowPowerMax-lowPowerMin) + lowPowerMin);
					motor[motorB] = -(random(highPowerMax-highPowerMin) + highPowerMin);
					sleep(random(500) + 250);
				}
			}
		}
		else
		{
			//Continue with biased random walk
			motor[motorA] = random(highPowerMax-highPowerMin) + highPowerMin;
			motor[motorB] = random(highPowerMax-highPowerMin) + highPowerMin;
			int counter = random(maxWalkTime - minWalkTime)+ minWalkTime;
			while(counter > 0) {
				if(getTouchValue(S1) == 1 || getTouchValue(S2) == 1) {
					counter = 0;
				} else {
					counter--;
				}
			}
			// Keeps track of the number of times the program enters this branch of the loop
			// If it's (presumably) not stuck in a corner, reset this count and bumped values
			// after 5 loops.
			if(loops > 5) {
				loops = 0;
				resetBumpedValue(S1);
				resetBumpedValue(S2);
			// Or else increment the number of loops 
			} else {
				loops++;
			}
		}
	}
}