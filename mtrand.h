#ifndef MTRAND_H
#define MTRAND_H
#include <ctime>

static const int n = 624;
static const int m = 397;

class MTRand_int32 { // Mersenne Twister random number generator
public:
	/*****
	* C O N S T R U C T O R
	*****/
	MTRand_int32() { 
		if (!init) 
			seed((int)time(NULL));
		init = true; 
	}
	
	void seed(unsigned long); // seed with 32 bit integer
	unsigned long operator()() { // overload operator() to make this a generator (functor)
		return rand_int32();
	}
	virtual ~MTRand_int32() {} // destructor
protected:
	unsigned long rand_int32(); // generate 32 bit random integer
private: 
	static unsigned long state[n];
	static int p; // position in state array
	static bool init; // true if init function is called
	// private functions used to generate the pseudo random numbers
	unsigned long twiddle(unsigned long, unsigned long); // used by gen_state()
	void gen_state(); // generate new state
};

// inline for speed, must therefore reside in header file
inline unsigned long MTRand_int32::twiddle(unsigned long u, unsigned long v) {
	return (((u & 0x80000000UL) | (v & 0x7FFFFFFFUL)) >> 1)
		^ ((v & 1UL) * 0x9908B0DFUL);
}

inline unsigned long MTRand_int32::rand_int32() { // generate 32 bit random int
	if (p == n) 
		gen_state(); // new state vector needed
	unsigned long x = state[p++];
	x ^= (x >> 11);
	x ^= (x << 7) & 0x9D2C5680UL;
	x ^= (x << 15) & 0xEFC60000UL;
	return x ^ (x >> 18);
}

#endif
