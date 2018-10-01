// John Stanco 9.29.18

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <iostream>

#ifndef QUANTUM_EMU_GATE_H
#define QUANTUM_EMU_GATE_H


typedef unsigned int qbit_t;
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

class XGate : public Gate {
public:
  State& apply( State &state, const QbitList &qbits );
};

class YGate : public Gate {
public:
  State& apply( State &state, const QbitList &qbits );
};

class ZGate : public Gate {
public:
  State& apply( State &state, const QbitList &qbits );
};

class IDGate : public Gate {
public:
  State& apply( State &state, const QbitList &qbits );
};

class CXGate : public Gate {
public:
  State& apply( State &state, const QbitList &qbits );
};

// Hadamard Gate
class HGate : public Gate {
public:
  State& apply( State &state, const QbitList &qbits );
};

class PhaseGate : public Gate {
public:
  double phase;
  PhaseGate( double phi );
  State& apply( State &state, const QbitList &qbits );
};

class CPhaseGate : public Gate {
public:
  double phase;
  CPhaseGate( double phi );
  State& apply( State &state, const QbitList &qbits );
};

#endif /* QUANTUM_EMU_GATE_H */
