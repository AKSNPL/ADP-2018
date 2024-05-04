#ifndef _Alignment_HPP
#define _Alignment_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


// additional includes can go here:
// ...
struct trace{
  int row;
  int col;
};
class Alignment
{
public:
  // DO NOT CHANGE THE PUBLIC INTERFACE!

  /// make the default Constructor private
  /// i.e. an object of the class can only be created with sequences
  Alignment() = delete;  // no NOT implement this function. Just leave it as is.

  /// Constructor with two sequences
  /// Makes an internal copy of the sequences.
  Alignment(const std::string& seq1, const std::string& seq2);

  /// compute the aligment (i.e. score and traceback)
  /// given the three alignment parameters match, mismatch and gap
  /// if local_align == true, compute the local Smith-Waterman (SW) alignment (extra points), or throw
  /// an exception if your implementation does not support SW.
  void compute(const int match, const int mismatch, const int gap, const bool local_align = false);

  /// return the score of the alignment;
  /// Throws an exception if compute(...) was not called first
  int score();

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
  void getAlignment(std::string& a1, std::string& gaps, std::string& a2);

private:
  // add your private functions and member variables here
  // ...
  std::string a,b;
  int m,n;
  bool check = false ;
  int row_max=0;
  int col_max=0;
  bool waterman =false;
  std::vector<std::vector<int>> F;
  std::vector<std::vector<trace>> T;
  std::vector<char> gaps;
};
#endif
