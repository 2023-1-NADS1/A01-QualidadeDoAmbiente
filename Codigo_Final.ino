// PIN e Token de acesso ao aplicativo
#define BLYNK_TEMPLATE_ID           "TMPL2igNYgXIX"
#define BLYNK_TEMPLATE_NAME         "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "4_I-ocvMVwZd1fFQ6BflzqsqIJdPrkj6"
#define BLYNK_PRINT Serial          //Serial do app

// Blibioteca para conexão WiFi
#include <WiFi.h>
#include <WiFiClient.h>

#include <BlynkSimpleEsp32.h>       // Sincronização do app com o ESP32

#include <DHT.h>                    // Biblioteca do sensor DHT

// Credenciais WiFi para conexão
char ssid[] = "esp32wireless";
char pass[] = "";

#define LED_BUILTIN 2     // Pino vinculado ao Led Integrado do ESP32
#define DHTPIN 4          // Pino Físico GPIO D4 no ESP32
#define LDRPIN 35         // Pino Analógico A7 no ESP32
#define DHTTYPE DHT11     // Tipo do sensor DHT = DHT11

DHT dht(DHTPIN, DHTTYPE); // Pino e tipo informado ao DHT11
BlynkTimer timerDHT;      // Automatização dos dados do DHT11 para o app
BlynkTimer timerLDR;      // Automatização dos dados do LDR para o app

void leituraDHT()                                      // Função para leitura do sensor DHT11
{
  float umidade = dht.readHumidity();                  // Variável para leitura da umidade
  float temperatura = dht.readTemperature();           // Variável para leitura da temperatura

  if (isnan(umidade) || isnan(temperatura))            // Função para checar se a leitura não for em números "ISNotANumber"
  {
    Serial.println("Failed to read from DHT sensor!"); // Informar o erro no monitor serial no caso do 'isnan' passar
    return;
  }

  // Impressão dos dados do sensor DHT11 no monitor serial
  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.print("% | ");

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print("°C | ");

  // Envio dos dados para os pinos virtuais do aplicativo
  Blynk.virtualWrite(V5, umidade);
  Blynk.virtualWrite(V6, temperatura);
}

void leituraLDR ()                           // Função para leitura do sensor LDR
{
  int valorLDR = analogRead(LDRPIN);         // Variável e leitura digital para o sensor LDR
  valorLDR = map(valorLDR, 0, 1023, 0, 100); // Remapeamento de valores do sensor para de 0 a 100

  // Impressão dos dados do sensor LDR no monitor serial
  Serial.print("Luminosidade: ");
  Serial.print(valorLDR);
  Serial.println("%");

  // Envio dos dados da luminosidade para o aplicativo
  Blynk.virtualWrite(V16, valorLDR);
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);              // Configuração do Led Integrado

  Serial.begin(115200);                      // Velocidade de leitura

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); // Configuração de autentificação do app
  
  /* // Especificação do servidor
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080); */

  dht.begin(); // Inicia o sensor DHT11

  // Configura uma função para ser chamada a cada segundo
  timerDHT.setInterval(2000L, leituraDHT);
  timerLDR.setInterval(200L, leituraLDR);
}

void loop()
{
  // Loop do Led Integrado para fácil visibilidade de êxito do código
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);

  // Envio dos dados e leituras para o app
  Blynk.run();
  timerDHT.run();
  timerLDR.run();
}

// by Thiago Santana Dias
