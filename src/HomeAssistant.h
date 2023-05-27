#pragma once

#include <map>
#include <ArduinoJson.h>

/**
 * Helpers to publish HomeAssistant Discovery Config
 */

class HomeAssistantDiscovery
{
public:
    HomeAssistantDiscovery();
    bool publishBinarySensorConfig(const char *friendlyName, const char *topicName, const std::map<String, String> &attributes);
    bool publishSensorConfig(const char *friendlyName, const char *topicName, const std::map<String, String> &attributes);
    bool publishStatConfig(const char *friendlyName, const char *topicName, const std::map<String, String> &attributes);
    bool publishSwitchConfig(const char *friendlyName, const char *topicName, const std::map<String, String> &attributes);
    bool publishButtonConfig(const char *friendlyName, const char *topicName, const char *buttonPayload, const std::map<String, String> &attributes);

    const String & getMachineTopic() const { return machineTopic; }

private:
    void preparePayload(DynamicJsonDocument &jsonPayload, const char *friendlyName, const char *topicName, const std::map<String, String> &attributes);

    String binarySensorTopic;
    String sensorTopic;
    String buttonTopic;
    String switchTopic;
    String machineTopic;
    String statsTopic;
    String availabilityTopic;
    String uniqueIdBase;
};
