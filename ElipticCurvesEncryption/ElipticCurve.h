#pragma once
#include"bigInt.h"
class ElipticCurveFF
{
public:

	ElipticCurveFF(){}
	ElipticCurveFF(BigInt::Rossi a, BigInt::Rossi b, BigInt::Rossi order, std::pair< BigInt::Rossi, BigInt::Rossi> G=std::make_pair(BigInt::Rossi(0), BigInt::Rossi(0))) {
		this->a = a;
		this->b = b;
		this->order = order;
		this->G = G;
	}
	BigInt::Rossi a;
	BigInt::Rossi b;
	BigInt::Rossi order;
	std::pair< BigInt::Rossi, BigInt::Rossi> G;
	BigInt::Rossi h;

	bool CheckPointOnCurve(BigInt::Rossi x, BigInt::Rossi y) {
		BigInt::Rossi res = ((x * x * x)+order - (y * y ) % order + (x * a) + b)%order;
		return (res == BigInt::Rossi(0));
	}
	BigInt::Rossi  FindyAt(BigInt::Rossi x) {
		BigInt::Rossi res = (x * x * x) % order + (x * a) % order + b;
		return res.sqrt() % order;
	}
	std::pair< BigInt::Rossi, BigInt::Rossi>  FindPointAt(BigInt::Rossi x) {
		BigInt::Rossi res =( (x * x * x) % order + (x * a)  + b) % order;
		res = sqrt(res, order);
		if(CheckPointOnCurve(x,res))
			return std::make_pair(x, res);
		else
			return std::make_pair(order,order);
	}

	BigInt::Rossi  sqrt(BigInt::Rossi number) {
		return sqrt(number, order);
	}
	BigInt::Rossi static sqrt(BigInt::Rossi number, BigInt::Rossi order) {
		for (BigInt::Rossi i(1); i < order; i++) {
			if (i * i % order == number)
				return i;
		}
		return BigInt::Rossi(0);
	}


	BigInt::Rossi FindNegative(BigInt::Rossi y) {
		return order - y % order;
	}

	bool Iszero(std::pair< BigInt::Rossi, BigInt::Rossi> p) {
		return (p.first == BigInt::Rossi(0) && p.second == BigInt::Rossi(0));
	}

	std::pair< BigInt::Rossi, BigInt::Rossi> AddPoints(BigInt::Rossi x1, BigInt::Rossi y1, BigInt::Rossi x2, BigInt::Rossi y2) {
		
		if (x1 == BigInt::Rossi(0) && y1 == BigInt::Rossi(0))
			return std::make_pair(x2, y2);
		if (x2 == BigInt::Rossi(0) && y2 == BigInt::Rossi(0))
			return std::make_pair(x1, y1);

		if (x1 != x2)
		{
			BigInt::Rossi inv = modInverse(x2 + order - x1,order);

			BigInt::Rossi L = ((y2 + order - y1) * inv) % order;

			BigInt::Rossi x = (L * L+ order +order- x1 - x2)%order;
			BigInt::Rossi y = (L * (x1+order - x)+order - y1)%order;
			return std::make_pair(x, y);
		}
		else if(x1==x2 && y1==y2) {

			BigInt::Rossi L = (BigInt::Rossi(3) * x1 * x1);// / (BigInt::Rossi(2) * y1);
			auto inv = modInverse(BigInt::Rossi(2) * y1, order);
			//auto inv2 = Inverse(BigInt::Rossi(2) * y1, order);
			auto invv = inv.toStrDec();
			//auto invv2 = inv2.toStrDec();

			L = (L *inv );
			if (inv > order)return std::make_pair(order,order);
			
			BigInt::Rossi x = ((L * L) +order - (BigInt::Rossi(2) * x1)%order)%order;
			BigInt::Rossi y = ((L * (x1 + order - x))+ order - y1)% order;
			return std::make_pair(x, y);
		}
		return std::make_pair(order, order);
	}
	std::pair< BigInt::Rossi, BigInt::Rossi> Double(BigInt::Rossi x, BigInt::Rossi y) {
		return AddPoints(x, y, x, y);
	}

	

	std::pair< BigInt::Rossi, BigInt::Rossi> MultiplyPointPyInt(BigInt::Rossi x, BigInt::Rossi y, BigInt::Rossi multiplier) {
		std::pair< BigInt::Rossi, BigInt::Rossi> mul= std::make_pair(BigInt::Rossi (0), BigInt::Rossi ( 0));
		std::pair< BigInt::Rossi, BigInt::Rossi> d = std::make_pair(x,y);
		int bit = 0;
		for (BigInt::Rossi i = BigInt::Rossi(1); i <= multiplier; i<<=1) {
			if ((i & multiplier).toUnit()) {
				//mul.first = (mul.first+ d.first) %order;
				//mul.second =(mul.second+ d.second)%order;
				mul = AddPoints(mul.first, mul.second, d.first, d.second);
			}
			
			d = Double(d.first, d.second);
			
		}
		return mul;
	}
	
	

	
	BigInt::Rossi static Inverse(BigInt::Rossi number, BigInt::Rossi order) {
		
		return BigInt::Rossi::pow(number, order - BigInt::Rossi(2), true)%order;
		number = number % order;
		for (BigInt::Rossi i = BigInt::Rossi(1); i < order; i++) {
			
			if ((i * number) % order == BigInt::Rossi(1))
				return i;
		}
		auto infinity = BigInt::Rossi( order);
		
		return infinity;
	}
	BigInt::Rossi  Inverse(BigInt::Rossi number) {
		return Inverse(number, order);
	}

	// Function to find modulo inverse of a
	BigInt::Rossi  modInverse(BigInt::Rossi  A, BigInt::Rossi  M)
	{
		BigInt::Rossi  x, y;
		BigInt::Rossi  g = gcdExtended(A, M, &x, &y);
		// m is added to handle negative x
		if (g != BigInt::Rossi(1))
			return BigInt::Rossi(0);
		BigInt::Rossi  res = x  % M;
			
		return res;
	}

	// Function for extended Euclidean Algorithm
	BigInt::Rossi gcdExtended(BigInt::Rossi  a, BigInt::Rossi  b, BigInt::Rossi* x, BigInt::Rossi* y)
	{

		// Base Case
		if (a == BigInt::Rossi (0)) {
			*x = BigInt::Rossi (0), * y = BigInt::Rossi (1);
			return b;
		}

		// To store results of recursive call
		BigInt::Rossi  x1, y1;
		BigInt::Rossi  gcd = gcdExtended(b % a, a, &x1, &y1);

		// Update x and y using results of recursive
		// call
		*x = y1	 +order - ((b / a) * x1)%order;
		*y = x1;

		return gcd;
	}



	BigInt::Rossi inverse(BigInt::Rossi number) {
		BigInt::Rossi t(0);  BigInt::Rossi  newt(1);
		BigInt::Rossi r(order); BigInt::Rossi newr(number);

		while (newr != BigInt::Rossi(0))
		{
			BigInt::Rossi  quotient(r / newr);
			t = (newt);
			newt =( t - (quotient * newt));
			r = (newr);
			newr =( r  - (quotient * newr));
		}
		if (r > BigInt::Rossi(1))
			return BigInt::Rossi();

		return t;
		
	}
};

