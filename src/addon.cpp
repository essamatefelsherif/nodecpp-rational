/**
 * @file    addon.cpp
 * @version v1.0.0
 * @author  Essam A. El-Sherif
 */

#include "rational-addon.h"

void addon::Init(Local<Object> exports, Local<Value>, void*){
	WrappedRational::Init(exports);
}

/*
 * C++ macro defined by 'node.h'
 *
 * This macro creates code that will (when loaded by Node.js)
 * register a module named 'rational' within the JavaScript
 * context (V8) ... bindings.gyp => targets.target_name
 *
 * In addition, it will ensure that a function called 'addon::Init'
 * is called when the module is required.
 */
NODE_MODULE(rational, addon::Init)
