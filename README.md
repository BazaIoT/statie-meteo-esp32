# Stație meteo cu ESP32 și DHT22

Stație meteo cu ESP32 care afișează temperatura și umiditatea pe o pagină web, accesibilă de pe telefon sau laptop din aceeași rețea Wi-Fi. Pagina se actualizează singură la 10 secunde.

Proiect [BazaIoT](https://bazaiot.ro): kituri Arduino, ESP32 și componente electronice, cu ghiduri în limba română.

## Ce îți trebuie

| Componentă | Unde o găsești |
|---|---|
| Placă ESP32 (ex. ESP32-WROOM-32D) | [Placă ESP32-WROOM-32D](https://bazaiot.ro/products/placa-esp32-wroom-32d-wifi-bluetooth) |
| Senzor DHT22 (modul cu 3 pini) | |
| Fire jumper, cablu USB de date | |

## Conexiuni

| DHT22 | ESP32 |
|---|---|
| VCC | 3.3V |
| DATA | GPIO 27 |
| GND | GND |

Dacă senzorul are 4 pini (fără modul), pune o rezistență de 4,7 kΩ între VCC și DATA.

## Pași

1. În Arduino IDE instalează suportul pentru ESP32 (Boards Manager → „esp32” de la Espressif).
2. Din Library Manager instalează **DHT sensor library** și **Adafruit Unified Sensor**.
3. Deschide `statie-meteo-esp32/statie-meteo-esp32.ino` și completează `WIFI_NUME` și `WIFI_PAROLA`.
4. Alege placa **ESP32 Dev Module** și portul, apoi încarcă programul.
5. Deschide Serial Monitor la 115200. Apare adresa, de exemplu `http://192.168.1.50`. Deschide-o în browser.

Datele sunt disponibile și ca JSON la `/date`, ca să le folosești în alte proiecte.

## Ghid complet

Explicațiile pas cu pas sunt pe blogul BazaIoT: [Stație meteo cu ESP32 și web server](https://bazaiot.ro/blogs/baza-iot/statie-meteo-esp32-dht22-web-server).

Codul din acest repo folosește serverul web inclus în pachetul ESP32, fără biblioteci descărcate manual.

## Licență

MIT. Poți folosi și modifica liber codul, inclusiv la clasă.
