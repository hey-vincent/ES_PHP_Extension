PHP_INCLUDE = `php-config --includes`
PHP_LIBS = `php-config --libs`
PHP_LDFLAGS = `php-config --ldflags`
PHP_INCLUDE_DIR = `php-config --include-dir`
PHP_EXTENSION_DIR = `php-config --extension-dir`
PHP_INI = ``

# c++ -DHAVE_CONFIG_H -g -o ElasticSearchClient.so -O0 -fPIC -shared -undefined dynamic_lookup wensheng.cpp Http.cpp -std=c++11 -lcurl -L/usr/local/lib/ -lphpx ${PHP_INCLUDE} -I${PHP_INCLUDE_DIR}
# -I /usr/local/include

ElasticSearchClient.so: wensheng.cpp Http.h Http.cpp common.h
	g++  wensheng.cpp Http.cpp -lcurl -lphpx ${PHP_INCLUDE} -I${PHP_INCLUDE_DIR} -std=c++11 -shared -undefined dynamic_lookup  -o ElasticSearchClient.so
install: ElasticSearchClient.so
	mv ElasticSearchClient.so ${PHP_EXTENSION_DIR}/

clean:
	rm *.so
