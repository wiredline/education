#include "logger.hpp"
#include "runner.hpp"

int main(int argc, char *argv[]) //'{"operation": "+", "args": [3, 5]}'
{
  Logger::Instance().LogInfo("Program started");
   // input = "{\"operation\": \"+\", \"args\": [3, 5]}";
  //help
   if(!Runner::helper(argc, argv)){
    return 0;
   }

  //start
  try{
  Runner runner(Runner::GetOptParse(argc, argv)); 
  runner.StartTask();
  }catch(const std::exception &error){
    Logger::Instance().LogError(error.what());
    return 1;
  }
  Logger::Instance().LogInfo("Program Closed");
  return 0;
}