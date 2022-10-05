#ifndef __SMOOTHIE_H__
#define __SMOOTHIE_H__

/**
 * Simple weighed moving average
 * from https://makeabilitylab.github.io/physcomp/advancedio/smoothing-input.html
 */
template<typename T> 
class Smoothie {

    float sigma, s;

  public:
  Smoothie(float _sigma, float _s = 0);
  T filter(T val);
};

template<typename T>
Smoothie<T>::Smoothie(float _sigma, float _s) {
  sigma = _sigma;
  s = _sigma;
}

template<typename T>
T Smoothie<T>::filter(T val) {
  s = (sigma * val) + (1 - sigma) * s;
  return s;
}

/// inlined functions

/// returns the average value of a few consecutive readings
inline int analogReadAvg(int pin, int iter = 4) {
  int retval = 0;

  int i = iter;
  while(i > 0) {
    retval += analogRead( pin );
  }

  return retval / iter;
} // analogReadAvg

#endif // __SMOOTHIE_H__