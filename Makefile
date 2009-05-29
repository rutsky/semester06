# Makefile
# Some common tasks for whole project.
# Vladimir Rutsky <altsysrq@gmail.com>
# 20.05.2009

all:
	@echo "Read Makefile contents for details of usage."

public:
	git push git@github.com:rutsky/semester06.git
	git push git@gitorious.org:spbstu/semester06.git

clean-light:
	find . -name '*.o' -exec rm '{}' \;
	find . -wholename '*.gch/c++' -exec rm '{}' \;
