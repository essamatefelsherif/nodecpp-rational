CC    = g++
CFLAG = -fPIC -fprofile-arcs -ftest-coverage
RM    = rm -rf

help: ## Makefile help
	@awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z_-]+:.*?## / {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}' $(MAKEFILE_LIST)

rational.test.o: rational.test.cpp
	$(CC) $(CFLAG) -c -Wall rational.test.cpp

build: rational.test.o ## Make build
	$(CC) $(CFLAG) -c -Wall rational.test.cpp
	$(CC) $(CFLAG) -o rational.test.out rational.test.o

coverage: ## Run code coverage
	gcov rational.test.cpp

lcov-report: coverage ## Generate lcov report
	mkdir -p ../docs/coverage/cpp/html
	lcov --capture --directory . --output-file ../docs/coverage/cpp/lcov.info
	lcov --remove ../docs/coverage/cpp/lcov.info '/usr/*' '/opt/*' --output-file ../docs/coverage/cpp/lcov.info
	genhtml ../docs/coverage/cpp/lcov.info --output-directory ../docs/coverage/cpp/html

clean: ## Clean all generated files
	$(RM) *.out *.o *.so *.gcno *.gcda *.gcov ../docs/coverage/cpp

clean-int: ## Clean intermediate generated files
	$(RM) *.out *.o *.so *.gcno *.gcda *.gcov
