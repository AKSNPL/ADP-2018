#include "BLAST_Neighborhood.hpp"
#include <omp.h>


//Score rechnen zwischen Substring von query und Möglichkeiten
int scoreSub (const ScoreMatrix& matrix,const std::string& substring,int word_size,std::vector<int>& sub0,std::string& abc){
  int scoreAli =0;
  for (int i=1;i<word_size+1;i++){
    scoreAli= scoreAli+matrix.score(toAminoAcid(toId(substring[i-1])),toAminoAcid(sub0[i]));
	abc.push_back(toAminoAcid(sub0[i]));
  //weil scoreSub alle Elemente in sub0 prüft , deshalb wird das benutzt um Elemente von sub0 zu beschränken
	if (sub0[i]>=19) {
           sub0[i]=0;
					 sub0[i-1]++;
						};
  };
  return scoreAli;
}
//END RECHNEN
std::vector<NHResult> BLAST_Neighborhood::generateNeighborhood(const std::string& query,
                                           const ScoreMatrix& matrix,
                                           const int word_size,
                                           const int score_threshold,
                                           const int threads){
		std::string abc="";
		std::vector<int> sub0 ;
        sub0.resize(word_size+1);

        if ((word_size<1) || (threads!=1)){
          throw std::exception();
        };

        std::vector<NHResult> ergebnis ;//VARIABLE TO RETRUN
        std::vector< std::pair <std::string, int> > my_vec;//VARIABLE IN NHResult
        int size = query.size()-word_size+1;
        NHResult r;
        //Substrings von query werden hier gespeichert
        std::vector<std::string> substring;
        for (int i=0;i<size;i++){
          substring.push_back(query.substr(i,word_size));
        };
        int sizesub=substring.size();

        //vergleichen alle Substrings mit alle Möglichkeiten
		for (int i=0; i<sizesub;i++){
      //setzen sub0 zu 0
			for (int j=0; j<word_size+1 ; j++){
	                 sub0[j]=0;
                };
            my_vec.clear();
			int scoreAl;
      //wenn das sub0[0] >0 gilt , dann wurden schon alle Möglichkeiten verglichen
			while (sub0[0]==0){

			scoreAl=scoreSub(matrix,substring[i],word_size,sub0,abc);
      //vergleich zwischen score und threshold
			if (scoreAl>=score_threshold){
					my_vec.push_back(make_pair(abc,scoreAl));
					sub0[word_size]++;
					abc.clear();
				} else {
						sub0[word_size]++;
						abc.clear();
						}
		};
		r.infix=substring[i];
		r.neighbours=my_vec;
		ergebnis.push_back(r);
		}

    return ergebnis;
}
