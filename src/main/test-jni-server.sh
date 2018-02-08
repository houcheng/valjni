echo "Run this in build"
cd ..

echo "#########################################################################"
build/jni-server < test_float.dat

echo "#########################################################################"
build/jni-server < test_int.dat
