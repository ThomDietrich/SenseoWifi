# SenseoWifi - Smart Home Coffee Maker

Wifi'ify the Senseo coffee maker.

The coffee maker is with no doubt a central element of our lives.
In the Smart Home where every device is connected and automated, the coffee making can not be left out.
Let's bring the [Philips Senseo](https://de.wikipedia.org/wiki/Senseo) (Basic or Classic) into your Wi-Fi and control it remotely!

> Yes, this is silly, you should do it too!

## Summary

This project contains information and material regarding:

- Soldering a **custom PCB** needed to bring Senseo onto a Wi-Fi
- Connecting the custom PCB to the Senseo PCB
- Compiling firmware with [PlatformIO](https://platformio.org/) (e.g. as plugin in [VisualStudioCode](https://code.visualstudio.com/))
- Flashing the custom PCB with the **provided firmware**
- Connecting to Wi-Fi and [MQTT](https://www.hivemq.com/mqtt-essentials)
- Connecting to [openHAB](https://openhab.org) (or any other home automation solution)
- *(optional)* Adding a buzzer for **audio feedback**
- *(optional)* Adding a **cup detector** for further automation

![](images/SenseoWifi-openHAB.png)

### Compatibility

This project is comatible with:

- Philips Senseo HD7810
- Philips Senseo HD7811
- Philips Senseo HD7812
- Philips Senseo HD7817
- … and most probably all similar models with one LED and three buttons

![installation example inside the machine](images/DSC09604.jpg)

## Features

After implementing the steps below your Senseo coffee machine will offer the following new features:

- Inform about the current state (Standby, Brewing, ...) of the machine via Wifi/MQTT
- Notify about an empty water tank via Wifi/MQTT
- Remote control the machine via Wifi/MQTT commands
- Go through a complete brewing cycle automatically (Turn on, brew, turn off) upon Wifi/MQTT command
- Audio feedback via a built-in speaker (optional)
- Detect a cup via an optical sensor (optional)
- Collect usage statistics, e.g. stored in InfluxDB and vidualized in Grafana

## Hardware Modification

The first challenge of this project is to hack the Senseo machine electronics.
You need to solder and connect a custom PCB to the machine.
In the end the PCB will not be visible from the outside, powered from the inside, connected via Wi-Fi and programmed over-the-air (OTA).

**⚠⚠⚠ Attention! For your own safety do not connect AC power while opened up. ⚠⚠⚠**

### Parts

| Part | Description |
|------|-------------|
| 1× Custom PCB | See [`SenseoWifi-PCB` folder](SenseoWifi-PCB) for schematic |
| 1× WeMos D1 Mini | Microcontroller, [learn more…](https://wiki.wemos.cc/products:d1:d1_mini) |
| 4× Sharp PC817 | Optocoupler interfacing with the Senseo board ([datasheet](http://www.sharp-world.com/products/device/lineup/data/pdf/datasheet/pc817xnnsz_e.pdf)). Thanks to these, the Senseo can be connected to AC power and to your PC via USB at the same time. Try to avoid this constellation and be cautious with active AC power. |
| 3× 470Ω Resistor | |
| 1× 1.5kΩ Resistor | |
| 1× Push button | To reset the microcontroller from the outside |
| 1× AC-DC Step Down Converter (220V to 5V) | As internally sourced power supply (e.g. [ebay.de](https://ebay.us/n9Cz6R) or [HLK-PM01](http://www.hlktech.net/product_detail.php?ProId=54)) |
| Generic male headers | A bunch of them |
| 1× Buzzer 12mm | *Optional* - Used for audio feedback, can be disabled in firmware (e.g. [reichelt.de](https://www.reichelt.de/Signalakustik/AL-60P01A/3/index.html?ACTION=3&GROUPID=6560&ARTICLE=145896&START=0&OFFSET=16&)) |
| 1× 150Ω Resistor | *Optional* - Size depends on buzzer |
| 1× TCRT5000 | *Optional* - To detect a cup. Reflective Optical Sensor, Regulated Module (e.g. [ebay.de](https://ebay.us/3Kf6fq)) |

![](images/PCBv1.6-assembled.jpg)

**Power Consumption:**
The question was raised how much energy is consumed by the modification. The PCB consumes approx. 110mA.
Assuming the Senseo machine is connected to power 24/7, the additional electronics raise your electrical bill by 2.00€ per year.

### Steps

1. Solder the custom PCB according to the schematics provided in the `SenseoWifi-PCB` folder (designed in [Fritzing](http://fritzing.org))
2. Hot clue the custom PCB inside the Senseo machine in a free position [such as shown here](images/DSC09604.jpg)
3. Wire the custom PCB via the "Senseo Connections" header (angled pin headers recommended) to interface with the Senseo PCB. The solder pads to use for the LED, the buttons, and ground connection are shown on [this image](images/DSC09627.jpg)
4. Connect a simple push button (the configuration reset button) to J3 and hot clue in the [base of the Senseo housing](images/resetbutton.jpg).
5. Connect the additional [power supply](images/DSC09646.jpg) to the Senseo power cable and wire to J4
6. Prepare a [small cutout](images/tcrt-cutout.jpg) in the Senseo front and [hot clue from behind](images/DSC09604.jpg). Connect to the TCRT header
7. Connect a USB cable to continue with firmware programming. Do not connect AC power

Please follow the details given in the schematics and the pictures in the [images](images) folder, which should make everything pretty clear.
Do not hessitate to create a support ticket on GitHub if we missed anything.
A pull request to improve this README is always welcome.

![schematics](images/PCBv1.8.png)

## Firmware Upload

The WeMos microcontroller needs to be programmed with the code provided in this repository.
Be aware that the custom functionality is build on top of the excellent [Homie v3.0](https://github.com/homieiot/homie-esp8266/) framework.

An MQTT broker is needed in your network (e.g. mosquitto).

Please follow these instructions:

1. Install PlatformIO via Visual Studio Code as decribed [here](https://platformio.org/platformio-ide) or upgrade your existing installation
2. Open PlatformIO and load a latest copy of this repository
3. Connect your Senseo via the USB cable to your PC (do not connect the Senseo to AC power!) and check the devices view of PlatformIO to verify
4. Transfer the firmware and the configuration web interface to the microcontroller.
  The full list of PlatformIO project tasks is:
   - Clean
   - Erase Flash
   - Build
   - Upload
   - Upload File System image
   - Monitor
  
   If any of the steps ends in a connection error dis- and reconnect the USB cable.
   You were successful when the monitoring terminal shows the SenseoWifi firmware version.
   For initial hardware testing see below

5. Use a smartphone to connect to the provided Wifi, you will be redirected to a configuration web frontend
6. Provide your Wifi, MQTT, and other settings (we recommend the default homie base topic)
7. Use an MQTT client to inspect messages sent to the MQTT broker.
  You were successful when the message "senseo-wifi" is published to the topic `homie/senseo-wifi/machine/$name`
8. Disconnect the USB cable, close the Senseo housing, and connect the Senseo machine to AC power.
  The machine should once again start communicating via MQTT

**For Hardware testing:** A special piece of code is provided to test that your fresh hardware modifications are working.
Enable `testIO()` in `SenseoWifi.cpp` and check `testIO.cpp` for details.

**Configuration mode:** The configuration web frontend is only available when in configuration mode, i.e. when not yet configured.
Press the configuration reset button previously mounted in the base of the machine for 5 seconds to resets the firmware configuration.
The machine switches in configuration mode and provides a Wifi accesspoint, which upon connection once again presents the configuration web frontend.
Follow these instructions if you ever loose connection, or want to change your Wifi or MQTT settings.

## Usage

If everything worked out your machine now communicates its status and accepts commands via MQTT.
You are now ready to interact with the machine and integrate it with other systems.

## Smart Home Integration

With the machine connected to Wifi and MQTT, you can now link a smart home system to the important MQTT topics to integrate the machine with the rest of your smart home.

### openHAB Configuration Example

The following openHAB configuration allows integration of the Senseo machine with the home automation solution openHAB. Please make sure the machine is connected to your MQTT broker. Insert the MQTT broker IP and replace "senseo-wifi-home" by your device name in the below example.

The [expire binding](https://www.openhab.org/addons/bindings/expire1/) is recommended for debug-value.

**Note:** Not updated to latest firmware changes. 

openHAB things file `mqtt.things`:
```
Bridge mqtt:systemBroker:LocalBroker [ host="your-broker-ip", secure=false ]
{
    Thing mqtt:topic:SenseoWiFi "SenseoWiFi" {
    Channels:
        Type string : Debug          "Debug"            [stateTopic="devices/senseo-wifi-home/machine/debug"]
        Type string : OpState        "Zustand"          [stateTopic="devices/senseo-wifi-home/machine/opState"]
        Type switch : OnOff          "OnOff"            [stateTopic="devices/senseo-wifi-home/machine/power", commandTopic="devices/senseo-wifi-home/machine/power/set", on="ON", off="OFF"]
        Type string : Brew           "Brew"             [stateTopic="devices/senseo-wifi-home/machine/brew",commandTopic="devices/senseo-wifi-home/machine/brew/set"]
        Type number : BrewedSize     "BrewedSize"       [stateTopic="devices/senseo-wifi-home/machine/brewedSize"]
        Type string : OutOfWater     "OutOfWater"       [stateTopic="devices/senseo-wifi-home/machine/outOfWater"]
        Type string : Recipe         "Recipe"           [stateTopic="devices/senseo-wifi-home/machine/recipe",commandTopic="devices/senseo-wifi-home/machine/recipe/set"]
        Type string : CupAvailable   "Tasse vorh."      [stateTopic="devices/senseo-wifi-home/machine/cupAvailable"]
        Type string : CupFull        "Tasse voll"       [stateTopic="devices/senseo-wifi-home/machine/cupFull"]
        Type string : Online         "Online"           [stateTopic="devices/senseo-wifi-home/$online"]

        Type string : version        "Version"          [ stateTopic="devices/senseo/$homie"]
        Type string : name           "Name"             [ stateTopic="devices/senseo/$name"]
        Type string : ip             "IP-address"       [ stateTopic="devices/senseo/$localip"]
        Type string : mac            "MAC-address"      [ stateTopic="devices/senseo/$mac"]
        Type string : fw_name        "Firmware Name"    [ stateTopic="devices/senseo/$fw/name"]
        Type string : fw_version     "Firmware Version" [ stateTopic="devices/senseo/$fw/version"]
        Type string : nodes          "Nodes"            [ stateTopic="devices/senseo/$nodes"]
        Type string : implementation "Implementation"   [ stateTopic="devices/senseo/$implementation"]
        Type string : interval       "Update interval"  [ stateTopic="devices/senseo/$stats/interval"]
    }
}
```

openHAB items file `SenseoWifi.items`:
```
String KU_Senseo_Debug         "Debug [%s]"                          {channel="mqtt:topic:SenseoWiFi:Debug", expire="10s"}
String KU_Senseo_OpState       "Zustand [MAP(senseo-wifi.map):%s]"   {channel="mqtt:topic:SenseoWiFi:OpState"}
Switch KU_Senseo_OnOff         "OnOff"                               {channel="mqtt:topic:SenseoWiFi:OnOff"}
String KU_Senseo_Brew          "Brew"                                {channel="mqtt:topic:SenseoWiFi:Brew"}
Number KU_Senseo_BrewedSize    "BrewedSize"                          {channel="mqtt:topic:SenseoWiFi:BrewedSize"}
String KU_Senseo_OutOfWater    "OutOfWater"                          {channel="mqtt:topic:SenseoWiFi:OutOfWater" [profile="transform:MAP", function="senseo-wifi.map"]}
String KU_Senseo_Recipe        "Recipe"                              {channel="mqtt:topic:SenseoWiFi:Recipe"}
String KU_Senseo_CupAvailable  "Tasse vorh."                         {channel="mqtt:topic:SenseoWiFi:CupAvailable" [profile="transform:MAP", function="senseo-wifi.map"]}
String KU_Senseo_CupFull       "Tasse voll"                          {channel="mqtt:topic:SenseoWiFi:CupFull" [profile="transform:MAP", function="senseo-wifi.map"]}
String KU_Senseo_Online        "Online"                              {channel="mqtt:topic:SenseoWiFi:Online"}
```

openHAB sitemap example `SenseoWifi.sitemap`:
```
sitemap testing label="Senseo-Tests"
{
    Text item=KU_Senseo_Online label="Fehler [Maschine nicht verfügbar!]" visibility=[KU_Senseo_Online == "false"] valuecolor=["false"="red"]
    Frame label="Status" visibility=[KU_Senseo_Online == "true"] {
        Text item=KU_Senseo_OpState label="Aktueller Zustand"
        Default item=KU_Senseo_Debug      label="Debug" visibility=[KU_Senseo_Debug != UNDEF] valuecolor=["red"]
        Text item=KU_Senseo_OutOfWater label="Wassertank [Leer]" visibility=[KU_Senseo_OpState == SENSEO_NOWATER] valuecolor=[ON="maroon"] icon="water"
        Switch item=KU_Senseo_CupAvailable label="Tasse vorhanden" icon="coffee_cup"
        Switch item=KU_Senseo_CupFull label="Tasse voll" visibility=[KU_Senseo_CupAvailable == ON] icon="coffee_cup_hot"
    }
    Frame label="Steuerung" visibility=[KU_Senseo_Online == "true"] {
        Switch item=KU_Senseo_OnOff label="Ein-/Ausschalten"
        Switch item=KU_Senseo_Brew label="Kaffee Kochen" mappings=[1cup="☕", 2cup="☕☕"]
    }
}
```

## Contribution

Further details outstanding. Don't hesitate to open a support issue!

Happy Hacking!

----

# Disclaimer and Legal

> *Philips* and *Senseo* are registered trademarks of *Philips GmbH*.
>
> This project is a community project not for commercial use.
> The authors will not be held responsible in the event of device failure or bad tasting coffee.
>
> This project is in no way affiliated with, authorized, maintained, sponsored or endorsed by *Philips* or any of its affiliates or subsidiaries.
