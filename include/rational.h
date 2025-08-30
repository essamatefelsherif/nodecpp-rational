/**
 * @file    rational.h
 * @author  Essam A. El-Sherif <esm.elsh@gmail.com>
 * @version v1.0.0
 *
 * A C++ header that defines a template based class for representing and manipulating rational numbers.
 */

#ifndef __RATIONAL_H__
#define __RATIONAL_H__

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cassert>
#include <limits>

/**
 * A namespace to enclose the C++ rational class and global helper classes and functions.
 */
namespace src{
	/**
	 * An exception class for bad rationals.
	 */
	class bad_rational : public std::domain_error{
		public:
			/** Default constructor. */
			explicit bad_rational() : std::domain_error("bad rational: zero denominator"){}

			/** One argument constructor. */
			explicit bad_rational(const char *what) : std::domain_error(what){}
	};

	/**
	 * A template based class for representing and manipulating rational numbers.
	 */
	template<typename I>
	class rational{
		private: /* Helper static functions [3] */

			/** Greatest common divisor. */
			static I inner_gcd(I, I, const I& = I(0));

			/** Absolute value. */
			static I inner_abs(I, const I& = I(0));

			/** Inspect the two given parameters are in normalized form. */
			static bool is_normalized(I, I, const I& = I(0), const I& = I(1));

		private: /* Helper member functions [2] */

			/** Normalize the rational number i.e. no common factors and denominator is positive. */
			void normalize();

			/** Light test of normalized rational number. */
			bool test_invariant()const;

		private:
			I num;  /**< Numerator (normalized).*/
			I den;  /**< Denominator (normalized).*/

		public:
			/* Constructors [4] */
			rational();      /**< Default constructor. */
			rational(I);     /**< One argument constructor. */
			rational(I, I);  /**< Two arguments constructor. */

			template<typename J>
			explicit rational(const rational<J>&);  /**< Copy constructor. */

			/* Access to representation [2] */
			const I& numerator() const { return num; }    /**< Get numerator. */
			const I& denominator() const { return den; }  /**< Get denominator. */

			/* Assignment [2] */
			rational& operator =(const I&);        /**< Assignment from Int type. */
			rational& assign(const I&, const I&);  /**< Assignment in place. */

			/* Arithmetic assignment operators [8] */
			rational& operator +=(const rational<I>&);  /**< Arithmetic assignment operator += */
			rational& operator -=(const rational<I>&);  /**< Arithmetic assignment operator -= */
			rational& operator *=(const rational<I>&);  /**< Arithmetic assignment operator *= */
			rational& operator /=(const rational<I>&);  /**< Arithmetic assignment operator /= */

			rational& operator +=(const I&);  /**< Arithmetic assignment operator += from int type. */
			rational& operator -=(const I&);  /**< Arithmetic assignment operator -= from int type. */
			rational& operator *=(const I&);  /**< Arithmetic assignment operator *= from int type. */
			rational& operator /=(const I&);  /**< Arithmetic assignment operator /= from int type. */

			/* Increment and decrement operators [4] */
			rational& operator ++();  /**< Pre-increment operator. */
			rational& operator --();  /**< Pre-decrement operator. */

			rational operator ++(int);  /**< Post-increment operator. */
			rational operator --(int);  /**< Post-decrement operator. */

			/* Operator not [1] */
			bool operator !() const;  /**< Not operator. */

			/* Boolean conversion [1] */
			operator bool () const;  /**< Boolean conversion. */

			/* Comparison operators [4] */
			bool operator  <(const rational&) const;  /**< Comparison operator < */
			bool operator  >(const rational&) const;  /**< Comparison operator < */
			bool operator ==(const rational&) const;  /**< Comparison operator == */
			bool operator !=(const rational&) const;  /**< Comparison operator != */

			/* Comparison with integers [4] */
			bool operator  <(const I&) const;  /**< Comparison operator < int type. */
			bool operator  >(const I&) const;  /**< Comparison operator > int type. */
			bool operator ==(const I&) const;  /**< Comparison operator == int type. */
			bool operator !=(const I&) const;  /**< Comparison operator != int type. */
	};

	/* Global unary operators [2] */
	template<typename I>
	rational<I> operator +(const rational<I>&); /**< Unary operator + */

	template<typename I>
	rational<I> operator -(const rational<I>&); /**< Unary operator - */

	/* Global binary operators [12] */
	template<typename I>
	rational<I> operator +(const rational<I>&, const rational<I>&);

	template<typename I>
	rational<I> operator +(const rational<I>&, const I&);

