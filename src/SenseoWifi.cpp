/*
SenseoWifi.cpp - base file for the SenseoWifi project.
Created by Thomas Dietrich, 2016-03-05.
Released under some license.
*/

#include <Homie.h>
#include <memory>

#include "pins.h"
#include "constants.h"
#include "testIO.cpp"
#include "HomeAssistant.h"

#include "SenseoFsm/SenseoFsm.h"
#include "SenseoFsm/Components/BuzzerComponent.h"
#include "SenseoFsm/Components/SenseoLedComponent.h"
#include "SenseoFsm/Components/ControlComponent.h"
#include "SenseoFsm/Components/programComponent.h"

#include "SenseoInputButtons.h"

#include "LedObserver/HwTimerLedObserver.h"
#include "LedObserver/LedObserver.h"

#ifdef EXECUTE_IF_COMPONENT_EXIST
#undef EXECUTE_IF_COMPONENT_EXIST
#endif
#define EXECUTE_IF_COMPONENT_EXIST(fsm, component, ...) \
    if (fsm.getComponent<component>() != nullptr)       \
    {                                                   \
        fsm.getComponent<component>()->__VA_ARGS__;     \
    }

HomieNode senseoNode("machine", "senseo-wifi", "senseo-wifi");
HomieSetting<bool> CupDetectorAvailableSetting("cupdetector", "Enable cup detection (TCRT5000)");
HomieSetting<bool> BuzzerSetting("buzzer", "Enable buzzer sounds (no water, cup finished, ...)");
HomieSetting<bool> PublishHomeAssistantDiscoveryConfig("homeassistantautodiscovery", "Publish HomeAssistant discovery config, ...)");
bool useCustomizableButtonsAddon = false;

SenseoFsm mySenseo(senseoNode);
// HwTimerLedObserver mySenseoLed(senseoNode,ocSenseLedPin);
LedObserver mySenseoLed(senseoNode, ocSenseLedPin);
std::unique_ptr<SenseoInputButtons> myInputbuttons;

/**
 * Called by Homie upon an MQTT message to '.../power'
 * MQTT response is sent from this routine, as pessimistic feedback from state machine is too slow and triggers a timeout in e.g. Home Assistant.
 */
bool powerHandler(const HomieRange &range, const String &value)
{
    if (value != "true" && value != "false" && value != "reset")
    {
        senseoNode.setProperty("debug").send("power: malformed message content. Allowed: [true,false,reset].");
        return false;
    }

    if (value == "true" && mySenseo.isOff())
    {
        mySenseo.sendCommands(CommandComponent::TurnOn);
        senseoNode.setProperty("power").send("true");
    }
    else if (value == "false" && !mySenseo.isOff())
    {
        mySenseo.sendCommands(CommandComponent::TurnOff);
        senseoNode.setProperty("power").send("false");
    }
    else if (value == "reset")
    {
        senseoNode.setProperty("power").send("false");
        EXECUTE_IF_COMPONENT_EXIST(mySenseo, BuzzerComponent, playMelody("reset"));
        Homie.reset();
    }
    return true;
}

bool program1CupHandler(const HomieRange &range, const String &value)
{
    if (value == "true")
    {
        EXECUTE_IF_COMPONENT_EXIST(mySenseo, ProgramComponent, requestProgram(ProgramComponent::oneCup));
    }
    else
    {
        EXECUTE_IF_COMPONENT_EXIST(mySenseo, ProgramComponent, clearProgram(ProgramComponent::oneCup));
    }
    return true;
}

bool program2CupHandler(const HomieRange &range, const String &value)
{
    if (value == "true")
    {
        EXECUTE_IF_COMPONENT_EXIST(mySenseo, ProgramComponent, requestProgram(ProgramComponent::twoCup));
    }
    else
    {
        EXECUTE_IF_COMPONENT_EXIST(mySenseo, ProgramComponent, clearProgram(ProgramComponent::twoCup));
    }
    return true;
}

