# SenseoWifi

Wifi'ify the Senseo coffee machine

## DIY

### Parts

| Part | Description |
|------|-------------|
| 1× Custom PCB | See [`SenseoWifi-PCB` folder](SenseoWifi-PCB) for schematic |
| 1× WeMos D1 Mini |  |
| 4× Sharp PC817 | Optocoupler interfacing with the Senseo board (e.g. [ebay.de](http://r.ebay.com/FofSOY)) |
| 3× 470Ω Resistor | |
| 1× 1.5kΩ Resistor | |
| 1× Push button | To reset the microcontroller from the outside |
| Generic male headers | A bunch of them |
| 1× Step Down AC-DC Converter (220V to 5V) | As power supply internally sourced (e.g. [ebay.de](http://r.ebay.com/rSZMfO)) |
| 1× Buzzer 12mm | *Optional* - Used for audio feedback, can be disabled in firmware (e.g. [reichelt.de](https://www.reichelt.de/Signalakustik/AL-60P01A/3/index.html?ACTION=3&GROUPID=6560&ARTICLE=145896&START=0&OFFSET=16&)) |
| 1× 150Ω Resistor | *Optional* - Size depends on buzzer |
| 1× TCRT5000 | *Optional* - To detect a cup. Reflective Optical Sensor, Regulated Module (e.g. [ebay.de](http://r.ebay.com/M1Iuci)) |
