# nodecpp-rational

A Node.js native C++ addon that defines a JavaScript class 'Rational' for representing and manipulating rational numbers.

## Installation

```
npm install [-g] @essamonline/nodecpp-rational
```

## Structure

```
/nodecpp-rational
	│
	├── binding.gyp                 # binding file
	├── demo                        # a package for demonstrating usage of the package
	│   ├── index.js
	│   ├── index.mjs
	│   └── package.json
	├── docs
	│   ├── coverage                # coverage reports
	│   │   ├── cpp                 # coverage reports for C++ code
	│   │   │   ├── html
	│   │   │   └── lcov.info
	│   │   └── js                  # coverage reports for JavaScript code
	│   │       ├── html
	│   │       └── lcov.info
	│   └── src                     # source code documentation
	│       ├── cpp                 # source code documentation using doxygen
	│       └── js                  # source code documentation using jsdoc
	├── include
	│   └── rational.h
	├── lib
	│   └── index.js
	├── LICENSE
	├── package.json
	├── README.md
	├── src
	│   ├── addon.cpp
	│   ├── rational-addon.cpp
	│   └── rational-addon.h
	└── test
	    ├── makefile
	    ├── rational.module.js
	    ├── rational.test.cpp
	    └── rational.test.js

```


## License

This software is licensed under the MIT license, see the [LICENSE](./LICENSE "LICENSE") file.


