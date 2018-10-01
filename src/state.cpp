// John Stanco 9.29.18

#include "state.h"

State::State( qbit_t n ) : qbits{ std::vector<cx_double>( 1 << n ) }, _nqbits{ n } {
  qbits[0] = 1;
}


State::iterator State::begin() { return qbits.begin(); }


State::const_iterator State::begin() const { return qbits.begin(); }


State::iterator State::end() { return qbits.end(); }


State::const_iterator State::end() const { return qbits.end(); }


const cx_double &State::operator[]( size_t index ) const { return qbits[index]; }


cx_double &State::operator[]( size_t index ) { return qbits[index]; }


qbit_t State::nqbits() const { return _nqbits; }


size_t State::size() const { return qbits.size(); }


void State::print() const {
  for( auto& z : qbits ) {
    std::cout << z << "\n";
  }
}


void State::normalize() {
  double norm = 0;
  for( auto& z : qbits ) {
    norm += std::norm( z );
  }
  if( norm != 1 ) {
    for( auto& coeff : qbits ) {
      coeff /= norm;
    }
  }
}
