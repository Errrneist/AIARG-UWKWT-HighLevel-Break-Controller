// Code for brake system
// CNC Single Axis 4A TB6600 Stepper Motor Driver Controller 
// use Serial Monitor to control 115200 baud
//max compress: 4228 steps?(based off math of 1.3 inches/0.0003125 max step is 4160)
//pull out completely: 
//~2200 from full compress->completely pulled out->full compression


word  X;
word NX;
word N;
word derp;
int moveSpeed=2;
int MoveSpeed=2000; //step in Microseconds
String     inputString = "help\n"; // a string to hold incoming data
boolean stringComplete = true;     // whether the string is completet
boolean        ComData = false;    // whether com data is on when motors are moving will slow them down

# define X_ENgnd  2 //ENA-(ENA) stepper motor enable , active low      
# define X_EN_5v  3 //ENA+(+5V) stepper motor enable , active low     
# define X_DIRgnd 4 //DIR-(DIR) axis stepper motor direction control  
# define X_DIR_5v 5 //DIR+(+5v) axis stepper motor direction control  
# define X_STPgnd 6 //PUL-(PUL) axis stepper motor step control       
# define X_STP_5v 7 //PUL+(+5v) axis stepper motor step control       

void setup() {// *************************************************************     setup
pinMode (X_ENgnd ,OUTPUT); //ENA-(ENA)
pinMode (X_EN_5v ,OUTPUT); //ENA+(+5V)
pinMode (X_DIRgnd,OUTPUT); //DIR-(DIR)
pinMode (X_DIR_5v,OUTPUT); //DIR+(+5v)
pinMode (X_STPgnd,OUTPUT); //PUL-(PUL)
pinMode (X_STP_5v,OUTPUT); //PUL+(+5v)
pinMode (13,OUTPUT);
digitalWrite (X_ENgnd,  LOW); //ENA-(ENA)
digitalWrite (X_EN_5v, LOW); //ENA+(+5V) low=enabled
digitalWrite (X_DIRgnd, LOW); //DIR-(DIR)
digitalWrite (X_DIR_5v, LOW); //DIR+(+5v)
digitalWrite (X_STPgnd, LOW); //PUL-(PUL)
digitalWrite (X_STP_5v, HIGH); //PUL+(+5v)

Serial.begin(115200);
}

void serialEvent()// ********************************************************      Serial in
{ while (Serial.available()) 
  {
    char inChar = (char)Serial.read();            // get the new byte:
    if (inChar > 0)     {inputString += inChar;}  // add it to the inputString:
    if (inChar == '\n') { stringComplete = true;} // if the incoming character is a newline, set a flag so the main loop can do something about it: 
  }
}
void Help(){ // **************************************************************   Help
 Serial.println("Commands step by step guide");
 Serial.println("Type xon  -turns TB6600 on");
 Serial.println("Type x+Number(0-60000) eg x1904 -to set next move steps");
 Serial.println("Type mx  -to make motor move to next postion");
 Serial.println("Type s+Number(0-2000) eg s500 -to set Microseconds betwean steps");
 Serial.println("Type cdon -turns on postion data when moving will increase time of step");
 Serial.println("Type cdoff -turns off postion data when moving");
 //Serial.println("Type push -completely compresses brake");
 //Serial.println("Type pull -completely retracts brake");
 Serial.println("Type zero -sets counter to 0");
 Serial.println("Type ten -sets counter to 10000");
 Serial.println("Type xoff -turns TB6600 off");
// Serial.println("Type cal");

 inputString="";
}

void ENAXON(){ // *************************************************************   ENAXON
 Serial.println("ENAXON");
 digitalWrite (X_EN_5v, LOW);//ENA+(+5V) low=enabled
 inputString="";
}
void ENAXOFF(){  // ***********************************************************   ENAXOFF
 Serial.println("ENAXOFF");
 digitalWrite (X_EN_5v, HIGH);//ENA+(+5V) low=enabled
 inputString="";
}
void MSpeed(){  // ************************************************************   MoveSpeed
 inputString.setCharAt(0,' ');
 MoveSpeed=inputString.toInt();
 Serial.print("Speed=");
 Serial.println(MoveSpeed);
 inputString="";
}
void ComDataON(){  // *********************************************************   ComDataON
 ComData=true;
 Serial.println("ComDataOn");
 inputString="";
}
void ComDataOFF(){  // ********************************************************   ComDataOFF
 ComData=false;
 Serial.println("ComDataOFF");
 inputString="";
}
void NextX(){ // **************************************************************    NextX
 inputString.setCharAt(0,' ');
 NX=inputString.toDouble();
 Serial.print("NX=");
 Serial.println(NX);
 inputString="";
}
void MoveX(){ // **************************************************************    Move
double xt;
N=0;
derp=X;
if (NX>X)
{xt=NX-X; digitalWrite (X_DIR_5v,HIGH);xt=1;}
else
{xt=X-NX; digitalWrite (X_DIR_5v,LOW);xt=-1;}
Serial.print("X=");
Serial.println(X);
//X=NX;
inputString="";
}

