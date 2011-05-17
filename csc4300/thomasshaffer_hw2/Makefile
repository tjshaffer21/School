USER=root
PASSWORD=friendship
DATABASE=homework
HOST=localhost
JAVA_MYSQL=/usr/share/java/mysql-connector-java.jar
CS_MYSQL=/usr/lib/cli/mysql-connector-net-5.0/MySql.Data.dll

.PHONY: program1
.PHONY: program2
.PHONY: program3
.PHONY: program4
.PHONY: program5

program1:
	tclsh program1/program1.tcl ${HOST} ${USER} ${DATABASE} ${PASSWORD}

program2:
	ruby program2/program2.rb $(HOST) $(USER) $(DATABASE) $(PASSWORD)

program3:
	python program3/program3.py $(HOST) $(USER) $(DATABASE) $(PASSWORD)

program4:
	javac -cp ${JAVA_MYSQL}:./ `pwd`/program4/program4.java
	cd program4;java -cp ${JAVA_MYSQL}:./ program4 ${HOST} ${USER} ${DATABASE} ${PASSWORD}

program5:
	cd program5;gmcs program5.cs -r:System.Data.dll -r:MySql.Data.dll
	cd program5;mono program5.exe ${HOST} ${USER} ${DATABASE} ${PASSWORD}

clean:
	rm program4/program4.class
	rm program5/program5.exe
