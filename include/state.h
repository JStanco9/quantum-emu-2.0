// John Stanco 9.29.18


#include <stdexcept>
#include <complex>
#include <vector>
#include <iostream>


#ifndef QUANTUM_EMU_STATE_H
#define QUANTUM_EMU_STATE_H


// Quantum state
typedef unsigned int qbit_t;
typedef std::complex<double> cx_double;

class State {
  std::vector<cx_double> qbits;
  qbit_t _nqbits;
public:
  State( qbit_t n );

  typedef std::vector<cx_double>::iterator iterator;
  typedef std::vector<cx_double>::const_iterator const_iterator;

  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

  const cx_double &operator[]( size_t index ) const;
  cx_double &operator[]( size_t index );

  qbit_t nqbits() const;
  size_t size() const;
  void print() const;
  void normalize();
private:
  void init();
};


#endif /* QUANTUM_EMU_STATE_H */
