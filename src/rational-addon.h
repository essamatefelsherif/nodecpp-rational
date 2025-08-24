/**
 * @file    rational-addon.h
 * @version v1.0.0
 * @author  Essam A. El-Sherif <esm.elsh@gmail.com>
 */

#ifndef __RATIONAL_ADDON_H__
#define __RATIONAL_ADDON_H__

#include <node.h>
#include <node_object_wrap.h>

#include "../include/rational.h"

/**
 * namespace addon
 */
namespace addon{

	using v8::FunctionCallbackInfo;
	using v8::PropertyCallbackInfo;
	using v8::Local;
	using v8::Value;
	using v8::Object;
	using v8::String;
	using v8::Persistent;
	using v8::Function;

	using esm::rational;

	/**
	 * A class that extends the base class ObjectWrap provided by Node.js and
	 * can be instantiated from JavaScript using the new operator.
	 */
	class WrappedRational : public node::ObjectWrap{
		public:
			/**
			 * A static function responsible for adding all class functions
			 * calleable from JavaScript code to the exports object.
			 */
			static void Init(Local<Object>);
		private:
			rational<long> *ptrRational;

			/** Default constructor */
			explicit WrappedRational()
				: ptrRational(new rational<long>()){}

			/** One argument constructor */
			explicit WrappedRational(const long& n)
				: ptrRational(new rational<long>(n)){}

			/** Two arguments constructor */
			explicit WrappedRational(const long& n, const long& d)
				: ptrRational(new rational<long>(n, d)){}

			~WrappedRational(){
				delete ptrRational;
			}

			static Persistent<Function> constructor;

			/**
			 * A static function associated with the JavaScript function 'Rational'
			 * which should be called within the context of 'new Rational(...)'.
			 */
			static void New(const FunctionCallbackInfo<Value>&);

			static void GetNumerator(const FunctionCallbackInfo<Value>&);
			static void GetDenominator(const FunctionCallbackInfo<Value>&);

			static void Assign(const FunctionCallbackInfo<Value>&);

			static void Add(const FunctionCallbackInfo<Value>&);
			static void Sub(const FunctionCallbackInfo<Value>&);
			static void Mul(const FunctionCallbackInfo<Value>&);
			static void Div(const FunctionCallbackInfo<Value>&);
			static void Pow(const FunctionCallbackInfo<Value>&);

			static void PreInc (const FunctionCallbackInfo<Value>&);
			static void PreDec (const FunctionCallbackInfo<Value>&);
			static void PostInc(const FunctionCallbackInfo<Value>&);
			static void PostDec(const FunctionCallbackInfo<Value>&);

			static void Neg (const FunctionCallbackInfo<Value>&);
			static void Abs (const FunctionCallbackInfo<Value>&);
			static void Not (const FunctionCallbackInfo<Value>&);
			static void Bool(const FunctionCallbackInfo<Value>&);

			static void LessThan(const FunctionCallbackInfo<Value>&);
			static void GreaterThan(const FunctionCallbackInfo<Value>&);
			static void EqualTo(const FunctionCallbackInfo<Value>&);
			static void NotEqualTo(const FunctionCallbackInfo<Value>&);

			static void ValueOf(const FunctionCallbackInfo<Value>&);
			static void ToString(const FunctionCallbackInfo<Value>&);

			static void GetField(Local<String>, const PropertyCallbackInfo<Value>&);
	};

	/**
	 * C++ function that will be called from JavaScript when the module is required.
	 *
	 * @param exports The JavaScript module.exports object.
	 * @param module  The JavaScript module object.
	 */
	void Init(Local<Object>, Local<Value>, void*);
}

#endif
