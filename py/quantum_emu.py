# John Stanco 10.5.18

import math
import cmath
import numpy as np
from secrets import randbelow

class State:

    #construct instance
    def __init__( self, n ):
        if( n < 1 ):
            raise Exception( "State should have positive number of qubits" )
        self.nqbits = n
        self.nstates = 1 << n
        self.qbits = np.zeros( ( self.nstates, 1 ), dtype=complex )
        self.qbits[0] = 1
        

    def __getitem__( self, index ):
        return self.qbits[index]

    def assign( self, index, val ):
        self.qbits[index] = val

    def print( self ):
        for i in range( self.nstates ):
            print( self.qbits[i] )

    def normalize( self ):
        norm = 0
        for coeff in self.qbits:
            norm += abs( coeff ) ** 2
        self.qbits /= norm


class UGate:

    #construct instance
    def __init__( self, phi1, phi2, phi3 ):
        w1 = cmath.exp( 1j * phi1 * .5 )
        w2 = cmath.exp( 1j * phi2 )
        w3 = cmath.exp( 1j * phi3 )
        self.u00 = w1.real
        self.u01 = -w3 * w1.imag
        self.u10 = w2 * w1.imag
        self.u11 = w2 * w3 * w1.real

    def apply( self, state, qbit ):
        if qbit < 0 or qbit >= state.nqbits:
            raise Exception( "Could not apply gate, invalid qubit entered" )

        stride = 1 << qbit
        nState = ( 1 << state.nqbits ) - stride

        i = 0
        while( i < nState ):
            max = i + stride
            while( i < max ):
                j = i + stride
                a0 = self.u00*state[i] + self.u01*state[j]
                a1 = self.u10*state[i] + self.u11*state[j]
                state.assign( i, a0 )
                state.assign( j, a1 )
                i += 1
            i += stride
            
    
class CUGate:

    #construct instance
    def __init__( self, phi1, phi2, phi3 ):
        w1 = cmath.exp( 1j * phi1 * .5 )
        w2 = cmath.exp( 1j * phi2 )
        w3 = cmath.exp( 1j * phi3 )
        self.u00 = w1.real
        self.u01 = -w3 * w1.imag
        self.u10 = w2 * w1.imag
        self.u11 = w2 * w3 * w1.real

    def apply( self, state, qbit1, qbit2 ):
        if qbit1 < 0 or qbit1 >= state.nqbits:
            raise Exception( "Could not apply gate, invalid qubit entered" )
        if qbit2 < 0 or qbit2 >= state.nqbits:
            raise Exception( "Could not apply gate, invalid qubit entered" )

        stride = 1 << qbit1
        mask = 1 << qbit2
        nState = ( 1 << state.nqbits ) - stride

        i = 0
        while( i < nState ):
            max = i + stride
            while( i < max ):
                if ( i & mask ) >> qbit2:
                    j = i + stride
                    a0 = self.u00*state[i] + self.u01*state[j]
                    a1 = self.u10*state[i] + self.u11*state[j]
                    state.assign( i, a0 )
                    state.assign( j, a1 )
                i += 1
            i += stride


##########  1-qubit gates  ##########


def XGate():
    return UGate( math.pi, 0, math.pi )

def YGate():
    return UGate( math.pi, math.pi / 2, math.pi / 2 )

def ZGate():
    return UGate( 0, 0, math.pi )

def SGate():
    return UGate( 0, 0, math.pi / 2 )

def SdGate():
    return UGate( 0, 0, -math.pi / 2 )

def TGate():
    return UGate( 0, 0, math.pi / 4 )

def TdGate():
    return UGate( 0, 0, -math.pi / 4 )

def IDGate():
    return UGate( 0, 0, 0 )

def HGate():
    return UGate( math.pi / 2, 0, math.pi )

def U1Gate( phi ):
    return UGate( 0, 0, phi )

def U2Gate( phi1, phi2 ):
    return UGate( math.pi / 2, phi1, phi2 )

def U3Gate( phi1, phi2, phi3 ):
    return UGate( phi1, phi2, phi3 )


##########  2-qubit control gates  ##########


def CXGate():
    return CUGate( math.pi, 0, math.pi )

def CYGate():
    return CUGate( math.pi, math.pi / 2, math.pi / 2 )

def CZGate():
    return CUGate( 0, 0, math.pi )

def CSGate():
    return CUGate( 0, 0, math.pi / 2 )

def CSdGate():
    return CUGate( 0, 0, -math.pi / 2 )

def CTGate():
    return CUGate( 0, 0, math.pi / 4 )

def CTdGate():
    return CUGate( 0, 0, -math.pi / 4 )

def CIDGate():
    return CUGate( 0, 0, 0 )

def CHGate():
    return CUGate( math.pi / 2, 0, math.pi )

def CU1Gate( phi ):
    return CUGate( 0, 0, phi )

def CU2Gate( phi1, phi2 ):
    return CUGate( math.pi / 2, phi1, phi2 )

def CU3Gate( phi1, phi2, phi3 ):
    return CUGate( phi1, phi2, phi3 )


#########  Main Method  #########


def fourierTransform( state ):
    for i in range( 0, state.nqbits ):
        HGate().apply( state, i )
        m = 1
        for j in range( i + 1, state.nqbits ):
            phase = math.pi / ( 1 << m )
            CU1Gate( phase ).apply( state, i, j )
            m += 1
    return state


#########   Shor Algorithm  ##########


def findPeriod( a, N ):
    a_mod_N = a % N
    ar_mod_N = a_mod_N
    for r in range( 1, N ):
        ar_mod_N = ( ar_mod_N * a ) % N
        if ar_mod_N == a_mod_N:
            return r
    return N


def shorFactor( N ):
    x = 0
    while True:
        a = randbelow( N )
        g = math.gcd( a, N )
        if g != 1:
            return g
        # quantum period-finding algorithm
        r = findPeriod( a, N )
        x = pow( a, r // 2, N )
        if ( not(r & 1) ) and ( x % N != N - 1 ):
            break
    return math.gcd( x + 1, N )

def main():
    print( shorFactor( 15 ) )

if __name__== "__main__":
    main()
