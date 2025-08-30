import addon from '@essamonline/nodecpp-rational';
const { Rational } = addon;

console.log(`index.mjs: Demonstrates usage of Rational class`);
console.log('===============================================');

const zero  = new Rational();
const one   = new Rational(1);
const two   = new Rational(2);
const half  = new Rational(1, 2);
const mhalf = new Rational(-1, 2);

console.log(`zero : ${zero}`);
console.log(`one  : ${one}`);
console.log(`two  : ${two}`);
console.log(`half : ${half}`);
console.log(`mhalf: ${mhalf}`);

console.log();

console.log(`one   +  one   = two  : ${one} + ${one} = ${one.add(one)}`);
console.log(`one   -  half  = half : ${one} - ${half} = ${one.sub(half)}`);
console.log(`two   *  half  = one  : ${two} * ${half} = ${two.mul(half)}`);
console.log(`one   /  half  = two  : ${one} / ${half} = ${one.div(half)}`);
console.log(`two   ** zero  = one  : ${two} ** ${zero} = ${two.pow(0)}`);
console.log(`mhalf /  mhalf = one  : ${mhalf} / ${mhalf} = ${mhalf.div(mhalf)}`);