/**
 * Called by Homie upon an MQTT message to '.../brew'.
 * No MQTT response is sent from this routine, as pessimistic feedback will be handled in the state machine.
 */
bool brewHandler(const HomieRange &range, const String &value)
{
    if (value != "1cup" && value != "2cup" && value != "descale")
    {
        senseoNode.setProperty("debug").send("brew: malformed message content. Allowed: [1cup,2cup,descale].");
        return false;
    }

    if (value == "1cup" || value == "2cup")
    {
        CommandComponent::CommandBitFields commands = value == "1cup" ? CommandComponent::Brew1Cup : CommandComponent::Brew2Cup;
        if (mySenseo.isOff()) commands |= CommandComponent::TurnOn | CommandComponent::TurnOffAfterBrewing;
        mySenseo.sendCommands(commands);
    }
    else if (value == "descale")
    {
        Homie.getLogger() << "!! Warning, descale has not been propertly tested and might messed up with the state machine !!" << endl;
        mySenseo.getComponent<ControlComponent>()->pressLeftRightButton();
    }
    return true;
}

/**
 * Called by Homie upon an MQTT message to '.../buzzer'.
 */
bool buzzerHandler(const HomieRange &range, const String &value)
{
    BuzzerComponent *buzzerComponent = mySenseo.getComponent<BuzzerComponent>();
    if (buzzerComponent != nullptr)
    {
        senseoNode.setProperty("buzzer").send(value);
        bool success = buzzerComponent->playMelody(value);
        if (!success)
        {
            String errorMsg = String("buzzer: malformed message content. Allowed: [") + buzzerComponent->getValidTunes() + "]";
            senseoNode.setProperty("debug").send(errorMsg);
        }
        senseoNode.setProperty("buzzer").send("");
        return success;
    }
    else
    {
        senseoNode.setProperty("debug").send("buzzer: not configured.");
        return false;
    }
}

bool autoDetectCustomizableButtonsAddon()
{
    pinMode(senseoButtonsInputPin, INPUT);
    int maxReading = 3;
    int readingAverage = 0;
    Homie.getLogger() << endl;
    delay(50); // let's wait a bit for the ADC to be ready, the first reading is better that way
    for (int i = 0; i < maxReading; i++)
    {
        int reading = analogRead(senseoButtonsInputPin);
        Homie.getLogger() << "Reading " << i + 1 << ": A0 = " << reading << endl;
        readingAverage += reading;
        delay(50);
    }

    readingAverage /= maxReading;
    Homie.getLogger() << "Average Reading: A0 = " << readingAverage << endl;

    if (abs(A0NoButtonPress - readingAverage) <= 3)
    {
        Homie.getLogger() << "Customizable Buttons Addon detected" << endl;
        return true;
    }
    else
    {
        Homie.getLogger() << "No Addon detected" << endl;
        return false;
    }
}