	template<typename I>
	rational<I> operator +(const I&, const rational<I>&);

	template<typename I>
	rational<I> operator -(const rational<I>&, const rational<I>&);

	template<typename I>
	rational<I> operator -(const rational<I>&, const I&);

	template<typename I>
	rational<I> operator -(const I&, const rational<I>&);

	template<typename I>
	rational<I> operator *(const rational<I>&, const rational<I>&);

	template<typename I>
	rational<I> operator *(const rational<I>&, const I&);

	template<typename I>
	rational<I> operator *(const I&, const rational<I>&);

	template<typename I>
	rational<I> operator /(const rational<I>&, const rational<I>&);

	template<typename I>
	rational<I> operator /(const rational<I>&, const I&);

	template<typename I>
	rational<I> operator /(const I&, const rational<I>&);

	/* Global absolute value function [1] */
	template<typename I>
	rational<I> abs(const rational<I>&);

	/* Global input and output operators [2] */
	template<typename I>
	std::istream& operator >>(std::istream&, rational<I>&);

	template<typename I>
	std::ostream& operator <<(std::ostream&, const rational<I>&);

	/* Global type conversion function [1] */
	template<typename T, typename I>
	T rational_cast(const rational<I>& r);
}

/* Constructors [4] */
template<typename I>
src::rational<I>::rational() : num(0), den(1){}

template<typename I>
src::rational<I>::rational(I n) : num(n), den(1){}

template<typename I>
src::rational<I>::rational(I n, I d) : num(n), den(d){
	normalize();
}

template<typename I>
template<typename J>
src::rational<I>::rational(const src::rational<J>& r){
	if(is_normalized(
			I(r.numerator()),
			I(r.denominator())
		)
	){
		num = r.numerator();
		den = r.denominator();
	}
	else{
		throw bad_rational("bad rational: denormalized conversion");
	}
}

/* Helper static functions [3] */
template<typename I>
I src::rational<I>::inner_gcd(I a, I b, const I& zero){
	return b == zero ? a : inner_gcd(b, a % b, zero);
}

template<typename I>
I src::rational<I>::inner_abs(I x, const I& zero){
	return x < zero ? -x : +x;
}

template<typename I>
bool src::rational<I>::is_normalized(I n, I d, const I& zero, const I& one){
	return
		d > zero &&
		(n != zero || d == one) &&
		inner_abs( inner_gcd(n, d, zero), zero ) == one;
}

/* Helper member functions [2] */
template<typename I>
void src::rational<I>::normalize(){
	I zero(0);

	if(den == zero) throw bad_rational();

	if(num == zero){
		den = I(1);
		return;
	}

	I g = inner_abs( inner_gcd(num, den) );

	num /= g;
	den /= g;

	if(den < -(std::numeric_limits<I>::max)()){
		throw bad_rational("bad rational: non-zero singular denominator");
	}

	if(den < zero){
		num = -num;
		den = -den;
	}

	assert(test_invariant());
}

template<typename I>
bool src::rational<I>::test_invariant()const{
	return
		this->den > I(0) &&
		inner_abs( inner_gcd(this->num, this->den) ) == I(1);
}

/* Assignment [2] */
template<typename I>
src::rational<I>& src::rational<I>::operator =(const I& n){
	return assign( static_cast<I>(n), static_cast<I>(1) );
}

template<typename I>
src::rational<I>& src::rational<I>::assign(const I& n, const I& d){
	return *this = rational<I>( static_cast<I>(n), static_cast<I>(d) );
}

/* Arithmetic assignment operators [8] */
template<typename I>
src::rational<I>& src::rational<I>::operator +=(const rational& r){
	I r_num = r.numerator();
	I r_den = r.denominator();

	I g = inner_gcd(den, r_den);

	den /= g;
	num = num * (r_den / g) + r_num * den;

	g = inner_abs( inner_gcd(num, g) );
	num /= g;
	den *= r_den/g;

	return *this;
}

template<typename I>
src::rational<I>& src::rational<I>::operator -=(const rational& r){
	I r_num = r.numerator();
	I r_den = r.denominator();

	I g = inner_gcd(den, r_den);

	den /= g;
	num = num * (r_den / g) - r_num * den;

	g = inner_abs( inner_gcd(num, g) );
	num /= g;
	den *= r_den/g;

	return *this;
}

