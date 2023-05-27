#include <idDHT11.h>


int idDHT11pin = 2;       //Porta Digital do Arduino onde o Sinal do Sensor DHT esta conectado
int idDHT11intNumber = 0; //Número da interrupção respectiva à porta definida no parametro anterior 

void dht11_wrapper();     // Declaração da funcão de controle da interrupção. 
void loopDHT();           // Atualiza a leitura do sensor

idDHT11 DHT11(idDHT11pin, idDHT11intNumber, dht11_wrapper);   //Instanciação do Objeto de Controle do Sensor

void setup()
{
  Serial.begin(9600);
  Serial.println("Inicio do Sketch");
}

//Variaveis que irao conter os valores lidos no Sensor DHT11
float temperaturaC;
float temperaturaF;
float temperaturaK;
float umidade;
float dewPoint;
float dewPointSlow;

void loop()
{

  loopDHT(); 
  
  Serial.print("Temperatura Celcius: ");
  Serial.println( temperaturaC );

  Serial.print("Umidade Relativa: ");
  Serial.println( umidade );

  Serial.print("Ponto de Orvalho: ");
  Serial.println( dewPoint );

  Serial.println();
}

void dht11_wrapper() {
  DHT11.isrCallback();
}

void loopDHT() {
#define tempoLeitura 1000
static unsigned long delayLeitura = millis() + tempoLeitura + 1;
static bool request = false;

  if ((millis() - delayLeitura) > tempoLeitura) { 
      if (!request) {
         DHT11.acquire(); 
         request = true;
      }
  }

  if (request && !DHT11.acquiring()) {
    request = false;

    int result = DHT11.getStatus();
      
    switch (result)
    {
    case IDDHTLIB_OK: 
        Serial.println("Leitura OK"); 
        break;
    case IDDHTLIB_ERROR_CHECKSUM: 
        Serial.println("Erro\n\r\tErro Checksum"); 
        break;
    case IDDHTLIB_ERROR_ISR_TIMEOUT: 
        Serial.println("Erro\n\r\tISR Time out"); 
        break;
    case IDDHTLIB_ERROR_RESPONSE_TIMEOUT: 
        Serial.println("Erro\n\r\tResponse time out"); 
        break;
    case IDDHTLIB_ERROR_DATA_TIMEOUT: 
        Serial.println("Erro\n\r\tData time out erro"); 
        break;
    case IDDHTLIB_ERROR_ACQUIRING: 
        Serial.println("Erro\n\r\tAcquiring"); 
        break;
    case IDDHTLIB_ERROR_DELTA: 
        Serial.println("Erro\n\r\tDelta time to small"); 
        break;
    case IDDHTLIB_ERROR_NOTSTARTED: 
        Serial.println("Erro\n\r\tNao iniciado"); 
        break;
    default: 
        Serial.println("Erro Desconhecido"); 
        break;
    }
    
    float valor = DHT11.getCelsius();
     
    if (!isnan(valor)) {             
      temperaturaC = valor;
    }

    valor = DHT11.getHumidity();
    if (!isnan(valor)) {      
      umidade = valor;
    }  

    valor = DHT11.getFahrenheit();
    if (!isnan(valor)) {      
      temperaturaF = valor;
    }  

    valor = DHT11.getKelvin();
    if (!isnan(valor)) {      
      temperaturaK = valor;
    }  

    valor = DHT11.getDewPoint();
    if (!isnan(valor)) {      
      dewPoint = valor;
    }  
   
    valor = DHT11.getDewPointSlow();
    if (!isnan(valor)) {      
      dewPointSlow = valor;
    } 

    delayLeitura = millis();     
  }
}
