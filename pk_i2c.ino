  /*
 * 13.09.2021
 * PK SM9336 I2C scanner
 */
/*
 * Change string to be sent to compute function to apply appropriate formula
 * List of devices supported (these are the strings to be sent too):
 *  1. SM7391
 *  2. SM9336
 */
#include<Wire.h>
// # define pi 3.17
 int a = 80;
// static long double timer;
void setup()
{
    Serial.begin(9600);
    Wire.begin();
    Serial.println("Beginning Normal mode communication");
    pinMode(10,OUTPUT);
}

void loop() 
{
    /*
     * 1. Read -> CRC4 and CRC8
     * 2. Compute
     * 3. Display
     */
    
     analogWrite(10,a);
     
     uint16_t buffer[] = {0, 0, 0};
     double pread,flow;
     
     readreg(buffer, 3);
     
     if(buffer[2] > 0)
     {
//         Serial.print("Value: " + String(buffer[1]));
          pread = compute(buffer[1], "SM7391"); 
          pread= pread+1.4;

              flow=sqrt(abs(pread)); 

        flow=  1.236*flow +(-2.672) ;  
             Serial.println("Pressure: " + String(pread) );
          Serial.println("flow is " + String(flow));
     }
     delay(100);

}

int readreg(uint16_t *buffer, int N)
{
    Wire.beginTransmission(0x6C);
    Wire.write(0x2E);
    Wire.endTransmission();
    Wire.requestFrom(0x6C, 2 * N);

    int bytes_available = Wire.available();
    if(bytes_available >= (N * 2))
    {
        for(int i = 0; i < N; ++i)
        {
            buffer[i] = Wire.read();
            buffer[i] = buffer[i] | (Wire.read() << 8);
        }   
    }
    return 0;
}
double compute(int buffer, char *string)
{
    double pread, lol = buffer;
    if(strcmp("SM7391", string) == 0)
    
    { Serial.print("raw data  ");
      Serial.println(lol);
//        pread = (+2.3) + (-0.5e+03) + (double)((lol + 26215) * 1e+03) / 52429;    
          pread = (-0.29) + (double)((lol+26214)/52428) * 0.58;
//              pread = (-0.7) + (double)((lol+26214)/52428) * 0.14;
          pread= pread * 6894.76;    //psi to pascal
    }
 
    if(strcmp("SM9336", string) == 0)
    {
        if(buffer < 32767.5)
        {
              pread = (double)buffer / 104.858;    
               
        }
        else if((buffer > 32767.5) && (buffer < 65535))
        {
              pread = (double)(round(32767.5 - buffer)) / 104.858;
        }
    }
    return pread;
}
