clang++ -std=c++11 -Wc++11-extensions -I/usr/include/mysql -I/usr/include/soci -lsoci_core -lsoci_mysql -lboost_regex -ldl -o blog -g blog.cpp template.cpp ../utils.cpp