void publishHomeAssistandDiscoveryConfig()
{
    Homie.getLogger() << endl << "Creating HomeAssistant entities..." << endl;
    HomeAssistantDiscovery ha;

    // binary_sensor
    bool success = ha.publishBinarySensorConfig("Out Of Water", "outOfWater", {{"icon", "mdi:water-off-outline"}, {"device_class", "problem"}});
    Homie.getLogger() << "OutOfWater: " << (success ? "success" : "failed") << endl;

    if (CupDetectorAvailableSetting.get())
    {
        success = ha.publishBinarySensorConfig("Cup Available", "cupAvailable", {{"icon", "mdi:coffee-outline"}});
        Homie.getLogger() << "cupAvailable: " << (success ? "success" : "failed") << endl;

        success = ha.publishBinarySensorConfig("Cup Full", "cupFull", {{"icon", "mdi:coffee"}});
        Homie.getLogger() << "cupFull: " << (success ? "success" : "failed") << endl;
    }

    // sensor
    success = ha.publishSensorConfig("Brewed Size", "brewedSize", {{"icon", "mdi:coffee-maker"}});
    Homie.getLogger() << "brewedSize: " << (success ? "success" : "failed") << endl;

    success = ha.publishSensorConfig("Operating State", "opState", {{"icon", "mdi:state-machine"}});
    Homie.getLogger() << "opState: " << (success ? "success" : "failed") << endl;

    success = ha.publishSensorConfig("Led State", "ledState", {{"icon", "mdi:state-machine"}});
    Homie.getLogger() << "ledState: " << (success ? "success" : "failed") << endl;

    // debug sensor
    success = ha.publishSensorConfig("Debug", "debug", {{"icon", "mdi:comment-text-multiple-outline"}, {"entity_category", "diagnostic"}});
    Homie.getLogger() << "debug: " << (success ? "success" : "failed") << endl;

    success = ha.publishStatConfig("Rssi", "signal", {{"icon", "mdi:signal-cellular-2"}, {"unit_of_measurement", "%"}, {"device_class", "signal_strength"}, {"entity_category", "diagnostic"}});
    Homie.getLogger() << "rssi: " << (success ? "success" : "failed") << endl;

    success = ha.publishStatConfig("Uptime", "uptime", {{"icon", "mdi:av-timer"}, {"unit_of_measurement", "s"}});
    Homie.getLogger() << "uptime: " << (success ? "success" : "failed") << endl;

    // switch
    success = ha.publishSwitchConfig("Power", "power", {{"icon", "mdi:power"}});
    Homie.getLogger() << "power: " << (success ? "success" : "failed") << endl;
    if (useCustomizableButtonsAddon)
    {
        success = ha.publishSwitchConfig("Program 1 Cup", "program1Cup", {{"icon", "mdi:coffee"}});
        Homie.getLogger() << "program1Cup: " << (success ? "success" : "failed") << endl;
        success = ha.publishSwitchConfig("Program 2 Cups", "program2Cup", {{"icon", "mdi:coffee"}});
        Homie.getLogger() << "program2Cup: " << (success ? "success" : "failed") << endl;
    }

    // button
    success = ha.publishButtonConfig("Brew Coffee Normal", "brew", "1cup", {{"icon", "mdi:coffee"}});
    Homie.getLogger() << "brew 1cup: " << (success ? "success" : "failed") << endl;

    success = ha.publishButtonConfig("Brew Coffee Double", "brew", "2cup", {{"icon", "mdi:coffee"}});
    Homie.getLogger() << "brew 2cup: " << (success ? "success" : "failed") << endl;

    Homie.getLogger() << endl;
}

/**
 * The device rebooted when attachInterrupt was called in setup()
 * before Wifi was connected and interrupts were already coming in.
 */
void onHomieEvent(const HomieEvent &event)
{
    switch (event.type)
    {
    case HomieEventType::WIFI_CONNECTED:
        mySenseoLed.attachInterrupt();
        break;
    case HomieEventType::WIFI_DISCONNECTED:
        mySenseoLed.detachInterrupt();
        break;
    default:
        break;
    case HomieEventType::MQTT_READY:
        if (PublishHomeAssistantDiscoveryConfig.get())
        {
            publishHomeAssistandDiscoveryConfig();
        }
        mySenseoLed.onMqttReady();
        break;
    }
}

void brewCup(CommandComponent::Command command)
{
    CommandComponent::CommandBitFields commands = command;
    if (mySenseo.isOff())
        commands |= CommandComponent::TurnOn | CommandComponent::TurnOffAfterBrewing;
    mySenseo.sendCommands(commands);
}

void togglePower()
{
    if (mySenseo.isOff())
        mySenseo.sendCommands(CommandComponent::TurnOn);
    else
        mySenseo.sendCommands(CommandComponent::TurnOff);
}

