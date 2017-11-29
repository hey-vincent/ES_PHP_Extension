PHP_INCLUDE = `php-config --includes`
PHP_LIBS = `php-config --libs`
PHP_LDFLAGS = `php-config --ldflags`
PHP_INCLUDE_DIR = `php-config --include-dir`
PHP_EXTENSION_DIR = `php-config --extension-dir`
PHP_INI = ``


wensheng.so: test.cpp
	g++  test.cpp -lphpx ${PHP_INCLUDE} -I${PHP_INCLUDE_DIR} -std=c++11 -shared -undefined dynamic_lookup  -o wensheng.so
install: wensheng.so
	mv wensheng.so ${PHP_EXTENSION_DIR}/

clean:
	rm *.so
