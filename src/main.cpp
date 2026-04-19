/**
  dumbssg - a simple static site generator in C++

  copyright (c) 2026 RGB (admin@bojan.social)
*/

#include "generator.h"

#include <filesystem>
#include <iostream>

#include <yaml-cpp/yaml.h>

SiteConfig load_config(const std::string &path) {
  YAML::Node cfg = YAML::LoadFile(path);

  return {
      .title = cfg["title"].as<std::string>(),
      .base_url = cfg["base_url"].as<std::string>(""),
      .output_dir = cfg["output_dir"].as<std::string>("dist"),
      .content_dir = cfg["content_dir"].as<std::string>("content"),
      .templates_dir = cfg["templates_dir"].as<std::string>("templates"),
      .assets_dir = cfg["assets_dir"].as<std::string>("assets"),
  };
}

int main(int argc, char *argv[]) {
  std::string config_path;

  if (argc > 1) {
    config_path = argv[1];
  } else if (std::filesystem::exists("config.yaml")) {
    config_path = "config.yaml";
  } else if (std::filesystem::exists("config.yml")) {
    config_path = "config.yml";
  } else {
    throw std::runtime_error(
        "no config file found (config.yaml or config.yml)");
  }

  try {
    auto config = load_config(config_path);
    generate(config);

    std::cout << "done\n";
  } catch (const std::exception &e) {
    std::cerr << "error: " << e.what() << "\n";
    return 1;
  }
}