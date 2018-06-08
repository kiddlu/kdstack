OS_NAME = $(shell uname -o)
LC_OS_NAME = $(shell echo $(OS_NAME) | tr '[A-Z]' '[a-z]')

ifeq ($(LC_OS_NAME), cygwin)
	CUR_OS = "current os is cygwin"
else
	CUR_OS = "current os not cygwin"
endif

checkos:
	@echo "$(CUR_OS)"
	@echo "$(LC_OS_NAME)"
