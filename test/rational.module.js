/**
 * @module  rational-module
 * @desc    A module that defines a Rational class for representing and manipulating rational numbers.
 * @version 1.0.0
 * @author  Essam A. El-Sherif
 */

/**
 * @class  Rational
 * @static
 * @desc   A class for representing and manipulating rational numbers.
 */
class Rational{

	/**
	 * @method
	 * @static
	 * @memberof module:rational-module.Rational
	 * @param    {number} a - An integer.
	 * @param    {number} b - An integer.
	 * @returns  {number} The greatest common divisor of two integers.
	 * @desc     Calculates the greatest common divisor of two integers.
	 */
	static gcd(a, b){
		return b === 0 ? a : Rational.gcd(b, a % b);
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @param    {object|number} arg1 - [Optional] Rational object or numerator.
	 * @param    {string} arg2 - [Optional] denominator.
	 * @desc     Constructs a new Rational object.
	 * @throws   {TypeError} If more than two arguments are given, or invalid argument type is used.
	 */
	constructor(arg1, arg2){
		switch(arguments.length){
			case 0:
				this.den = 1;
				this.num = 0;
				break;
			case 1:
				if(typeof arg1 === 'object' && arg1 instanceof Rational){
					this.den = arg1.getDenominator();
					this.num = arg1.getNumerator();
				}
				else{
					this.den = 1;
					this.num = this._validate(arg1);
				}
				break;
			case 2:
				this.den = this._validate(arg2);
				this.num = this._validate(arg1);
				this._normalize();
				break;
			default:
				throw TypeError(`Rational: invalid number of arguments`);
		}

		Object.defineProperty(this, 'den', { enumerable: false });
		Object.defineProperty(this, 'num', { enumerable: false });
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @param    {any} arg - The argument to be validated as a numerator or denominator.
	 * @returns  {number} An integer number.
	 * @desc     Validates the given argument to be used as a numerator or denominator.
	 * @throws   {TypeError} If the given argument is an invalid numerator or denominator.
	 */
	_validate(arg){
		if(typeof arg === 'number' && !Number.isNaN(arg)){
			return Math.trunc(arg);
		}
		else
		if(typeof arg === 'string' && arg && !isNaN(arg)){
			return Math.trunc(Number(arg));
		}
		else
		if(typeof arg === 'bigint'){
			throw TypeError('Rational: BigInt type is not accepted');
		}
		else{
			throw TypeError(`Rational: invalid argument`);
		}
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @desc     Normalizes the rational number i.e. no common factors and denominator is positive.
	 * @throws   {TypeError} In case of zero denominator.
	 */
	_normalize(){
		if(this.den === 0){
			throw TypeError('Rational: bad rational, zero denominator');
		}
		else
		if(this.num === 0){
			this.den = 1;
		}
		else{
			let g = Math.abs( Rational.gcd(this.num, this.den) );

			this.num /= g;
			this.den /= g;

			if(this.den < 0){
				this.num = -this.num;
				this.den = -this.den;
			}
		}
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @desc     An accessor function to return the numerator.
	 * @returns  {number} The numerator.
	 */
	getNumerator(){ return this.num; }

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @desc     An accessor function to return the denominator.
	 * @returns  {number} The denominator.
	 */
	getDenominator(){ return this.den; }

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @param    {object|number} arg1 - [Optional] Rational object or numerator.
	 * @param    {string} arg2 - [Optional] denominator.
	 * @desc     Assignment to this Rational object.
	 * @throws   {TypeError} If neither of one or two arguments are given, or invalid argument type is used.
	 */
	assign(arg1, arg2){
		switch(arguments.length){
			case 1:
				if(typeof arg1 === 'object' && arg1 instanceof Rational){
					this.num = arg1.getNumerator();
					this.den = arg1.getDenominator();
				}
				else{
					this.num = this._validate(arg1);
					this.den = 1;
				}
				break;
			case 2:
				this.num = this._validate(arg1);
				this.den = this._validate(arg2);
				this._normalize();
				break;
			default:
				throw TypeError(`Rational: invalid number of arguments`);
		}
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @param    {object|number} arg - Rational object or integer number.
	 * @desc     Adds to this Rational object another Rational object or an integer value.
	 * @returns  {object} The Rational object denoted by this.
	 * @throws   {TypeError} If other than one arguments is given, or invalid argument type is used.
	 */
	add(arg){
		switch(arguments.length){
			case 1:
				if(typeof arg === 'object' && arg instanceof Rational){
					let r_num = arg.getNumerator();
					let r_den = arg.getDenominator();

					let g = Rational.gcd(this.den, r_den);

					this.den /= g;
					this.num = this.num * (r_den / g) + r_num * this.den;

					g = Math.abs( Rational.gcd(this.num, g) );
					this.num /= g;
					this.den *= (r_den/g);
				}
				else{
					arg = this._validate(arg);
					this.num += arg * this.den;
				}
				return this;
			default:
				throw TypeError(`Rational: invalid number of arguments`);
		}
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @param    {object|number} arg - Rational object or integer number.
	 * @desc     Subtracts from this Rational object another Rational object or an integer value.
	 * @returns  {object} The Rational object denoted by this.
	 * @throws   {TypeError} If other than one arguments is given, or invalid argument type is used.
	 */
	sub(arg){
		switch(arguments.length){
			case 1:
				if(typeof arg === 'object' && arg instanceof Rational){
					let r_num = arg.getNumerator();
					let r_den = arg.getDenominator();

					let g = Rational.gcd(this.den, r_den);

					this.den /= g;
					this.num = this.num * (r_den / g) - r_num * this.den;

					g = Math.abs( Rational.gcd(this.num, g) );
					this.num /= g;
					this.den *= (r_den/g);
				}
				else{
					arg = this._validate(arg);
					this.num -= arg * this.den;
				}
				return this;
			default:
				throw TypeError(`Rational: invalid number of arguments`);
		}
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @param    {object|number} arg - Rational object or integer number.
	 * @desc     Multiplies this Rational object by another Rational object or an integer value.
	 * @returns  {object} The Rational object denoted by this.
	 * @throws   {TypeError} If other than one arguments is given, or invalid argument type is used.
	 */
	mul(arg){
		switch(arguments.length){
			case 1:
				if(typeof arg === 'object' && arg instanceof Rational){
					let r_num = arg.getNumerator();
					let r_den = arg.getDenominator();

					let gcd1 = Math.abs( Rational.gcd(this.num, r_den) );
					let gcd2 = Math.abs( Rational.gcd(r_num, this.den) );

					this.num = (this.num/gcd1) * (r_num/gcd2);
					this.den = (this.den/gcd2) * (r_den/gcd1);
				}
				else{
					arg = this._validate(arg);

					let gcd = Math.abs( Rational.gcd( arg, this.den ) );
					this.num *= arg / gcd;
					this.den /= gcd;
				}
				return this;
			default:
				throw TypeError(`Rational: invalid number of arguments`);
		}
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @param    {object|number} arg - Rational object or integer number.
	 * @desc     Divides this Rational object by another Rational object or an integer value.
	 * @returns  {object} The Rational object denoted by this.
	 * @throws   {TypeError} If other than one arguments is given, or invalid argument type is used.
	 */
	div(arg){
		switch(arguments.length){
			case 1:
				if(typeof arg === 'object' && arg instanceof Rational){

					let r_num = arg.getNumerator();
					let r_den = arg.getDenominator();

					if(r_num === 0){
						throw TypeError(`Rational: division by zero`);
					}

					if(this.num === 0) return this;

					let gcd1 = Math.abs( Rational.gcd(this.num, r_num) );
					let gcd2 = Math.abs( Rational.gcd(r_den, this.den) );

					this.num = (this.num/gcd1) * (r_den/gcd2);
					this.den = (this.den/gcd2) * (r_num/gcd1);

					if(this.den < 0){
						this.num = -this.num;
						this.den = -this.den;
					}
				}
				else{
					arg = this._validate(arg);

					if(arg === 0){
						throw TypeError(`Rational: division by zero`);
					}

 					if(this.num == 0) return this;

					let gcd = Math.abs( Rational.gcd(this.num, arg) );
					this.num = this.num / gcd;
					this.den *= (arg / gcd);

					if(this.den < 0){
						this.num = -this.num;
						this.den = -this.den;
					}
				}
				return this;
			default:
				throw TypeError(`Rational: invalid number of arguments`);
		}
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @param    {number} arg - Integer number.
	 * @desc     Raise this Rational object to the power given.
	 * @returns  {object} The Rational object denoted by this.
	 * @throws   {TypeError} If other than one arguments is given, or invalid argument type is used.
	 */
	pow(arg){
		switch(arguments.length){
			case 1:
				arg = this._validate(arg);

				let r = new Rational(1);
				for(let i = 0; i < Math.abs(arg); i++){
					r = r.mul(this);
				}
				if(arg < 0){
					r = (new Rational(1)).div(r);
				}

				this.num = r.getNumerator();
				this.den = r.getDenominator();
				return this;
			default:
				throw TypeError(`Rational: invalid number of arguments`);
		}
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @desc     Increment and return this Rational object.
	 * @returns  {object} The Rational object denoted by this.
	 * @throws   {TypeError} If an argument was given.
	 */
	preInc(){
		if(arguments.length > 0){
			throw TypeError(`Rational: invalid number of arguments`);
		}
		this.num += this.den;
		return this;
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @desc     Decrement and return this Rational object.
	 * @returns  {object} The Rational object denoted by this.
	 * @throws   {TypeError} If an argument was given.
	 */
	preDec(){
		if(arguments.length > 0){
			throw TypeError(`Rational: invalid number of arguments`);
		}
		this.num -= this.den;
		return this;
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @desc     Return this Rational object, then increment it.
	 * @returns  {object} Copy of this Rational object before increment.
	 * @throws   {TypeError} If an argument was given.
	 */
	postInc(){
		if(arguments.length > 0){
			throw TypeError(`Rational: invalid number of arguments`);
		}
		let t = new Rational(this);
		this.num += this.den;
		return t;
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @desc     Return this Rational object, then decrement it.
	 * @returns  {object} Copy of this Rational object before decrement.
	 * @throws   {TypeError} If an argument was given.
	 */
	postDec(){
		if(arguments.length > 0){
			throw TypeError(`Rational: invalid number of arguments`);
		}
		let t = new Rational(this);
		this.num -= this.den;
		return t;
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @desc     Set this Rational object to its absolute value.
	 * @returns  {object} The Rational object denoted by this.
	 * @throws   {TypeError} If an argument was given.
	 */
	abs(){
		if(arguments.length > 0){
			throw TypeError(`Rational: invalid number of arguments`);
		}
		if(this.num < 0) this.num = -this.num;
		return this;
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @desc     Negate this Rational object sign.
	 * @returns  {object} The Rational object denoted by this.
	 * @throws   {TypeError} If an argument was given.
	 */
	neg(){
		if(arguments.length > 0){
			throw TypeError(`Rational: invalid number of arguments`);
		}
		if(this.num !== 0) this.num = -this.num;
		return this;
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @desc     Returns true if this Rational object is zero, false otherwise.
	 * @returns  {boolean} True if this Rational object is zero, false otherwise.
	 * @throws   {TypeError} If an argument was given.
	 */
	not(){
		if(arguments.length > 0){
			throw TypeError(`Rational: invalid number of arguments`);
		}
		return !this.num;
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @desc     Returns true if this Rational object is not zero, false otherwise.
	 * @returns  {boolean} True if this Rational object is not zero, false otherwise.
	 * @throws   {TypeError} If an argument was given.
	 */
	bool(){
		if(arguments.length > 0){
			throw TypeError(`Rational: invalid number of arguments`);
		}
		return !!this.num;
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @param    {object|number} arg - Rational object or integer number.
	 * @desc     Compare this Rational object to the given argument.
	 * @returns  {boolean} True if this Rational object is less than the given argument.
	 * @throws   {TypeError} If other than one arguments is given, or invalid argument type is used.
	 */
	lessThan(arg){
		switch(arguments.length){
			case 1:
				if(typeof arg === 'object' && arg instanceof Rational){

					let ts = {
						n: this.num,
						d: this.den,
						q: Math.trunc(this.num / this.den),
						r: this.num % this.den
					};

					let rs = {
						n: arg.getNumerator(),
						d: arg.getDenominator(),
						q: Math.trunc(arg.getNumerator() / arg.getDenominator()),
						r: arg.getNumerator() % arg.getDenominator()
					};

					while (ts.r < 0){ ts.r += ts.d; --ts.q; }
					while (rs.r < 0){ rs.r += rs.d; --rs.q; }

					let reverse = 0x0;

					for( ;; ){
						if(ts.q !== rs.q){
							return reverse ? ts.q > rs.q : ts.q < rs.q;
						}

						reverse ^= 0x1;

						if(ts.r === 0 || rs.r === 0) break;

						ts.n = ts.d;         ts.d = ts.r;
						ts.q = Math.trunc(ts.n / ts.d);  ts.r = ts.n % ts.d;
						rs.n = rs.d;         rs.d = rs.r;
						rs.q = Math.trunc(rs.n / rs.d);  rs.r = rs.n % rs.d;
					}

					if(ts.r === rs.r)
						return false;
					else
						return ( ts.r !== zero ) !== ( !!reverse );
				}
				else{
					arg = this._validate(arg);

					let q = Math.trunc(this.num / this.den);
					let r = this.num % this.den;

					while(r < 0){ r += this.den; --q; }

					return q < arg;
				}
			default:
				throw TypeError(`Rational: invalid number of arguments`);
		}
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @param    {object|number} arg - Rational object or integer number.
	 * @desc     Compare this Rational object to the given argument.
	 * @returns  {boolean} True if this Rational object is greater than the given argument.
	 * @throws   {TypeError} If other than one arguments is given, or invalid argument type is used.
	 */
	greaterThan(arg){
		switch(arguments.length){
			case 1:
				if(typeof arg === 'object' && arg instanceof Rational){
					return !(this.lessThan(arg) || this.equalTo(arg));
				}
				else{
					arg = this._validate(arg);
					return !(this.lessThan(arg) || this.equalTo(arg));
				}
			default:
				throw TypeError(`Rational: invalid number of arguments`);
		}
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @param    {object|number} arg - Rational object or integer number.
	 * @desc     Compare this Rational object to the given argument.
	 * @returns  {boolean} True if this Rational object is equal to the given argument.
	 * @throws   {TypeError} If other than one arguments is given, or invalid argument type is used.
	 */
	equalTo(arg){
		switch(arguments.length){
			case 1:
				if(typeof arg === 'object' && arg instanceof Rational){
					return this.num === arg.getNumerator() && this.den === arg.getDenominator();
				}
				else{
					arg = this._validate(arg);
					return this.num === arg && this.den === 1;
				}
			default:
				throw TypeError(`Rational: invalid number of arguments`);
		}
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @param    {object|number} arg - Rational object or integer number.
	 * @desc     Compare this Rational object to the given argument.
	 * @returns  {boolean} True if this Rational object is not equal to the given argument.
	 * @throws   {TypeError} If other than one arguments is given, or invalid argument type is used.
	 */
	notEqualTo(arg){
		switch(arguments.length){
			case 1:
				if(typeof arg === 'object' && arg instanceof Rational){
					return !this.equalTo(arg);
				}
				else{
					arg = this._validate(arg);
					return !this.equalTo(arg);
				}
			default:
				throw TypeError(`Rational: invalid number of arguments`);
		}
	}

	/**
	 * @method
	 * @instance
	 * @memberof module:rational-module.Rational
	 * @desc     Returns this Rational object converted to a real number.
	 * @returns  {number} Real number.
	 * @throws   {TypeError} If an argument was given.
	 */
	valueOf(){
		if(arguments.length > 0){
			throw TypeError(`Rational: invalid number of arguments`);
		}
		return this.num / this.den;
	}

	toString(){
		if(arguments.length > 0){
			throw TypeError(`Rational: invalid number of arguments`);
		}
		return `${this.num < 0 ? '-' : ''}${Math.abs(this.num)}/${this.den}`;
	}
}

module.exports.Rational = Rational;
