#include "QGramIndex.hpp"
#include <iostream>
#include "a5_util.hpp"
#include <math.h>


void QGramIndex::Countingsort (const std::string& text, std::vector<uint32_t>& dir, std::vector<uint32_t>& Sa,uint8_t qrsize,uint32_t txsize){
  int Ssize =txsize-qrsize+1;
  dir.resize(pow(4,qrsize));
  Sa.resize(Ssize);
  //1. Zähler auf Null setzen
  for (int j=0; j<(pow(4,qrsize));j++){
    dir[j]=0;
  };
  //2.q-gramme zählen
  for (int i=0;i<Ssize;i++){
    uint32_t j=hashW[i];
    dir[j]++;
  };
  //3.Kumulative Summe bilden
  for (int j=1;j<(pow(4,qrsize));j++){
    dir[j]=dir[j]+dir[j-1];
  };
  //4.Vorkomme einsortieren
  for (int i=0; i<Ssize;i++){
    uint32_t j=hashW[i];
    dir[j]=dir[j]-1;
    Sa[dir[j]]=i;
  };
}
/**
 @brief Constructor: build up a q-gram index from @p text.

 Internally, @p text is stored as const reference (const std::string& ),
 i.e. make sure the text does not go out of scope as long as the class object is used.

 The index is build immediately.

 @param text The sequence (genome, ...) to be indexed
 @param q Length of q-gram (from 1 to 13).
 @throws std::invalid_argument("Invalid q!"); if NOT: 1 <= q <= 13
*/
QGramIndex::QGramIndex(const std::string& text, const uint8_t q) {
	if ((q < 1) || (q > 13)) { throw std::invalid_argument("Invalid q!"); };
	// Text wird als reference gespeichert
	oriText = text;
	Pointer = &text;

	txsize = oriText.size();
	qrsize = q;

	int Ssize = txsize - qrsize + 1;
	hashW.resize(Ssize);
	//speichert alle Hashwerte von q-gramm aus Text
	hashW[0] = hash(text.substr(0, qrsize));
	for (int i = 1; i < Ssize; i++) {
		hashW[i] = hashNext(hashW[i - 1], oriText[i + qrsize - 1]);
	};
	Countingsort(oriText, dir, Sa, qrsize, txsize);
	/*hashW[0]=hash(text.substr(0,qrsize));
	std::cout << hashW[0] << '\n';
	for (int i=1;i<Ssize;i++){
	  std::cout<<oriText[i+qrsize-1]<<std::endl;
	  hashW[i]=hashNext(hashW[i-1],oriText[i+qrsize-1]);
	  std::cout<<hashW[i]<<std::endl;
	}
	*/


}

/**
  @brief Returns the text.

  @return the full text
*/
const std::string& QGramIndex::getText() const {

	return (*Pointer);
}
/**
  @brief Returns a vector of indices into text, where the q-gram with hash @p h occurs.

  The vector might be empty, if the q-gram does not exist, i.e. empty range within the suffix array.
  The hash value must be within [0...|SIGMA|^q), otherwise an exception is thrown.

  @note The indices are in reverse order of occurrence in the text, or might be even "unordered"
  when OpenMP multithreading is used.

  @param h Hash value of the q-gram
  @return Vector of hits (indices into text)
  @throws std::invalid_argument("Invalid hash!"); if h is outside of valid hash values
*/
std::vector<uint32_t> QGramIndex::getHits(const uint32_t h) const {
	if (h > (pow(4, qrsize) - 1)) { throw std::invalid_argument("Invalid h"); };
	uint32_t Ssize = txsize - qrsize + 1;
	std::vector<uint32_t> result;
	if (h > (pow(4, qrsize))) { throw std::invalid_argument("Invalid hash!"); };
	uint32_t i = 0;
	//suchen ob query in Text vorkommt
	while ((i < Ssize) && (hashW[i] != h)) {
		i++;
	};
	if (i == Ssize) { return result; }
	else {
		uint32_t h1 = h;
		//suchen auf Sa alle Vorkommen von query 
		while (dir[h1] == dir[h1 + 1]) { h1++; }
		for (uint32_t i = dir[h]; i < dir[h1 + 1]; i++) {
			result.push_back(Sa[i]);
		};
		return result;
	}
}
/**
  @brief Get the length of q-grams, i.e. 'q'.
*/
uint8_t QGramIndex::getQ() const{

  return (qrsize);
}

/**
  @brief Compute a full hash for a given q-gram.

  @param qgram A q-gram; must have size 'q'
  @return hash value of @p qgram
  @throws std::invalid_argument("Invalid q-gram. Wrong length!"); if qgram.size() != q

*/
uint32_t QGramIndex::hash(const std::string& qgram) const {
	int n = qgram.size();
	if (n != qrsize) { throw std::invalid_argument("Invalid q-gram. Wrong length!"); };
	uint32_t hashValue = 0;
	for (int i = 0; i < n; i++) {
		hashValue *= 4;
		hashValue += ordValue(qgram[i]);
	}
	return hashValue;
}
/**
   @brief Returns the next rolling hash for given new character and previous hash (of previous q-gram).

   The first character going out-of-scope is removed internally by bitmasking (i.e. no need to specify it).

   @param prev_hash Previous hash value
   @param new_pos new character (last position of current q-gram)
   @return The updated hash value
*/

uint32_t QGramIndex::hashNext(const uint32_t prev_hash, const char new_pos) const {
	int i = 3;
	//suchen die erste Buchstabe von vorherige Hash
	while (prev_hash < (i*pow(4, qrsize - 1))) { i--; }
	uint32_t hashValue = 0;
	//benutzen den Formel aus den Folien
	hashValue = prev_hash * 4 - i * (pow(4, qrsize)) + ordValue(new_pos);
	return hashValue;
}