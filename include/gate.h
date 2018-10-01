// John Stanco 9.29.18

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <complex>

#ifndef QUANTUM_EMU_GATE_H
#define QUANTUM_EMU_GATE_H


typedef unsigned int qbit_t;
typedef std::complex<double> cx_double;
typedef std::vector<qbit_t> QbitList;
class State;


class Gate {
public:
  virtual ~Gate() {}
  virtual State& apply( State &state, const QbitList &qbits ) = 0;
};

struct GateSpecifier {
  // specifies at what time ( discrete )
  uint32_t time;
  // specifies which qbit to act on
  QbitList qbits;
  GateSpecifier( uint32_t t, QbitList qs ) : time{ t }, qbits{ qs } {}
};

//might not need
class ConflictingEventException : public std::logic_error {
public:
  explicit ConflictingEventException() :
  std::logic_error{ "Cannot operate with two gates on same qbit at same time." } {}
};

struct GateComparator {
  // less than
  bool operator()( const GateSpecifier &lhs, const GateSpecifier &rhs ) const {
    if( lhs.time > rhs.time ) {
      return false;
    } else if( lhs.time < rhs.time ) {
      return true;
    }

    // same time
    if( lhs.qbits.size() > rhs.qbits.size() ) {
      return false;
    } else if( lhs.qbits.size() < rhs.qbits.size() ) {
      return true;
    }

    // same length qbit list
    for( size_t i = 0; i < rhs.qbits.size(); ++i ) {
      if( lhs.qbits[i] > rhs.qbits[i] ) {
        return false;
      } else if( lhs.qbits[i] < rhs.qbits[i] ) {
        return true;
      }
    }
    //equal
    return false;
  }
};

class UGate : public Gate {
  cx_double u00, u01, u10, u11;
public:
  State& apply( State &state, const QbitList &qbits );
  UGate( double phi1, double phi2, double phi3 );
};

class CUGate : public Gate {
  cx_double u00, u01, u10, u11;
public:
  State& apply( State &state, const QbitList &qbits );
  CUGate( double phi1, double phi2, double phi3 );
};

#endif /* QUANTUM_EMU_GATE_H */
