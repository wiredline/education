#ifndef RUNNER_H
#define RUNNER_H

#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include <memory>
#include "DataCalculate.hpp"
#include "JsonCheck.hpp"
#include "JsonParse.hpp"
#include "Printer.hpp"
#include "logger.hpp"
#include "DbRepository.hpp"
#include "cache.hpp"
#include "KeyUtil.hpp"

class Runner {
private:
  std::vector<int> DataSet; // fnum, snum, result
  std::string m_operation;
  std::string m_input;
  int result;
  std::unique_ptr<PgClient> m_db;
  std::unique_ptr<DbRepository> m_repo;
  inline static Cache m_cache;

public:
  explicit Runner(const std::string& input) : m_input(input) {
    m_db = std::make_unique<PgClient>(
      "host=localhost port=5432 dbname=calc_db user=calc_user password=P@ssw0rd"
    );
    m_repo = std::make_unique<DbRepository>(*m_db);

    WarmUpCache();
  }
  // help
  static bool helper(int argc, char *argv[]){
      
    if (argc == 1 || (argc > 1 && std::string(argv[1]) == "-h")) {
      std::cout << "This program use for calculation"<<std::endl;
      std::cout << "How to use:"<<std::endl;
      std::cout << "Can addition(+), subtraction(-), multiplication(*), division(/), "
             "exponentiation(^), factorial(!)"<<std::endl;
      std::cout << "you need to insert json"<<std::endl;
      std::cout << "'{\"operation\": \"+\", \"args\": [3, 5]}'"<<std::endl;
      return false;
    }
    return true;
  }

  static std::string GetOptParse(int argc, char *argv[]){
    std::string json_input;
    int opt;

    while ((opt = getopt(argc, argv, "j:")) != -1) {
        switch (opt) {
            case 'j':
                json_input = optarg;
                return json_input;
            default:
                throw std::runtime_error("Invalid options");
        }
    }
    throw std::runtime_error("JSON input not provided. Use -j '<json>'");
  }
  
  void WarmUpCache(){
    Logger::Instance().LogInfo("Loading Cache from DB");
    auto data = m_repo->LoadAll();

    for(const auto& [key, result, status] : data){
      m_cache.Put(key, {result, status});
    }

    Logger::Instance().LogInfo("cache loaded with "+ std::to_string(data.size()));
  }

   void StartTask(){
    // 1.Parser
    JsonParse parser;
    parser.Parse(m_input);
    DataSet = parser.GetArgs();
    m_operation = parser.GetOperation();

    //2.Checker
    JsonCheck checker;
    try {
      checker.Validate(m_operation, DataSet);
    } catch (const std::exception &exep) {
      Logger::Instance().LogError(exep.what());
      throw;
    }

    //3. Key
    std::string key = MakeKey(m_operation, DataSet);

    //4. Cache check
    auto cached = m_cache.Get(key);
    if (cached.has_value()){
      Logger::Instance().LogInfo("Cache hit");
      Printer print(cached->result);
      print.PrintToCMD();
      return;
    }
    Logger::Instance().LogInfo("Cache miss");

    // 5.Calculate
    auto operation = DataCalculate::Create(m_operation);

    int result =0;
    int status =0; //0 -ok 1-error

    try{
      result = operation->calculate(DataSet);
    }catch(...){
      status =1;
    }

    //6.Save to cache
    m_cache.Put(key, {result, status});

    //7. Save to DB
    m_repo->InsertOperation(m_input,key, result, status);

    // Printable
    Printer print(result);
    print.PrintToCMD();
  }

  const std::string &GetOperation() const { return m_operation; }
  const std::vector<int> &GetArgs() const { return DataSet; }
};
#endif