template<typename I>
src::rational<I>& src::rational<I>::operator *=(const rational& r){
	I r_num = r.num;
	I r_den = r.den;

	I gcd1 = inner_abs( inner_gcd(num, r_den) );
	I gcd2 = inner_abs( inner_gcd(r_num, den) );

	num = (num/gcd1) * (r_num/gcd2);
	den = (den/gcd2) * (r_den/gcd1);

	return *this;
}

template<typename I>
src::rational<I>& src::rational<I>::operator /=(const rational& r){
	I r_num = r.num;
	I r_den = r.den;

	I zero(0);

	if(r_num == zero){
		throw bad_rational();
	}

	if(num == zero){
		return *this;
	}

	I gcd1 = inner_abs( inner_gcd(num, r_num) );
	I gcd2 = inner_abs( inner_gcd(r_den, den) );

	num = (num/gcd1) * (r_den/gcd2);
	den = (den/gcd2) * (r_num/gcd1);

	if(den < zero){
		num = -num;
		den = -den;
	}

	return *this;
}

template<typename I>
src::rational<I>& src::rational<I>::operator +=(const I& i){
	num += i * den;
	return *this;
}

template<typename I>
src::rational<I>& src::rational<I>::operator -=(const I& i){
	num -= i * den;
	return *this;
}

template<typename I>
src::rational<I>& src::rational<I>::operator *=(const I& i){
	I gcd = inner_abs( inner_gcd( static_cast<I>(i), den ) );
	num *= i / gcd;
	den /= gcd;
	return *this;
}

template<typename I>
src::rational<I>& src::rational<I>::operator /=(const I& i){
	const I zero(0);

	if(i == zero) throw bad_rational();
	if(num == zero) return *this;

	const I gcd = inner_abs( inner_gcd(num, static_cast<I>(i)) );
	num /= gcd;
	den *= i / gcd;

	if(den < zero){
		num = -num;
		den = -den;
	}

	return *this;
}

/* Increment and decrement operators [4] */
template<typename I>
src::rational<I>& src::rational<I>::operator ++(){
	num += den;
	return *this;
}

template<typename I>
src::rational<I>& src::rational<I>::operator --(){
	num -= den;
	return *this;
}

template<typename I>
src::rational<I> src::rational<I>::operator ++(int){
	rational t(*this);
	++(*this);
	return t;
}

template<typename I>
src::rational<I> src::rational<I>::operator --(int){
	rational t(*this);
	--(*this);
	return t;
}

/* Operator not [1] */
template<typename I>
bool src::rational<I>::operator !() const{
	return !num;
}

/* Boolean conversion [1] */
template<typename I>
src::rational<I>::operator bool() const{
	return static_cast<bool>(num);
}

/* Comparison operators [4] */
template<typename I>
bool src::rational<I>::operator <(const rational& r) const{
	const I zero(0);

	assert( this->den > zero );
	assert( r.den > zero );

	struct{
		I  n, d, q, r;
	}
	ts = {
		this->num,
		this->den,
		static_cast<I>( this->num / this->den ),
		static_cast<I>( this->num % this->den )
	},
	rs = {
		r.num,
		r.den,
		static_cast<I>(r.num / r.den),
		static_cast<I>(r.num % r.den)
	};

	unsigned int reverse = 0u;

	while (ts.r < zero){ ts.r += ts.d; --ts.q; }
	while (rs.r < zero){ rs.r += rs.d; --rs.q; }

	for( ;; ){
		if(ts.q != rs.q){
			return reverse ? ts.q > rs.q : ts.q < rs.q;
		}

		reverse ^= 1u;

		if(ts.r == zero || rs.r == zero){
			break;
		}

		ts.n = ts.d;         ts.d = ts.r;
		ts.q = ts.n / ts.d;  ts.r = ts.n % ts.d;
		rs.n = rs.d;         rs.d = rs.r;
		rs.q = rs.n / rs.d;  rs.r = rs.n % rs.d;
	}

	if(ts.r == rs.r){
		return false;
	}
	else{
		return ( ts.r != zero ) != static_cast<bool>( reverse );
	}
}

template<typename I>
bool src::rational<I>::operator >(const rational& r) const{
	return !(*this < r || *this == r);
}

template<typename I>
bool src::rational<I>::operator ==(const rational& r) const{
	return (num == r.numerator() && den == r.denominator());
}

template<typename I>
bool src::rational<I>::operator !=(const rational& r) const{
	return (num != r.numerator() || den != r.denominator());
}

/* Comparison with integers [4] */
template<typename I>
bool src::rational<I>::operator <(const I& i) const{
	const I zero(0);

	assert(this->den > zero);
	I q = this->num / this->den;
	I r = this->num % this->den;
	while(r < zero){ r += this->den; --q; }

	return q < i;
}

