USER=
PASS=
DATABASE=
HOST=

.PHONY: install
.PHONY: run

install:
	mysql -h ${HOST} -u ${USER} -p -D ${DATABASE} < sql_scripts/main.sql

run:
	ruby main.rb ${HOST} ${USER} ${DATABASE} ${PASS}
