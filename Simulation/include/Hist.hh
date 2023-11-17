
#ifndef HIST_H
#define HIST_H


/**
 * @file    Hist.hh
 * @class   Hist
 * @author  M. Novak
 * @date    July 2023
 *
 * @brief A simple histogram only to collect some data during the simulation.
 */


#include <vector>
#include <string>

class Hist {

public:

  /** Constructor
    *
    * @param filename String to be used as file name when writing into file.
    * @param min      Minimum bin value.
    * @param max      Maximum bin value.
    * @param numbin   Number of bins required between `min` and `max`.
    */
  Hist(const std::string& filename, double min, double max, int numbin);

  /** Constructor
    *
    * @param filename String to be used as file name when writing into file.
    * @param min      Minimum bin value.
    * @param max      Maximum bin value.
    * @param delta    Required width of a bin.
    */
  Hist(const std::string& filename, double min, double max, double delta);

  /** Default constructor */
  Hist();

   /** Destructor */
  ~Hist() {/*nothing to do*/}

  /** Auxiliary method to setup the initial state of the histogram.*/
  void Initialize();

  /** Method to modify the properties of the histogram
    *
    * @param filename The new name of the filename.
    * @param min      The new minimum bin value.
    * @param max      The new maximum bin value.
    * @param numbins  The new number of bins required between `min` and `max`.
    */
  void ReSet(const std::string& filename, double min, double max, int numbins);

  /** Method to populate the histogram with data: the corresponding bin content is increased by 1.
    *
    * @param x Value to add.
    */
  void Fill(double x);

  /** Method to populate the histogram with data and a weight: the corresponding bin content is increased by the weight.
    *
    * @param x Value to add.
    * @param w The corresponding weight.
    */
  void Fill(double x, double w);

  /** Method to scale all bin content by a constant.
    *
    * @param sc Scaling factor.
    */
  void Scale(double sc);

  /** Method to provide the number of bins.
    * @return Number of bins.
    */
  int     GetNumBins() const { return fNumBins; }
  double  GetDelta()   const { return fDelta;   }
  double  GetMin()     const { return fMin;     }
  double  GetMax()     const { return fMax;     }
  double  GetSum()     const { return fSum;     }
  const std::vector<double>& GetX() const { return fx; }
  const std::vector<double>& GetY() const { return fy; }

  // write result to file without (default) or after normalising
  void WriteToFile(bool isNorm=false);
  void WriteToFile(double norm);

  // Add the argument histogram to this (would be even nicer with operator overloading
  // but we keep it simple)
  void Add(const Hist* hist);


// Data members
private:
  std::string         fFileName;
  std::vector<double> fx;
  std::vector<double> fy;
  double              fMin;
  double              fMax;
  double              fDelta;
  double              fInvDelta;
  double              fSum;
  int                 fNumBins;
};

#endif
