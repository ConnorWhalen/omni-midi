#ifndef OMNI_CONFIG_H
#define OMNI_CONFIG_H

#include <string>

#include <yaml-cpp/yaml.h>

class Config {
public:
    Config(const std::string &override_filename);
    std::string Get(const std::string &key, const std::string &default_value = "");
private:
    YAML::Node default_config;
    YAML::Node override_config;
};

#endif
