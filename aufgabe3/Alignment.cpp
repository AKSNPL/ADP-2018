


#include "Alignment.hpp"




/// Constructor with two sequences
/// Makes an internal copy of the sequences.
 Alignment::Alignment(const std::string& seq1, const std::string& seq2){
  a=seq1;
  b=seq2;
  m=a.size()+1;
  n=b.size()+1;
}
int max_fc1(std::vector<std::vector<trace>>& T,const std::vector<std::vector<int>>& F,int i,int j,int a,int gap){
    int temp;
    if ((F[i-1][j-1]+a) >(F[i-1][j]+gap)){
                                                temp=F[i-1][j-1]+a;
                                                T[i][j].row=i-1;
                                                T[i][j].col=j-1;
                                                }
    else if ((F[i-1][j-1]+a)==(F[i-1][j]+gap)){temp=F[i-1][j-1]+a;
                                              T[i][j].row=i-1;
                                              T[i][j].col=j-1;}
    else {                                      temp=F[i-1][j]+gap;
                                                T[i][j].row=i-1;
                                                T[i][j].col=j;
                                                };
    if (temp<(F[i][j-1]+gap))                 {
                                                 T[i][j].row=i;
                                                 T[i][j].col=j-1;
                                                 temp= F[i][j-1]+gap;
                                               };
    if (temp <0)                               {T[i][j].row=-1;
                                                T[i][j].col=-1;
                                                temp=0;
                                                }
    return temp;
}

