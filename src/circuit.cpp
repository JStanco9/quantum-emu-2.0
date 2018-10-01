// John Stanco 9.29.18

#include "gate.h"
#include "state.h"
#include "circuit.h"

#include <iostream>
#include <map>

Circuit::Circuit( qbit_t nq ) :
inputRegister{ State{ nq } },
outputRegister{ State{ nq } } {}


Circuit::~Circuit() {
  clear();
}


//bool Circuit::addGate( std::shared_ptr<Gate> pGate, const QbitList &qbits, uint32_t time ) {
bool Circuit::addGate( Gate* pGate, const QbitList &qbits, uint32_t time ) {
  //check for overlapping events ( from gates of different sizes )
  auto itOcc = occupied.find( time );
  if( itOcc != occupied.end() ) {
    for( auto &qbit : qbits ) {
      if( !itOcc->second.insert( qbit ).second ) {
        return false;
      }
    }
  } else {
    occupied.emplace( std::make_pair( time, std::set<qbit_t>{ qbits.begin(), qbits.end() } ) );
  }
  return schedule.emplace( std::make_pair( GateSpecifier{ time, qbits }, pGate ) ).second;
}


bool Circuit::removeGate( const QbitList &qbits, uint32_t time ) {
  auto itSch = schedule.find( GateSpecifier{ time, qbits } );
  if( itSch == schedule.end() ) {
    // gate not found
    return false;
  }

  // gate found, delete gate from memory
  delete itSch->second;
  schedule.erase( itSch );
  return true;
}


//bool Circuit::replaceGate( std::shared_ptr<Gate> pGate, const QbitList &qbits, uint32_t time ) {
bool Circuit::replaceGate( Gate* pGate, const QbitList &qbits, uint32_t time ) {
  auto itSch = schedule.find( GateSpecifier{ time, qbits } );
  if( itSch == schedule.end() ) {
    // gate not found
    return false;
  }

  // gate found, replace
  delete itSch->second;
  itSch->second = pGate;
  return true;
}


const State& Circuit::getInput() const {
  return inputRegister;
}


const State& Circuit::getOutput() const {
  return outputRegister;
}


void Circuit::apply( Gate *pGate, const QbitList &qbits ) {
  pGate->apply( inputRegister, qbits );
}


void Circuit::execute() {
  for( auto& event : schedule ) {
   apply( event.second, event.first.qbits );
  }
}


void Circuit::clear() {
  for( auto& event : schedule ) {
    // deleting gates in memory
    delete event.second;
  }
  schedule.clear();
  occupied.clear();
}


bool addX( Circuit &circuit, qbit_t qbit, uint32_t time ) {
  //auto x = std::shared_ptr<XGate>( new XGate );
  auto x = new XGate;
  if( circuit.addGate( x, { qbit }, time ) ) {
    return true;
  }
  delete x;
  return false;
}


bool addY( Circuit &circuit, qbit_t qbit, uint32_t time ) {
  //auto y = std::shared_ptr<YGate>( new YGate );
  auto y = new YGate;
  if( circuit.addGate( y, { qbit }, time ) ) {
    return true;
  }
  delete y;
  return false;
}


bool addZ( Circuit &circuit, qbit_t qbit, uint32_t time ) {
  //auto z = std::shared_ptr<ZGate>( new ZGate );
  auto z = new ZGate;
  if( circuit.addGate( z, { qbit }, time ) ) {
    return true;
  }
  delete z;
  return false;
}


bool addID( Circuit &circuit, qbit_t qbit, uint32_t time ) {
  //auto z = std::shared_ptr<ZGate>( new ZGate );
  auto id = new IDGate;
  if( circuit.addGate( id, { qbit }, time ) ) {
    return true;
  }
  delete id;
  return false;
}


bool addH( Circuit &circuit, qbit_t qbit, uint32_t time ) {
  //auto z = std::shared_ptr<ZGate>( new ZGate );
  auto h = new HGate;
  if( circuit.addGate( h, { qbit }, time ) ) {
    return true;
  }
  delete h;
  return false;
}


bool addPhase( Circuit &circuit, qbit_t qbit, double phase, uint32_t time ) {
  auto p = new PhaseGate{ phase };
  if( circuit.addGate( p, { qbit }, time ) ) {
    return true;
  }
  delete p;
  return false;
}


bool addCPhase( Circuit &circuit, qbit_t qbit, qbit_t control, double phase, uint32_t time ) {
  auto cP = new CPhaseGate{ phase };
  if( circuit.addGate( cP, { qbit, control }, time ) ) {
    return true;
  }
  delete cP;
  return false;
}
