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
	├── binding.gyp
	├── demo
	│   ├── index.js
	│   ├── index.mjs
	│   └── package.json
	├── docs
	│   ├── coverage
	│   │   ├── cpp
	│   │   │   ├── html
	│   │   │   └── lcov.info
	│   │   └── js
	│   │       ├── html
	│   │       └── lcov.info
	│   └── src
	│       ├── cpp
	│       └── js
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


