PHP_INCLUDE = `php-config --includes`
PHP_LIBS = `php-config --libs`
PHP_LDFLAGS = `php-config --ldflags`
PHP_INCLUDE_DIR = `php-config --include-dir`
PHP_EXTENSION_DIR = `php-config --extension-dir`
PHP_INI = ``

wensheng.so: wensheng.cpp Http.h Http.cpp
	c++ -DHAVE_CONFIG_H -g -o wensheng.so -O0 -fPIC -shared -undefined dynamic_lookup wensheng.cpp Http.cpp -std=c++11 -lcurl -lphpx ${PHP_INCLUDE} -I${PHP_INCLUDE_DIR}

install: wensheng.so
	mv wensheng.so ${PHP_EXTENSION_DIR}/

clean:
	rm *.so
