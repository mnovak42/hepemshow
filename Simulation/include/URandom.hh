#ifndef URandom_HH
#define URandom_HH

/**
 * @file    URandom.hh
 * @class   URandom
 * @author  M. Novak
 * @date    July 2023
 *
 * @brief A uniform random number generator.
 *
 * This is the uniform random number generator, i.e. the only thing that is
 * need to make the `G4HepEm` physics implementation complete (see more at
 * the `Physics` documentation). This random number generator relies on the
 * c++11 implementation of the 64-bit Mersenne Twister engine. The `URandom::flat()`
 * method can be used to provide uniform random numbers on the \f$(0,1)\f$.
 * An object from this class is constructed in the `HepEmShow` main and set to
 * be used in the `G4HepEmRandomEngine`.
 *
 * @note This random number generator can be replaced with anything that can provide
 * uniform fandom numbers on \f$(0,1)\f$. One need to modify the corresponding
 * implementations in `Physics` (namely, one line in the `G4HepEmRandomEngine::flat()`
 * and `G4HepEmRandomEngine::flatArray()` implementations in `Physics.cc`) and
 * replace the `URandom` object construction in the `HepEmShow` main.
 */

#include <random>

class URandom {
public:

   /** CTR
    *
    * @param seed seed of the random number generator.
    */
   URandom(int seed=123);
   /** DTR*/
  ~URandom();

   /** Method to provide uniform random numbers on \f$(0,1)\f$ */
   double flat();

public:
   /** c++11 implementation of the 64-bit Mersenne Twister engine */
   std::mt19937_64 fEngine;
   /** uniform distribution: utilises the above random engine to provide random numbers on \f$(0,1)\f$ */
   std::uniform_real_distribution<double>* fDist;

};

#endif // URandom_HH
