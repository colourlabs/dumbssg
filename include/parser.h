/**
  dumbssg - a simple static site generator in C++

  copyright (c) 2026 RGB (admin@bojan.social)
*/

#pragma once

#include <map>
#include <string>
#include <vector>

struct Post {
  std::string title;
  std::string date;
  std::string slug;
  std::vector<std::string> tags;
  std::map<std::string, std::string> meta;

  // content
  std::string markdown;
  std::string html; // rendered markdown
  std::string path; // source file path
};

struct SiteConfig {
  std::string title;
  std::string base_url;
  std::string output_dir;
  std::string content_dir;
  std::string templates_dir;
  std::string assets_dir;
};

Post parse_file(const std::string &filepath);