void holdCupButtonHandler(ProgramComponent::Program program)
{
    if (mySenseo.isOff())
    {
        ProgramComponent *programComponent = mySenseo.getComponent<ProgramComponent>();
        SenseoLedComponent *ledComponent = mySenseo.getComponent<SenseoLedComponent>();
        // I should not reach that code path without those component since the function is only called if the CustomizableButtonAddon has been detected
        if (programComponent && ledComponent)
        {
            ledComponent->burst({100, 100, 100, 100, 100});
            programComponent->requestProgram(program);
        }
    }
}

void holdPowerButtonHandler()
{
    if (mySenseo.isOff())
    {
        ProgramComponent *programComponent = mySenseo.getComponent<ProgramComponent>();
        SenseoLedComponent *ledComponent = mySenseo.getComponent<SenseoLedComponent>();
        // I should not reach that code path without those component since the function is only called if the CustomizableButtonAddon has been detected
        if (programComponent && ledComponent)
        {
            programComponent->clearProgram(ProgramComponent::all);
            ledComponent->burst({100, 100, 100, 100, 100});
            EXECUTE_IF_COMPONENT_EXIST(mySenseo, BuzzerComponent, playMelody("beep"));
        }
    }
}

/**
 *
 */
void setupHandler()
{
    // configuring the state machine
    mySenseo.setup(mySenseoLed, CupDetectorAvailableSetting.get(), BuzzerSetting.get(), useCustomizableButtonsAddon);

    // configuring the button handler
    if (useCustomizableButtonsAddon)
    {
        senseoNode.setProperty("program1Cup").send("false");
        senseoNode.setProperty("program2Cup").send("false");

        myInputbuttons = std::make_unique<SenseoInputButtons>(senseoButtonsInputPin);

        // Power Button
        myInputbuttons->addButtonReleaseHandler(A0buttonPwr, 75, togglePower);
        myInputbuttons->addButtonHoldHandler(A0buttonPwr, 2000, holdPowerButtonHandler);
        myInputbuttons->addButtonReleaseHandler(A0buttonPwr, 1000, []() {}); // this one is to prevent the Power release to trigger

        // Senseo Reset
        myInputbuttons->addButtonHoldHandler(A0buttonPwr1Cup2Cup, 2000, []() { EXECUTE_IF_COMPONENT_EXIST(mySenseo, BuzzerComponent, playMelody("beep")); });
        myInputbuttons->addButtonHoldHandler(A0buttonPwr1Cup2Cup, 3000, []() { EXECUTE_IF_COMPONENT_EXIST(mySenseo, BuzzerComponent, playMelody("beep")); });
        myInputbuttons->addButtonHoldHandler(A0buttonPwr1Cup2Cup, 4000, []() { EXECUTE_IF_COMPONENT_EXIST(mySenseo, BuzzerComponent, playMelody("beep")); });
        myInputbuttons->addButtonReleaseHandler(A0buttonPwr1Cup2Cup, 4000, []() { Homie.reset(); });
        // myInputbuttons->addButtonReleaseHandler(A0buttonPwr1Cup2Cup,000,[]() { Homie.getLogger() << "Reset Canceled" << endl; });

        // 1 cup
        myInputbuttons->addButtonReleaseHandler(A0button1Cup, 50, []() { brewCup(CommandComponent::Brew1Cup); });
        myInputbuttons->addButtonHoldHandler(A0button1Cup, 2000, []() { holdCupButtonHandler(ProgramComponent::oneCup); });
        myInputbuttons->addButtonReleaseHandler(A0button1Cup, 1000, []() {}); // this one is to prevent the BrewCup release to trigger

        // 2 cup
        myInputbuttons->addButtonReleaseHandler(A0button2Cup, 50, []() { brewCup(CommandComponent::Brew2Cup); });
        myInputbuttons->addButtonHoldHandler(A0button2Cup, 2000, []() { holdCupButtonHandler(ProgramComponent::twoCup); });
        myInputbuttons->addButtonReleaseHandler(A0button2Cup, 1000, []() {}); // this one is to prevent the BrewCup release to trigger
    }

    senseoNode.setProperty("buttonsAddon").send(useCustomizableButtonsAddon ? "true" : "false");
    Homie.getLogger() << endl << "☕☕☕☕ Enjoy your SenseoWifi ☕☕☕☕" << endl << endl;
}

