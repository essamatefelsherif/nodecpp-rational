/**
 * @file    rational-addon.cpp
 * @version v1.0.0
 * @author  Essam A. El-Sherif
 */

#include <cstring>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cmath>

#include "rational-addon.h"

namespace addon{

	using v8::Isolate;
	using v8::Context;
	using v8::FunctionTemplate;
	using v8::ObjectTemplate;
	using v8::Exception;
	using v8::Number;
	using v8::Boolean;
	using v8::DEFAULT;
	using v8::DontEnum;
	using v8::MaybeLocal;

	Persistent<Function> WrappedRational::constructor;

	/**
	 * Throws an exception
	 */
	inline void throwException(Isolate *isolate, Local<Value>(*func)(Local<String>), const char* msg){
		isolate->ThrowException(
			func(String::NewFromUtf8(isolate, msg).ToLocalChecked())
		);
	}
}

void addon::WrappedRational::Init(Local<Object> exports){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = exports->GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	const char* className = "Rational";

	/*
	 * Create a new JavaScript function using a FunctionTemplate.
	 * This function is going to serve as a constructor function in JavaScript.
	 * It will be initially invoked when JavaScript executes 'r = new Rational()'.
	 */
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, WrappedRational::New);

	/* Set the class name of the v8::FunctionTemplate. */
	tpl->SetClassName(
		String::NewFromUtf8(isolate, className).ToLocalChecked()
	);

	/* The v8::ObjectTemplate associated with this v8::FunctionTemplate. */
	Local<ObjectTemplate> otpl = tpl->InstanceTemplate();

	/*
	 * To allow for a single instance of a C++ object (the WrappedRational)
	 * to be associated with the ObjectTemplate.
	 */
	otpl->SetInternalFieldCount(1);

	/*
	 * C++ inline function defined by 'node.h'
	 *
	 * Adds a function to the given v8::FunctionTemplate prototype,
	 * and makes it callable from JavaScript by the name given.
	 */
	NODE_SET_PROTOTYPE_METHOD(tpl, "getNumerator",   GetNumerator);
	NODE_SET_PROTOTYPE_METHOD(tpl, "getDenominator", GetDenominator);

	NODE_SET_PROTOTYPE_METHOD(tpl, "assign",   Assign);

	NODE_SET_PROTOTYPE_METHOD(tpl, "selfAdd", SelfAdd);
	NODE_SET_PROTOTYPE_METHOD(tpl, "selfSub", SelfSub);
	NODE_SET_PROTOTYPE_METHOD(tpl, "selfMul", SelfMul);
	NODE_SET_PROTOTYPE_METHOD(tpl, "selfDiv", SelfDiv);
	NODE_SET_PROTOTYPE_METHOD(tpl, "selfPow", SelfPow);

	NODE_SET_PROTOTYPE_METHOD(tpl, "add", Add);
	NODE_SET_PROTOTYPE_METHOD(tpl, "sub", Sub);
	NODE_SET_PROTOTYPE_METHOD(tpl, "mul", Mul);
	NODE_SET_PROTOTYPE_METHOD(tpl, "div", Div);
	NODE_SET_PROTOTYPE_METHOD(tpl, "pow", Pow);

	NODE_SET_PROTOTYPE_METHOD(tpl, "preInc", PreInc);
	NODE_SET_PROTOTYPE_METHOD(tpl, "preDec", PreDec);
	NODE_SET_PROTOTYPE_METHOD(tpl, "postInc", PostInc);
	NODE_SET_PROTOTYPE_METHOD(tpl, "postDec", PostDec);

	NODE_SET_PROTOTYPE_METHOD(tpl, "selfNeg", SelfNeg);
	NODE_SET_PROTOTYPE_METHOD(tpl, "selfAbs", SelfAbs);

	NODE_SET_PROTOTYPE_METHOD(tpl, "neg", Neg);
	NODE_SET_PROTOTYPE_METHOD(tpl, "abs", Abs);

	NODE_SET_PROTOTYPE_METHOD(tpl, "not",  Not);
	NODE_SET_PROTOTYPE_METHOD(tpl, "bool", Bool);

	NODE_SET_PROTOTYPE_METHOD(tpl, "lessThan",    LessThan);
	NODE_SET_PROTOTYPE_METHOD(tpl, "greaterThan", GreaterThan);
	NODE_SET_PROTOTYPE_METHOD(tpl, "equalTo",     EqualTo);
	NODE_SET_PROTOTYPE_METHOD(tpl, "notEqualTo",  NotEqualTo);

	NODE_SET_PROTOTYPE_METHOD(tpl, "valueOf", ValueOf);
	NODE_SET_PROTOTYPE_METHOD(tpl, "toString", ToString);

	otpl->SetAccessor(
		String::NewFromUtf8(isolate, "num").ToLocalChecked(),
		GetField, nullptr, Local<Value>(), DEFAULT,	DontEnum);

	otpl->SetAccessor(
		String::NewFromUtf8(isolate, "den").ToLocalChecked(),
		GetField, nullptr, Local<Value>(), DEFAULT,	DontEnum);

	constructor.Reset(isolate, tpl->GetFunction(context).ToLocalChecked());

	/** @see https://v8docs.nodesource.com/node-20.3/db/d85/classv8_1_1_object.html#afa385ca327840d11af853537eacf8e8b */
	exports->Set(
		context,
		String::NewFromUtf8(isolate, className).ToLocalChecked(),
		tpl->GetFunction(context).ToLocalChecked()
	).Check();
}

