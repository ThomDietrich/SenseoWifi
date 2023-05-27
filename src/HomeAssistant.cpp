#include <Homie.h>
#include "HomeAssistant.h"

HomeAssistantDiscovery::HomeAssistantDiscovery()
{
    binarySensorTopic = String("homeassistant/binary_sensor/") + String(Homie.getConfiguration().deviceId) + String("/");
    sensorTopic = String("homeassistant/sensor/") + String(Homie.getConfiguration().deviceId) + String("/");
    switchTopic = String("homeassistant/switch/") + String(Homie.getConfiguration().deviceId) + String("/");
    buttonTopic = String("homeassistant/button/") + String(Homie.getConfiguration().deviceId) + String("/");

    machineTopic = String(Homie.getConfiguration().mqtt.baseTopic) + String(Homie.getConfiguration().deviceId) + String("/machine/");
    statsTopic = String(Homie.getConfiguration().mqtt.baseTopic) + String(Homie.getConfiguration().deviceId) + String("/$stats/");
    availabilityTopic = String(Homie.getConfiguration().mqtt.baseTopic) + String(Homie.getConfiguration().deviceId) + String("/$state");
    uniqueIdBase = String("uniqueid__") + String(Homie.getConfiguration().deviceId) + String("_");
}

void HomeAssistantDiscovery::preparePayload(DynamicJsonDocument & jsonPayload, const char * friendlyName,const char * topicName, const std::map<String,String> &attributes)
{
    jsonPayload["name"] = String(Homie.getConfiguration().name) + String(" ") + String(friendlyName);
    jsonPayload["unique_id"] = uniqueIdBase + String(topicName);
    
    jsonPayload["availability_topic"] = availabilityTopic.c_str();
    jsonPayload["payload_available"] = "ready";
    jsonPayload["payload_not_available"] = "lost";

    for(auto attribute: attributes) 
    {
        jsonPayload[attribute.first] = attribute.second;
    }
}

bool HomeAssistantDiscovery::publishBinarySensorConfig(const char * friendlyName, const char * topicName,const std::map<String,String> &attributes)
{
    DynamicJsonDocument jsonPayload(1024);
    
    preparePayload(jsonPayload,friendlyName,topicName,attributes);

    jsonPayload["state_topic"] = machineTopic + String(topicName);
    jsonPayload["payload_off"] = "false";
    jsonPayload["payload_on"] = "true";

    char payloadBuffer[1024];
    size_t payloadSize = serializeJson(jsonPayload,payloadBuffer);
    return Homie.getMqttClient().publish((binarySensorTopic + String(topicName) + String("/config")).c_str(),0,true,payloadBuffer,payloadSize) != 0;
}

bool HomeAssistantDiscovery::publishSensorConfig(const char * friendlyName, const char * topicName,const std::map<String,String> &attributes)
{
    DynamicJsonDocument jsonPayload(1024);
    
    preparePayload(jsonPayload,friendlyName,topicName,attributes);

    jsonPayload["state_topic"] = machineTopic + String(topicName);
    
    char payloadBuffer[1024];
    size_t payloadSize = serializeJson(jsonPayload,payloadBuffer);
    return Homie.getMqttClient().publish((sensorTopic + String(topicName) + String("/config")).c_str(),0,true,payloadBuffer,payloadSize) != 0;
}

bool HomeAssistantDiscovery::publishStatConfig(const char * friendlyName, const char * topicName,const std::map<String,String> &attributes)
{
    DynamicJsonDocument jsonPayload(1024);
    
    preparePayload(jsonPayload,friendlyName,topicName,attributes);

    jsonPayload["state_topic"] = statsTopic + String(topicName);
    
    char payloadBuffer[1024];
    size_t payloadSize = serializeJson(jsonPayload,payloadBuffer);
    return Homie.getMqttClient().publish((sensorTopic + String(topicName) + String("/config")).c_str(),0,true,payloadBuffer,payloadSize) != 0;
}

bool HomeAssistantDiscovery::publishSwitchConfig(const char * friendlyName, const char * topicName,const std::map<String,String> &attributes)
{
    DynamicJsonDocument jsonPayload(1024);
    
    preparePayload(jsonPayload,friendlyName,topicName,attributes);

    jsonPayload["state_topic"] = machineTopic + String(topicName);
    jsonPayload["state_on"] = "true";
    jsonPayload["state_off"] = "false";
    
    jsonPayload["command_topic"] = machineTopic + String(topicName) + String("/set");
    jsonPayload["payload_on"] = "true";
    jsonPayload["payload_off"] = "false";
    
    char payloadBuffer[1024];
    size_t payloadSize = serializeJson(jsonPayload,payloadBuffer);
    return Homie.getMqttClient().publish((switchTopic + String(topicName) + String("/config")).c_str(),0,true,payloadBuffer,payloadSize) != 0;
}

bool HomeAssistantDiscovery::publishButtonConfig(const char * friendlyName, const char * topicName,const char * buttonPayload,const std::map<String,String> &attributes)
{
    DynamicJsonDocument jsonPayload(1024);
    
    String buttonHaTopic = String(topicName) + String("_") + String(buttonPayload);
    preparePayload(jsonPayload,friendlyName,buttonHaTopic.c_str(),attributes);

    jsonPayload["command_topic"] = machineTopic + String(topicName) + String("/set");
    jsonPayload["payload_press"] = buttonPayload;
    
    char payloadBuffer[1024];
    size_t payloadSize = serializeJson(jsonPayload,payloadBuffer);
    return Homie.getMqttClient().publish((buttonTopic + String(buttonHaTopic) + String("/config")).c_str(),0,true,payloadBuffer,payloadSize) != 0;
}

