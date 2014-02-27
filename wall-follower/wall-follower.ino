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
  #define ENABLEPIN_RIGHT     5  //PWM pin
  #define ENABLEPIN_LEFT      6  //PWM pin
  #define OUTPUTPIN_RIGHT_1  22
  #define OUTPUTPIN_RIGHT_2  23
  #define OUTPUTPIN_LEFT_1   24
  #define OUTPUTPIN_LEFT_2   25
  #define TRIGPIN_FRONT      53
  #define ECHOPIN_FRONT      52
  #define TRIGPIN_RIGHT      46
  #define ECHOPIN_RIGHT      47


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
          pinMode(TRIGPIN_RIGHT,OUTPUT);
          pinMode(ECHOPIN_RIGHT,INPUT);
          //Serial.begin(9600);


// create  two new FuzzyInput objects
  FuzzyInput* distSensorFront = new FuzzyInput(1);
  FuzzyInput* distSensorRight = new FuzzyInput(2);
 
 // create the FuzzySets that  copose the FuzzyInput objects 
  FuzzySet* smallFront = new FuzzySet(MIN_DISTANCE,MIN_DISTANCE,MAX_DISTANCE/4,MAX_DISTANCE/2);
  distSensorFront->addFuzzySet(smallFront);//add the FuzzySet in the FuzzyInput object
  FuzzySet* smallRight = new FuzzySet(MIN_DISTANCE,MIN_DISTANCE,MAX_DISTANCE/4,MAX_DISTANCE/2);
  distSensorRight->addFuzzySet(smallRight);
  
  FuzzySet*  safeFront = new FuzzySet(MAX_DISTANCE/4,MAX_DISTANCE/2,MAX_DISTANCE/2,MAX_DISTANCE*3/4);
  distSensorFront->addFuzzySet(safeFront);
  FuzzySet*  safeRight = new FuzzySet(MAX_DISTANCE/4,MAX_DISTANCE/2,MAX_DISTANCE/2,MAX_DISTANCE*3/4);
  distSensorRight->addFuzzySet(safeRight);
  
  FuzzySet*  bigFront = new FuzzySet(MAX_DISTANCE/2,MAX_DISTANCE*3/4,MAX_DISTANCE,MAX_DISTANCE);
  distSensorFront->addFuzzySet(bigFront);
  FuzzySet*  bigRight = new FuzzySet(MAX_DISTANCE/2,MAX_DISTANCE*3/4,MAX_DISTANCE,MAX_DISTANCE);
  distSensorRight->addFuzzySet(bigRight);

  // add the FuzzyInputs in the Fuzzy Object
  fuzzy->addFuzzyInput(distSensorRight);
  fuzzy->addFuzzyInput(distSensorFront);
  
  //create two new FuzzyOutput objects
  FuzzyOutput* velocityWheelRight = new FuzzyOutput(1);
  FuzzyOutput* velocityWheelLeft = new FuzzyOutput(2);

  // create the FuzzySets that  copose the FuzzyOutput objects 
  FuzzySet* slowRight = new FuzzySet(MIN_SPEED,MIN_SPEED,MAX_SPEED/4,MAX_SPEED/2);
  velocityWheelRight->addFuzzySet(slowRight);//add the FuzzySet in the FuzzyInput object
  FuzzySet* slowLeft = new FuzzySet(MIN_SPEED,MIN_SPEED,MAX_SPEED/4,MAX_SPEED/2);
  velocityWheelLeft->addFuzzySet(slowLeft);

  FuzzySet* averageRight = new FuzzySet(MAX_SPEED/4,MAX_SPEED/2,MAX_SPEED/2,MAX_SPEED*3/4);
  velocityWheelRight->addFuzzySet(averageRight);
  FuzzySet* averageLeft = new FuzzySet(MAX_SPEED/4,MAX_SPEED/2,MAX_SPEED/2,MAX_SPEED*3/4);
  velocityWheelLeft->addFuzzySet(averageLeft);

  FuzzySet* fastRight = new FuzzySet(MAX_SPEED/2,MAX_SPEED*3/4,MAX_SPEED,MAX_SPEED);
  velocityWheelRight->addFuzzySet(fastRight);
  FuzzySet* fastLeft = new FuzzySet(MAX_SPEED/2,MAX_SPEED*3/4,MAX_SPEED,MAX_SPEED);
  velocityWheelLeft->addFuzzySet(fastLeft);


  //add the FuzzyOutputs in the Fuzzy object
  fuzzy->addFuzzyOutput(velocityWheelRight);
  fuzzy->addFuzzyOutput(velocityWheelLeft);


  //building the antecedents fuzzy rules
  FuzzyRuleAntecedent* smallFrontAndSmallRight = new FuzzyRuleAntecedent();
  	smallFrontAndSmallRight->joinWithAND(smallFront,smallRight); 
  FuzzyRuleAntecedent* smallFrontAndSafeRight = new FuzzyRuleAntecedent();
  	smallFrontAndSafeRight->joinWithAND(smallFront,safeRight);
  FuzzyRuleAntecedent* smallFrontAndBigRight = new FuzzyRuleAntecedent();
  	smallFrontAndBigRight->joinWithAND(smallFront,bigRight);
  FuzzyRuleAntecedent* safeFrontAndSmallRight = new FuzzyRuleAntecedent();
  	safeFrontAndSmallRight->joinWithAND(safeFront,smallRight);
  FuzzyRuleAntecedent* safeFrontAndSafeRight = new FuzzyRuleAntecedent();
  	safeFrontAndSafeRight->joinWithAND(safeFront,safeRight);
  FuzzyRuleAntecedent* safeFrontAndBigRight = new FuzzyRuleAntecedent();
  	safeFrontAndBigRight->joinWithAND(safeFront,bigRight);
  FuzzyRuleAntecedent* bigFrontAndSmallRight = new FuzzyRuleAntecedent();
  	bigFrontAndSmallRight->joinWithAND(bigFront,smallRight);
  FuzzyRuleAntecedent* bigFrontAndSafeRight = new FuzzyRuleAntecedent();
  	bigFrontAndSafeRight->joinWithAND(bigFront,safeRight);
  FuzzyRuleAntecedent* bigFrontAndBigRight = new FuzzyRuleAntecedent();
  	bigFrontAndBigRight->joinWithAND(bigFront,bigRight);


  //building the consequent fuzzy rules
  FuzzyRuleConsequent* fastVeolityRight = new FuzzyRuleConsequent();
  	fastVeolityRight->addOutput(fastRight);
  FuzzyRuleConsequent* averageVelocityRight = new FuzzyRuleConsequent();
  	averageVelocityRight->addOutput(averageRight);
  FuzzyRuleConsequent* slowVelocityRight = new FuzzyRuleConsequent();
  	slowVelocityRight->addOutput(slowRight);
  FuzzyRuleConsequent* fastVelocityLeft = new FuzzyRuleConsequent();
  	fastVelocityLeft->addOutput(fastLeft);
  FuzzyRuleConsequent* averageVelocityLeft = new FuzzyRuleConsequent();
  	averageVelocityLeft->addOutput(averageLeft);
  FuzzyRuleConsequent* slowVelocityLeft = new FuzzyRuleConsequent();
  	slowVelocityLeft->addOutput(slowLeft);
  	
  //connection the antecedent rules with consequent
  FuzzyRule* fuzzyRule01 = new FuzzyRule(1,smallFrontAndSmallRight,fastVeolityRight);
  	fuzzy->addFuzzyRule(fuzzyRule01);
  FuzzyRule* fuzzyRule02 = new FuzzyRule(2,smallFrontAndSmallRight,slowVelocityLeft);
  	fuzzy->addFuzzyRule(fuzzyRule02);
  FuzzyRule* fuzzyRule03 = new FuzzyRule(3,smallFrontAndSafeRight,fastVeolityRight);
  	fuzzy->addFuzzyRule(fuzzyRule03);
  FuzzyRule* fuzzyRule04 = new FuzzyRule(4,smallFrontAndSafeRight,slowVelocityLeft);
  	fuzzy->addFuzzyRule(fuzzyRule04);
  FuzzyRule* fuzzyRule05 = new FuzzyRule(5,smallFrontAndBigRight,fastVeolityRight);
  	fuzzy->addFuzzyRule(fuzzyRule05);
  FuzzyRule* fuzzyRule06 = new FuzzyRule(6,smallFrontAndBigRight,slowVelocityLeft);	
  	fuzzy->addFuzzyRule(fuzzyRule06);
  FuzzyRule* fuzzyRule07 = new FuzzyRule(7,safeFrontAndSmallRight,fastVeolityRight);
  	fuzzy->addFuzzyRule(fuzzyRule07);
  FuzzyRule* fuzzyRule08 = new FuzzyRule(8,safeFrontAndSmallRight,averageVelocityLeft);
  	fuzzy->addFuzzyRule(fuzzyRule08);
  FuzzyRule* fuzzyRule09 = new FuzzyRule(9,safeFrontAndSafeRight,fastVeolityRight);
  	fuzzy->addFuzzyRule(fuzzyRule09);
  FuzzyRule* fuzzyRule10 = new FuzzyRule(10,safeFrontAndSafeRight,fastVelocityLeft);
  	fuzzy->addFuzzyRule(fuzzyRule10);
  FuzzyRule* fuzzyRule11 = new FuzzyRule(11,safeFrontAndBigRight,averageVelocityRight);
  	fuzzy->addFuzzyRule(fuzzyRule11);
  FuzzyRule* fuzzyRule12 = new FuzzyRule(12,safeFrontAndBigRight,fastVelocityLeft);
  	fuzzy->addFuzzyRule(fuzzyRule12);
  FuzzyRule* fuzzyRule13 = new FuzzyRule(13,bigFrontAndSmallRight,fastVeolityRight);
  	fuzzy->addFuzzyRule(fuzzyRule13);
  FuzzyRule* fuzzyRule14 = new FuzzyRule(14,bigFrontAndSmallRight,averageVelocityLeft);
  	fuzzy->addFuzzyRule(fuzzyRule14);
  FuzzyRule* fuzzyRule15 = new FuzzyRule(15,bigFrontAndSafeRight,fastVeolityRight);
  	fuzzy->addFuzzyRule(fuzzyRule15);
  FuzzyRule* fuzzyRule16 = new FuzzyRule(16,bigFrontAndSafeRight,fastVelocityLeft);
  	fuzzy->addFuzzyRule(fuzzyRule16);
  FuzzyRule* fuzzyRule17 = new FuzzyRule(17,bigFrontAndBigRight,slowVelocityRight);
  	fuzzy->addFuzzyRule(fuzzyRule17);
  FuzzyRule* fuzzyRule18 = new FuzzyRule(18,bigFrontAndBigRight,fastVelocityLeft);
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
        distanceRight = calculateDistanceSensor(TRIGPIN_RIGHT,ECHOPIN_RIGHT);


        //modifying the fuzzy inputs according to the calculated distances
        fuzzy->setInput(1,distanceFront);
        fuzzy->setInput(2,distanceRight);

        fuzzy->fuzzify();

        //receives the output values ​​of the fuzzy system
        outputRight = fuzzy->defuzzify(1);
        outputLeft = fuzzy->defuzzify(2);


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
