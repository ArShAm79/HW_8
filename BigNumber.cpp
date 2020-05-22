#include "BigNumber.h"
#include <stdexcept>
#include <iostream>
#include <string>

using namespace std;

BigNumber::BigNumber(const std::string &str) {
    setValues(str);
}

BigNumber::BigNumber(const long &intNum) {
    sign = (intNum >= 0);

    long temp1 = abs(intNum);
    numOfDigits = 0;
    while (temp1 > 0) {
        ++numOfDigits;
        temp1 /= 10;
    }

    numArray = new int8_t[numOfDigits];
    long temp2 = abs(intNum);
    for (size_t i{0}; i < numOfDigits; ++i) {
        numArray[i] = temp2 % 10;
        temp2 /= 10;
    }
}

BigNumber::BigNumber(const BigNumber &myBig) {
    sign = myBig.sign;
    numOfDigits = myBig.numOfDigits;
    numArray = new int8_t[numOfDigits];
    for (size_t i{0}; i < numOfDigits; ++i) {
        numArray[i] = myBig.numArray[i];
    }
}

BigNumber::~BigNumber() {
    delete[] numArray;
}


unsigned BigNumber::numOfTrimCharsOnLeft(const std::string &str) {
    unsigned numOfChars = 0;
    size_t i = 0;
    while (i < str.size() - 1 && str[i] == '0' || str[i] == '-' || str[i] == '+') {
        ++numOfChars;
        ++i;
    }
    return numOfChars;
}

void BigNumber::setValues(const std::string &str) {
    if (!validate(str, "[+-]?[0-9]+")) {
        throw invalid_argument("A number must include just decimal digits and + or -");
    }

    sign = (str[0] != '-');

    numOfDigits = str.size() - numOfTrimCharsOnLeft(str);

    delete[] numArray;
    numArray = new int8_t[numOfDigits];
    for (size_t i{0}; i < numOfDigits; ++i) {
        numArray[i] = str[str.size() - 1 - i] - 48;
    }
    if (numOfDigits == 1 && numArray[0] == 0) {
        sign = true;
    }
}

bool BigNumber::getSign() const {
    return sign;
}

unsigned int BigNumber::getNumOfDigits() const {
    return numOfDigits;
}

bool BigNumber::validate(const std::string &str, const std::string &pattern) {
    regex regPattern(pattern);
    return regex_match(str, regPattern);
}

std::ostream &operator<<(std::ostream &output, const BigNumber &myBig) {
    if (myBig.sign == false) {
        output << '-';
    }
    for (int i = myBig.numOfDigits - 1; i >= 0; --i) {
        output << (int8_t) (myBig.numArray[i] + 48);
    }
    return output;
}

std::istream &operator>>(std::istream &input, BigNumber &myBig) {
    string str;
    input >> str;
    myBig.setValues(str);
    return input;
}

BigNumber &BigNumber::operator=(const BigNumber &rightNum) {
    // x = y;
    if (&rightNum != this) {
        sign = rightNum.sign;
        numOfDigits = rightNum.numOfDigits;
        delete[] numArray;
        numArray = new int8_t[numOfDigits];
        for (size_t i{0}; i < numOfDigits; ++i) {
            numArray[i] = rightNum.numArray[i];
        }
    }
    return *this;
}

BigNumber BigNumber::operator-() const {
    BigNumber temp;
    temp.sign = !sign;
    temp.numOfDigits = numOfDigits;
    temp.numArray = new int8_t[numOfDigits];
    for (size_t i{0}; i < numOfDigits; ++i) {
        temp.numArray[i] = numArray[i];
    }
    return temp;
}

int8_t &BigNumber::operator[](size_t index) {
    if (index < 0 || index >= numOfDigits) {
        throw out_of_range("The index is out of range.");
    }
    return numArray[index];
}

int BigNumber::operator[](size_t index) const {
    if (index < 0 || index >= numOfDigits) {
        throw out_of_range("The index is out of range.");
    }
    return numArray[index];
}

