#include <Fuzzy.h>
#include <FuzzyComposition.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzyOutput.h>
#include <FuzzyRule.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzySet.h>



//setting the maximum and minimum values
#define MAX_DISTANCE 50
#define MIN_DISTANCE 1
#define MAX_SPEED 255
#define MIN_SPEED 0



//declaration pins to be used in due arduino
  #define ENABLEPIN_RIGHT     	5  //PWM pin
  #define ENABLEPIN_LEFT      	6  //PWM pin
  #define OUTPUTPIN_RIGHT_1  	22
  #define OUTPUTPIN_RIGHT_2  	23
  #define OUTPUTPIN_LEFT_1   	24
  #define OUTPUTPIN_LEFT_2   	25
  #define TRIGPIN_FRONT      	53
  #define ECHOPIN_FRONT       	52
  #define TRIGPIN_LATERAL      	46
  #define ECHOPIN_LATERAL      	47


//variables to be used
float outputLeft,outputRight,distanceFront,distanceRight;

//function to calculate the distance from the wall until the sensor in cm
float calculateDistanceSensor(int trigPin, int echoPin);

// create a new fuzzy object
Fuzzy* fuzzy  = new Fuzzy();

void setup() {


     //the operation mode setting pins
	  pinMode(ENABLEPIN_RIGHT,OUTPUT);  
          pinMode(ENABLEPIN_LEFT,OUTPUT);  
          pinMode(OUTPUTPIN_RIGHT_1,OUTPUT);
          pinMode(OUTPUTPIN_RIGHT_2,OUTPUT);
          pinMode(OUTPUTPIN_LEFT_1,OUTPUT);
          pinMode(OUTPUTPIN_LEFT_2,OUTPUT);
          pinMode(TRIGPIN_FRONT,OUTPUT);
          pinMode(ECHOPIN_FRONT,INPUT);
          pinMode(TRIGPIN_LATERAL,OUTPUT);
          pinMode(ECHOPIN_LATERAL,INPUT);
          //Serial.begin(9600);


// create  two new FuzzyInput objects
  FuzzyInput* frontDistanceSensor = new FuzzyInput(1);
  FuzzyInput* lateralDistanceSensor = new FuzzyInput(2);
 
 // create the FuzzySets that  copose the FuzzyInput objects 
  FuzzySet* smallFrontDistance = new FuzzySet(MIN_DISTANCE,MIN_DISTANCE,MAX_DISTANCE/4,MAX_DISTANCE/2);
  frontDistanceSensor->addFuzzySet(smallFrontDistance);//add the FuzzySet in the FuzzyInput object
  FuzzySet* smallLateralDistance = new FuzzySet(MIN_DISTANCE,MIN_DISTANCE,MAX_DISTANCE/4,MAX_DISTANCE/2);
  lateralDistanceSensor->addFuzzySet(smallLateralDistance);
  
  FuzzySet*  safeFrontDistance = new FuzzySet(MAX_DISTANCE/4,MAX_DISTANCE/2,MAX_DISTANCE/2,MAX_DISTANCE*3/4);
  frontDistanceSensor->addFuzzySet(safeFrontDistance);
  FuzzySet*  safeLateralDistance = new FuzzySet(MAX_DISTANCE/4,MAX_DISTANCE/2,MAX_DISTANCE/2,MAX_DISTANCE*3/4);
  lateralDistanceSensor->addFuzzySet(safeLateralDistance);
  
  FuzzySet*  bigFrontDistance = new FuzzySet(MAX_DISTANCE/2,MAX_DISTANCE*3/4,MAX_DISTANCE,MAX_DISTANCE);
  frontDistanceSensor->addFuzzySet(bigFrontDistance);
  FuzzySet*  bigLateralDistance = new FuzzySet(MAX_DISTANCE/2,MAX_DISTANCE*3/4,MAX_DISTANCE,MAX_DISTANCE);
  lateralDistanceSensor->addFuzzySet(bigLateralDistance);

  // add the FuzzyInputs in the Fuzzy Object
  fuzzy->addFuzzyInput(lateralDistanceSensor);
  fuzzy->addFuzzyInput(frontDistanceSensor);
  
  //create two new FuzzyOutput objects
  FuzzyOutput* rightWheelSpeed = new FuzzyOutput(1);
  FuzzyOutput* leftWheelSpeed = new FuzzyOutput(2);

  // create the FuzzySets that  copose the FuzzyOutput objects 
  FuzzySet* lowerRightSpeed = new FuzzySet(MIN_SPEED,MIN_SPEED,MAX_SPEED/4,MAX_SPEED/2);
  rightWheelSpeed->addFuzzySet(lowerRightSpeed);//add the FuzzySet in the FuzzyInput object
  FuzzySet* lowerLeftSpeed = new FuzzySet(MIN_SPEED,MIN_SPEED,MAX_SPEED/4,MAX_SPEED/2);
  leftWheelSpeed->addFuzzySet(lowerLeftSpeed);

  FuzzySet* averageRightSpeed = new FuzzySet(MAX_SPEED/4,MAX_SPEED/2,MAX_SPEED/2,MAX_SPEED*3/4);
  rightWheelSpeed->addFuzzySet(averageRightSpeed);
  FuzzySet* averageLeftSpeed = new FuzzySet(MAX_SPEED/4,MAX_SPEED/2,MAX_SPEED/2,MAX_SPEED*3/4);
  leftWheelSpeed->addFuzzySet(averageLeftSpeed);

  FuzzySet* fastRightSpeed = new FuzzySet(MAX_SPEED/2,MAX_SPEED*3/4,MAX_SPEED,MAX_SPEED);
  rightWheelSpeed->addFuzzySet(fastRightSpeed);
  FuzzySet* fastLeftSpeed = new FuzzySet(MAX_SPEED/2,MAX_SPEED*3/4,MAX_SPEED,MAX_SPEED);
  leftWheelSpeed->addFuzzySet(fastLeftSpeed);


  //add the FuzzyOutputs in the Fuzzy object
  fuzzy->addFuzzyOutput(rightWheelSpeed);
  fuzzy->addFuzzyOutput(leftWheelSpeed);


  //building the antecedents fuzzy rules
  FuzzyRuleAntecedent* smallFrontDistanceAndsmallLateralDistance = new FuzzyRuleAntecedent();
  	smallFrontDistanceAndsmallLateralDistance->joinWithAND(smallFrontDistance,smallLateralDistance); 
  FuzzyRuleAntecedent* smallFrontDistanceAndsafeLateralDistance = new FuzzyRuleAntecedent();
  	smallFrontDistanceAndsafeLateralDistance->joinWithAND(smallFrontDistance,safeLateralDistance);
  FuzzyRuleAntecedent* smallFrontDistanceAndbigLateralDistance = new FuzzyRuleAntecedent();
  	smallFrontDistanceAndbigLateralDistance->joinWithAND(smallFrontDistance,bigLateralDistance);
  FuzzyRuleAntecedent* safeFrontDistanceAndsmallLateralDistance = new FuzzyRuleAntecedent();
  	safeFrontDistanceAndsmallLateralDistance->joinWithAND(safeFrontDistance,smallLateralDistance);
  FuzzyRuleAntecedent* safeFrontDistanceAndsafeLateralDistance = new FuzzyRuleAntecedent();
  	safeFrontDistanceAndsafeLateralDistance->joinWithAND(safeFrontDistance,safeLateralDistance);
  FuzzyRuleAntecedent* safeFrontDistanceAndbigLateralDistance = new FuzzyRuleAntecedent();
  	safeFrontDistanceAndbigLateralDistance->joinWithAND(safeFrontDistance,bigLateralDistance);
  FuzzyRuleAntecedent* bigFrontDistanceAndsmallLateralDistance = new FuzzyRuleAntecedent();
  	bigFrontDistanceAndsmallLateralDistance->joinWithAND(bigFrontDistance,smallLateralDistance);
  FuzzyRuleAntecedent* bigFrontDistanceAndsafeLateralDistance = new FuzzyRuleAntecedent();
  	bigFrontDistanceAndsafeLateralDistance->joinWithAND(bigFrontDistance,safeLateralDistance);
  FuzzyRuleAntecedent* bigFrontDistanceAndbigLateralDistance = new FuzzyRuleAntecedent();
  	bigFrontDistanceAndbigLateralDistance->joinWithAND(bigFrontDistance,bigLateralDistance);


  //building the consequent fuzzy rules
  FuzzyRuleConsequent* fastRightSpeedWheel = new FuzzyRuleConsequent();
  	fastRightSpeedWheel->addOutput(fastRightSpeed);
  FuzzyRuleConsequent* averageRightSpeedWheel = new FuzzyRuleConsequent();
  	averageRightSpeedWheel->addOutput(averageRightSpeed);
  FuzzyRuleConsequent* lowerRightSpeedWheel = new FuzzyRuleConsequent();
  	lowerRightSpeedWheel->addOutput(lowerRightSpeed);
  FuzzyRuleConsequent* fastLeftSpeedWheel = new FuzzyRuleConsequent();
  	fastLeftSpeedWheel->addOutput(fastLeftSpeed);
  FuzzyRuleConsequent* averageLeftSpeedWheel = new FuzzyRuleConsequent();
  	averageLeftSpeedWheel->addOutput(averageLeftSpeed);
  FuzzyRuleConsequent* lowerLeftSpeedWheel = new FuzzyRuleConsequent();
  	lowerLeftSpeedWheel->addOutput(lowerLeftSpeed);
  	
  //connection the antecedent rules with consequent
  FuzzyRule* fuzzyRule01 = new FuzzyRule(1,smallFrontDistanceAndsmallLateralDistance,fastRightSpeedWheel);
  	fuzzy->addFuzzyRule(fuzzyRule01);
  FuzzyRule* fuzzyRule02 = new FuzzyRule(2,smallFrontDistanceAndsmallLateralDistance,lowerLeftSpeedWheel);
  	fuzzy->addFuzzyRule(fuzzyRule02);
  FuzzyRule* fuzzyRule03 = new FuzzyRule(3,smallFrontDistanceAndsafeLateralDistance,fastRightSpeedWheel);
  	fuzzy->addFuzzyRule(fuzzyRule03);
  FuzzyRule* fuzzyRule04 = new FuzzyRule(4,smallFrontDistanceAndsafeLateralDistance,lowerLeftSpeedWheel);
  	fuzzy->addFuzzyRule(fuzzyRule04);
  FuzzyRule* fuzzyRule05 = new FuzzyRule(5,smallFrontDistanceAndbigLateralDistance,fastRightSpeedWheel);
  	fuzzy->addFuzzyRule(fuzzyRule05);
  FuzzyRule* fuzzyRule06 = new FuzzyRule(6,smallFrontDistanceAndbigLateralDistance,lowerLeftSpeedWheel);	
  	fuzzy->addFuzzyRule(fuzzyRule06);
  FuzzyRule* fuzzyRule07 = new FuzzyRule(7,safeFrontDistanceAndsmallLateralDistance,fastRightSpeedWheel);
  	fuzzy->addFuzzyRule(fuzzyRule07);
  FuzzyRule* fuzzyRule08 = new FuzzyRule(8,safeFrontDistanceAndsmallLateralDistance,averageLeftSpeedWheel);
  	fuzzy->addFuzzyRule(fuzzyRule08);
  FuzzyRule* fuzzyRule09 = new FuzzyRule(9,safeFrontDistanceAndsafeLateralDistance,fastRightSpeedWheel);
  	fuzzy->addFuzzyRule(fuzzyRule09);
  FuzzyRule* fuzzyRule10 = new FuzzyRule(10,safeFrontDistanceAndsafeLateralDistance,fastLeftSpeedWheel);
  	fuzzy->addFuzzyRule(fuzzyRule10);
  FuzzyRule* fuzzyRule11 = new FuzzyRule(11,safeFrontDistanceAndbigLateralDistance,averageRightSpeedWheel);
  	fuzzy->addFuzzyRule(fuzzyRule11);
  FuzzyRule* fuzzyRule12 = new FuzzyRule(12,safeFrontDistanceAndbigLateralDistance,fastLeftSpeedWheel);
  	fuzzy->addFuzzyRule(fuzzyRule12);
  FuzzyRule* fuzzyRule13 = new FuzzyRule(13,bigFrontDistanceAndsmallLateralDistance,fastRightSpeedWheel);
  	fuzzy->addFuzzyRule(fuzzyRule13);
  FuzzyRule* fuzzyRule14 = new FuzzyRule(14,bigFrontDistanceAndsmallLateralDistance,averageLeftSpeedWheel);
  	fuzzy->addFuzzyRule(fuzzyRule14);
  FuzzyRule* fuzzyRule15 = new FuzzyRule(15,bigFrontDistanceAndsafeLateralDistance,fastRightSpeedWheel);
  	fuzzy->addFuzzyRule(fuzzyRule15);
  FuzzyRule* fuzzyRule16 = new FuzzyRule(16,bigFrontDistanceAndsafeLateralDistance,fastLeftSpeedWheel);
  	fuzzy->addFuzzyRule(fuzzyRule16);
  FuzzyRule* fuzzyRule17 = new FuzzyRule(17,bigFrontDistanceAndbigLateralDistance,lowerRightSpeedWheel);
  	fuzzy->addFuzzyRule(fuzzyRule17);
  FuzzyRule* fuzzyRule18 = new FuzzyRule(18,bigFrontDistanceAndbigLateralDistance,fastLeftSpeedWheel);
  	fuzzy->addFuzzyRule(fuzzyRule18);	




  	
}

