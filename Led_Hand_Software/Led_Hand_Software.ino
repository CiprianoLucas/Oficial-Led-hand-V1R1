#include <EEPROM.h>

#define Pol_        PD5
#define Ind_        PD4
#define Med_        PD1
#define Ane_        PA3
#define Min_        PC7
#define Polegar     PC6
#define Indicador   PC5
#define Medio       PC4
#define Anelar      PD3
#define Minimo      PC3
#define Off         PB4
#define On          PB5
#define Next        PA1
#define Back        PA2
#define Analogic    PD6
#define Geral       PD2


byte gesto = 0;
byte m1 = 0;
byte m2 = 0;
byte on_off = 1;
byte stateNext = 0;
byte stateBack = 0;
byte stateOn = 0;
byte tipoTime = 0;
byte stateFreio = 0;

unsigned long tempo;
unsigned long tempo1;
unsigned long tempoM;

void desligar(){
   digitalWrite(Pol_,HIGH);
  digitalWrite(Ind_,HIGH);
  digitalWrite(Med_,HIGH);
  digitalWrite(Ane_,HIGH);
  digitalWrite(Min_,HIGH);
  digitalWrite(Polegar,LOW);
  digitalWrite(Indicador,LOW);
  digitalWrite(Medio,LOW);
  digitalWrite(Anelar,LOW);
  digitalWrite(Minimo,LOW);
  digitalWrite(Geral,LOW);
}

void ligar(){
 digitalWrite(Geral,HIGH);
  
     if(gesto%2>=1){
    digitalWrite(Minimo, HIGH);
    digitalWrite(Min_, HIGH);
  }
  else{
    digitalWrite(Minimo, LOW);
    digitalWrite(Min_, LOW);
  }

  if(gesto%4>=2){
    digitalWrite(Anelar, HIGH);
    digitalWrite(Ane_, HIGH);
  }
  else{
    digitalWrite(Anelar, LOW);
    digitalWrite(Ane_, LOW);
  }

  if(gesto%8>=4){
    digitalWrite(Medio, HIGH);
    digitalWrite(Med_, HIGH);
  }
  else{
    digitalWrite(Medio, LOW);
    digitalWrite(Med_, LOW);
  }

  if(gesto%16>=8){
    digitalWrite(Indicador, HIGH);
    digitalWrite(Ind_, HIGH);
  }
  else{
    digitalWrite(Indicador, LOW);
    digitalWrite(Ind_, LOW);
  }


  if(gesto%32>=16){
    digitalWrite(Polegar, HIGH);
    digitalWrite(Pol_, HIGH);
  }
  else{
    digitalWrite(Polegar, LOW);
    digitalWrite(Pol_, LOW);
  }
}


void setup() {
  
pinMode(Pol_, OUTPUT);
pinMode(Ind_, OUTPUT);
pinMode(Med_, OUTPUT);
pinMode(Ane_, OUTPUT);
pinMode(Min_, OUTPUT);
pinMode(Polegar, OUTPUT);
pinMode(Indicador, OUTPUT);
pinMode(Medio,OUTPUT);
pinMode(Anelar, OUTPUT);
pinMode(Minimo, OUTPUT);
pinMode(Geral, OUTPUT);

pinMode(Off, INPUT);
pinMode(On, INPUT);
pinMode(Next, INPUT);
pinMode(Back, INPUT);

  gesto=31;
  ligar();
  delay(300);
  gesto=9;
  ligar();
  delay(300);
  gesto=12;
  ligar();
  delay(300);
  gesto=17;
  ligar();
  delay(300);
  gesto=28;
  ligar();
  delay(300);
  
gesto=EEPROM_read(0);
m1=EEPROM_read(1);
m2=EEPROM_read(2);
tipoTime=EEPROM_read(3);
}

void loop() {

if(on_off>0){

  ligar();
  
  if(digitalRead(Next)==HIGH && stateNext==0){
    stateNext=1;
    tempo1=0;
    tempoM=tempo;
  }
  
  if(digitalRead(Next)==LOW && stateNext==1){
    stateNext=0;
    gesto++;
    if(gesto>31){  gesto=0;}
    EEPROM_write(0,gesto);
  }
  
  if(tempoM+200<=tempo && stateNext==1){
    m1=gesto;
    stateNext=2;
    EEPROM_write(1,m1);
    desligar();
    delay(200);
    ligar();
    delay(200);
    desligar();
    delay(200);
  }
  
  if(digitalRead(Next)==LOW && stateNext==2){
    stateNext=0;
  }



  if(digitalRead(Back)==HIGH && stateBack==0){
    stateBack=1;
    tempo1=0;
    tempoM=tempo;
  }
  
  if(digitalRead(Back)==LOW && stateBack==1){
    stateBack=0;
    gesto--;
    if(gesto>31){  gesto=31;}
    EEPROM_write(0,gesto);
  }
  
  if(tempoM+200<=tempo && stateBack==1){
    m2=gesto;
    stateBack=2;
    EEPROM_write(2,m2);
    desligar();
    delay(200);
    ligar();
    delay(200);
    desligar();
    delay(200);
  }
  if(digitalRead(Back)==LOW && stateBack==2){
    stateBack=0;
  }



  if(digitalRead(On)==HIGH && stateOn==0){
    stateOn=1;
    tempo1=0;
    tempoM=tempo;
  }
  
  if(digitalRead(On)==LOW && stateOn==1){ stateOn=0; }
  
  if(tempoM+300<=tempo && stateOn==1){
    tipoTime++;
    tempo1=0;
    if(tipoTime>2){  tipoTime=0;}

    stateOn=2;
    EEPROM_write(3,tipoTime);
    
    for(int i = 0; i < tipoTime+3; i++){      
    desligar();
    delay(200);
    ligar();
    delay(200);
  }
  }
  
  if(digitalRead(On)==LOW && stateOn==2){
    stateOn=0;
  }

  


  if(tipoTime==1){   tempo1++;  }
  if(analogRead(Analogic)<431 && tipoTime==2){  tempo1++; }
  if(digitalRead(Off)== HIGH || (tempo1>60000 && tipoTime==2) || (tempo1>500 && tipoTime==1 && stateFreio != 1)
  || (analogRead(Analogic)<590 && stateFreio == 1)){   on_off=0;  }

 tempo++;
}

else{
  desligar();
  tempo=0;
  tempo1=0;
  stateBack=0;
  stateNext=0;
  stateOn=0;

  if(stateFreio != 2){
  stateFreio=0;}

  if(digitalRead(On)== HIGH){
    on_off=1;
    gesto=EEPROM_read(0);
  }
  
  if(digitalRead(Next)== HIGH){
    on_off=1;
    gesto=m1;
    stateNext=3;
    stateBack=3;
    stateFreio=0;
  }
  
  if(digitalRead(Back)== HIGH){
    on_off=1;
    gesto=m2;
    stateBack=3;
    stateNext=3;
    
  }

  if(digitalRead(Off)==HIGH){
    stateFreio=2;
  }

  if(analogRead(Analogic)>620 && tipoTime == 1 && stateFreio == 0){
    on_off=1;
    gesto=m1;
    stateNext=3;
    stateBack=3;
    stateFreio=1;
  }
  
}


delay(10);

}
