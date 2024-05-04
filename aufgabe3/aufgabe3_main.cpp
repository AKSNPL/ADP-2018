#include "Alignment.hpp"



using namespace std;
int main(int argc, char* argv[]){
  if ((argc < 3) || (argc > 7) || (argc ==4) || (argc==5)){
      std::cout << "Problem mit Argumenten nicht genuge Argumente" << std::endl;
      return 1;};
  if (argc==3){
    int match = 3;
    int mismatch=-1;
    int gap =-2;
    std::string gaps="";
    std::string text1(argv[1]);
    std::string text2(argv[2]);
    Alignment Input=Alignment(text1,text2);
    Input.compute(match,mismatch,gap,false);
    Input.getAlignment(text1,gaps,text2);
    std::cout << text2 << '\n';
    std::cout << gaps<< '\n';
    std::cout << text1 << '\n';
    std::cout <<"score:"<<Input.score()<<std::endl;
  };
  if (argc==6){
    std::string text1(argv[1]);
    std::string text2(argv[2]);
    std::string gaps="";
    int match = atoi(argv[3]);
    int mismatch=atoi(argv[4]);
    int gap =atoi(argv[5]);
    Alignment Input=Alignment(text1,text2);
    Input.compute(match,mismatch,gap,false);
    Input.getAlignment(text1,gaps,text2);
    std::cout << text1 << std::endl;
    std::cout << gaps<< std::endl;
    std::cout << text2 << std::endl;
    std::cout <<"score:"<<Input.score()<<std::endl;
  }
  if (argc==7){
    std::string text1(argv[1]);
    std::string text2(argv[2]);
    std::string gaps="";
    int match = atoi(argv[3]);
    int mismatch=atoi(argv[4]);
    int gap =atoi(argv[5]);
    Alignment Input=Alignment(text1,text2);
    Input.compute(match,mismatch,gap,true);
    Input.getAlignment(text1,gaps,text2);
    std::cout << text1 << std::endl;
    std::cout << gaps<< std::endl;
    std::cout << text2 << std::endl;
    std::cout <<"score:"<<Input.score()<<std::endl;
  }
}
