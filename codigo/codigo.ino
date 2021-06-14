//programa abrir e fechar toldo, com sensor de chuva

#include "EmonLib.h" //incluisao biblioteca do sensor de corrente
 
#define CURRENT_CAL 240.0 //valor de calibração
float ruido = 14.5; //ruino com carga desligada, ajustar até dar zero com carga desligada
EnergyMonitor emon1; //cria uma estância

const int pinoSensor = A0; //sensor de corrente no pino A0
const int botao_abre = 3; //botao abre no pino 3
const int botao_fecha = 2; //botao fecha no pino 4
const int sensor_chuva = 4; //sensor de chuva no pino 5
const int rele_abre = 12; // rele abre no pino 11
const int rele_fecha = 11; //rele fecha no pino 12


int estado_botao_abre = 0;
int estado_botao_fecha = 0;
int estado_sensor_chuva = 0;
int selo_abre = 0;  
int selo_fecha = 0;
int ler_botao_abre = 0;
int ler_botao_fecha = 0;
int ler_sensor_chuva = 0;
int esta_fechado = 0;
int esta_aberto = 0;
 
void setup() {
  pinMode(botao_abre, INPUT);
  pinMode(botao_fecha, INPUT);
  pinMode(sensor_chuva, INPUT);
  pinMode(rele_abre, OUTPUT);
  pinMode(rele_fecha, OUTPUT);
  
  Serial.begin(9600);

  emon1.current(pinoSensor, CURRENT_CAL); //passa os parametros do pino analogico para função

}
 
void loop() {
  emon1.calcVI(17,100); //função de cálculo, 17 ms/tempo limite para fazer medição
  double currentDraw = emon1.Irms; //variavel recebe valor de corrente RMS obtido
  currentDraw = currentDraw-ruido; //variável recebe valor resultante da corrente RMS menos o ruido
  
  if(currentDraw < 0){ //se o valor da variável for menor que 0, faz 
      currentDraw = 0; //variável recebe 0
  }
  
  
  estado_botao_abre = digitalRead(botao_abre);
  estado_botao_fecha = digitalRead(botao_fecha);
  estado_sensor_chuva = digitalRead(sensor_chuva);  

  if (currentDraw > 30){
    selo_abre = LOW;
    selo_fecha = LOW;
  }

//abrir toldo
        
   if (selo_fecha == LOW) {     
      
      if (ler_botao_abre == LOW){
      
         if (estado_botao_abre == HIGH) {
        selo_abre = !selo_abre;
        ler_botao_abre = HIGH; 
        }
      }

      if (estado_botao_abre == LOW) {
      ler_botao_abre = LOW;
      }
         
   }
 
   if (selo_abre == HIGH){
          digitalWrite(rele_abre, LOW);
      } else{
      digitalWrite(rele_abre, HIGH);
      }
 
//fechar toldo
     
   if (selo_abre == LOW){
   
         if (ler_botao_fecha == LOW){
      
            if (estado_botao_fecha == HIGH) {
              selo_fecha = !selo_fecha;
              ler_botao_fecha = HIGH;
            }
            }

         if (ler_sensor_chuva == LOW){
      
            if (estado_sensor_chuva == LOW) {
            selo_fecha = !selo_fecha;
            ler_sensor_chuva = HIGH;
            }
            }

         if (estado_botao_fecha == LOW) {
            ler_botao_fecha = LOW;
         }
  
         if (estado_sensor_chuva == HIGH){
            ler_sensor_chuva = LOW;
         }

         if (selo_fecha == HIGH){
            digitalWrite(rele_fecha, LOW);
         } else{
            digitalWrite(rele_fecha, HIGH);
         }
   }

   if (selo_fecha == HIGH){
      digitalWrite(rele_fecha, LOW);
   } else{
      digitalWrite(rele_fecha, HIGH);
   }   
 
}
