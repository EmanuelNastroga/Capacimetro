#define pinCargaMF 12
#define pinCargaUF 11
#define pinCargaNF 10
#define pinCargaPF 9

#define pinDescarga 8
double relacaoCarga= 0.628;

double resVal[]={1000000,470000,1000,390};

double tBase=0;
double tCarga=0;
int limiarCarga=0;
double tDescarga=0;
double multiplicador;
String unidadeCap;
double cap=0;
double res=0;
double tempoMaxMedida=1000000;//1s

void setup() {
  // put your setup code here, to run once:
ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2)); // clear prescaler bits

ADCSRA |= bit (ADPS0);                               //   2  
//ADCSRA |= bit (ADPS1);                               //   4  
//ADCSRA |= bit (ADPS0) | bit (ADPS1);                 //   8  
//ADCSRA |= bit (ADPS2);                               //  16 
//ADCSRA |= bit (ADPS0) | bit (ADPS2);                 //  32 
//ADCSRA |= bit (ADPS1) | bit (ADPS2);                 //  64 
//ADCSRA |= bit (ADPS0) | bit (ADPS1) | bit (ADPS2);   // 128
//////////////////////////////////////////////////////////////////////////////////////
pinMode(pinDescarga,OUTPUT);

pinMode(pinCargaMF,INPUT);
pinMode(pinCargaUF,INPUT);
pinMode(pinCargaNF,INPUT);
pinMode(pinCargaPF,INPUT);

Serial.begin(115000);

limiarCarga = (5*relacaoCarga)*1023/5;
Serial.println(limiarCarga);
delay(500);

}

void loop() {
  

Serial.println(String((int)capacimetro()+unidadeCap));
delay(500);

}

double capacimetro()
{
  bool medido=false;
  int escala =0;//0=PF 1=NF 2=UF 3=MF

 while(!medido && escala < 4)
 {
    
  medido=true;
  res= resVal[escala];
  Serial.println(escala);
  //carga do capacitor
  switch(escala)
  {
    case 0:
            limiarCarga = (5*(relacaoCarga-0.01))*1023/5;
            tempoMaxMedida=1000;//em us t= rPF*capPfMax
            pinMode(pinCargaMF,INPUT);
            pinMode(pinCargaUF,INPUT);
            pinMode(pinCargaNF,INPUT);
            pinMode(pinCargaPF,OUTPUT); 
            
            digitalWrite(pinCargaPF,HIGH); 
            break;
    case 1:
            limiarCarga = (5*(relacaoCarga+0.03))*1023/5;
            tempoMaxMedida=469530;//em us t= rNF*capNfMax
            pinMode(pinCargaMF,INPUT);
            pinMode(pinCargaUF,INPUT);
            pinMode(pinCargaNF,OUTPUT);
            pinMode(pinCargaPF,INPUT);

            digitalWrite(pinCargaNF,HIGH); 
            break;
    case 2:
            limiarCarga = (5*relacaoCarga)*1023/5;
            tempoMaxMedida=999000;//em us t= rUF*capUfMax
            pinMode(pinCargaMF,INPUT);
            pinMode(pinCargaUF,OUTPUT);
            pinMode(pinCargaNF,INPUT);
            pinMode(pinCargaPF,INPUT); 

            digitalWrite(pinCargaUF,HIGH); 
            break;
    case 3:
            limiarCarga = (5*relacaoCarga)*1023/5;
            tempoMaxMedida=4000000;//em us t= rMF*capMfMax
            pinMode(pinCargaMF,OUTPUT);
            pinMode(pinCargaUF,INPUT);
            pinMode(pinCargaNF,INPUT);
            pinMode(pinCargaPF,INPUT); 

            digitalWrite(pinCargaMF,HIGH); 
            break;
  }

  
 digitalWrite(pinDescarga,LOW); 
 tBase=micros();

  while(analogRead(A0)<limiarCarga)
  {

    if(micros()-tBase>tempoMaxMedida)
    {
      Serial.println("ERRO CARGA");
      medido=false;
      break;
    }
    
  }
  
  if(medido)
  {
    tCarga=micros()-tBase;// calcula o tempo de carga
  }
  else
  {
    escala++;
  }
  
  //descarga do capacitor
      
  switch(escala)
  {
    case 0:
            pinMode(pinCargaMF,INPUT);
            pinMode(pinCargaUF,INPUT);
            pinMode(pinCargaNF,INPUT);
            pinMode(pinCargaPF,OUTPUT); 
            
            digitalWrite(pinCargaPF,LOW); 
            break;
    case 1:
            pinMode(pinCargaMF,INPUT);
            pinMode(pinCargaUF,INPUT);
            pinMode(pinCargaNF,OUTPUT);
            pinMode(pinCargaPF,INPUT);

             digitalWrite(pinCargaNF,LOW); 
            break;
    case 2:
            pinMode(pinCargaMF,INPUT);
            pinMode(pinCargaUF,OUTPUT);
            pinMode(pinCargaNF,INPUT);
            pinMode(pinCargaPF,INPUT); 

            digitalWrite(pinCargaUF,LOW); 
            break;
    case 3:
            pinMode(pinCargaMF,OUTPUT);
            pinMode(pinCargaUF,INPUT);
            pinMode(pinCargaNF,INPUT);
            pinMode(pinCargaPF,INPUT); 

            digitalWrite(pinCargaMF,LOW); 
            break;
  }
      
      digitalWrite(pinDescarga,HIGH); //inicia descarga

      tBase=micros();
      while(analogRead(A0)>3)
      {
        if(micros()-tBase>4000000)
        {
          Serial.println("ERRO DESCARGA");
          break;
        }
      }
}   
////////////////////////////////////////////////////////
  //Serial.println(tCarga); 
  if(tCarga>=(res*0.000001)*1000000)//res/cap
  {
    multiplicador=1000000;
    unidadeCap="uf";
  }
  else
  if(tCarga>=(res*0.000000001)*1000000)
  {
    multiplicador=1000000000;
    unidadeCap="nf";
  }
  else
  if(tCarga>=(res*0.000000000001)*1000000)
  {
    multiplicador=1000000000000;
    unidadeCap="pf";
  }
  return cap=((tCarga/1000000)/res)*multiplicador;//resultado capacitancia
 

  }


