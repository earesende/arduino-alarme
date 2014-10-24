/*
 * Eduardo Augusto Resende
 * Arduino Alarme V 0.1
 *
 /*

// Define os Pinos

int pinoLedVermelho  = 6;
int pinoLedAmarelo   = 5;
int pinoLedVerde     = 3; 
int pinoFalante      = 10;
int pinoBotao        = 4;
int pinoSensor       = 7;

// Define os valores

int estadoBotao       = 0;
int valorSensor       = 0;
long contagemBotao    = 0;
long aguardarAtivar   = 0;
long espera           = 0;
boolean ativando      = false;
boolean sensorEspera  = false;
boolean sensorAtivo   = false;
boolean alarmeTocando = false;

// Define o tempo padrao de espera

long tempoDisparoEspera = 600000;
long tempoDisparoAlarme = 600000;

void setup()
{
    pinMode(pinoLedVermelho, OUTPUT);
    pinMode(pinoLedAmarelo, OUTPUT);
    pinMode(pinoLedVerde, OUTPUT);
    pinMode(pinoBotao, INPUT);
    pinMode(pinoFalante, OUTPUT);
    pinMode(pinoSensor, INPUT);
    digitalWrite(pinoBotao, HIGH);
    digitalWrite(pinoSensor, LOW);
}

void loop()
{
    byte debounce = 0;
    estadoBotao = digitalRead(pinoBotao);
    valorSensor = digitalRead(pinoSensor);

    // Sensor de presenca

    if(valorSensor == HIGH)
    {
        digitalWrite(pinoLedAmarelo, HIGH); // Ativa o luz amarela para informar a presenca 
        if(sensorAtivo == true) // Se o sensor estiver ativo, dispara o alarme
        {
           alarmeTocando = true;
        }
    }
    else if(valorSensor == LOW) // Apaga a luz amarela depois de alguns segundos
    {
        digitalWrite(pinoLedAmarelo, LOW);
    }

    if(estadoBotao == LOW)
    {
         unsigned long tempo1 = millis();
         while(tempo1 + 500 > millis());
         if(estadoBotao == LOW) debounce = 1;
    }

    if(debounce && alarmeTocando == true)
    {            
        alarmeTocando = false;
        sensorAtivo   = false;
        ativando      = false;
        espera        = 0;
        contagemBotao = 0;
        verificaSensor(false);
    }
    else if(debounce && alarmeTocando == false){
        if(sensorAtivo == false && ativando == true)
        {
            verificaSensor(false);
            aguardarAtivar = 0;
        }
        else if(sensorAtivo == true && ativando == false)
        {
            verificaSensor(false);
            aguardarAtivar = 0;
        }
        else if(sensorAtivo == false && ativando == false)
        {
            aguardarAtivar = 0;
            ativando = true;
        }
        
        contagemBotao = 0;
    }

    // Define o tempo para sair da presenca do sensor antes que ele arme.

    if(ativando == true)
    {
      aguardarAtivar++;
        digitalWrite(pinoLedVerde, HIGH);
        if(aguardarAtivar >= tempoDisparoEspera)
        {
            verificaSensor(true);
            sensorAtivo = true;
            tone(pinoFalante, 100, 200);
        }
    }
    
    // Define o tempo para desligar o alarme antes que ele dispare.

    if(alarmeTocando == true)
    {
        espera++;        
        sensorEspera = true;
        digitalWrite(pinoLedVermelho, HIGH);
        if(espera > tempoDisparoAlarme)
        {
            tone(pinoFalante, 600, 10);
        }
    } 
}

// Verifica o estado do Alarme
// true - ativa o alarme
// false - desativa o alarme

void verificaSensor(boolean verifica)
{
    long contaMillis = 0;
    long intervalo   = 150;
    int valorLed     = LOW;
    
    if(verifica == true)
    {
        digitalWrite(pinoLedVermelho, LOW);
        
        ativando           = false;
        aguardarAtivar     = 0;
        long repeticaoTrue = 0;
        
        while(repeticaoTrue <= 40000){
            digitalWrite(pinoLedVerde, HIGH);
            repeticaoTrue++;
        }
        digitalWrite(pinoLedVerde, LOW);
    }
    else
    {
        ativando            = false;
        sensorAtivo         = false;
        long repeticaoFalse = 0;
        
        while(repeticaoFalse <= 40000){
            digitalWrite(pinoLedVermelho, HIGH);
            digitalWrite(pinoLedVerde, HIGH);
            repeticaoFalse++;
        }
        digitalWrite(pinoLedVermelho, LOW);
        digitalWrite(pinoLedVerde, LOW);
    }
}
