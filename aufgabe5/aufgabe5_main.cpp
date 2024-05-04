#include <iostream>
#include "a5_util.hpp"
#include <string>
#include "QGramIndex.hpp"
#include <fstream>
int main(int argc,char* argv[]){
  if (argc!=3){
    std::cout<<"Wrong Input"<<std::endl;
  };
  std::string text;
  std::string Inputfile=argv[1];
  std::string query=argv[2];
  uint8_t qsize=query.size();
  std::ifstream myfile(Inputfile);
  std::getline(myfile,text);
  QGramIndex input = QGramIndex(text,qsize);
  uint32_t queryHash=input.hash(query);
  //std::cout<<queryHash<<std::endl;
 std::vector<uint32_t> Ergebnis=input.getHits(queryHash);
  std::cout<<query<<" :";
  int Rsize=Ergebnis.size();
  for (int i=0;i<Rsize;i++){
    std::cout<<Ergebnis[i]<<" ";
  };
  std::cout << std::endl;
  //std::cout << input.getText() << std::endl;
}