bool BigNumber::operator==(const BigNumber &myBig) const {
    if (sign != myBig.sign) {
        return false;
    }
    if (numOfDigits != myBig.numOfDigits) {
        return false;
    }
    for (size_t i{0}; i < numOfDigits; ++i) {
        if ((*this)[i] != myBig[i]) {
            return false;
        }
    }
    return true;
}

bool BigNumber::operator!=(const BigNumber &myBig) const {
    return !(*this == myBig);
}

BigNumber::BigNumber(BigNumber &&myBig) noexcept
        : sign{myBig.sign}, numOfDigits{myBig.numOfDigits}, numArray{myBig.numArray} {
    myBig.numArray = nullptr;
}

BigNumber &BigNumber::operator=(BigNumber &&rightNum) noexcept {
    if (&rightNum != this) {
        sign = rightNum.sign;
        numOfDigits = rightNum.numOfDigits;
        delete[] numArray;
        numArray = rightNum.numArray;
        rightNum.numArray = nullptr;
    }
    return *this;
}

BigNumber BigNumber::unsignedMax(const BigNumber &num1, const BigNumber &num2) {
    if (num1.numOfDigits > num2.numOfDigits) {
        return num1;
    }

    if (num1.numOfDigits < num2.numOfDigits) {
        return num2;
    }

    size_t index = num1.numOfDigits - 1;
    while ((num1[index] == num2[index]) && (index > 0)) {
        --index;
    }
    if (num1[index] >= num2[index]) {
        return num1;
    }
    if (num1[index] < num2[index]) {
        return num2;
    }
}

BigNumber BigNumber::unsignedMin(const BigNumber &num1, const BigNumber &num2) {
    if (num1.numOfDigits > num2.numOfDigits) {
        return num2;
    }

    if (num1.numOfDigits < num2.numOfDigits) {
        return num1;
    }

    size_t index = num1.numOfDigits - 1;
    while ((num1[index] == num2[index]) && (index > 0)) {
        --index;
    }
    if (num1[index] >= num2[index]) {
        return num2;
    }
    if (num1[index] < num2[index]) {
        return num1;
    }
}

bool BigNumber::unsignedGreaterOrEqual(const BigNumber &num1, const BigNumber &num2) {
    return (unsignedMax(num1, num2) == num1);
}

bool BigNumber::unsignedLessOrEqual(const BigNumber &num1, const BigNumber &num2) {
    return (unsignedMin(num2, num1) == num1);
}

bool BigNumber::operator>=(const BigNumber &myBig) const {
    if (sign == true && myBig.sign == false) {
        return true;
    }

    if (sign == false && myBig.sign == true) {
        return false;
    }

    if (sign == true && myBig.sign == true) {
        return unsignedGreaterOrEqual(*this, myBig);
    }

    if (sign == false && myBig.sign == false) {
        return unsignedLessOrEqual(*this, myBig);
    }
}

bool BigNumber::operator<=(const BigNumber &myBig) const {
    if (sign == true && myBig.sign == false) {
        return false;
    }

    if (sign == false && myBig.sign == true) {
        return true;
    }

    if (sign == true && myBig.sign == true) {
        return unsignedLessOrEqual(*this, myBig);
    }

    if (sign == false && myBig.sign == false) {
        return unsignedGreaterOrEqual(*this, myBig);
    }
}

bool BigNumber::operator>(const BigNumber &myBig) const {
    return !(*this <= myBig);
}

bool BigNumber::operator<(const BigNumber &myBig) const {
    return !(*this >= myBig);
}

BigNumber BigNumber::absoluteValue() const {
    BigNumber temp;
    temp.sign = true;
    temp.numOfDigits = numOfDigits;
    temp.numArray = new int8_t[numOfDigits];
    for (size_t i{0}; i < numOfDigits; ++i) {
        temp.numArray[i] = numArray[i];
    }
    return temp;
}

