#include <iostream>
#include <string>
#include <algorithm>
#include <math.h>
#include <vector>
#include <array>
#include <stdint.h>
#include <functional>
#include "aufgabe2.hpp"

using namespace std;

/// Build suffix array from text.
/// Input: the text (might be empty)
/// Output: a suffix array (sorted). The variable 'sa' is cleared before it's filled and returned.
int compare ( uint32_t suf1 , uint32_t suf2 ,const std::string& text){
	if(suf1==suf2){return -1;}
  else if(suf1!=suf2){
  while (text[suf1]==text[suf2]){
		suf1++;
		suf2++;
	}
	if (text[suf1]>text[suf2]){return 1;}
	else if (text[suf1]<text[suf2]){return 0;};

}
return 0;
}
int partition (std::vector<uint32_t>& sa,const std::string& text, int low, int high)
{
    uint32_t pivot = sa[high];    // pivot
    int left = low;
    int right = high - 1;
    while(true){
        while (left <= right && (compare(sa[left],pivot,text)==0)) left++;
        while(right >= left && (compare(sa[right],pivot,text)==1)) right--;
        if (left >= right) break;
        std::swap(sa[left], sa[right]);
        left++;
        right--;
    }
    std::swap(sa[left], sa[high]);
    return left;
}
void quickSort(std::vector<uint32_t>& sa,const std::string& text, int low, int high)
{
    if (low < high)
    {

        int pi = partition(sa,text, low, high);


        quickSort(sa,text, low, pi - 1);
        quickSort(sa,text, pi + 1, high);
    }
}

void construct(std::vector<uint32_t>& sa, const std::string& text){
  //pruefen ob sa leer ist
  if(text.empty()) {
      cout << "Leeres String" << endl;
      return;
  }

  if (sa.size()!=0) {
      sa.clear();
  }

  for (uint32_t a = 0; a<text.length(); ++a) {
      sa.push_back(a);
  }
  int size=text.size();
  quickSort(sa,text,0,size-1);
}



void findemlr(uint32_t& mlr, const uint32_t L, const uint32_t R, const std::string& query, const std::vector<uint32_t>& sa, const std::string& text) {
    uint32_t mlrl = mlr;
    uint32_t mlrr = mlr;


    /* für L */
    for(uint32_t l = mlr; l<query.size(); ++l) {
        if (query[mlr] == text[sa[L]+mlrl]) {
            mlrl+=1;
        }
        else{
            break;
        }
    }


    for(uint32_t r = mlr; r<query.size(); ++r) {
        //cout << query[mlr] << text[sa[R]+mlrr] << endl;
        if (query[mlr] == text[sa[R]+mlrr]) {
            mlrr+=1;
        }
        else{
            break;
        }
    }

    mlr = min(mlrl,mlrr);
    return;
}

/// Search a string in a text via suffix array. If the query is empty, return empty hits.
/// Input: search for a 'query' string in a suffix array 'sa' build from 'text'.
/// Output: Results are returned in 'hits' (sorted ascending by position!). The variable is cleared before its filled and returned.
void find(const std::string& query, const std::vector<uint32_t>& sa, const std::string& text, std::vector<uint32_t>& hits){


    if(hits.size()!=0) {
    hits.clear();
    }

    if (cin.fail()) {
        cin.clear();
        cout << "geht nicht mit dem input" << endl;
    }

    if (text.size()==0) {
        cout << "leeres String";
        return;
    }

    else if (query.size()==0) {
        cout << "leeres Query";
        return;
    }


    /* Searching Lp */
    uint32_t Lp;
    if (query <= text.substr(sa[0])){
            Lp = 0;
    }

    else if (query > text.substr(sa[text.length()-1])){
            Lp = text.length();
    }

    else {
        uint32_t L = 0;
        uint32_t R = text.length()-1;
        uint32_t mlr = 0;
        while (R-L > 1) {


                uint32_t M = ceil((L+R)/2);

                findemlr(mlr,L,R,query,sa,text);

                if (query.substr(mlr) <= (text.substr(sa[M]+mlr))) {
                    R = M;
                }
                else {
                    L = M;
                }
        }
        Lp = R;
    }

    for (uint32_t i = Lp; i < text.length(); ++i) {
        if ((text.substr(sa[i])).substr(0,query.length()) == query) {
            if(find(hits.begin(), hits.end(), sa[i]) != hits.end()){
                //tue nix
            }
            else{
            hits.push_back(sa[i]);
            }
        }
        else {
             break;
        }
    }

    sort(hits.begin(), hits.end());
    return;
}
