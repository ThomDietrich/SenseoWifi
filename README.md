# SenseoWifi
Wifi'ify the Senseo coffee machine

## DIY

### Parts

| Part | Description |
|------|-------------|
| 1x Custom PCB | See [`SenseoWifi-PCB` folder](SenseoWifi-PCB) for schematic |
| 1y WeMos D1 Mini |  |
| 4x Sharp PC817 | Optocoupler interfacing with the Senseo board (e.g. [ebay.de](http://r.ebay.com/FofSOY)) |
| 3x 470Ω Resistor | |
| 1x 1.5kΩ Resistor | |
| 1x Push button | To reset the microcontroller from the outside |
| Generic male headers | A bunch of them |
| 1x Step Down AC-DC Converter (220V to 5V) (e.g. [ebay.de](http://r.ebay.com/rSZMfO)) | As power supply internally sourced |
| 1x Buzzer 12mm | *Optional* - Used for audio feedback, can be disabled in firmware (e.g. [reichelt.de](https://www.reichelt.de/Signalakustik/AL-60P01A/3/index.html?ACTION=3&GROUPID=6560&ARTICLE=145896&START=0&OFFSET=16&)) |
| 1x 150Ω Resistor | *Optional* - Size depends on buzzer |
| 1x TCRT5000 | *Optional* - To detect a cup. Reflective Optical Sensor, Regulated Module (e.g. [ebay.de](http://r.ebay.com/M1Iuci)) |
