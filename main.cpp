#include <iostream>
#include <cstdint>

#include "BigNumber.h"

using namespace std;

int main() {
    cout << boolalpha;

//    for(unsigned char i{0}; i <= 127; ++i){
//        std::cout << (int) i << '\t' << i << std::endl;
//    }
    //std::cout << x << std::endl;
//    BigNumber x{-37};
//    string str = "-0000";
//    BigNumber y{str};
//    cout << y << endl;
//    cout << "Enter a number: ";
//    cin >> x;
//    x = -y;
//    BigNumber z = -y;
//    cout << (x != y) << endl;
    BigNumber myBig1 = -17319;
    BigNumber myBig2 = "0000173";
    //cout << BigNumber::unsignedLessOrEqual(myBig1, myBig2) << endl;
    //BigNumber a = BigNumber::unsignedSubtract(myBig1, myBig2);
    //   cout << (myBig1>>3) << "\t" << (myBig1>>3).getSign() << endl;
//    try{
//        cout << BigNumber::unsignedSubtract(myBig1, myBig2) << endl;
//    }
//    catch( out_of_range e){
//        cout << e.what() << endl;
//    }
    BigNumber num1 = "1234567890";
    BigNumber num2 = "123456789";
    BigNumber num3=num1/num2;
    cout << num3;
    /* BigNumber num3 = num1(num1.getNumOfDigits()-1, num2.getNumOfDigits());
     num1 += num2;
     BigNumber num4 = "15728839";
     BigNumber num5=num4(6,3);
     BigNumber num6=num4(7,4);*/
    // num1 << 5;
    //  cout << num3<<endl;
    // cout << num2<<endl;
    //cout << num5<<endl;
    // cout << num6<<endl;

    return 0;
}

