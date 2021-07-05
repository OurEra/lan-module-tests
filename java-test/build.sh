find ./ -name "*.java" > sources.txt
javac @sources.txt -cp `pwd`:libs/json-20200518.jar:libs/commons-codec-1.15.jar

# run
java -cp `pwd`:libs/commons-codec-1.15.jar:libs/json-20200518.jar com.srw.javatest.TestEntry

