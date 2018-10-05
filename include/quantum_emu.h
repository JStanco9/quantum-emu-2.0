// John Stanco 10.4.18

#include <stdexcept>
#include <complex>
#include <vector>
#include <iostream>
#include <vector>
#include <cstdint>
#include <map>
#include <set>
#include <memory>


#ifndef QUANTUM_EMU_H
#define QUANTUM_EMU_H


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
#ifndef QUANTUM_EMU_CIRCUIT_H
#define QUANTUM_EMU_CIRCUIT_h

typedef std::complex<double> cx_double;


//struct GetSpecifier;
//struct GateComparator;
//typedef std::map<const GateSpecifier, std::shared_ptr<Gate>, GateComparator> GateScheduler;
typedef std::map<const GateSpecifier, Gate*, GateComparator> GateScheduler;

class Circuit {
  State inputRegister;
  State outputRegister;
  GateScheduler schedule;
  std::map<uint32_t, std::set<qbit_t> > occupied;
public:
  Circuit( qbit_t nq );
  ~Circuit();
  // no copying of circuits for now
  Circuit( const Circuit &other ) = delete;
  Circuit& operator=( Circuit other ) = delete;

  //bool addGate( std::shared_ptr<Gate> pGate, const QbitList &qbits, uint32_t time );
  bool addGate( Gate* pGate, const QbitList &qbits, uint32_t time );
  bool removeGate( const QbitList &qbits, uint32_t time );
  //bool replaceGate( std::shared_ptr<Gate> pGate, const QbitList &qbits, uint32_t time );
  bool replaceGate( Gate* pGate, const QbitList &qbits, uint32_t time );

  const State& getInput() const;
  const State& getOutput() const;

  void execute();
  void clear();
private:
  // Applies gate to the qubits listed
  void apply( Gate *pGate, const QbitList &qbits );
};

// Single qbit gates
bool addX( Circuit &circuit, qbit_t qbit, uint32_t time );
bool addY( Circuit &circuit, qbit_t qbit, uint32_t time );
bool addZ( Circuit &circuit, qbit_t qbit, uint32_t time );
bool addS( Circuit &circuit, qbit_t qbit, uint32_t time );
bool addSd( Circuit &circuit, qbit_t qbit, uint32_t time );
bool addT( Circuit &circuit, qbit_t qbit, uint32_t time );
bool addTd( Circuit &circuit, qbit_t qbit, uint32_t time );
bool addID( Circuit &circuit, qbit_t qbit, uint32_t time );
bool addH( Circuit &circuit, qbit_t qbit, uint32_t time );
bool addU1( Circuit &circuit, qbit_t qbit, double phase, uint32_t time );
bool addU2( Circuit &circuit, qbit_t qbit, double phi1, double phi2, uint32_t time );
bool addU3( Circuit &circuit, qbit_t qbit, double phi1, double phi2, double phi3, uint32_t time );

// Double qbit gates
bool addCX( Circuit &circuit, qbit_t qbit, qbit_t control, uint32_t time );
bool addCU1( Circuit &circuit, qbit_t qbit, qbit_t control, double phase, uint32_t time );
bool addCU2( Circuit &circuit, qbit_t qbit, qbit_t control, double phi1, double phi2, uint32_t time );
bool addCU3( Circuit &circuit, qbit_t qbit, qbit_t control, double phi1, double phi2, double phi3, uint32_t time );

#endif /* QUANTUM_EMU_CIRCUIT_h */
#endif /* QUANTUM_EMU_H */
