#include <Fuzzy.h>
#include <FuzzyComposition.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzyOutput.h>
#include <FuzzyRule.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzySet.h>




  const int enableRight = 5;
  const int enableLeft = 6;
  const int inputRight1 = 22;
  const int inputRight2 = 23;
  const int inputLeft1 = 24;
  const int inputLeft2 = 25;
  const int trigPinFront = 53;
  const int echoPinFront = 52;
  const int trigPinRight = 46;
  const int echoPinRight = 47;

// create a new fuzzy object
Fuzzy* fuzzy  = new Fuzzy();

void setup() {


	  pinMode(enableRight,OUTPUT);
          pinMode(enableLeft,OUTPUT);
          pinMode(inputRight1,OUTPUT);
          pinMode(inputRight2,OUTPUT);
          pinMode(inputLeft1,OUTPUT);
          pinMode(inputLeft2,OUTPUT);
          pinMode(trigPinFront,OUTPUT);
          pinMode(echoPinFront,INPUT);
          pinMode(trigPinRight,OUTPUT);
          pinMode(echoPinRight,INPUT);
          Serial.begin(9600);


// create  two new FuzzyInput objects
  FuzzyInput* distSensorFront = new FuzzyInput(1);
  FuzzyInput* distSensorRight = new FuzzyInput(2);
 
 // create the FuzzySets that  copose the FuzzyInput objects 
  FuzzySet* smallFront = new FuzzySet(0,0,12.5,25.0);
  distSensorFront->addFuzzySet(smallFront);//add the FuzzySet in the FuzzyInput object
  FuzzySet* smallRight = new FuzzySet(0,0,12.5,25.0);
  distSensorRight->addFuzzySet(smallRight);
  
  FuzzySet*  safeFront = new FuzzySet(12.5,25.0,25.0,37.5);
  distSensorFront->addFuzzySet(safeFront);
  FuzzySet*  safeRight = new FuzzySet(12.5,25.0,25.0,37.5);
  distSensorRight->addFuzzySet(safeRight);
  
  FuzzySet*  bigFront = new FuzzySet(25.0,37.5,50.0,50.0);
  distSensorFront->addFuzzySet(bigFront);
  FuzzySet*  bigRight = new FuzzySet(25.0,37.5,50.0,50.0);
  distSensorRight->addFuzzySet(bigRight);

  // add the FuzzyInputs in the Fuzzy Object
  fuzzy->addFuzzyInput(distSensorRight);
  fuzzy->addFuzzyInput(distSensorFront);
  
  //create two new FuzzyOutput objects
  FuzzyOutput* velocityWheelRight = new FuzzyOutput(1);
  FuzzyOutput* velocityWheelLeft = new FuzzyOutput(2);

  // create the FuzzySets that  copose the FuzzyOutput objects 
  FuzzySet* slowRight = new FuzzySet(0,0,63.75,127.5);
  velocityWheelRight->addFuzzySet(slowRight);//add the FuzzySet in the FuzzyInput object
  FuzzySet* slowLeft = new FuzzySet(0,0,63.75,127.5);
  velocityWheelLeft->addFuzzySet(slowLeft);

  FuzzySet* averageRight = new FuzzySet(63.75,127.5,127.5,191.25);
  velocityWheelRight->addFuzzySet(averageRight);
  FuzzySet* averageLeft = new FuzzySet(63.75,127.5,127.5,191.25);
  velocityWheelLeft->addFuzzySet(averageLeft);

  FuzzySet* fastRight = new FuzzySet(127.5,191.25,255,255);
  velocityWheelRight->addFuzzySet(fastRight);
  FuzzySet* fastLeft = new FuzzySet(127.5,191.25,255,255);
  velocityWheelLeft->addFuzzySet(fastLeft);


  //add the FuzzyOutputs in the Fuzzy object
  fuzzy->addFuzzyOutput(velocityWheelRight);
  fuzzy->addFuzzyOutput(velocityWheelLeft);


  // Building FuzzyRule
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


  	digitalWrite(inputRight1,LOW);
        digitalWrite(inputRight2,HIGH);
        digitalWrite(inputLeft1,LOW);
        digitalWrite(inputLeft2,HIGH);


   	digitalWrite(trigPinFront,LOW);
        delayMicroseconds(2);
        digitalWrite(trigPinFront,HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPinFront,LOW);
  
        unsigned long duracaoFront = pulseIn(echoPinFront,HIGH);
        int distanciaFront = duracaoFront/58;

        digitalWrite(trigPinRight,LOW);
        delayMicroseconds(2);
        digitalWrite(trigPinRight,HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPinRight,LOW);
  
        unsigned long duracaoRight = pulseIn(echoPinRight,HIGH);
        int distanciaRight = duracaoRight/58;



        fuzzy->setInput(1,distanciaFront);
        fuzzy->setInput(2,distanciaRight);

        fuzzy->fuzzify();

        float outputRight = fuzzy->defuzzify(1);
        float outputLeft = fuzzy->defuzzify(2);


        analogWrite(enableRight,int(outputRight));
        analogWrite(enableLeft,int(outputLeft));

        Serial.print(distanciaFront);
        Serial.print("|");
        Serial.print(distanciaRight);
        Serial.print("|");
        Serial.print(int(outputRight));
        Serial.print("|");
        Serial.println(int(outputLeft));
        
        delay(200);
        

}

