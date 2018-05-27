
#include <Servo.h> //BIBLIOTECA DE FUNÇÕES COM MOTORES SERVOS

// DECLARAÇÃO DOS SERVOS.
Servo servoBase; //BASE DO BRAÇO...
Servo servo1;  //SERVO 1...
Servo servo2;  //SERVO 2...
//O X, Y, Z DO PONTO
double x, y, z;


void setup()
{
  //ATRIBUIÇÃO DE PINOS PARA OS SERVOS
  servoBase.attach(3);
  servo1.attach(5);
  servo2.attach(6);
  //LEITURA DE ENTRADAS SERIAIS
  Serial.begin(9600);
  //RANDOMICO A PARTIR DO RUIDO DA PORTA A0
  randomSeed(analogRead(0));
}

//AG
void algoritimo(double x, double y, double z)
{
  //INICIALIZA POPULAÇAO
  double individuos[11][3];
  
  //MELHOR DE TODOS
  individuos[10][0] = 0;
  individuos[10][1] = 0;
  individuos[10][2] = 0;

  //INCIALIZA NOTAS
  double nota[11];
  //MELHOR NOTA
  nota[10] = 1000.0;

  double d, xi, yi, zi;

  //PARA TODOS OS SERVOS, GENES RANDOMICOS NO INICIO
  for (unsigned char i = 0; i < 10; i++)
  {
    for (unsigned char j = 0; j < 3; j++)
    {
        individuos[i][j] = random(170);
    }
  }
  int k;
  //MIL GERACOES DO AG
  while(nota[10]>0.2)
  {
    double media;
    //AVALIA E DETERMINA O MELHOR DE TODOS
    for (unsigned char i = 0; i < 10; i++)
    {
      d = (10 * cos(individuos[i][1] * 0.0174533)) + (10 * cos((individuos[i][1] * 0.0174533) + (individuos[i][2] * 0.0174533)));
      xi = (d * cos(individuos[i][0] * 0.0174533));
      yi = d * sin(individuos[i][0] * 0.0174533);
      zi = (10 * sin( individuos[i][1] * 0.0174533 ) ) + (10 * sin( (individuos[i][1] + individuos[i][2]) * 0.0174533 ) );

      //AVALIANDO
      nota[i] = sqrt(pow((x - xi), 2.0) + pow((y - yi), 2.0) + pow((z - zi), 2.0));
      media += nota[i]/10;
      if (nota[i] < nota[10])
      {
        individuos[10][0] = individuos[i][0];
        individuos[10][1] = individuos[i][1];
        individuos[10][2] = individuos[i][2];
        nota[10] = nota[i];
        //Serial.print("\nNOTA:");
        Serial.println(nota[10]);
        /*Serial.print("\nX:");
        //Serial.print(xi);
        Serial.print("\nY:");
        Serial.print(yi);
        Serial.print("\nZ:");
        Serial.print(zi);  
        Serial.print("\nANGULO BASE:");
        Serial.print(individuos[10][0]);
        Serial.print("\nANGULO S1:");
        Serial.print(individuos[10][1]);
        Serial.print("\nANGULO S2:");
        Serial.print(individuos[10][2]);*/
      }
      Serial.println(nota[10]);
    }
    
    //CROSSOVER
    for (unsigned char i = 0; i < 10; i++)
    {
      individuos[i][0] = (individuos[i][0] + individuos[10][0]) / 2.0;
      individuos[i][1] = (individuos[i][1] + individuos[10][1]) / 2.0;
      individuos[i][2] = (individuos[i][2] + individuos[10][2]) / 2.0;
    }

    //MUTAÇAO
    for (int i = 0; i < 10; i++)
    {
      //DEFINE MUTAÇAO EM SOMA OU SUBTRAÇAO;
      unsigned char num = random(8);
      
      switch (num)
      {
        case 0:
          individuos[i][0] += random(3);
          individuos[i][1] += random(3);
          individuos[i][2] += random(3);
          break;
        case 1:
          individuos[i][0] -= random(3);
          individuos[i][1] += random(3);
          individuos[i][2] += random(3);
          break;
        case 2:
          individuos[i][0] += random(3);
          individuos[i][1] -= random(3);
          individuos[i][2] += random(3);
          break;
        case 3:
          individuos[i][0] += random(3);
          individuos[i][1] += random(3);
          individuos[i][2] -= random(3);
          break;
        case 4:
          individuos[i][0] -= random(3);
          individuos[i][1] -= random(3);
          individuos[i][2] += random(3);
          break;
        case 5:
          individuos[i][0] -= random(3);
          individuos[i][1] += random(3);
          individuos[i][2] -= random(3);
          break;
        case 6:
          individuos[i][0] += random(3);
          individuos[i][1] -= random(3);
          individuos[i][2] -= random(3);
          break;
        case 7:
          individuos[i][0] -= random(3);
          individuos[i][1] -= random(3);
          individuos[i][2] -= random(3);
          break;
      }
      //NAO DEIXA O VALOR EXCEDER OS VALORES
      //BASE
      if(individuos[i][0] > 180)
        individuos[i][0] = 180;
      
      //S1
      if(individuos[i][1] > 180)
        individuos[i][1] = 180;
      
      //S2  
      if(individuos[i][2] > 180)
        individuos[i][2] = 180;
     
      
    }
    
    //SE APOS 50 GERACOES AINDA NAO ECONTROU O LOCAL, MATAMOS UMA PARTE DA POPULACAO
    if(k%150 == 0 && nota[10] > 1)
    {
      unsigned char n;
      for (unsigned char i = 0; i < 10; i++)
      {
        //DEFINE SE O INDIVIDUO VAI SER MORTO OU NAO
        n = random(1);
        if(n == 0)
        {
          for (unsigned char j = 0; j < 3; j++)
          {
            //gerando valores aleatórios
              individuos[i][j] = random(180);
          }
        }
      }
     }
  //MOVIMENTANDO  
  servoBase.write(individuos[10][0]);//SERVO GIRA NO ANGULO A
  servo1.write(individuos[10][1]);//SERVO GIRA NO ANGULO B
  servo2.write(individuos[10][2]);//SERVO GIRA NO ANGULO C
  k++;
  }
}


void loop()
{
  x = 0.0;
  y = 0.0;
  z = 0.0;
  while(! Serial.available()){};
  
    delay(10);
    x = (Serial.read()-'0')*100+(Serial.read()-'0')*10+(Serial.read()-'0');
     Serial.flush();
    Serial.println(x);
while(! Serial.available()){};
  
    delay(10);
    y = (Serial.read()-'0')*100+(Serial.read()-'0')*10+(Serial.read()-'0');
        
    Serial.println(y);
while(! Serial.available()){};
  
    delay(10);
    z = (Serial.read()-'0')*100+(Serial.read()-'0')*10+(Serial.read()-'0');
        
    Serial.println(z);


  //ENTRA NO AG
  algoritimo(x,y,z);
  
}
