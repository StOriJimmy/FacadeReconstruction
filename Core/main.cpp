
#include "PolyGTM.h"

double data[10]={0.8884373299935291,0.2078277839120586,0.1411772438154148,0.0110670701815004,0.2243877701379398,0.0285283331680854,0.7818396238324006,0.9470385632632157,0.4370904789001863,0.9051133241835381};
int main(int argv, char** arg)
{
    (void) argv;
    (void) arg;
    Cu::Matrix dat = Core::ConvertToEigen(data,2,5);
    Core::Input in{ data, 2, 5, 2, 2, 1, 2};
    Core::Output o = Core::CreateStartPointUsingPCA(in);
    std::cout << "Start Point: " << std::endl ;
    std::cout << "Phi" << o.Phi <<std::endl;
    std::cout << "Pi" << o.Pi <<std::endl;
    std::cout << "W" << o.W <<std::endl;

    std::cout << o.W*o.Phi <<std::endl;

    Core::Output ogt = Core::Polynomial(in);
    std::cout << std::endl << "GTM: " << std::endl ;
    std::cout << "Phi" << ogt.Phi <<std::endl;
    std::cout << "Pi" << ogt.Pi <<std::endl;
    std::cout << "W" << ogt.W <<std::endl;

    std::cout << o.W*o.Phi <<std::endl;
}
