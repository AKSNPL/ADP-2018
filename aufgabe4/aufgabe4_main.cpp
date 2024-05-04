#include "BLAST_Neighborhood.hpp"
#include <omp.h>
int main(int argc , char* argv[]){

  std::vector<NHResult> mainergebnis;
  double start,end;
  std::string query =argv[1];
  ScoreMatrix a;
  a.load(argv[2]);
  int word_size=atoi(argv[3]);
  int score_threshold=atoi(argv[4]);
  int thread =atoi(argv[5]);
  int b=query.size();
  BLAST_Neighborhood ergebnis;

  start=omp_get_wtime();
  if (b>=word_size){
  mainergebnis=ergebnis.generateNeighborhood(query,a,word_size,score_threshold,thread);};
  end=omp_get_wtime();
  int result = mainergebnis.size();
  for (int i=0; i<result;i++){
    std::cout<<mainergebnis[i].infix<<": ";
    int size2=mainergebnis[i].neighbours.size();
    for (int j=0;j<size2;j++){
      std::cout << "("<<mainergebnis[i].neighbours[j].first<<","<<mainergebnis[i].neighbours[j].second<<")"<< ' ';
    };
    std::cout<<std::endl;
  }
  std::cout<<"time: "<<end-start<<"s"<<std::endl;
  return 0;
  }