BigNumber BigNumber::unsignedAdd(const BigNumber &num1, const BigNumber &num2) {
    BigNumber bMax = unsignedMax(num1, num2);
    BigNumber bMin = unsignedMin(num1, num2);

    BigNumber sum;

    sum.sign = true;
    sum.numOfDigits = bMax.numOfDigits + 1;
    sum.numArray = new int8_t[sum.numOfDigits];
    size_t i{0};
    int8_t carry = 0;
    int8_t s;

    for (; i < bMin.numOfDigits; ++i) {
        s = bMax[i] + bMin[i] + carry;
        sum[i] = s % 10;
        carry = s / 10;
    }

    for (; i < bMax.numOfDigits; ++i) {
        s = bMax[i] + carry;
        sum[i] = s % 10;
        carry = s / 10;
    }

    if (carry == 1) {
        sum[i] = 1;
    } else if (carry == 0) {
        //sum[i] = 0;
        sum.numOfDigits -= 1;
    }

    return sum;
}

BigNumber BigNumber::unsignedSubtract(const BigNumber &num1, const BigNumber &num2) {
    BigNumber bMax = unsignedMax(num1, num2);
    BigNumber bMin = unsignedMin(num1, num2);
    int8_t *nArray = new int8_t[bMax.numOfDigits]{};
    size_t i{0};

    ;
    for (; i < bMin.numOfDigits; ++i) {
        if (bMax[i] >= bMin[i]) {
            nArray[i] = bMax[i] - bMin[i];
        } else if (bMax[i] < bMin[i] && bMax[i + 1] != 0) {
            nArray[i] = 10 + bMax[i] - bMin[i];
            bMax[i + 1] -= 1;
        } else if (bMax[i] < bMin[i] && bMax[i + 1] == 0) {
            size_t j = i;
            while (bMax[j + 1] == 0) {
                bMax[j + 1] = 9;
                ++j;
            }
            bMax[j + 1] -= 1;
            nArray[i] = 10 + bMax[i] - bMin[i];
        }
    }

    for (; i < bMax.numOfDigits; ++i) {
        nArray[i] = bMax[i];
    }

    int numOfZerosOnTheLeft = 0;
    size_t index{bMax.numOfDigits - 1};
    while (nArray[index] == 0 && index > 0) {
        ++numOfZerosOnTheLeft;
        --index;
    }

    BigNumber sub;
    sub.sign = true;
    sub.numOfDigits = bMax.numOfDigits - numOfZerosOnTheLeft;
    sub.numArray = new int8_t[sub.numOfDigits];

    for (size_t i{0}; i < sub.numOfDigits; ++i) {
        sub[i] = nArray[i];
    }
    return sub;
}

BigNumber operator+(const BigNumber &num1, const BigNumber &num2) {
    BigNumber sum;
    if (num1.sign == num2.sign) {   // -5 + 5
        sum = BigNumber::unsignedAdd(num1, num2);
        sum.sign = num1.sign;
    } else {
        sum = BigNumber::unsignedSubtract(num1, num2);
        sum.sign = BigNumber::unsignedMax(num1, num2).sign;
    }
    if (sum.numOfDigits == 1 && sum[0] == 0) {
        sum.sign = true;
    }
    return sum;
}

BigNumber BigNumber::operator>>(unsigned shift) {
    if (numOfDigits < shift) {
        throw invalid_argument("Shift must be less than number of digits.");
    }

    BigNumber temp;

    if (numOfDigits == shift) {
        temp = "0";
    } else {
        temp.sign = sign;
        temp.numOfDigits = numOfDigits - shift;
        temp.numArray = new int8_t[temp.numOfDigits];
        for (size_t i{0}; i < temp.numOfDigits; ++i) {
            temp[i] = numArray[i + shift];
        }
    }
    return temp;
}

BigNumber operator-(const BigNumber &num1, const BigNumber &num2) {
    BigNumber ans;
    if (num1.getSign() != num2.getSign()) {
        ans = BigNumber::unsignedAdd(num1, num2);
        ans.sign = num1.getSign();
    } else {
        ans = BigNumber::unsignedSubtract(num1, num2);
        if (num1 > num2)
            ans.sign = num1.getSign();
        else
            ans.sign = num2.getSign();

    }
    return ans;

}

BigNumber &BigNumber::operator--() {
    *this = *this - 1;
    return *this;
}

