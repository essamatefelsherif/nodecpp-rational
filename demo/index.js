const { Rational } = require('@essamonline/nodecpp-rational');

console.log(`index.js: Demonstrates usage of Rational class`);
console.log('==============================================');

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

console.log(`one   +  one   = two  : ${one} + ${one} = ${new Rational(1).add(one)}`);
console.log(`one   -  half  = half : ${one} - ${half} = ${new Rational(1).sub(half)}`);
console.log(`two   *  half  = one  : ${two} * ${half} = ${new Rational(2).mul(half)}`);
console.log(`one   /  half  = two  : ${one} / ${half} = ${new Rational(1).div(half)}`);
console.log(`two   ** zero  = one  : ${two} ** ${zero} = ${new Rational(2).pow(0)}`);
console.log(`mhalf /  mhalf = one  : ${mhalf} / ${mhalf} = ${new Rational(-1, 2).div(mhalf)}`);

console.log();
