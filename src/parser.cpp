/**
  dumbssg - a simple static site generator in C++

  copyright (c) 2026 RGB (admin@bojan.social)
*/

#include "parser.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <md4c-html.h>
#include <yaml-cpp/yaml.h>

static void md4c_callback(const MD_CHAR *output, MD_SIZE size, void *userdata) {
  auto *result = static_cast<std::string *>(userdata);
  result->append(output, size);
}

std::string markdown_to_html(const std::string &markdown) {
  std::string html;

  md_html(markdown.c_str(), markdown.size(), md4c_callback, &html,
          MD_DIALECT_GITHUB, 0);

  return html;
}

std::string read_file(const std::filesystem::path &path) {
  std::ifstream file(path);

  if (!file)
    throw std::runtime_error("cannot open: " + path.string());

  std::ostringstream ss;
  ss << file.rdbuf();
  return ss.str();
}

Post parse_file(const std::string &filepath) {
  std::string content = read_file(filepath);

  Post post;
  post.path = filepath;

  if (content.substr(0, 3) == "---") {
    auto end = content.find("---", 3);
    if (end == std::string::npos)
      throw std::runtime_error("unclosed frontmatter in: " + filepath);

    std::string yaml_str = content.substr(3, end - 3);
    post.markdown = content.substr(end + 3);

    YAML::Node front = YAML::Load(yaml_str);
    post.title = front["title"].as<std::string>("");
    post.date = front["date"].as<std::string>("");
    post.slug = front["slug"].as<std::string>("");

    if (front["tags"]) {
      for (auto tag : front["tags"])
        post.tags.push_back(tag.as<std::string>());
    }

    // store anything else in meta
    for (auto it = front.begin(); it != front.end(); ++it) {
      std::string key = it->first.as<std::string>();
      if (key != "title" && key != "date" && key != "slug" && key != "tags")
        post.meta[key] = it->second.as<std::string>("");
    }
  } else {
    post.markdown = content;
  }

  if (post.slug.empty()) {
    std::filesystem::path p(filepath);
    post.slug = p.stem().string();
  }

  post.html = markdown_to_html(post.markdown);

  return post;
}