void addon::WrappedRational::New(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	WrappedRational *obj = nullptr;

	// Invoked as constructor: `new Rational(...)`
	if(args.IsConstructCall()){

		switch(args.Length()){

			/* Default constructor */
			case 0:
				obj = new WrappedRational();
			break;

			/* One argument constructor */
			case 1:
				if(args[0]->IsObject()){

					Local<String> constructor = args[0].As<Object>()->GetConstructorName();
					String::Utf8Value str(isolate, constructor);

					if(std::strcmp(*str, "Rational")){
						throwException(isolate, Exception::TypeError, "Rational: invalid argument");
						return;
					}

					Local<Value> numObj = args[0].As<Object>()->Get(
							context,
							String::NewFromUtf8(
								isolate, "num").ToLocalChecked()
							).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

					Local<Value> denObj = args[0].As<Object>()->Get(
							context,
							String::NewFromUtf8(
								isolate, "den").ToLocalChecked()
							).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

					long n = numObj.As<Number>()->Value();
					long d = denObj.As<Number>()->Value();

					obj = new WrappedRational(n, d);
				}
				else
				if(args[0]->IsNumber()){

					double value = args[0].As<Number>()->Value();
					if(std::isnan(value)){
						throwException(isolate, Exception::TypeError, "Rational: invalid argument");
						return;
					}

					long n = value;
					long d = 1L;

					obj = new WrappedRational(n, d);
				}
				else
				if(args[0]->IsBigInt()){
					throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
					return;
				}
				else
				if(args[0]->IsString()){

					String::Utf8Value str(isolate, args[0]);
					char *end = NULL;

					double value = std::strtol(*str, &end, 10);
					if(*str == end || std::isnan(value)){
						throwException(isolate, Exception::TypeError, "Rational: invalid argument");
						return;
					}

					long n = value;
					long d = 1L;

					obj = new WrappedRational(n, d);
				}
				else{
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}
			break;

			/* Two argument constructor */
			case 2: {
				if(args[0]->IsNumber() && args[1]->IsNumber()){

					double val1 = args[0].As<Number>()->Value();
					double val2 = args[1].As<Number>()->Value();
					if(std::isnan(val1) || std::isnan(val2)){
						throwException(isolate, Exception::TypeError, "Rational: invalid argument");
						return;
					}

					long n = val1;
					long d = val2;

					if(d == 0){
						throwException(isolate, Exception::TypeError, "Rational: bad rational, zero denominator");
						return;
					}
					obj = new WrappedRational(n, d);
				}
				else
				if(args[0]->IsBigInt() || args[1]->IsBigInt()){
					throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
					return;
				}
				else
				if(args[0]->IsString() || args[1]->IsString()){
					long n, d;

					if(args[0]->IsString()){
						String::Utf8Value str(isolate, args[0]);
						char *end = NULL;

						double value = std::strtol(*str, &end, 10);
						if(*str == end || std::isnan(value)){
							throwException(isolate, Exception::TypeError, "Rational: invalid argument");
							return;
						}
						n = value;

						if(args[1]->IsNumber()){
							double value = args[1].As<Number>()->Value();
							if(std::isnan(value)){
								throwException(isolate, Exception::TypeError, "Rational: invalid argument");
								return;
							}
							d = value;
						}
						else
						if(args[1]->IsString()){
							String::Utf8Value str(isolate, args[1]);
							char *end = NULL;

							double value = std::strtol(*str, &end, 10);
							if(*str == end || std::isnan(value)){
								throwException(isolate, Exception::TypeError, "Rational: invalid argument");
								return;
							}
							d = value;
						}
						else{
							throwException(isolate, Exception::TypeError, "Rational: invalid argument");
							return;
						}
					}
					else
					if(args[0]->IsNumber()){
						double value = args[0].As<Number>()->Value();
						if(std::isnan(value)){
							throwException(isolate, Exception::TypeError, "Rational: invalid argument");
							return;
						}
						n = value;

						String::Utf8Value str(isolate, args[1]);
						char *end = NULL;

						value = std::strtol(*str, &end, 10);
						if(*str == end || std::isnan(value)){
							throwException(isolate, Exception::TypeError, "Rational: invalid argument");
							return;
						}
						d = value;
					}
					else{
						throwException(isolate, Exception::TypeError, "Rational: invalid argument");
						return;
					}

					if(d == 0){
						throwException(isolate, Exception::TypeError, "Rational: bad rational, zero denominator");
						return;
					}
					obj = new WrappedRational(n, d);
				}
				else{
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}
			}
			break;

			default:
				throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
				return;
		}

		obj->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	}
	// Invoked as plain function `Rational(...)`, turn into construct call.
	else{
		Local<Function> cons = Local<Function>::New(isolate, constructor);
		int argc = args.Length();

		switch(args.Length()){
			case 0: {
				Local<Value> *argv = nullptr;
				args.GetReturnValue()
					.Set(cons->NewInstance(context, argc, argv)
						.ToLocalChecked()
				);
			}
			break;
			case 1: {
				Local<Value> argv[] = { args[0] };
				args.GetReturnValue()
					.Set(cons->NewInstance(context, argc, argv)
						.ToLocalChecked()
				);
			}
			break;
			case 2: {
				Local<Value> argv[] = { args[0], args[1] };
				args.GetReturnValue()
					.Set(cons->NewInstance(context, argc, argv)
						.ToLocalChecked()
				);
			}
			break;
			default: {
				isolate->ThrowException(
					Exception::TypeError(
						String::NewFromUtf8(
							isolate,
							"Rational: invalid number of arguments"
						).ToLocalChecked()
					)
				);
				return;
			}
			break;
		}
	}
}

void addon::WrappedRational::GetNumerator(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
	long num = ptrWrappedRational->ptrRational->numerator();

	args.GetReturnValue().Set(Number::New(isolate, num));
}

