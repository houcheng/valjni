export CC=clang
export CXX=clang++

rm -fr build/
mkdir -p build
cd build
# cmake -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE -DCMAKE_ECLIPSE_MAKE_ARGUMENTS=-j8 ../jnisvr
cmake -DCMAKE_BUILD_TYPE=Debug  ..
make -j4