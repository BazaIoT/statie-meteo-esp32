// Stație meteo cu ESP32 și DHT22: temperatura și umiditatea pe o pagină web locală.
// Proiect BazaIoT · https://bazaiot.ro · licență MIT
//
// Conexiuni: DHT22 VCC → 3.3V, DATA → GPIO 27, GND → GND
// Biblioteci: "DHT sensor library" și "Adafruit Unified Sensor" (Library Manager).
// Serverul web (WebServer.h) vine cu pachetul ESP32, nu trebuie instalat separat.

#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

const char* WIFI_NUME = "NUMELE_RETELEI_TALE";
const char* WIFI_PAROLA = "PAROLA_TA";

const int PIN_DHT = 27;
DHT dht(PIN_DHT, DHT22);
WebServer server(80);

// DHT22 nu trebuie citit mai des de o dată la 2 secunde; păstrăm ultima citire bună.
float temperatura = NAN;
float umiditate = NAN;
unsigned long ultimaCitire = 0;
const unsigned long INTERVAL_CITIRE_MS = 5000;

const char PAGINA[] PROGMEM = R"html(<!doctype html>
<html lang="ro"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Stație meteo ESP32</title>
<style>
body{font-family:system-ui,sans-serif;text-align:center;margin:2rem;background:#f5f7fa;color:#1e293b}
.v{font-size:3rem;font-weight:700}.u{font-size:1.2rem;color:#64748b}
.card{background:#fff;border-radius:12px;padding:1rem;margin:1rem auto;max-width:320px;box-shadow:0 1px 4px #0002}
</style></head><body>
<h1>Stație meteo ESP32</h1>
<div class="card"><div class="u">Temperatură</div><div class="v"><span id="t">--</span> °C</div></div>
<div class="card"><div class="u">Umiditate</div><div class="v"><span id="h">--</span> %</div></div>
<p class="u">Se actualizează la 10 secunde.</p>
<script>
async function actualizeaza(){
  try{
    const r=await fetch('/date');const d=await r.json();
    document.getElementById('t').textContent=d.temperatura??'--';
    document.getElementById('h').textContent=d.umiditate??'--';
  }catch(e){}
}
actualizeaza();setInterval(actualizeaza,10000);
</script></body></html>)html";

void citesteSenzor() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (isnan(t) || isnan(h)) {
    Serial.println("Citire DHT22 eșuată, verifică firele.");
    return;
  }
  temperatura = t;
  umiditate = h;
}

void trimitePagina() {
  server.send_P(200, "text/html; charset=utf-8", PAGINA);
}

void trimiteDate() {
  char json[64];
  if (isnan(temperatura) || isnan(umiditate)) {
    snprintf(json, sizeof(json), "{\"temperatura\":null,\"umiditate\":null}");
  } else {
    snprintf(json, sizeof(json), "{\"temperatura\":%.1f,\"umiditate\":%.0f}", temperatura, umiditate);
  }
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NUME, WIFI_PAROLA);
  Serial.print("Conectare la Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Deschide în browser: http://");
  Serial.println(WiFi.localIP());

  server.on("/", trimitePagina);
  server.on("/date", trimiteDate);
  server.begin();
  citesteSenzor();
}

void loop() {
  server.handleClient();
  if (millis() - ultimaCitire >= INTERVAL_CITIRE_MS) {
    ultimaCitire = millis();
    citesteSenzor();
  }
}
