#pragma once
#include"ElipticCurve.h"
#include<random>
class Host
{
	std::string name;
	ElipticCurveFF curve;
	BigInt::Rossi PrivateKey;
	std::pair< BigInt::Rossi, BigInt::Rossi> sharedKey;

	std::pair< BigInt::Rossi, BigInt::Rossi> Gen_Point;

public:
	std::pair< BigInt::Rossi, BigInt::Rossi> public_key;
	Host(ElipticCurveFF curve,std::string name) {
		this->curve = curve;
		PrivateKey = BigInt::Rossi(rand() % 1000);
		Gen_Point = curve.G;
		public_key = curve.MultiplyPointPyInt(Gen_Point.first, Gen_Point.second, PrivateKey);
		this->name = name;
	}

	void generateSharedKey(std::pair< BigInt::Rossi, BigInt::Rossi> otherPublicKey) {
		sharedKey = curve.MultiplyPointPyInt(otherPublicKey.first, otherPublicKey.second, PrivateKey);
	}

	void printSharedKey() {
		std::cout <<"sharedKey of "<< name<<" : (" << sharedKey.first.toStrDec() << ", " << sharedKey.second.toStrDec()<<")\n";
	}
};

