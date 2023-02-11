#include <Wire.h>
#define pin1 4
#define pin2 2
#define pin3 15
#define pin4 32
#define pin5 35
#define pin6 34
#define I2C_SLAVE_ADDR 0x04 // 4 in hexadecimal
int angle=86,right=110,left=110;
int error=0, sum=0,errlast=0,errall=0,u=0;
float kp=3,ki=0.02,kd=0.02,ks=0.5,kw=0.05;
int val[6]={0,0,0,0,0,0};
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  errlast=error;
  error=0;
  sum=0;
  u=0;
  val[1] = analogRead(pin1);  // read the input pin
  val[2] = analogRead(pin2);
  val[3] = analogRead(pin3);
  val[4] = analogRead(pin4);
  val[5] = analogRead(pin5);
  val[6] = analogRead(pin6);
  val[1]=constrain(val[1],310,2900);
  val[2]=constrain(val[2],710,4095);
  val[3]=constrain(val[3],0,700);
  val[4]=constrain(val[4],740,4095);
  val[5]=constrain(val[5],470,3300);
  val[6]=constrain(val[6],400,2680);
  val[1]=map(val[1],310,2900,0,4095);
  val[2]=map(val[2],710,4095,0,4095);
  val[3]=map(val[3],0,700,0,4095);
  val[4]=map(val[4],740,4095,0,4095);
  val[5]=map(val[5],470,3300,0,4095);
  val[6]=map(val[6],400,2680,0,4095);
  error=error+val[1]*37+val[2]*22+val[3]*6+val[4]*(-10)+val[4]*(-28)+val[4]*(-43);
  for(int i=1;i<=6;i++)
    sum=sum+val[i];
  error=error/sum;
  errall=errall+error;
  u=kp*error+ki*errall+kd*(error-errlast);
  angle=86+u*ks;
  right=110-u*kw;
  left=110+u*kw;

  if(val[1]>2000&&(val[2]>2000&&(val[3]>2000&&(val[4]>000&&(val[5]>2000&&(val[6]>2000))))))
  {
    if (error<0)
    {
      angle=106;
      left=-110;
      right=-100;
    }

    if (error>0)
    {
      angle=66;
      left=-100;
      right=-110;
    }
  }

  /*Serial.println(val[1]);
  Serial.println("2 value is");
  Serial.println(val[2]);
  Serial.println("3 value is");
  Serial.println(val[3]);
  Serial.println("4 value is");
  Serial.println(val[4]);
  Serial.println("5 value is");
  Serial.println(val[5]);
  Serial.println("6 value is");
  Serial.println(val[6]);
  Serial.println(u);  
  delay(3000);*/

  transmit(left,right,angle);  
}

void transmit(int left,int right,int angle)
{
  Wire.beginTransmission(I2C_SLAVE_ADDR); // transmit to device #4
  Wire.write((byte)((left & 0x0000FF00) >> 8));    // first byte of x, containing bits 16 to 9
  Wire.write((byte)(left & 0x000000FF));           // second byte of x, containing the 8 LSB - bits 8 to 1
  Wire.write((byte)((right & 0x0000FF00) >> 8));   
  Wire.write((byte)(right & 0x000000FF));          
  Wire.write((byte)((angle & 0x0000FF00) >> 8));    
  Wire.write((byte)(angle & 0x000000FF));
  Wire.endTransmission();   // stop transmitting
}