/*void Pull() {//***************************************************************pulls out pushrod (incomplete)
double count;
double MaxStep=2000; //steps to travel distance of piston in master cylinder
digitalWrite (X_DIR_5v,LOW);
if (ComData==true) 
{for (count = 1; count <= MaxStep; count++) {
  digitalWrite (X_STP_5v, HIGH);
  Serial.print("X=");
  delayMicroseconds (MoveSpeed);
  digitalWrite (X_STP_5v, LOW);
  delayMicroseconds (MoveSpeed);
  Serial.println(X+count);
}
X=X-MaxStep;
}
else
{for (count = 1; count <= MaxStep; count++) {
  if(digitalRead(X_DIR_5v)==LOW && digitalRead(limitPin2)==LOW) {//retract
      digitalWrite (X_STP_5v, HIGH);
      delayMicroseconds (MoveSpeed);
      digitalWrite (X_STP_5v, LOW);
      delayMicroseconds (MoveSpeed);
      N++;
  }else if(digitalRead(X_DIR_5v)==LOW && digitalRead(limitPin1)==HIGH) {//switch2
      break;
    }      
}
Serial.print("X=");
Serial.println(X-MaxStep);
X=X-MaxStep;
}
inputString="";
}

void Push() {//**********compresses brake (incomplete)
double count;
double MaxStep=2000;
digitalWrite (X_DIR_5v,HIGH);
//MoveSpeed = 600;
if (ComData==true) 
{for (count = 1; count <= MaxStep; count++) {
  digitalWrite (X_STP_5v, HIGH);
  Serial.print("X=");
  delayMicroseconds (MoveSpeed);
  digitalWrite (X_STP_5v, LOW);
  delayMicroseconds (MoveSpeed);
  Serial.println(X+count);
}
X=X-MaxStep;
}
else
{for (count = 1; count <= MaxStep; count++) {
  digitalWrite (X_STP_5v, HIGH);
  delayMicroseconds (MoveSpeed);
  digitalWrite (X_STP_5v, LOW);
  delayMicroseconds (MoveSpeed);
      
}
Serial.print("X=");
X=X+MaxStep;
Serial.println(X);
}
//MoveSpeed = 1000;
inputString="";
}*/

void tenThousand() {//**********************Sets counter to 10000
  X=10000;
  Serial.print("X=");
  Serial.println(X);
  inputString="";
}
void Zero() {//******************sets counter to 0
X=0;
Serial.print("X=");
Serial.println(X);
inputString="";
}


/*void cal() {**************************calibration
 digitalWrite(X_DIR_5v,LOW); (problem with going in opposite direction?)
 for (int i = 0; i < 2;) {
 if (digitalRead(limitPin1)== HIGH && digitalRead(X_DIR_5v)== HIGH) {
  digitalWrite(X_DIR_5v,LOW);

 }else if (digitalRead(limitPin2)== HIGH && digitalRead(X_DIR_5v)==LOW) {
  digitalWrite(X_DIR_5v,HIGH);
  i++;
 }
 else {
rotate();
 }
}
inputString="";
}*/
  
void rotate() {
  digitalWrite(X_STP_5v,HIGH);
  delay(moveSpeed);
  //delayMicroseconds(MoveSpeed);
  digitalWrite(X_STP_5v,LOW);
  delay(moveSpeed);
  //delayMicroseconds(MoveSpeed);
}
void rotate2() {
   digitalWrite (X_STP_5v, HIGH);
   Serial.print("X=");
   delay(moveSpeed);
   //delayMicroseconds (MoveSpeed);
   digitalWrite (X_STP_5v, LOW);
   delay(moveSpeed);
   //delayMicroseconds (MoveSpeed);
   Serial.println(X);
}


void loop() { // **************************************************************     loop
 serialEvent(); 
 if (stringComplete) 
 {
  if (inputString=="help\n")      {Help();}  
  if (inputString=="xon\n")       {ENAXON();}   
  if (inputString=="xoff\n")      {ENAXOFF();}
  if (inputString=="cdon\n")      {ComDataON();}
  if (inputString=="cdoff\n")     {ComDataOFF();}
  if (inputString=="mx\n")        {MoveX();}
  if (inputString.charAt(0)=='s')   {MSpeed();}
  if (inputString.charAt(0)=='x')   {NextX();} 
//  if (inputString=="push\n")  {Push();}
//  if (inputString=="pull\n")   {Pull();}
  if (inputString=="zero\n")      {Zero();}
  if (inputString=="ten\n") {tenThousand();}
//  if (inputString=="cal\n")    {cal();}
  
  if (inputString !="") {Serial.println("BAD COMMAND="+inputString);}// Serial.print("\n"); }// "\t" tab      
  inputString = ""; stringComplete = false; // clear the string:
 }

}
