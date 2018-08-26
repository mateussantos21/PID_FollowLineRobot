#define motor1p1    0
#define motor1p2    1
#define motor2p1    2 
#define motor2p2    4
#define motor1pwm   3
#define motor2pwm   5
#define sensordir0  6
#define sensordir1  7
#define sensordir2  8
#define sensordir3  9
#define sensoresq0  10
#define sensoresq1  11
#define sensoresq2  12
#define sensoresq3  13


boolean dirbit0 = false, dirbit1 = false, dirbit2 = false, dirbit3 = false, 
        esqbit0 = false, esqbit1 = false, esqbit2 = false, esqbit3 = false;
int erro = 0, lasterro = 0;                   //sinal de erro
int vel1 = 0, vel2 = 0;                       //velocidade dos motores
double controle = 0, integral = 0, derivada;  //sinal de controle, integral e derivada;
double smptime;
double P = 0, I = 0, D = 0;    

ISR(TIMER1_OVF_vect)          // Rotina responsavel pelo funcionamento do timer
{
  
  TCNT1L = 0x7F;              //Inicializa o nibel mais baixo do timer1 em 0x7F
  TCNT1H = 0xC1;              //Inicializa o nibel mais alto do timer 1 em 0xC1

  dirbit0 = digitalRead (sensordir0);
  dirbit1 = digitalRead (sensordir1);  
  dirbit2 = digitalRead (sensordir2);
  dirbit3 = digitalRead (sensordir3);
  esqbit0 = digitalRead (sensoresq0);
  esqbit1 = digitalRead (sensoresq1);
  esqbit2 = digitalRead (sensoresq2);
  esqbit3 = digitalRead (sensoresq3);
  
  erro = dirbit3*8 + dirbit2*4 + dirbit1*2 + dirbit0 
       - esqbit3*8 - esqbit2*4 - esqbit1*2 - esqbit0;
  integral += erro*smptime;
  derivada = (erro-lasterro)/smptime;
  controle = (D*derivada + I*integral + P*erro);
  lasterro = erro;
}

void setup()
{
  pinMode (motor1p1, OUTPUT);
  pinMode (motor1p2, OUTPUT);
  pinMode (motor2p1, OUTPUT); 
  pinMode (motor2p2, OUTPUT);
  pinMode (motor1pwm, OUTPUT);
  pinMode (motor2pwm, OUTPUT);
  pinMode (sensordir0, INPUT);
  pinMode (sensordir1, INPUT);
  pinMode (sensordir2, INPUT);
  pinMode (sensordir3, INPUT);
  pinMode (sensoresq0, INPUT);
  pinMode (sensoresq1, INPUT);
  pinMode (sensoresq2, INPUT);
  pinMode (sensoresq3, INPUT);
  
  TCCR1A = 0x00;              //Timer 1 operando em modo normal
  TCCR1B = 0x01;              //Prescaler 1:1
  TCNT1L = 0x7F;              //Inicializa o nibel mais baixo do timer1 em 0x7F
  TCNT1H = 0xC1;              //Inicializa o nibel mais alto do timer 1 em 0xC1
  TIMSK1 = 0x01;              //Habilita interrupção do timer 1
  smptime = 0.001;            //Tempo de amostragem em segundos
  Serial.begin (9600);
}

void loop()
{
  P = analogRead (A0);
  I = 0;//(double)analogRead (A1)/50; 
  D = (double)analogRead (A2)/50;
  Serial.print (P);
  Serial.print ("   ");
  Serial.print (D);
  Serial.print ("   "); 
  Serial.println (vel2);
 
  vel1 = 80 - controle/10;
  vel2 = 80 + controle/10;
  if (vel1 > 255) vel1 = 255;
  if (vel2 > 255) vel2 = 255;
  if (vel1 < 40) vel1 = 0;
  if (vel1 < 40) vel1 = 0;
  
  digitalWrite (motor1p1, HIGH);
  digitalWrite (motor1p2, LOW);
  digitalWrite (motor2p1, HIGH);
  digitalWrite (motor2p2, LOW);
  analogWrite (motor1pwm, vel1);
  analogWrite (motor2pwm, vel2);

}