template<typename I>
bool src::rational<I>::operator >(const I& i) const{
	return operator ==(i) ? false : !(operator <(i));
}

template<typename I>
bool src::rational<I>::operator ==(const I& i) const{
	return ((den == I(1)) && (num == i));
}

template<typename I>
bool src::rational<I>::operator !=(const I& i) const{
	return ((den != I(1)) || (num != i));
}

/* Global unary operators [2] */
template<typename I>
inline src::rational<I> src::operator +(const rational<I>& r){
	return r;
}

template<typename I>
inline src::rational<I> src::operator -(const rational<I>& r){
	return rational<I>( static_cast<I>(-r.numerator()), r.denominator() );
}

/* Global binary operators [12] */
template<typename I>
inline src::rational<I> src::operator+ (const rational<I>& a, const rational<I>& b){
	rational<I> t(a);
	t += b;
	return t;
}

template<typename I>
inline src::rational<I> src::operator+ (const rational<I>& r, const I& i){
	rational<I> t(r);
	t += i;
	return t;
}

template<typename I>
inline src::rational<I> src::operator+ (const I& i, const rational<I>& r){
	rational<I> t(r);
	t += i;
	return t;
}

template<typename I>
inline src::rational<I> src::operator- (const rational<I>& a, const rational<I>& b){
	rational<I> t(a);
	t -= b;
	return t;
}

template<typename I>
inline src::rational<I> src::operator- (const rational<I>& r, const I& i){
	rational<I> t(r);
	t -= i;
	return t;
}

template<typename I>
inline src::rational<I> src::operator- (const I& i, const rational<I>& r){
	rational<I> t(r);
	t -= i;
	return -t;
}

template<typename I>
inline src::rational<I> src::operator* (const rational<I>& a, const rational<I>& b){
	rational<I> t(a);
	t *= b;
	return t;
}

template<typename I>
inline src::rational<I> src::operator* (const rational<I>& r, const I& i){
	rational<I> t(r);
	t *= i;
	return t;
}

template<typename I>
inline src::rational<I> src::operator* (const I& i, const rational<I>& r){
	rational<I> t(r);
	t *= i;
	return t;
}

template<typename I>
inline src::rational<I> src::operator/ (const rational<I>& a, const rational<I>& b){
	rational<I> t(a);
	t /= b;
	return t;
}

template<typename I>
inline src::rational<I> src::operator/ (const rational<I>& r, const I& i){
	rational<I> t(r);
	t /= i;
	return t;
}

template<typename I>
inline src::rational<I> src::operator/ (const I& i, const rational<I>& r){
	rational<I> t(i);
	t /= r;
	return t;
}

/* Global absolute value function [1] */
template<typename I>
inline src::rational<I> src::abs(const rational<I>& r){
	return r.numerator() >= I(0) ? r : -r;
}

/* Global input and output operators [2] */
template<typename I>
std::istream& src::operator >>(std::istream& is, rational<I>& r){
    using std::ios;

    I n = I(0), d = I(1);
    char c = 0;

	if( is >> n ){
		if( is.get(c) ){
			if( c == '/' ){
				if( is >> std::noskipws >> d )
					try{
						r.assign( n, d );
					}
					catch( bad_rational & ){
						try{
							is.setstate(ios::failbit);
						}
						catch( ... ) {}

						if( is.exceptions() & ios::failbit )
							throw;
					}
			}
			else{
				is.setstate( ios::failbit );
			}
		}
	}

	return is;
}

template<typename I>
std::ostream& src::operator <<(std::ostream& os, const rational<I>& r){
	std::ostringstream ss;

	ss.copyfmt( os );
	ss.tie( NULL );
	ss.exceptions( std::ios::goodbit );
	ss.width( 0 );
	ss << std::noshowpos << std::noshowbase << '/' << r.denominator();

	const std::string tail = ss.str();
	std::streamsize const w =
		os.width() - static_cast<std::streamsize>( tail.size() );

	ss.clear();
	ss.str( "" );
	ss.flags( os.flags() );
	ss << std::setw( w < 0 || (os.flags() & std::ios::adjustfield) !=
					 std::ios::internal ? 0 : w ) << r.numerator();
	return os << ss.str() + tail;
}

/* Global type conversion function [1] */
template<typename T, typename I>
inline T src::rational_cast(const rational<I>& r){

	return static_cast<T>(r.numerator()) / static_cast<T>(r.denominator());
}

#endif
