#include <vector>
#include <string>
#include <algorithm>
#include "DCACuts.hpp"
/// Constructor, which copies the sequences to an internal member
/// @throws Exception std::logic_error if one or no sequences are given (i.e. at least two sequences are required)
DCACuts::DCACuts(const std::vector<std::string>& sequences){
  if (sequences.size() < 2)
	{
		throw std::logic_error("Not enough sequences");
	}
	else {
    seqs.resize(sequences.size());
    size_seqs.resize(sequences.size());
    int size=sequences.size();
		for (int i=0;i<size;i++){
      seqs[i]=sequences[i];
      size_seqs[i]=seqs[i].size();
      possible_size*=(seqs[i].size()+1);
    }
    possible_size/=(seqs[0].size()+1);
		sqsize=seqs.size();
	}
}
//Matrixaufbau
int minM(const std::string& sq1, const std::string& sq2 ,std::vector<std::vector<int>> Mt,int i, int j ){
	if (sq1[i-1]==sq2[j-1]){
		return std::min(std::min(Mt[i-1][j-1]+0,Mt[i-1][j]+1),Mt[i][j-1]+1);
	}
	else {
		return std::min(std::min(Mt[i-1][j-1]+1,Mt[i-1][j]+1),Mt[i][j-1]+1);
	}
}
int minMrev(const std::string& sq1, const std::string& sq2 ,std::vector<std::vector<int>> Mtrev,int i, int j ){
	if (sq1[i]==sq2[j]){
		return std::min(std::min(Mtrev[i+1][j+1]+0,Mtrev[i+1][j]+1),Mtrev[i][j+1]+1);
	}
	else {
		return std::min(std::min(Mtrev[i+1][j+1]+1,Mtrev[i+1][j]+1),Mtrev[i][j+1]+1);
	}
}
//Needleman
CostMatrix Needleman (const std::string& sq1,const std::string& sq2){
	int sz1=sq1.size();
	int sz2=sq2.size();
	std::vector<std::vector<int>> Mt,Mtrev;
	CostMatrix result;
	Mt.resize(sz1+1,std::vector<int>(sz2+1));
	Mtrev.resize(sz1+1,std::vector<int>(sz2+1));
  result.m.resize(sz1+1,std::vector<int>(sz2+1));
	//Initial
	for (int i=0;i<sz1+1;i++){
                  Mt[i][0]=i;
                  Mtrev[sz1-i][sz2]=i;
                };
	for (int j=1;j<sz2+1;j++){
                  Mt[0][j]=j;
                  Mtrev[sz1][sz2-j]=j;
                };

	//Build the Matrix
	for (int i=1;i<sz1+1;i++){
		for(int j=1;j<sz2+1;j++){
			Mt[i][j]=minM(sq1,sq2,Mt,i,j);
			Mtrev[sz1-i][sz2-j]=minMrev(sq1,sq2,Mtrev,sz1-i,sz2-j);
		}
	}
 int Bscore=Mt[sz1][sz2];
	//build CostMatrix
	for(int i=0;i<sz1+1;i++){
		for (int j=0;j<sz2+1;j++){
			result.m[i][j]=Mt[i][j]+Mtrev[i][j]-Bscore;
		}
	}
	return result;
}
/// Compute the cost matrices of all pairs s and t
/// Stores result internally.
// Here the algorithm iterates over all pairs of sequences i and j,
// whereby i < j.
// For every pair the forward matrix and the backward matrix is computed.
// Then the cost matrix is computed from these two matrices and finally stored internally.
void DCACuts::computeCostMatrices(){
  costm.resize(sqsize,std::vector<CostMatrix>(sqsize));
  for (unsigned i=0; i<sqsize-1;i++){
		for (unsigned j=i+1;j<sqsize;j++){
			CostMatrix result =Needleman(seqs[i],seqs[j]);
			costm[i][j]=result;
		};
	}
  Matrix=true;
}


