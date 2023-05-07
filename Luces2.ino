boolean sensor[] = {false,false,false,false};// Estado Sensores de movimiento
boolean rta = true;
unsigned long tempos[] = { 0, 0, 0, 0 }; // Tiempo de encendido de Luz (Manual (desde App) o por Sensores de movimiento)
unsigned long tact=0, tmen=5000; // Variables control tiempo activo luz por sensores y Alarma
// tact = Tiempo actual (tiempo transcurrido desde encendido de arduino)
// tmen = Tiempo de espera para mostrar mensaje de apagado en la App

void setup () { 
  Serial.begin(9600);

  for (int i = 4; i < 8; i++) {  // Habilita pines 4 a 7 como salida para Luces
    pinMode(i,OUTPUT);
  }

  for (int i = 8; i < 12; i++) {  // Habilita pines 8 a 11 como entrada para Sensores de movimiento
    pinMode(i,INPUT);
  }
}

void loop () {
  tact = millis();  // Tiempo que lleva encendido el Arduino

  if(char dato=Serial.read()) {  // Lee la señal en el Bluetoth enviada desde la App
    switch (dato) {
    // Enciende o Apaga Luz al recibir orden manual (desde App)
    case 'A':  // Luz cocina
      digitalWrite(4,HIGH);
      tempos[0] = tact;
      break;
    case 'B':
      digitalWrite(4,LOW);
      tempos[0] = 0;
      rta=true;
      break;
    case 'C':  // Luz sala
      digitalWrite(5,HIGH);
      tempos[1] = tact;
      break;
    case 'D':
      digitalWrite(5,LOW);
      tempos[1] = 0;
      rta=true;
      break;
    case 'E':  // Luz baño
      digitalWrite(6,HIGH);
      tempos[2] = tact;
      break;
    case 'F':
      digitalWrite(6,LOW);
      tempos[2] = 0;
      rta=true;
      break;
    case 'G':  // Luz habitación
      digitalWrite(7,HIGH);
      tempos[3] = tact;
      break;
    case 'H':
      digitalWrite(7,LOW);
      tempos[3] = 0;
      rta=true;
      break;

    // Activa o Desactiva Sensor al recibir orden manual (desde App)
    case 'I':  // Sensor cocina
      sensor[0] = true;
      break;
    case 'J':
      sensor[0] = false;
      break;
    case 'K':  // Sensor sala
      sensor[1] = true;
      break;
    case 'L':
      sensor[1] = false;
      break;
    case 'M':  // Sensor baño
      sensor[2] = true;
      break;
    case 'N':
      sensor[2] = false;
      break;
    case 'O':  // Sensor habitación
      sensor[3] = true;
      break;
    case 'P':
      sensor[3] = false;
      break;

    case 'R':
      rta = true;
      for (int i = 0; i < 4; i++) {
        if(tempos[i]>0) { tempos[i] = tact; }
      }
      break;

    case 'X':  // Apaga todas las luces
      for (int i = 4; i < 8; i++) {
        digitalWrite(i,LOW);
        tempos[i-4] = 0;
      }
      break;

    case 'Y':  // Desactiva todos los sensores
      for (int i = 0; i < 4; i++) {
        sensor[i] = false;
      }
      break;
    }
  }

  // Enciende luz al detectar movimiento en sensor
  //Cocina
  if(tempos[0]==0 and digitalRead(8)==HIGH and sensor[0]) {
    digitalWrite(4,HIGH);
    Serial.print("I");
    delay(100);
  }
  if(digitalRead(8)==HIGH and digitalRead(4)==HIGH){
    tempos[0]=0;
    tempos[0] = tact;
    delay(100);
  }

  //   Sala
  if(tempos[1]==0 and digitalRead(9)==HIGH and sensor[1]) {
    digitalWrite(5,HIGH);
    Serial.print("K");
    delay(100);
  }
  if(digitalRead(9)==HIGH and digitalRead(5)==HIGH){
    tempos[1]=0;
    tempos[1] = tact;
    delay(100);
  }

  //Baño
  if(tempos[2]==0 and digitalRead(10)==HIGH and sensor[2]) {
    digitalWrite(6,HIGH);
    Serial.print("M");
    delay(100);
  }
  if(digitalRead(10)==HIGH and digitalRead(6)==HIGH){
    tempos[2]=0;
    tempos[2] = tact;
    delay(100);
  }

  //Habitación
  if(tempos[3]==0 and digitalRead(11)==HIGH and sensor[3]) {
    digitalWrite(7,HIGH);
    Serial.print("O");
    delay(100);
  }
  if(digitalRead(11)==HIGH and digitalRead(7)==HIGH){
    tempos[3]=0;
    tempos[3] = tact;
    delay(100);
  }


  // Pregunta si Apagar luz al no detectar movimiento en sensor (y que la luz esté encendida)
  if(rta) {
    if (tempos[0]>0 and (tact-tempos[0])>tmen) {
      Serial.print("J");
      tempos[0] = tact;
      rta=false;
    }
    if (tempos[1]>0 and (tact-tempos[1])>tmen) {
      Serial.print("L");
      tempos[1] = tact;
      rta=false;
    }
    if (tempos[2]>0 and (tact-tempos[2])>tmen) {
      Serial.print("N");
      tempos[2] = tact;
      rta=false;
    }
    if (tempos[3]>0 and (tact-tempos[3])>tmen) {
      Serial.print("P");
      tempos[3] = tact;
      rta=false;
    }
  }
}
