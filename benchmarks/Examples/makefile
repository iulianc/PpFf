.IGNORE:

def: students_java

students_java: StudentsExample.class
	java -cp . StudentsExample

StudentsExample.class: StudentsExample.java
	javac StudentsExample.java

clean: 
	\rm -f *.class
	\rm -f *.o *~

cleanx cleanall cleanxtra: clean

