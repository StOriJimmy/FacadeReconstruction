eiglib:=../libs/Eigen/Eigen/

all: standard

standard: oct zbuffer.cpp zbufferoct.cpp
	LFLAGS="`mkoctfile -p LFLAGS` -Ofast" CXXFLAGS="`mkoctfile -p CXXFLAGS` -Ofast -std=c++11" mkoctfile zbuffer.cpp zbufferoct.cpp -o zbufferoct.oct -I$(eiglib) -v
	mv *.o oct
	mv *.oct oct

oct:
	mkdir oct

clean:
	rm -rf oct