void addon::WrappedRational::GetDenominator(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
	long den = ptrWrappedRational->ptrRational->denominator();

	args.GetReturnValue().Set(Number::New(isolate, den));
}

void addon::WrappedRational::Assign(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());

	switch(args.Length()){
		case 1:
			if(args[0]->IsObject()){

				Local<String> constructor = args[0].As<Object>()->GetConstructorName();
				String::Utf8Value str(isolate, constructor);

				if(std::strcmp(*str, "Rational")){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				Local<Value> numObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "num").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				Local<Value> denObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "den").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				long n = numObj.As<Number>()->Value();
				long d = denObj.As<Number>()->Value();

				ptrWrappedRational->ptrRational->assign(n, d);
			}
			else
			if(args[0]->IsNumber()){

				double value = args[0].As<Number>()->Value();
				if(std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;

				ptrWrappedRational->ptrRational->assign(n, d);
			}
			else
			if(args[0]->IsBigInt()){
				throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
				return;
			}
			else
			if(args[0]->IsString()){

				String::Utf8Value str(isolate, args[0]);
				char *end = NULL;

				double value = std::strtol(*str, &end, 10);
				if(*str == end || std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;

				ptrWrappedRational->ptrRational->assign(n, d);
			}
			else{
				throwException(isolate, Exception::TypeError, "Rational: invalid argument");
				return;
			}
		break;

		case 2: {
			if(args[0]->IsNumber() && args[1]->IsNumber()){

				double val1 = args[0].As<Number>()->Value();
				double val2 = args[1].As<Number>()->Value();
				if(std::isnan(val1) || std::isnan(val2)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = val1;
				long d = val2;

				if(d == 0){
					throwException(isolate, Exception::TypeError, "Rational: bad rational, zero denominator");
					return;
				}

				ptrWrappedRational->ptrRational->assign(n, d);
			}
			else
			if(args[0]->IsBigInt() || args[1]->IsBigInt()){
				throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
				return;
			}
			else
			if(args[0]->IsString() || args[1]->IsString()){
				long n, d;

				if(args[0]->IsString()){
					String::Utf8Value str(isolate, args[0]);
					char *end = NULL;

					double value = std::strtol(*str, &end, 10);
					if(*str == end || std::isnan(value)){
						throwException(isolate, Exception::TypeError, "Rational: invalid argument");
						return;
					}
					n = value;

					if(args[1]->IsNumber()){
						double value = args[1].As<Number>()->Value();
						if(std::isnan(value)){
							throwException(isolate, Exception::TypeError, "Rational: invalid argument");
							return;
						}
						d = value;
					}
					else
					if(args[1]->IsString()){
						String::Utf8Value str(isolate, args[1]);
						char *end = NULL;

						double value = std::strtol(*str, &end, 10);
						if(*str == end || std::isnan(value)){
							throwException(isolate, Exception::TypeError, "Rational: invalid argument");
							return;
						}
						d = value;
					}
					else{
						throwException(isolate, Exception::TypeError, "Rational: invalid argument");
						return;
					}
				}
				else
				if(args[0]->IsNumber()){
					double value = args[0].As<Number>()->Value();
					if(std::isnan(value)){
						throwException(isolate, Exception::TypeError, "Rational: invalid argument");
						return;
					}
					n = value;

					String::Utf8Value str(isolate, args[1]);
					char *end = NULL;

					value = std::strtol(*str, &end, 10);
					if(*str == end || std::isnan(value)){
						throwException(isolate, Exception::TypeError, "Rational: invalid argument");
						return;
					}
					d = value;
				}
				else{
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				if(d == 0){
					throwException(isolate, Exception::TypeError, "Rational: bad rational, zero denominator");
					return;
				}
				ptrWrappedRational->ptrRational->assign(n, d);
			}
			else{
				throwException(isolate, Exception::TypeError, "Rational: invalid argument");
				return;
			}
		}
		break;

		default:
			throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
			return;
	}
}

void addon::WrappedRational::SelfAdd(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());

	switch(args.Length()){
		case 1:
			if(args[0]->IsObject()){

				Local<String> constructor = args[0].As<Object>()->GetConstructorName();
				String::Utf8Value str(isolate, constructor);

				if(std::strcmp(*str, "Rational")){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				Local<Value> numObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "num").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				Local<Value> denObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "den").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				long n = numObj.As<Number>()->Value();
				long d = denObj.As<Number>()->Value();

				src::rational<long> rational(n, d);
				ptrWrappedRational->ptrRational->operator +=(rational);
			}
			else
			if(args[0]->IsNumber()){

				double value = args[0].As<Number>()->Value();
				if(std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;

				src::rational<long> rational(n, d);
				ptrWrappedRational->ptrRational->operator +=(rational);
			}
			else
			if(args[0]->IsBigInt()){
				throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
				return;
			}
			else
			if(args[0]->IsString()){

				String::Utf8Value str(isolate, args[0]);
				char *end = NULL;

				double value = std::strtol(*str, &end, 10);
				if(*str == end || std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;

				src::rational<long> rational(n, d);
				ptrWrappedRational->ptrRational->operator +=(rational);
			}
			else{
				throwException(isolate, Exception::TypeError, "Rational: invalid argument");
				return;
			}
		break;

		default:
			throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
			return;
	}

	args.GetReturnValue().Set(args.This());
}

void addon::WrappedRational::SelfSub(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());

	switch(args.Length()){
		case 1:
			if(args[0]->IsObject()){

				Local<String> constructor = args[0].As<Object>()->GetConstructorName();
				String::Utf8Value str(isolate, constructor);

				if(std::strcmp(*str, "Rational")){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				Local<Value> numObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "num").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				Local<Value> denObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "den").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				long n = numObj.As<Number>()->Value();
				long d = denObj.As<Number>()->Value();

				src::rational<long> rational(n, d);
				ptrWrappedRational->ptrRational->operator -=(rational);
			}
			else
			if(args[0]->IsNumber()){

				double value = args[0].As<Number>()->Value();
				if(std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;

				src::rational<long> rational(n, d);
				ptrWrappedRational->ptrRational->operator -=(rational);
			}
			else
			if(args[0]->IsBigInt()){
				throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
				return;
			}
			else
			if(args[0]->IsString()){

				String::Utf8Value str(isolate, args[0]);
				char *end = NULL;

				double value = std::strtol(*str, &end, 10);
				if(*str == end || std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;

				src::rational<long> rational(n, d);
				ptrWrappedRational->ptrRational->operator -=(rational);
			}
			else{
				throwException(isolate, Exception::TypeError, "Rational: invalid argument");
				return;
			}
		break;

		default:
			throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
			return;
	}

	args.GetReturnValue().Set(args.This());
}

void addon::WrappedRational::SelfMul(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());

	switch(args.Length()){
		case 1:
			if(args[0]->IsObject()){

				Local<String> constructor = args[0].As<Object>()->GetConstructorName();
				String::Utf8Value str(isolate, constructor);

				if(std::strcmp(*str, "Rational")){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				Local<Value> numObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "num").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				Local<Value> denObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "den").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				long n = numObj.As<Number>()->Value();
				long d = denObj.As<Number>()->Value();

				src::rational<long> rational(n, d);
				ptrWrappedRational->ptrRational->operator *=(rational);
			}
			else
			if(args[0]->IsNumber()){

				double value = args[0].As<Number>()->Value();
				if(std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;

				src::rational<long> rational(n, d);
				ptrWrappedRational->ptrRational->operator *=(rational);
			}
			else
			if(args[0]->IsBigInt()){
				throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
				return;
			}
			else
			if(args[0]->IsString()){

				String::Utf8Value str(isolate, args[0]);
				char *end = NULL;

				double value = std::strtol(*str, &end, 10);
				if(*str == end || std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;

				src::rational<long> rational(n, d);
				ptrWrappedRational->ptrRational->operator *=(rational);
			}
			else{
				throwException(isolate, Exception::TypeError, "Rational: invalid argument");
				return;
			}
		break;

		default:
			throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
			return;
	}

	args.GetReturnValue().Set(args.This());
}

void addon::WrappedRational::SelfDiv(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());

	switch(args.Length()){
		case 1:
			if(args[0]->IsObject()){

				Local<String> constructor = args[0].As<Object>()->GetConstructorName();
				String::Utf8Value str(isolate, constructor);

				if(std::strcmp(*str, "Rational")){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				Local<Value> numObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "num").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				Local<Value> denObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "den").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				long n = numObj.As<Number>()->Value();
				long d = denObj.As<Number>()->Value();

				if(n == 0){
					throwException(isolate, Exception::TypeError, "Rational: division by zero");
					return;
				}

				src::rational<long> rational(n, d);
				ptrWrappedRational->ptrRational->operator /=(rational);
			}
			else
			if(args[0]->IsNumber()){

				double value = args[0].As<Number>()->Value();
				if(std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;

				if(n == 0){
					throwException(isolate, Exception::TypeError, "Rational: division by zero");
					return;
				}

				src::rational<long> rational(n, d);
				ptrWrappedRational->ptrRational->operator /=(rational);
			}
			else
			if(args[0]->IsBigInt()){
				throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
				return;
			}
			else
			if(args[0]->IsString()){

				String::Utf8Value str(isolate, args[0]);
				char *end = NULL;

				double value = std::strtol(*str, &end, 10);
				if(*str == end || std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;

				if(n == 0){
					throwException(isolate, Exception::TypeError, "Rational: division by zero");
					return;
				}

				src::rational<long> rational(n, d);
				ptrWrappedRational->ptrRational->operator /=(rational);
			}
			else{
				throwException(isolate, Exception::TypeError, "Rational: invalid argument");
				return;
			}
		break;

		default:
			throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
			return;
	}

	args.GetReturnValue().Set(args.This());
}

void addon::WrappedRational::SelfPow(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());

	switch(args.Length()){
		case 1:
			if(args[0]->IsNumber()){

				double value = args[0].As<Number>()->Value();
				if(std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;

				src::rational<long> r(1);
				for(long i = 0; i < std::abs(n); i++)
					r *= *(ptrWrappedRational->ptrRational);

				if(n < 1){
					if(r.numerator() == 0){
						throwException(isolate, Exception::TypeError, "Rational: division by zero");
						return;
					}
					src::rational<long> u(1);
					u /= r;
					r.assign(u.numerator(), u.denominator());
				}

				ptrWrappedRational->ptrRational->assign(r.numerator(), r.denominator());
			}
			else
			if(args[0]->IsBigInt()){
				throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
				return;
			}
			else
			if(args[0]->IsString()){

				String::Utf8Value str(isolate, args[0]);
				char *end = NULL;

				double value = std::strtol(*str, &end, 10);
				if(*str == end || std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;

				src::rational<long> r(1);
				for(long i = 0; i < std::abs(n); i++)
					r *= *(ptrWrappedRational->ptrRational);

				if(n < 1){
					if(r.numerator() == 0){
						throwException(isolate, Exception::TypeError, "Rational: division by zero");
						return;
					}
					src::rational<long> u(1);
					u /= r;
					r.assign(u.numerator(), u.denominator());
				}

				ptrWrappedRational->ptrRational->assign(r.numerator(), r.denominator());
			}
			else{
				throwException(isolate, Exception::TypeError, "Rational: invalid argument");
				return;
			}
		break;

		default:
			throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
			return;
	}

	args.GetReturnValue().Set(args.This());
}

void addon::WrappedRational::Add(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
	long n, d;

	switch(args.Length()){
		case 1:
			if(args[0]->IsObject()){

				Local<String> constructor = args[0].As<Object>()->GetConstructorName();
				String::Utf8Value str(isolate, constructor);

				if(std::strcmp(*str, "Rational")){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				Local<Value> numObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "num").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				Local<Value> denObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "den").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				n = numObj.As<Number>()->Value();
				d = denObj.As<Number>()->Value();
			}
			else
			if(args[0]->IsNumber()){

				double value = args[0].As<Number>()->Value();
				if(std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				n = value;
				d = 1L;
			}
			else
			if(args[0]->IsBigInt()){
				throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
				return;
			}
			else
			if(args[0]->IsString()){

				String::Utf8Value str(isolate, args[0]);
				char *end = NULL;

				double value = std::strtol(*str, &end, 10);
				if(*str == end || std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				n = value;
				d = 1L;
			}
			else{
				throwException(isolate, Exception::TypeError, "Rational: invalid argument");
				return;
			}
		break;

		default:
			throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
			return;
	}

	src::rational<long> rational(n, d);
	src::rational<long> retRational = src::operator+(*(ptrWrappedRational->ptrRational), rational);

	Local<Function> cons = Local<Function>::New(isolate, constructor);
	int argc = 2;
	Local<Value> argv[] = {
		Number::New(isolate, retRational.numerator()),
		Number::New(isolate, retRational.denominator())
	};

	args.GetReturnValue()
		.Set(cons->NewInstance(context, argc, argv)
		.ToLocalChecked()
	);
}

void addon::WrappedRational::Sub(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
	long n, d;

	switch(args.Length()){
		case 1:
			if(args[0]->IsObject()){

				Local<String> constructor = args[0].As<Object>()->GetConstructorName();
				String::Utf8Value str(isolate, constructor);

				if(std::strcmp(*str, "Rational")){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				Local<Value> numObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "num").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				Local<Value> denObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "den").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				n = numObj.As<Number>()->Value();
				d = denObj.As<Number>()->Value();
			}
			else
			if(args[0]->IsNumber()){

				double value = args[0].As<Number>()->Value();
				if(std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				n = value;
				d = 1L;
			}
			else
			if(args[0]->IsBigInt()){
				throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
				return;
			}
			else
			if(args[0]->IsString()){

				String::Utf8Value str(isolate, args[0]);
				char *end = NULL;

				double value = std::strtol(*str, &end, 10);
				if(*str == end || std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				n = value;
				d = 1L;
			}
			else{
				throwException(isolate, Exception::TypeError, "Rational: invalid argument");
				return;
			}
		break;

		default:
			throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
			return;
	}

	src::rational<long> rational(n, d);
	src::rational<long> retRational = src::operator-(*(ptrWrappedRational->ptrRational), rational);

	Local<Function> cons = Local<Function>::New(isolate, constructor);
	int argc = 2;
	Local<Value> argv[] = {
		Number::New(isolate, retRational.numerator()),
		Number::New(isolate, retRational.denominator())
	};

	args.GetReturnValue()
		.Set(cons->NewInstance(context, argc, argv)
		.ToLocalChecked()
	);
}

void addon::WrappedRational::Mul(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
	long n, d;

	switch(args.Length()){
		case 1:
			if(args[0]->IsObject()){

				Local<String> constructor = args[0].As<Object>()->GetConstructorName();
				String::Utf8Value str(isolate, constructor);

				if(std::strcmp(*str, "Rational")){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				Local<Value> numObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "num").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				Local<Value> denObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "den").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				n = numObj.As<Number>()->Value();
				d = denObj.As<Number>()->Value();
			}
			else
			if(args[0]->IsNumber()){

				double value = args[0].As<Number>()->Value();
				if(std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				n = value;
				d = 1L;
			}
			else
			if(args[0]->IsBigInt()){
				throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
				return;
			}
			else
			if(args[0]->IsString()){

				String::Utf8Value str(isolate, args[0]);
				char *end = NULL;

				double value = std::strtol(*str, &end, 10);
				if(*str == end || std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				n = value;
				d = 1L;
			}
			else{
				throwException(isolate, Exception::TypeError, "Rational: invalid argument");
				return;
			}
		break;

		default:
			throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
			return;
	}

	src::rational<long> rational(n, d);
	src::rational<long> retRational = src::operator*(*(ptrWrappedRational->ptrRational), rational);

	Local<Function> cons = Local<Function>::New(isolate, constructor);
	int argc = 2;
	Local<Value> argv[] = {
		Number::New(isolate, retRational.numerator()),
		Number::New(isolate, retRational.denominator())
	};

	args.GetReturnValue()
		.Set(cons->NewInstance(context, argc, argv)
		.ToLocalChecked()
	);
}

void addon::WrappedRational::Div(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
	long n, d;

	switch(args.Length()){
		case 1:
			if(args[0]->IsObject()){

				Local<String> constructor = args[0].As<Object>()->GetConstructorName();
				String::Utf8Value str(isolate, constructor);

				if(std::strcmp(*str, "Rational")){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				Local<Value> numObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "num").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				Local<Value> denObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "den").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				n = numObj.As<Number>()->Value();
				d = denObj.As<Number>()->Value();

				if(n == 0){
					throwException(isolate, Exception::TypeError, "Rational: division by zero");
					return;
				}
			}
			else
			if(args[0]->IsNumber()){

				double value = args[0].As<Number>()->Value();
				if(std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				n = value;
				d = 1L;

				if(n == 0){
					throwException(isolate, Exception::TypeError, "Rational: division by zero");
					return;
				}
			}
			else
			if(args[0]->IsBigInt()){
				throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
				return;
			}
			else
			if(args[0]->IsString()){

				String::Utf8Value str(isolate, args[0]);
				char *end = NULL;

				double value = std::strtol(*str, &end, 10);
				if(*str == end || std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				n = value;
				d = 1L;

				if(n == 0){
					throwException(isolate, Exception::TypeError, "Rational: division by zero");
					return;
				}
			}
			else{
				throwException(isolate, Exception::TypeError, "Rational: invalid argument");
				return;
			}
		break;

		default:
			throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
			return;
	}

	src::rational<long> rational(n, d);
	src::rational<long> retRational = src::operator/(*(ptrWrappedRational->ptrRational), rational);

	Local<Function> cons = Local<Function>::New(isolate, constructor);
	int argc = 2;
	Local<Value> argv[] = {
		Number::New(isolate, retRational.numerator()),
		Number::New(isolate, retRational.denominator())
	};

	args.GetReturnValue()
		.Set(cons->NewInstance(context, argc, argv)
		.ToLocalChecked()
	);
}

void addon::WrappedRational::Pow(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
	src::rational<long> r(1);

	switch(args.Length()){
		case 1:
			if(args[0]->IsNumber()){

				double value = args[0].As<Number>()->Value();
				if(std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				for(long i = 0; i < std::abs(n); i++)
					r *= *(ptrWrappedRational->ptrRational);

				if(n < 1){
					if(r.numerator() == 0){
						throwException(isolate, Exception::TypeError, "Rational: division by zero");
						return;
					}
					src::rational<long> u(1);
					u /= r;
					r.assign(u.numerator(), u.denominator());
				}
			}
			else
			if(args[0]->IsBigInt()){
				throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
				return;
			}
			else
			if(args[0]->IsString()){

				String::Utf8Value str(isolate, args[0]);
				char *end = NULL;

				double value = std::strtol(*str, &end, 10);
				if(*str == end || std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				for(long i = 0; i < std::abs(n); i++)
					r *= *(ptrWrappedRational->ptrRational);

				if(n < 1){
					if(r.numerator() == 0){
						throwException(isolate, Exception::TypeError, "Rational: division by zero");
						return;
					}
					src::rational<long> u(1);
					u /= r;
					r.assign(u.numerator(), u.denominator());
				}
			}
			else{
				throwException(isolate, Exception::TypeError, "Rational: invalid argument");
				return;
			}
		break;

		default:
			throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
			return;
	}

	Local<Function> cons = Local<Function>::New(isolate, constructor);
	int argc = 2;
	Local<Value> argv[] = {
		Number::New(isolate, r.numerator()),
		Number::New(isolate, r.denominator())
	};

	args.GetReturnValue()
		.Set(cons->NewInstance(context, argc, argv)
		.ToLocalChecked()
	);
}

void addon::WrappedRational::PreInc(const FunctionCallbackInfo<Value>& args){

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());

	if(args.Length() > 0){
		Isolate *isolate = args.GetIsolate();
		throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
		return;
	}

	++(*ptrWrappedRational->ptrRational);
	args.GetReturnValue().Set(args.This());
}

void addon::WrappedRational::PreDec(const FunctionCallbackInfo<Value>& args){

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());

	if(args.Length() > 0){
		Isolate *isolate = args.GetIsolate();
		throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
		return;
	}

	--(*ptrWrappedRational->ptrRational);
	args.GetReturnValue().Set(args.This());
}

void addon::WrappedRational::PostInc(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
	long n = ptrWrappedRational->ptrRational->numerator();
	long d = ptrWrappedRational->ptrRational->denominator();

	if(args.Length() > 0){
		throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
		return;
	}

	(*ptrWrappedRational->ptrRational)++;

	Local<Function> cons = Local<Function>::New(isolate, constructor);
	int argc = 2;
	Local<Value> argv[] = {
		Number::New(isolate, n),
		Number::New(isolate, d)
	};

	args.GetReturnValue()
		.Set(cons->NewInstance(context, argc, argv)
			.ToLocalChecked()
		);
}

void addon::WrappedRational::PostDec(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
	long n = ptrWrappedRational->ptrRational->numerator();
	long d = ptrWrappedRational->ptrRational->denominator();

	if(args.Length() > 0){
		throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
		return;
	}

	(*ptrWrappedRational->ptrRational)--;

	Local<Function> cons = Local<Function>::New(isolate, constructor);
	int argc = 2;
	Local<Value> argv[] = {
		Number::New(isolate, n),
		Number::New(isolate, d)
	};

	args.GetReturnValue()
		.Set(cons->NewInstance(context, argc, argv)
			.ToLocalChecked()
		);
}

void addon::WrappedRational::SelfNeg(const FunctionCallbackInfo<Value>& args){

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());

	if(args.Length() > 0){
		Isolate *isolate = args.GetIsolate();
		throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
		return;
	}

	*ptrWrappedRational->ptrRational *= -1;

	args.GetReturnValue().Set(args.This());
}

void addon::WrappedRational::SelfAbs(const FunctionCallbackInfo<Value>& args){

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());

	if(args.Length() > 0){
		Isolate *isolate = args.GetIsolate();
		throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
		return;
	}

	if(*ptrWrappedRational->ptrRational < 0L)
		*ptrWrappedRational->ptrRational *= -1;

	args.GetReturnValue().Set(args.This());
}

void addon::WrappedRational::Neg(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());

	if(args.Length() > 0){
		Isolate *isolate = args.GetIsolate();
		throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
		return;
	}

	src::rational<long> retRational = src::operator-(*(ptrWrappedRational->ptrRational));

	Local<Function> cons = Local<Function>::New(isolate, constructor);
	int argc = 2;
	Local<Value> argv[] = {
		Number::New(isolate, retRational.numerator()),
		Number::New(isolate, retRational.denominator())
	};

	args.GetReturnValue()
		.Set(cons->NewInstance(context, argc, argv)
		.ToLocalChecked()
	);
}

void addon::WrappedRational::Abs(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());

	if(args.Length() > 0){
		Isolate *isolate = args.GetIsolate();
		throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
		return;
	}

	src::rational<long> retRational = src::abs(*(ptrWrappedRational->ptrRational));

	Local<Function> cons = Local<Function>::New(isolate, constructor);
	int argc = 2;
	Local<Value> argv[] = {
		Number::New(isolate, retRational.numerator()),
		Number::New(isolate, retRational.denominator())
	};

	args.GetReturnValue()
		.Set(cons->NewInstance(context, argc, argv)
		.ToLocalChecked()
	);
}

void addon::WrappedRational::Not(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());

	if(args.Length() > 0){
		throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
		return;
	}

	Local<Boolean> retval = Boolean::New(isolate, !(*ptrWrappedRational->ptrRational));
	args.GetReturnValue().Set(retval);
}

void addon::WrappedRational::Bool(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());

	if(args.Length() > 0){
		throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
		return;
	}

	Local<Boolean> retval = Boolean::New(isolate, static_cast<bool>(*ptrWrappedRational->ptrRational));
	args.GetReturnValue().Set(retval);
}

void addon::WrappedRational::LessThan(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	switch(args.Length()){
		case 1:
			if(args[0]->IsObject()){

				Local<String> constructor = args[0].As<Object>()->GetConstructorName();
				String::Utf8Value str(isolate, constructor);

				if(std::strcmp(*str, "Rational")){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				Local<Value> numObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "num").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				Local<Value> denObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "den").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				long n = numObj.As<Number>()->Value();
				long d = denObj.As<Number>()->Value();
				src::rational<long> r(n, d);

				WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
				Local<Boolean> retval = Boolean::New(isolate, *ptrWrappedRational->ptrRational < r);

				args.GetReturnValue().Set(retval);
			}
			else
			if(args[0]->IsNumber()){

				double value = args[0].As<Number>()->Value();
				if(std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;
				src::rational<long> r(n, d);

				WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
				Local<Boolean> retval = Boolean::New(isolate, *ptrWrappedRational->ptrRational < r);

				args.GetReturnValue().Set(retval);
			}
			else
			if(args[0]->IsBigInt()){
				throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
				return;
			}
			else
			if(args[0]->IsString()){

				String::Utf8Value str(isolate, args[0]);
				char *end = NULL;

				double value = std::strtol(*str, &end, 10);
				if(*str == end || std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;
				src::rational<long> r(n, d);

				WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
				Local<Boolean> retval = Boolean::New(isolate, *ptrWrappedRational->ptrRational < r);

				args.GetReturnValue().Set(retval);
			}
			else{
				throwException(isolate, Exception::TypeError, "Rational: invalid argument");
				return;
			}
		break;

		default:
			throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
			return;
	}
}

void addon::WrappedRational::GreaterThan(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	switch(args.Length()){
		case 1:
			if(args[0]->IsObject()){

				Local<String> constructor = args[0].As<Object>()->GetConstructorName();
				String::Utf8Value str(isolate, constructor);

				if(std::strcmp(*str, "Rational")){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				Local<Value> numObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "num").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				Local<Value> denObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "den").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				long n = numObj.As<Number>()->Value();
				long d = denObj.As<Number>()->Value();
				src::rational<long> r(n, d);

				WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
				Local<Boolean> retval = Boolean::New(isolate, *ptrWrappedRational->ptrRational > r);

				args.GetReturnValue().Set(retval);
			}
			else
			if(args[0]->IsNumber()){

				double value = args[0].As<Number>()->Value();
				if(std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;
				src::rational<long> r(n, d);

				WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
				Local<Boolean> retval = Boolean::New(isolate, *ptrWrappedRational->ptrRational > r);

				args.GetReturnValue().Set(retval);
			}
			else
			if(args[0]->IsBigInt()){
				throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
				return;
			}
			else
			if(args[0]->IsString()){

				String::Utf8Value str(isolate, args[0]);
				char *end = NULL;

				double value = std::strtol(*str, &end, 10);
				if(*str == end || std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;
				src::rational<long> r(n, d);

				WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
				Local<Boolean> retval = Boolean::New(isolate, *ptrWrappedRational->ptrRational > r);

				args.GetReturnValue().Set(retval);
			}
			else{
				throwException(isolate, Exception::TypeError, "Rational: invalid argument");
				return;
			}
		break;

		default:
			throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
			return;
	}
}

void addon::WrappedRational::EqualTo(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	switch(args.Length()){
		case 1:
			if(args[0]->IsObject()){

				Local<String> constructor = args[0].As<Object>()->GetConstructorName();
				String::Utf8Value str(isolate, constructor);

				if(std::strcmp(*str, "Rational")){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				Local<Value> numObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "num").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				Local<Value> denObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "den").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				long n = numObj.As<Number>()->Value();
				long d = denObj.As<Number>()->Value();
				src::rational<long> r(n, d);

				WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
				Local<Boolean> retval = Boolean::New(isolate, *ptrWrappedRational->ptrRational == r);

				args.GetReturnValue().Set(retval);
			}
			else
			if(args[0]->IsNumber()){

				double value = args[0].As<Number>()->Value();
				if(std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;
				src::rational<long> r(n, d);

				WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
				Local<Boolean> retval = Boolean::New(isolate, *ptrWrappedRational->ptrRational == r);

				args.GetReturnValue().Set(retval);
			}
			else
			if(args[0]->IsBigInt()){
				throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
				return;
			}
			else
			if(args[0]->IsString()){

				String::Utf8Value str(isolate, args[0]);
				char *end = NULL;

				double value = std::strtol(*str, &end, 10);
				if(*str == end || std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;
				src::rational<long> r(n, d);

				WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
				Local<Boolean> retval = Boolean::New(isolate, *ptrWrappedRational->ptrRational == r);

				args.GetReturnValue().Set(retval);
			}
			else{
				throwException(isolate, Exception::TypeError, "Rational: invalid argument");
				return;
			}
		break;

		default:
			throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
			return;
	}
}

void addon::WrappedRational::NotEqualTo(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	/* Context of the currently running JavaScript instance. */
	Local<Context> context = isolate->GetCurrentContext();

	switch(args.Length()){
		case 1:
			if(args[0]->IsObject()){

				Local<String> constructor = args[0].As<Object>()->GetConstructorName();
				String::Utf8Value str(isolate, constructor);

				if(std::strcmp(*str, "Rational")){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				Local<Value> numObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "num").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				Local<Value> denObj = args[0].As<Object>()->Get(
					context,
					String::NewFromUtf8(
						isolate, "den").ToLocalChecked()
					).ToLocalChecked(); /* Converts this MaybeLocal<> to a Local<>. */

				long n = numObj.As<Number>()->Value();
				long d = denObj.As<Number>()->Value();
				src::rational<long> r(n, d);

				WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
				Local<Boolean> retval = Boolean::New(isolate, !(*ptrWrappedRational->ptrRational == r));

				args.GetReturnValue().Set(retval);
			}
			else
			if(args[0]->IsNumber()){

				double value = args[0].As<Number>()->Value();
				if(std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;
				src::rational<long> r(n, d);

				WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
				Local<Boolean> retval = Boolean::New(isolate, !(*ptrWrappedRational->ptrRational == r));

				args.GetReturnValue().Set(retval);
			}
			else
			if(args[0]->IsBigInt()){
				throwException(isolate, Exception::TypeError, "Rational: BigInt type is not accepted");
				return;
			}
			else
			if(args[0]->IsString()){

				String::Utf8Value str(isolate, args[0]);
				char *end = NULL;

				double value = std::strtol(*str, &end, 10);
				if(*str == end || std::isnan(value)){
					throwException(isolate, Exception::TypeError, "Rational: invalid argument");
					return;
				}

				long n = value;
				long d = 1L;
				src::rational<long> r(n, d);

				WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());
				Local<Boolean> retval = Boolean::New(isolate, !(*ptrWrappedRational->ptrRational == r));

				args.GetReturnValue().Set(retval);
			}
			else{
				throwException(isolate, Exception::TypeError, "Rational: invalid argument");
				return;
			}
		break;

		default:
			throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
			return;
	}
}

void addon::WrappedRational::ValueOf(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());

	if(args.Length() > 0){
		throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
		return;
	}

	Local<Number> retval = Number::New(isolate, src::rational_cast<double, long>(*ptrWrappedRational->ptrRational));

	args.GetReturnValue().Set(retval);
}

void addon::WrappedRational::ToString(const FunctionCallbackInfo<Value>& args){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = args.GetIsolate();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(args.Holder());

	if(args.Length() > 0){
		throwException(isolate, Exception::TypeError, "Rational: invalid number of arguments");
		return;
	}

	std::ostringstream ss;

	ss.tie( NULL );
	ss.exceptions( std::ios::goodbit );
	ss.width( 0 );
	ss << std::noshowpos << std::noshowbase << '/' << ptrWrappedRational->ptrRational->denominator();

	const std::string tail = ss.str();
	std::streamsize const w =
		ss.width() - static_cast<std::streamsize>( tail.size() );

	ss.clear();
	ss.str( "" );
	ss << std::setw( w < 0 || (ss.flags() & std::ios::adjustfield) !=
					 std::ios::internal ? 0 : w ) << ptrWrappedRational->ptrRational->numerator();

	std::string s = ss.str() + tail;
	Local<String> retval = String::NewFromUtf8(isolate, s.c_str()).ToLocalChecked();

	args.GetReturnValue().Set(retval);
}

void addon::WrappedRational::GetField(Local<String> property, const PropertyCallbackInfo<Value>& info){

	/* Pointer to v8::Isolate object that represents the v8 instance itself. */
	Isolate *isolate = info.GetIsolate();

	WrappedRational *ptrWrappedRational = ObjectWrap::Unwrap<WrappedRational>(info.This());

	String::Utf8Value s(isolate, property);
	std::string str(*s, s.length());

	if(str == "num"){
		info.GetReturnValue().Set(Number::New(isolate, ptrWrappedRational->ptrRational->numerator()));
	}
	else
	if(str == "den"){
		info.GetReturnValue().Set(Number::New(isolate, ptrWrappedRational->ptrRational->denominator()));
	}
}
