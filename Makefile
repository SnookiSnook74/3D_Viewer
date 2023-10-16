CC=gcc
WAY=./logic/
OS=$(shell uname)
WILD=$(wildcard *.c)
WILD_SORT=$(shell find . -name "*.c" ! -name "*test*")
SORT=$(shell find . -name "*.c")
FLAGS=-Wall -Werror -Wextra
VERSION = 1.0
PACKAGE = s21_3Dviwer-$(VERSION)
ifeq ($(OS), Linux)
  FLAGS_LCHECK = -lcheck -pthread -lsubunit -lrt -lm
  OPEN=xdg-open
  START=./build/s21_3Dviwer
else
  FLAGS_LCHECK = -lcheck
  OPEN=open
  START=open ./build/s21_3Dviwer.app
endif
FLAGS_GCOV = -coverage -fprofile-arcs -ftest-coverage


all: clean test

install: clean
	mkdir build
	#cp logic/*.c gui && cp logic/*.h gui
	cd build/ && qmake ../frontend/s21_3Dviwer.pro && make
	$(START)

uninstall: clean

dvi: clean
	mkdir -p docs
	doxygen Doxyfile
	@mv html latex docs
	open docs/html/index.html

dist:
	mkdir $(PACKAGE)
	cp -r ./backend $(PACKAGE)
	cp -r ./frontend $(PACKAGE)
	cp Makefile $(PACKAGE)
	tar -czf $(PACKAGE).tar.gz $(PACKAGE)
	rm -rf $(PACKAGE)

.PHONY: test
test: clean s21_3Dviwer.a
	$(CC) ./backend/test.c $(FLAGS) $(FLAGS_LCHECK) s21_3Dviwer.a -o test

.PHONY: gcov_report
gcov_report: s21_3Dviwer.a
		$(CC) $(SORT) $(FLAGS)  \
		$(FLAGS_LCHECK) s21_3Dviwer.a $(FLAGS_GCOV) \
		$(WILD_REPORT) -o info
	./info
	lcov -t "info" -o gcovreport.info -c -d .
	genhtml -o report gcovreport.info
	$(OPEN) report/index.html

s21_3Dviwer.a:
	$(CC) -c $(WILD_SORT)
	ar -rcs $@ *.o
	ranlib $@
	rm -rf *.o

.PHONY: docker
docker:
	docker rmi -f string && docker build -t string . && docker run  --name unit string
	# docker cp unit:/hello/report .
	docker rm unit

.PHONY: cppcheck
cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h

style:
	clang-format -i */*.c
	clang-format -i */*.h
	clang-format -i */*.cpp

clean_no_all:
	rm -rf *.o *.gcda *.gcno *.info
	rm -f info

clean:
	rm -rf build/
	rm -rf docs
	rm -rf build-s21*
	rm -rf frontend/*.c
	rm -rf *.o *.gcda *.gcno *.info
	rm -f info
	rm -f *.a
	rm -rf report
	rm -f test

.PHONY: git
git: style
	rm -rf report
	rm -rf build-s21*
	git add .
	git commit -m "commit"
	git push

valgrind:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./test