int max_fc(std::vector<std::vector<trace>>& T,const std::vector<std::vector<int>>& F,int i,int j,int a,int gap){
    int temp=0;
    if ((F[i-1][j-1]+a) >(F[i-1][j]+gap)){
                                                temp=F[i-1][j-1]+a;
                                                T[i][j].row=i-1;
                                                T[i][j].col=j-1;
                                                }
    else if ((F[i-1][j-1]+a)==(F[i-1][j]+gap)){temp=F[i-1][j-1]+a;
                                              T[i][j].row=i-1;
                                              T[i][j].col=j-1;}
    else {                                      temp=F[i-1][j]+gap;
                                                T[i][j].row=i-1;
                                                T[i][j].col=j;
                                                };
    if (temp<(F[i][j-1]+gap))                 {
                                                 T[i][j].row=i;
                                                 T[i][j].col=j-1;
                                                 return F[i][j-1]+gap;
                                                }
    else {return temp;};
}
/// compute the aligment (i.e. score and traceback)
/// given the three alignment parameters match, mismatch and gap
/// if local_align == true, compute the local Smith-Waterman (SW) alignment (extra points), or throw
/// an exception if your implementation does not support SW.
void Alignment::compute(const int match, const int mismatch, const int gap, const bool local_align){
  if (local_align==false) {
  F.resize(m);
  T.resize(m);
  for (int i=0;i<m;i++){
    F[i].resize(n);
    T[i].resize(n);
  }
  F[0][0]=0;
  //Übergabe die Werte für erste Reihe von dem Matrix
  for (int i=1;i<m;i++){
    F[i][0]=(i*gap);
    T[i][0].row=i-1;
    T[i][0].col=0;
  };
  //Übergabe die Werte für erste Spalte von dem Matrix
  for (int j=1;j<n;j++){
    F[0][j]=(j*gap);
    T[0][j].row=0;
    T[0][j].col=j-1;
  };
  //builds the Rests from the Matrix
  for(int i=1;i<m;i++){
    for (int j=1;j<n;j++){
      if (a[i-1]==b[j-1]){
                          F[i][j]=max_fc(T,F,i,j,match,gap);}
                else  {   F[i][j]=max_fc(T,F,i,j,mismatch,gap);}
  }
};
}
  else {
    F.resize(m);
    T.resize(m);
    for (int i=0;i<m;i++){
      F[i].resize(n);
      T[i].resize(n);
    }
    F[0][0]=0;
    T[0][0].row=-1;
    T[0][0].col=-1;
    for (int i=1;i<m;i++){
      F[i][0]=0;
      T[i][0].row=-1;
      T[i][0].col=-1;
    };
    for (int j=1;j<n;j++){
      F[0][j]=0;
      T[0][j].row=-1;
      T[0][j].col=-1;
    };
    for(int i=1;i<m;i++){
      for (int j=1;j<n;j++){
        if (a[i-1]==b[j-1]){
                            F[i][j]=max_fc1(T,F,i,j,match,gap);
                            if (F[i][j]>F[row_max][col_max]){row_max=i;col_max=j;};
                          }
                  else  {   F[i][j]=max_fc1(T,F,i,j,mismatch,gap);
                          if (F[i][j]>F[row_max][col_max]){row_max=i;col_max=j;};
                  }
    }
  };
  waterman=true;
  }
  check=true;
}
/// return the score of the alignment;
/// Throws an exception if compute(...) was not called first
int Alignment::score(){
    if (check) {
      if(waterman){return F[row_max][col_max];}
      else {return F[m-1][n-1];}
    }
    else {throw std::logic_error("compute zuerst");}
}
/// output alignment into three strings.
/// Gaps are denoted as '-' in sequences.
/// The gap-string uses '|' (match) and ' ' (mismatch/gap).
/// Note: all strings are equal in size
/// e.g.
/// a1:   "IMISSMISSIS-SIPPI-"
/// gaps: " |   ||||||  |||| "
/// a2:   "-M--YMISSISAHIPPIE"
/// , where a1 corresponds to seq1, etc.
/// Throws an exception if compute(...) was not called first
void Alignment:: getAlignment(std::string& a1, std::string& gaps, std::string& a2){

if (check){
  if (waterman==false){
  int i=m-1;
  int j=n-1;
  int temp;

  if (a1.size() != 0){a1.clear();}
  if (a2.size() != 0){a2.clear();}
  if (gaps.size() != 0){gaps.clear();}
  while ((i!=0) || (j!=0)){
    if ((T[i][j].row == i-1) && (T[i][j].col==j-1)){
                                                    if (b[j-1]==a[i-1]){gaps.push_back ('|');}
                                                    else {gaps.push_back (' ');}
                                                    a2.push_back(b[j-1]);
                                                    a1.push_back(a[i-1]);

                                                }
      else if ((T[i][j].row == i-1) && (T[i][j].col==j)){gaps.push_back(' ');
                                                        a2.push_back('-') ;
                                                        a1.push_back(a[i-1]) ;

                                                    }
      else {gaps.push_back(' ');
            a2.push_back(b[j-1]) ;
            a1.push_back('-') ;

    }
    temp=i;
    i=T[i][j].row;
    j=T[temp][j].col;
  }
}
else {

  int i=row_max;
  int j=col_max;
  int temp;
  if (a1.size() != 0){a1.clear();}
  if (a2.size() != 0){a2.clear();}
  if (gaps.size() != 0){gaps.clear();}
  while (F[i][j]!=0){
    if ((T[i][j].row == i-1) && (T[i][j].col==j-1)){
                                                    if (b[j-1]==a[i-1]){gaps.push_back ('|');}
                                                    else {gaps.push_back (' ');}
                                                    a2.push_back(b[j-1]);
                                                    a1.push_back(a[i-1]);

                                                }
      else if ((T[i][j].row == i-1) && (T[i][j].col==j)){gaps.push_back(' ');
                                                        a2.push_back('-') ;
                                                        a1.push_back(a[i-1]) ;

                                                    }
      else {gaps.push_back(' ');
            a2.push_back(b[j-1]) ;
            a1.push_back('-') ;

    }
    temp=i;
    i=T[i][j].row;
    j=T[temp][j].col;
    
  }
}
  std::reverse(a1.begin(),a1.end());
  std::reverse(gaps.begin(),gaps.end());
  std::reverse(a2.begin(),a2.end());
}
else {throw std::logic_error("compute zuerst");}
}
