// John Stanco 9.29.18

#include "gate.h"
#include "state.h"

#define I cx_double{ 0, 1 }

#include <iostream>


State& XGate::apply( State &state, const QbitList &qbits ) {

  if( qbits.size() != 1 ) {
    throw "incorrect number of qbits";
  }

  if( qbits[0] >= state.nqbits() ) {
    throw "invalid qbit entered";
  }

  qbit_t n = state.nqbits();
  qbit_t m = qbits[0];

  size_t stride = 1 << m;
  size_t nStates = 1 << n;

  for( size_t i = 0; i < nStates - stride; i += stride ) {
    size_t max = i + stride;
    for( ; i < max; ++i ) {
      // simply swap the bits
      auto tmp = state[i];
      state[i] = state[i + stride];
      state[i + stride] = tmp;
    }
  }

  return state;
}


State& YGate::apply( State &state, const QbitList &qbits ) {
  if( qbits.size() != 1 ) {
    throw "incorrect number of qbits";
  }

  if( qbits[0] >= state.nqbits() ) {
    throw "invalid qbit entered";
  }

  qbit_t n = state.nqbits();
  qbit_t m = qbits[0];

  size_t stride = 1 << m;
  size_t nStates = 1 << n;

  for( size_t i = 0; i < nStates - stride; i += stride ) {
    size_t max = i + stride;
    for( ; i < max; ++i ) {
      auto tmp = state[i];
      state[i] = I * state[i + stride];
      state[i + stride] = -I * tmp;
    }
  }

  return state;
}


State& ZGate::apply( State &state, const QbitList &qbits ) {
  if( qbits.size() != 1 ) {
    throw "incorrect number of qbits";
  }

  if( qbits[0] >= state.nqbits() ) {
    throw "invalid qbit entered";
  }

  qbit_t n = state.nqbits();
  qbit_t m = qbits[0];

  size_t stride = 1 << m;
  size_t nStates = 1 << n;

  for( size_t i = 0; i < nStates - stride; i += stride ) {
    size_t max = i + stride;
    for( ; i < max; ++i ) {
      state[i] = -state[i];
    }
  }

  return state;
}


State& IDGate::apply( State &state, const QbitList &qbits ) {
  if( qbits.size() != 1 ) {
    throw "incorrect number of qbits";
  }

  if( qbits[0] >= state.nqbits() ) {
    throw "invalid qbit entered";
  }

  //returns the same state
  return state;
}


State& CXGate::apply( State &state, const QbitList &qbits ) {
  if( qbits.size() != 1 ) {
    throw "incorrect number of qbits";
  }

  if( qbits[0] >= state.nqbits() ) {
    throw "invalid qbit entered";
  }

  qbit_t n = state.nqbits();
  qbit_t m = qbits[0];

  size_t stride = 1 << m;
  size_t nStates = 1 << n;

  for( size_t i = 0; i < nStates - stride; i += stride ) {
    size_t max = i + stride;
    for( ; i < max; ++i ) {
      // simply swap the bits
      auto tmp = state[i];
      state[i] = state[i + stride];
      state[i + stride] = tmp;
    }
  }

  return state;
}

State& HGate::apply( State &state, const QbitList &qbits ) {
  if( qbits.size() != 1 ) {
    throw "incorrect number of qbits";
  }

  if( qbits[0] >= state.nqbits() ) {
    throw "invalid qbit entered";
  }

  qbit_t n = state.nqbits();
  qbit_t m = qbits[0];

  size_t stride = 1 << m;
  size_t nStates = ( 1 << n ) - stride;

  double A = .5 * sqrt( 2 );

  for( size_t i = 0; i < nStates; i += stride ) {
    size_t max = i + stride;
    for( ; i < max; ++i ) {
      auto tmp = ( state[i] + state[i + stride] ) * A;
      state[i] = ( state[i] - state[i + stride] ) * A;
      state[i + stride] = tmp;
    }
  }

  return state;
}


PhaseGate::PhaseGate( double phi ) : phase{ phi } {}


State& PhaseGate::apply( State &state, const QbitList &qbits ) {
  if( qbits.size() != 1 ) {
    throw "incorrect number of qbits";
  }

  if( qbits[0] >= state.nqbits() ) {
    throw "invalid qbit entered";
  }

  qbit_t n = state.nqbits();
  qbit_t m = qbits[0];

  size_t stride = 1 << m;
  size_t nStates = ( 1 << n ) - stride;

  auto omega = exp( phase * I );

  for( size_t i = 0; i < nStates; i += stride ) {
    size_t max = i + stride;
    for( ; i < max; ++i ) {
      state[i] *= omega;
    }
  }

  return state;
}


CPhaseGate::CPhaseGate( double phi ) : phase{ phi } {}


State& CPhaseGate::apply( State &state, const QbitList &qbits ) {
  if( qbits.size() != 2 ) {
    throw "incorrect number of qbits";
  }

  if( qbits[0] >= state.nqbits() || qbits[1] >= state.nqbits() ) {
    throw "invalid qbit entered";
  }

  qbit_t n = state.nqbits();
  qbit_t m = qbits[0];
  qbit_t ctrl = qbits[1];

  size_t stride = 1 << m;
  size_t nStates = ( 1 << n ) - stride;
  size_t ctrlMask = ( 1 << ctrl );

  auto omega = exp( I * phase );

  for( size_t i = 0; i < nStates; i += stride ) {
    size_t max = i + stride;
    for( ; i < max; ++i ) {
      if( ( i & ctrlMask ) >> ctrl ) {
        state[i] *= omega;
      }
    }
  }

  return state;
}