/**
 *
 */
void loopHandler()
{
    /**
     * Update the low level LED state machine based on the measured LED timings.
     * (off, slow blinking, fast blinking, on)
     */
    mySenseoLed.updateState();

    /**
     * Update the inputButtons if the CustomizableButtonAddon has been detected
     */
    if (myInputbuttons)
        myInputbuttons->update();

    /**
     * Update of the main state machine
     */
    mySenseo.update(millis());
}

void setup()
{
    Serial.begin(115200);

    /**
     * Testing routine. Activate only in development environemt.
     * Tests the circuit and Senseo connections, loops indefinitely.
     *
     * Wifi will NOT BE AVAILABLE, no OTA!
     */
    if (false) testIO();

    /**
     * Homie specific settings
     */
    Homie_setFirmware("senseo-wifi", "2.0");
    Homie_setBrand("SenseoWifi");

    useCustomizableButtonsAddon = autoDetectCustomizableButtonsAddon();
    if (!useCustomizableButtonsAddon)
    {
        // setResetTrigger need to be call before the setup and therefor can't be based on the configuration variable
        pinMode(resetButtonPin, INPUT_PULLUP);
        Homie.setResetTrigger(resetButtonPin, LOW, 5000);
    }

    Homie.disableLedFeedback();
    Homie.setSetupFunction(setupHandler);
    Homie.setLoopFunction(loopHandler);

    /**
     * Homie: Options, see at the top of this file.
     */
    CupDetectorAvailableSetting.setDefaultValue(false);
    BuzzerSetting.setDefaultValue(false);
    PublishHomeAssistantDiscoveryConfig.setDefaultValue(false);

    /**
     * Homie: Advertise custom SenseoWifi MQTT topics
     * Nodes apparently need to be configured before setup
     * Unfortunately the config is only available after the setup so we'll end up configuring useless properties
     */
    senseoNode.advertise("buzzer").setName("Buzzer").settable(buzzerHandler).setDatatype("enum").setFormat(BuzzerComponent::getValidTunes());
    senseoNode.advertise("debug").setName("Debugging Information").setDatatype("string").setRetained(false);
    senseoNode.advertise("pendingCommands").setName("Current Commands (debug)").setDatatype("string").setRetained(false);
    senseoNode.advertise("processedCommands").setName("Current Commands (debug)").setDatatype("string").setRetained(false);
    senseoNode.advertise("opState").setName("Operational State").setDatatype("enum").setFormat("SENSEO_unknown,SENSEO_OFF,SENSEO_HEATING,SENSEO_READY,SENSEO_BREWING,SENSEO_NOWATER");
    senseoNode.advertise("ledState").setName("Led State").setDatatype("enum").setFormat("LED_unknown,LED_OFF,LED_SLOW,LED_FAST,LED_ON");
    senseoNode.advertise("power").setName("Power").setDatatype("boolean").settable(powerHandler);
    senseoNode.advertise("brew").setName("Brew").settable(brewHandler).setDatatype("enum").setFormat("1cup,2cup");
    senseoNode.advertise("brewedSize").setName("Brew Size").setDatatype("string").setRetained(false);
    senseoNode.advertise("outOfWater").setName("Out of Water").setDatatype("boolean");
    senseoNode.advertise("cupAvailable").setName("Cup Available");
    senseoNode.advertise("cupFull").setName("Cup Full");
    senseoNode.advertise("program1Cup").setName("Program1Cup").setDatatype("boolean").settable(program1CupHandler);
    senseoNode.advertise("program2Cup").setName("Program2Cup").setDatatype("boolean").settable(program2CupHandler);
    senseoNode.advertise("buttonsAddon").setName("Use Customizable Buttons Addon").setDatatype("boolean");

    Homie.onEvent(onHomieEvent);
    Homie.setup();
}

void loop()
{
    Homie.loop();
}
