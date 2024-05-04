#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "aufgabe2.hpp"
#include <stdint.h>


using namespace std;

int main(int argc, char* argv[]){

if (argc < 2){
    cout << "Problem mit Argumenten)nicht genuge Argumente" << endl;
    return 1;}

if (argc == 2){

    vector<uint32_t> sa;
    string input_string(argv[1]);
    construct(sa,input_string);

    for(uint32_t i = 0; i<input_string.length(); ++i) {
        cout << sa[i] << endl;
    }

    return 0;
    }

if (argc > 2){
    vector<uint32_t> hits;
    string text(argv[1]);
    vector<uint32_t> sa;

    if (sa.size() != 0) {
        sa.clear();
    }
    if (hits.size() != 0) {
        hits.clear();
    }

    int z;
    z = 2;
        while(z < argc){
           if (sa.size()!=0) {
            sa.clear();
            }
            for (int unsigned a = 0; a<text.length(); ++a)
                {
                sa.push_back(a);
                }
                sort(sa.begin(),sa.end(),[&text] (uint32_t s1, uint32_t s2) {
                return(text.substr(s1)<=text.substr(s2));
                });

            string query(argv[z]);
            string input_string(argv[1]);

            if (query.size() == 0) {
                //ßß tue nix
            }
            else if (input_string.size() == 0) {
                 //ßß tue nix
            }
            else {
            cout << (argv[z]) << ":";
            }

            find(query,sa,input_string,hits);

                for (uint32_t i = 0; i < hits.size(); ++i) {
                cout << " " << hits[i];
                }

            cout << endl;
            ++z;
        }
    }
    return 0;
}