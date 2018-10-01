// John Stanco 9.29.18

#include "gate.h"
#include "state.h"
#include "circuit.h"

#include <iostream>
#include <map>

const double PI = std::acos( -1 );

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
  auto x = new UGate{ PI, 0, PI };
  if( circuit.addGate( x, { qbit }, time ) ) {
    return true;
  }
  delete x;
  return false;
}


bool addY( Circuit &circuit, qbit_t qbit, uint32_t time ) {
  //auto y = std::shared_ptr<YGate>( new YGate );
  auto y = new UGate{ PI, PI / 2, PI / 2 };
  if( circuit.addGate( y, { qbit }, time ) ) {
    return true;
  }
  delete y;
  return false;
}


bool addZ( Circuit &circuit, qbit_t qbit, uint32_t time ) {
  //auto z = std::shared_ptr<ZGate>( new ZGate );
  auto z = new UGate{ 0, 0, PI };
  if( circuit.addGate( z, { qbit }, time ) ) {
    return true;
  }
  delete z;
  return false;
}


bool addS( Circuit &circuit, qbit_t qbit, uint32_t time ) {
  //auto z = std::shared_ptr<ZGate>( new ZGate );
  auto s = new UGate{ 0, 0, PI / 2 };
  if( circuit.addGate( s, { qbit }, time ) ) {
    return true;
  }
  delete s;
  return false;
}


bool addSd( Circuit &circuit, qbit_t qbit, uint32_t time ) {
  //auto z = std::shared_ptr<ZGate>( new ZGate );
  auto sd = new UGate{ 0, 0, -PI / 2 };
  if( circuit.addGate( sd, { qbit }, time ) ) {
    return true;
  }
  delete sd;
  return false;
}


bool addT( Circuit &circuit, qbit_t qbit, uint32_t time ) {
  //auto z = std::shared_ptr<ZGate>( new ZGate );
  auto t = new UGate{ 0, 0, PI / 4 };
  if( circuit.addGate( t, { qbit }, time ) ) {
    return true;
  }
  delete t;
  return false;
}


bool addTd( Circuit &circuit, qbit_t qbit, uint32_t time ) {
  //auto z = std::shared_ptr<ZGate>( new ZGate );
  auto td = new UGate{ 0, 0, -PI / 4 };
  if( circuit.addGate( td, { qbit }, time ) ) {
    return true;
  }
  delete td;
  return false;
}


bool addID( Circuit &circuit, qbit_t qbit, uint32_t time ) {
  //auto z = std::shared_ptr<ZGate>( new ZGate );
  auto id = new UGate{ 0, 0, 0 };
  if( circuit.addGate( id, { qbit }, time ) ) {
    return true;
  }
  delete id;
  return false;
}


bool addH( Circuit &circuit, qbit_t qbit, uint32_t time ) {
  //auto z = std::shared_ptr<ZGate>( new ZGate );
  auto h = new UGate{ PI / 2, 0, PI };
  if( circuit.addGate( h, { qbit }, time ) ) {
    return true;
  }
  delete h;
  return false;
}


bool addU1( Circuit &circuit, qbit_t qbit, double phase, uint32_t time ) {
  auto u1 = new UGate{ 0, 0, phase };
  if( circuit.addGate( u1, { qbit }, time ) ) {
    return true;
  }
  delete u1;
  return false;
}


bool addU2( Circuit &circuit, qbit_t qbit, double phi1, double phi2, uint32_t time ) {
  auto u2 = new UGate{ PI / 2, phi1, phi2 };
  if( circuit.addGate( u2, { qbit }, time ) ) {
    return true;
  }
  delete u2;
  return false;
}


bool addU3( Circuit &circuit, qbit_t qbit, double phi1, double phi2, double phi3, uint32_t time ) {
  auto u3 = new UGate{ phi1, phi2, phi3 };
  if( circuit.addGate( u3, { qbit }, time ) ) {
    return true;
  }
  delete u3;
  return false;
}


bool addCX( Circuit &circuit, qbit_t qbit, qbit_t control, uint32_t time ) {
  auto cX = new CUGate{ PI, 0, PI };
  if( circuit.addGate( cX, { qbit, control }, time ) ) {
    return true;
  }
  delete cX;
  return false;
}


bool addCU1( Circuit &circuit, qbit_t qbit, qbit_t control, double phase, uint32_t time ) {
  auto cU1 = new CUGate{ 0, 0, phase };
  if( circuit.addGate( cU1, { qbit, control }, time ) ) {
    return true;
  }
  delete cU1;
  return false;
}


bool addCU2( Circuit &circuit, qbit_t qbit, qbit_t control, double phi1, double phi2, uint32_t time ) {
  auto cU2 = new CUGate{ PI / 2, phi1, phi2 };
  if( circuit.addGate( cU2, { qbit, control }, time ) ) {
    return true;
  }
  delete cU2;
  return false;
}


bool addCU3( Circuit &circuit, qbit_t qbit, qbit_t control, double phi1, double phi2, double phi3, uint32_t time ) {
  auto cU3 = new CUGate{ phi1, phi2, phi3 };
  if( circuit.addGate( cU3, { qbit, control }, time ) ) {
    return true;
  }
  delete cU3;
  return false;
}
