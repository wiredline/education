#ifndef JSONPARSE_H
#define JSONPARSE_H

#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "logger.hpp"

class JsonParse {
private:
  std::string m_op;
  std::vector<int> m_DataSet;

public:
  JsonParse() {}
  void Parse(const std::string &input) {
    using json = nlohmann::json;
    try {
      json j = json::parse(input);
      if (!j.contains("operation") || !j.contains("args"))
        throw std::invalid_argument("Missing required fields");
      m_op = j.at("operation").get<std::string>();
      m_DataSet = j.at("args").get<std::vector<int>>();

    } catch (const json::parse_error &e) {
      Logger::Instance().LogError(e.what());
      throw;
    } catch (const std::exception &e) {
      Logger::Instance().LogError(e.what());
      throw;
    }
  }
  const std::string &GetOperation() const { return m_op; }
  const std::vector<int> &GetArgs() const { return m_DataSet; }
};

#endif