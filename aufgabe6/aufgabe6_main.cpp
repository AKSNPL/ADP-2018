#include "DCACuts.hpp"
#include <iostream>

int main(int argc,char* argv[]){
  if (argc < 4) {std::cout<<"Wrong Input"<<std::endl;};
  std::vector<std::string> sequences;
  sequences.resize(argc-1);
  for (int i=1;i<argc;i++){
    sequences[i-1]=argv[i];
  };
  DCACuts Input=DCACuts(sequences);
  CostMatrix m;
  Input.computeCostMatrices();
  Input.computeCuts(sequences[0].size()/2);
  std::cout<<"additional cost :"<<Input.getOptimalCosts()<<std::endl;
  std::vector<std::vector<int>> Output=Input.getOptimalCuts();
  int row = Output.size();
  int col=Output[0].size();
  for (int i=0;i<row;i++){
    for (int j=0;j<col;j++){
      std::cout<<Output[i][j]<<" ";
    };
    std::cout<<std::endl;
  };
}
