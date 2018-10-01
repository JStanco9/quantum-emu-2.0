// John Stanco 9.29.18

#include "gate.h"

#include <cstdint>
#include <map>
#include <set>
#include <memory>

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

// Single qbit gates for now
bool addX( Circuit &circuit, qbit_t qbit, uint32_t time );
bool addCX( Circuit &circuit, qbit_t qbit, qbit_t control, uint32_t time );
bool addY( Circuit &circuit, qbit_t qbit, uint32_t time );
bool addZ( Circuit &circuit, qbit_t qbit, uint32_t time );
bool addID( Circuit &circuit, qbit_t qbit, uint32_t time );
bool addH( Circuit &circuit, qbit_t qbit, uint32_t time );
bool addPhase( Circuit &circuit, qbit_t qbit, double phase, uint32_t time );
bool addCPhase( Circuit &circuit, qbit_t qbit, qbit_t control, double phase, uint32_t time );

#endif /* QUANTUM_EMU_CIRCUIT_h */
