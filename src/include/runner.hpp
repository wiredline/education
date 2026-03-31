#ifndef RUNNER_H
#define RUNNER_H

#include <iostream>
#include <string>
#include <vector>
#include "DataCalculate.hpp"
#include "JsonCheck.hpp"
#include "JsonParse.hpp"
#include "Printer.hpp"
#include "logger.hpp"
#include <getopt.h>

class Runner {
private:
  std::vector<int> DataSet; // fnum, snum, result
  std::string m_operation;
  std::string m_input;
  int result;

public:
  explicit Runner(const std::string& input) : m_input(input) {}
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
  
   void StartTask(){
    // Parser
    JsonParse parser;
    parser.Parse(m_input);
    DataSet = parser.GetArgs();
    m_operation = parser.GetOperation();

    // Checker
    JsonCheck checker;
    try {
      checker.Validate(m_operation, DataSet);
    } catch (const std::exception &exep) {
      Logger::Instance().LogError(exep.what());
      throw;
    }

    // Calculate
    auto operation = DataCalculate::Create(m_operation);
    result = operation->calculate(DataSet);

    // Printable
    Printer print(result);
    print.PrintToCMD();
  }

  const std::string &GetOperation() const { return m_operation; }
  const std::vector<int> &GetArgs() const { return DataSet; }
};
#endif