void loop() {
  // put your main code here, to run repeatedly:

  	//set values ​​of control pins to allow the forward movement of the robot
  	digitalWrite(OUTPUTPIN_RIGHT_1,LOW);
        digitalWrite(OUTPUTPIN_RIGHT_2,HIGH);
        digitalWrite(OUTPUTPIN_LEFT_1,LOW);
        digitalWrite(OUTPUTPIN_LEFT_2,HIGH);

        //calculating the distance from the front sensor
        distanceFront = calculateDistanceSensor(TRIGPIN_FRONT,ECHOPIN_FRONT);

        //calculating the distance from the right sensor
        distanceRight = calculateDistanceSensor(TRIGPIN_LATERAL,ECHOPIN_LATERAL);


        //modifying the fuzzy inputs according to the calculated distances
        fuzzy->setInput(1,distanceFront);
        fuzzy->setInput(2,distanceRight);

        fuzzy->fuzzify();


        //receives the output values ​​of the fuzzy system

        //if the lateral sensor is on the right side then the outputs are:
        outputRight = fuzzy->defuzzify(1);
        outputLeft = fuzzy->defuzzify(2);


        //if the lateral sensor is on the left side then the outputs are:
        //outputRight = fuzzy->defuzzify(2);
        //outputLeft = fuzzy->defuzzify(1);

        analogWrite(ENABLEPIN_RIGHT,int(outputRight));
        analogWrite(ENABLEPIN_LEFT,int(outputLeft));

        //Serial.print(distanceFront);
        //Serial.print("|");
        //Serial.print(distanceRight);
        //Serial.print("|");
        //Serial.print(int(outputRight));
        //Serial.print("|");
        //Serial.println(int(outputLeft));
        
        delay(200);
        

}

float calculateDistanceSensor(int trigPin, int echoPin){
	digitalWrite(trigPin,LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin,HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin,LOW);
  
        unsigned long duration = pulseIn(echoPin,HIGH);
        return duration/58;
}
