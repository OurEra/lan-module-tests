find -name "*.java" > sources.txt
javac @sources.txt -cp libs/json-20200518.jar

# run
java com.srw.javatest.TestEntry

