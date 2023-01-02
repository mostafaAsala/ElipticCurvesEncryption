#pragma once

#ifndef BIGINT_H_
#define BIGINT_H_

#include <iostream>

using namespace std;

class BigInteger {
private:
	string integer;
public:
	/* Constructs a big integer representation of the integer given as an argument */
	BigInteger(unsigned int integer);

	/* Parses the string given as an argument looking for an integer.
	 * Stops as soon as it finds a non-digit character.
	 * Trailing zeros will eventually get removed.
	 */
	BigInteger(string integer);

	/* Constructs a big integer representation of the integer given as an argument
	 * and assigns it to the internal representation of the big integer.
	 */
	void setInteger(unsigned int integer);

	/* Parses the string given as an argument looking for an integer and assigns it to
	 * the internal representation of the big integer.
	 * Stops as soon as it finds a non-digit character.
	 * Trailing zeros will eventually get removed.
	 */
	void setInteger(string integer);

	/* Returns 0 if the value of the internal big integer won't fit in 32 bits.
	 * Otherwise it returns the integer values.
	 */
	unsigned int getIntValue() const;

	/* Returns the internal big integer as a string */
	string toString() const;

	/* Adds the big integer given as an argument to the internal big integer
	 * and returns the result as a string.
	 */
	BigInteger addInteger(const BigInteger& integer_to_add) const;

	/* Adds the integer represented by the string given as an argument to the internal
	 * big integer and returns the result as a string.
	 */
	BigInteger addInteger(const string& integer_to_add) const;

	/* Multiplies the big integer given as an argument by the internal big integer
	 * and returns the result as a string.
	 */
	BigInteger multiplyInteger(const BigInteger& integer_to_multiply) const;

	/* Multiplies the integer represented by the string given as an argument by the internal
	 * big integer and returns the result as a string.
	 */
	BigInteger multiplyInteger(const string& integer_to_multiply) const;

	/* Returns the index of the first non-zero digit in the string given as an argument.
	 * This function is used to trim trailing zeros from a string representation of an integer.
	 * A trimmed version of the string is a substring that starts at the index returned by this
	 * function.
	 */

	BigInteger DivideInteger(const BigInteger& integer_to_divide_By) const;

	static size_t getTrimIndex(const string& integer);

	bool operator==(const BigInteger& integer) const;

	BigInteger operator+(const BigInteger& integer) const;

	BigInteger operator*(const BigInteger& integer) const;

	friend ostream& operator<<(ostream& in, BigInteger& integer);
};

#endif /* BIGINT_H_ */

