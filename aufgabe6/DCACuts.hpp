#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <cstdio>

struct CostMatrix
{
  /// Cost matrix representation with indexing m[row, column]
  std::vector<std::vector<int> > m;

  /// prints the Cost matrix m (unless empty)
  inline void print()
  {
    int rows_ = m.size();
    if (rows_ == 0) return;
    int cols_ = m[0].size();
    for (int i = 0; i < rows_; ++i)
    {
      for (int j = 0; j < cols_; ++j)
      {
        printf(" %3i", m[i][j]);
      }
      std::cout << std::endl;
    }
  }
};

class DCACuts
{
public:
    /// DO NOT CHANGE THE PUBLIC INTERFACE

    /// Constructor, which copies the sequences to an internal member
    /// @throws Exception std::logic_error if one or no sequences are given (i.e. at least two sequences are required)
    DCACuts(const std::vector<std::string>& sequences);

    /// Compute the cost matrices of all pairs s and t
    /// Stores result internally.
    /// Here the algorithm iterates over all pairs of sequences i and j,
    /// whereby i < j.
    /// For every pair the forward matrix and the backward matrix is computed.
    /// Then the cost matrix is computed from these two matrices and finally stored internally.
    void computeCostMatrices();

    /// returns the Cost-Matrix for sequences i [0..N-1) and j [0..N-1).
    /// @throws Exception std::logic_error if computeCostMatrices() was not called before (i.e. internal matrices are missing)
    /// @throws Exception std::logic_error if i == j
    /// @throws Exception std::out_of_range if i >= N or j >= N.
    CostMatrix getCostMatrix(unsigned i, unsigned j);

    /// Compute the c optimal cuts, by fixing the first sequence at the given position.
    /// This function is invoked by the user (in main()) to compute the optimal cuts,
    /// after the cost matrices have been computed.
    /// It takes a position of the first sequence which is fixed.
    /// Using this position, cut positions of the remaining sequences are enumerated and the optimal position(s) are found.
    /// @throws Exception std::logic_error if computeCostMatrices() was not called before (i.e. internal matrices are missing).
    void computeCuts(int p1);

    /// Return the optimal cost computed during computeCuts() method.
    /// @throws Exception std::logic_error if computeCuts() was not called before (i.e. internal results are missing).
    int getOptimalCosts();

    /// Return a vector containing all optimal cuts (inner vector is of size N, where N is the number of sequences).
    /// The outer vector is of size 1 or longer (since there could be multiple optimal cuts with the same score).
    const std::vector<std::vector<int> >& getOptimalCuts();

protected:
    // your members and functions here...
    std::vector <std::string> seqs;
    std::vector<int> pos;
    std::vector<int> size_seqs;
    std::vector <std::vector <CostMatrix>> costm;
    unsigned sqsize=0;
    int possible_size=1;
    int minimum;
    std::vector<std::vector<int>> optimalcut;
    std::vector<std::vector<int>> Ergebnis;
    bool Matrix=false;
    bool Cut =false;
    int ct=0;
};
