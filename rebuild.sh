rm -fr build/libs && ./gradlew jar && ./gradlew exampleJar && ./gradlew build -x test
echo "Warning: test is skipped."