BigNumber &BigNumber::operator--(int) {
    BigNumber *myBum = new BigNumber(*this);
    *this = *this - 1;
    return *myBum;
}

BigNumber &BigNumber::operator++() {
    *this = *this - 1;
    return *this;
}

BigNumber &BigNumber::operator++(int) {
    BigNumber *myBum = new BigNumber(*this);
    *this = *this + 1;
    return *myBum;
}

BigNumber BigNumber::zarb(int i) {
    BigNumber temp = *new BigNumber(*this);
    for (int j = 1; j < i; ++j) {
        *this += temp;
    }
    if (i < 0)
        this->sign = !this->sign;
    return *this;
}

BigNumber &BigNumber::operator-=(const BigNumber &num1) {
    *this = *this - num1;
    return *this;
}

BigNumber &BigNumber::operator+=(const BigNumber &num1) {
    *this = *this + num1;
    return *this;
}

BigNumber &BigNumber::operator<<(int j) {
    string num = "";
    if (!this->sign) {
        num += '-';
    }
    num += this->to_string();
    for (int k = 0; k < j; ++k) {
        num += '0';
    }
    *this = BigNumber(num);
    return *this;
}

string BigNumber::to_string() const {
    string num = "";
    for (int i = this->numOfDigits - 1; i >= 0; --i) {
        num += (int8_t) (this->numArray[i] + 48);
    }
    return num;
}

BigNumber operator*(const BigNumber &num1, const BigNumber &num2) {
    string num = num2.to_string();

    BigNumber ans = *new BigNumber("0");
    for (int i = 0; i < num.size(); ++i) {
        if (num.at(i) - 48 != 0) {
            BigNumber temp = *new BigNumber(num1);
            for (int j = 1; j < num.at(i) - 48; ++j) {
                temp += num1;
            }
            temp << (int) num.size() - i - 1;
            ans += temp;
        }
    }


    return ans;
}

BigNumber BigNumber::pow(int i) {
    BigNumber ans = *new BigNumber(*this);
    if (i == 0)
        return 1;
    else {
        for (int j = 1; j < i; ++j) {
            ans *= *this;
        }
        return ans;
    }
}

BigNumber &BigNumber::operator*=(const BigNumber &num1) {
    *this = *this * num1;
    return *this;
}

BigNumber BigNumber::operator()(int first, int last) {
//string str=this->to_string();
//BigNumber ans=str.substr(numOfDigits-first,last);
    string str = "";
    for (int i = 0; i < last; ++i) {
        str += ::to_string(this->numArray[first - i]);
    }
    BigNumber ans = str;
    return ans;
}

BigNumber operator/(const BigNumber &num1, const BigNumber &num2) {
    BigNumber fakeNum1 = num1;
    string str = "";
    int i=1;
    while (fakeNum1 > num2) {
        BigNumber temp = fakeNum1(fakeNum1.getNumOfDigits() - 1, num2.getNumOfDigits());
        if (temp >= num2) {
            int div = fun(temp, num2);
            str += to_string(div);
            BigNumber mines = div * num2;
            mines << fakeNum1.getNumOfDigits() - num2.getNumOfDigits();
            fakeNum1 -= mines;
        } else {
            temp = fakeNum1(fakeNum1.getNumOfDigits() - 1, num2.getNumOfDigits() + 1);
            int div = fun(temp, num2);
            str += to_string(div);
            BigNumber mines = div * num2;
            mines << fakeNum1.getNumOfDigits() - num2.getNumOfDigits() - 1;
            fakeNum1 -= mines;
        }

    }
    if (num1 < num2)
        str += "0";
    BigNumber ans = str;
    return ans;
}

int fun(const BigNumber &num1, const BigNumber &num2) {
    int ans = 0;
    BigNumber temp = num1;
    while (temp >= num2) {
        ans++;
        temp -= num2;
    }
    return ans;
}

BigNumber operator%(const BigNumber &num1, const BigNumber &num2) {
    BigNumber div = num1 / num2;
    BigNumber ans = num1 - div * num2;
    return ans;
}


