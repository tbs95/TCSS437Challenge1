#pragma config(Sensor, S1,     leftTouch,      sensorEV3_Touch)//configure left touch sensor
#pragma config(Sensor, S2,     rightTouch,     sensorEV3_Touch)//configure right touch sensor
#pragma config(Motor,  motorA,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft)//configure left motor
#pragma config(Motor,  motorB,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)//configure right motor
//TCSS 437 Challenge 1: Travis Stinebaugh, Daniel Wood


//minimum value for random high speed motor power
const int HIGH_POWER_MIN = 50;
//maximum value for random high speed motor power
const int HIGH_POWER_MAX = 80;
//minimum value for random low speed motor power
const int LOW_POWER_MIN = 0;
//maximum value for random low speed motor power
const int LOW_POWER_MAX = 10;
//minimum value for biased random walk time interval
const int MIN_WALK_TIME = 10000;
//maximum value for biased random walk time interval
const int MAX_WALK_TIME = 30000;

//number of times a sensor was triggered; used for detecting if stuck in corner.
int loops = 0;

void bothSensorsTriggered(){
	//Play sound
	playSound(soundBeepBeep);
	//backup
	motor[motorA] = -(HIGH_POWER_MAX);
	motor[motorB] = -(HIGH_POWER_MAX);
	sleep(500);

	//wait two seconds
	motor[motorA] = 0;
	motor[motorB] = 0;
	sleep(2000);

	//Randomly choose between turning right or left a random degree.
	int power = (random(HIGH_POWER_MAX-HIGH_POWER_MIN) + HIGH_POWER_MIN);
	if(random(1) == 0){
		motor[motorA] = power;
		motor[motorB] = -power;
		} else {
		motor[motorA] = -power;
		motor[motorB] = power;
	}
	sleep(250);
}

void singleSensorTriggered(tMotor motor1, tMotor motor2) {
		// If total number of bumps on both sensors over 2 within loops number of iterations,
	// maybe we're stuck at a corner/wall and need to turn around.
	if((getBumpedValue(S1) + getBumpedValue(S2)) > 2) {
		motor[motor1] = -65;
		motor[motor2] = 65;
		sleep(500);
		} else {
		//turn a random amount and continue
		motor[motor1] = -(random(LOW_POWER_MAX-HIGH_POWER_MIN) + HIGH_POWER_MIN);
		motor[motor2] = -(random(LOW_POWER_MAX-LOW_POWER_MIN) + LOW_POWER_MIN);

		sleep(random(500) + 250);
	}
}


void biasedRandomWalk() {
	//Continue with biased random walk
	motor[motorA] = random(HIGH_POWER_MAX-HIGH_POWER_MIN) + HIGH_POWER_MIN;
	motor[motorB] = random(HIGH_POWER_MAX-HIGH_POWER_MIN) + HIGH_POWER_MIN;
	int counter = random(MAX_WALK_TIME - MIN_WALK_TIME)+ MIN_WALK_TIME;
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

task main()
{
	//number of times a sensor was triggered; used for detecting if stuck in corner.
//	int loops = 0;

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
				bothSensorsTriggered();
			}
			//check if sensor 1 (left) is triggered
			else if (getTouchValue(S1) == 1)
			{
				singleSensorTriggered(motorA, motorB);
			}
			//check if sensor2 (right) is triggered
			else if (getTouchValue(S2) == 1)
			{
				singleSensorTriggered(motorB, motorA);
			}
		}
		else
		{
			//continue biased random walk
			biasedRandomWalk();
		}
	}
}