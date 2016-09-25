


ALL=sortmon

all: $(ALL) 

clean:
	rm -vf  $(ALL)

%: %.cpp
	g++ -g $< -o $@ -std=c++11 

doc:	sortmon.cpp
	doxygen $<