/// returns the Cost-Matrix for sequences i [0..N-1) and j [0..N-1).
/// @throws Exception std::logic_error if computeCostMatrices() was not called before (i.e. internal matrices are missing)
//need do
/// @throws Exception std::logic_error if i == j
/// @throws Exception std::out_of_range if i >= N or j >= N.
  CostMatrix DCACuts::getCostMatrix(unsigned i, unsigned j) {
  	unsigned gross,klein;
    if (Matrix==false){throw std::logic_error("computeCostMatrix has not been called");};
    if (i==j) {throw std::logic_error("Cost-Matrix doesn't exist");};
  	if((i>=sqsize)||(j>=sqsize)){throw std::out_of_range("out of range");};
  	if (i>j){gross=i;klein=j;}
  		else {klein=i;gross=j;};
  	return costm[klein][gross];
  }

//get additional cost
int getAddCost (std::vector<int> & pos ,std::vector<std::vector<CostMatrix>> costm,unsigned seqnumb ){
	int summe=0;
	for (unsigned i=0; i<seqnumb;i++){
		for (unsigned j=i+1;j<seqnumb+1;j++){
			summe += costm[i][j].m[pos[i]][pos[j]];
		}
	}
  return summe;
}
//branch and bound
void BandB (std::vector<int> & pos,std::vector<int>& size_seqs,unsigned seqnumb,int add_cost,std::vector<std::vector<int>> & optimalcut,std::vector<std::vector<CostMatrix>> & costm,int& minimum,int possible_size,unsigned sqsize,int& ct){
  if (add_cost>minimum){
    return;
  }
	//recursion
  if ((add_cost==minimum)&&(seqnumb==sqsize)){
      optimalcut[ct]=pos;
      ct++;
      return;
  }
	if ((add_cost<minimum)&&(seqnumb==sqsize)){
    optimalcut.clear();
    optimalcut.resize(possible_size,std::vector<int>(sqsize));
    ct=0;
    minimum=add_cost;
    optimalcut[ct]=pos;
    ct++;
    return;
  }
	for (pos[seqnumb]=0;pos[seqnumb] <= size_seqs[seqnumb];++pos[seqnumb]){
		//add cost
		int add_cost=getAddCost(pos,costm,seqnumb);
		BandB(pos,size_seqs,seqnumb+1,add_cost,optimalcut,costm,minimum,possible_size,sqsize,ct);
	};
	return;
}
/// Compute the c optimal cuts, by fixing the first sequence at the given position.
/// This function is invoked by the user (in main()) to compute the optimal cuts,
/// after the cost matrices have been computed.
/// It takes a position of the first sequence which is fixed.
/// Using this position, cut positions of the remaining sequences are enumerated and the optimal position(s) are found.
/// @throws Exception std::logic_error if computeCostMatrices() was not called before (i.e. internal matrices are missing).
void DCACuts::computeCuts(int p1){
  if (Matrix==false) {
		throw std::logic_error("computeCostMatrix has not been called");
	};
	pos.resize(sqsize);
	pos[0]=p1;
	int seqnumb=1;
	int add_cost=0;
  optimalcut.resize(possible_size,std::vector<int>(sqsize));
	minimum=getAddCost(pos,costm,seqnumb);
	BandB(pos,size_seqs,seqnumb,add_cost,optimalcut,costm,minimum,possible_size,sqsize,ct);
  Cut=true;
}

/// Return the optimal cost computed during computeCuts() method.
/// @throws Exception std::logic_error if computeCuts() was not called before (i.e. internal results are missing).
int DCACuts::getOptimalCosts(){
  if (Cut==false){throw std::logic_error("computeCuts() was not called");};
  return minimum;
}

/// Return a vector containing all optimal cuts (inner vector is of size N, where N is the number of sequences).
/// The outer vector is of size 1 or longer (since there could be multiple optimal cuts with the same score).
const std::vector<std::vector<int> >& DCACuts::getOptimalCuts(){
  Ergebnis.resize(ct,std::vector<int>(sqsize));
  for (int i=0;i<ct;i++){
    Ergebnis[i]=optimalcut[i];
  };
  return Ergebnis;
}
