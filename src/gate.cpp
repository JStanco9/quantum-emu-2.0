// John Stanco 9.29.18

#include "gate.h"
#include "state.h"

#include <iostream>

const cx_double I = cx_double{ 0, 1 };
const double PI = std::acos( -1 );


UGate::UGate( double phi1, double phi2, double phi3 ) {
  auto w1 = exp( I * phi1 * .5 );
  auto w2 = exp( I * phi2 );
  auto w3 = exp( I * phi3 );
  u00 = w1.real();
  u01 = -w3 * w1.imag();
  u10 = w2 * w1.imag();
  u11 = w2 * w3 * w1.real();
}


State& UGate::apply( State &state, const QbitList &qbits ) {
  if( qbits.size() != 1 ) {
    throw "incorrect number of qbits";
  }

  if( qbits[0] >= state.nqbits() ) {
    throw "invalid qbit entered";
  }

  qbit_t n = state.nqbits();
  qbit_t m = qbits[0];

  size_t stride = 1 << m;
  size_t nState = ( 1 << n ) - stride;

  for( size_t i = 0; i < nState; i += stride ) {
    size_t max = i + stride;
    for( ; i < max; ++i ) {
      auto j = i + stride;
      auto a0 = state[i];
      auto a1 = state[j];
      state[i] = u00*a0 + u01*a1;
      state[j] = u10*a0 + u11*a1;
    }
  }
  return state;
}


CUGate::CUGate( double phi1, double phi2, double phi3 ) {
  auto w1 = exp( I * phi1 * .5 );
  auto w2 = exp( I * phi2 );
  auto w3 = exp( I * phi3 );
  u00 = w1.real();
  u01 = -w3 * w1.imag();
  u10 = w2 * w1.imag();
  u11 = w1 * w2 * w1.real();
}


State& CUGate::apply( State &state, const QbitList &qbits ) {
  if( qbits.size() != 2 ) {
    throw "incorrect number of qbits";
  }

  if( qbits[0] >= state.nqbits() || qbits[2] >= state.nqbits() ) {
    throw "invalid qbit entered";
  }

  qbit_t n = state.nqbits();
  qbit_t m = qbits[0];
  qbit_t ctrl = qbits[1];

  size_t stride = 1 << m;
  size_t nState = ( 1 << n ) - stride;
  size_t ctrlMask = ( 1 << ctrl );

  for( size_t i = 0; i < nState; i += stride ) {
    size_t max = i + stride;
    for( ; i < max; ++i ) {
      if( ( i & ctrlMask ) >> ctrl ) {
        auto j = i + stride;
        auto a0 = state[i];
        auto a1 = state[j];
        state[i] = u00*a0 + u01*a1;
        state[j] = u10*a0 + u11*a1;
      }
    }
  }
  return state;
}
