/**
  dumbssg - a simple static site generator in C++

  copyright (c) 2026 RGB (admin@bojan.social)
*/

#include "generator.h"
#include "parser.h"

#include <algorithm>
#include <filesystem>
#include <iostream>

#include <inja/inja.hpp>

void generate(const SiteConfig &config) {
  inja::Environment env;

  auto post_template = env.parse_template(config.templates_dir + "/post.html");
  auto index_template =
      env.parse_template(config.templates_dir + "/index.html");

  std::vector<Post> posts;
  for (auto &entry : std::filesystem::directory_iterator(config.content_dir)) {
    if (entry.path().extension() == ".md") {
      std::cout << "parsing: " << entry.path() << "\n";
      posts.push_back(parse_file(entry.path().string()));
    }
  }

  // sort by date descending
  std::sort(posts.begin(), posts.end(),
            [](const Post &a, const Post &b) { return a.date > b.date; });

  std::filesystem::create_directories(config.output_dir);

  // render each post
  for (auto &post : posts) {
    nlohmann::json data;
    data["site_title"] = config.title;
    data["title"] = post.title;
    data["date"] = post.date;
    data["content"] = post.html;
    data["tags"] = post.tags;

    std::string out_dir = config.output_dir + "/" + post.slug;
    std::filesystem::create_directories(out_dir);

    std::ofstream out(out_dir + "/index.html");
    out << env.render(post_template, data);

    std::cout << "generated: " << out_dir << "/index.html\n";
  }

  nlohmann::json index_data;
  index_data["site_title"] = config.title;
  index_data["posts"] = nlohmann::json::array();

  for (auto &post : posts) {
    nlohmann::json p;
    p["title"] = post.title;
    p["date"] = post.date;
    p["slug"] = post.slug;
    p["tags"] = post.tags;
    index_data["posts"].push_back(p);
  }

  if (std::filesystem::exists(config.assets_dir)) {
    std::filesystem::copy(config.assets_dir, config.output_dir + "/assets",
                          std::filesystem::copy_options::recursive |
                              std::filesystem::copy_options::update_existing);

    std::cout << "copied assets\n";
  }

  std::ofstream index_out(config.output_dir + "/index.html");
  index_out << env.render(index_template, index_data);
  std::cout << "generated: index.html\n";
}