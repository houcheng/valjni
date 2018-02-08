cd src; make clean; cd ..

rm -f CMakeCache.txt
rm -f Makefile
rm -f *.cmake
rm -fr CMakeFiles
rm -fr build/

find ./src -iwholename '*cmake*' -not -name CMakeLists.txt -delete
find ./src -iwholename '*.o' -not -name CMakeLists.txt -delete
