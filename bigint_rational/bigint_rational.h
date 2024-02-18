#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class BigInteger {
 private:
  bool is_positive = true;
  std::vector<long long> big_number;

  static const int BASE = 1'000'000'0;
  static const short BASE_LEN = 7;

  void Shift() {
    if (big_number.empty()) {
      big_number.push_back(0);
      return;
    }
    big_number.push_back(big_number[big_number.size() - 1]);
    for (size_t i = big_number.size() - 2; i > 0; --i) {
      big_number[i] = big_number[i - 1];
    }

    big_number[0] = 0;
  }

  void remove_leading_zeroes() {
    for (int i = static_cast<int>(big_number.size()) - 1; i >= 0; --i) {
      if (big_number[i] == 0) {
        big_number.pop_back();
      } else {
        break;
      }
    }
  }

 public:

  BigInteger absVal() const {
    auto copy = *this;
    copy.is_positive = true;
    return copy;
  }

  BigInteger negativeVal() const {
    auto copy = *this;
    copy.is_positive = false;
    return copy;
  }

  bool getSign() const {
    return is_positive;
  }

  std::vector<long long>& getNum() {
    return big_number;
  }

  const std::vector<long long>& getNum() const {
    return big_number;
  }

  std::string toString() const {
    std::ostringstream oss;
    if (big_number.empty()) {
      return "0";
    }
    if (big_number[0] == 0 && big_number.size() == 1) {
      return "0";
    }
    if (!is_positive) {
      oss << '-';
    }
    for (int i = static_cast<int>(big_number.size()) - 1; i >= 0; --i) {
      if (i != static_cast<int>(big_number.size()) - 1 &&
          static_cast<int>(std::log10(big_number[i])) < BASE_LEN - 1) {
        if (big_number[i] == 0) {
          for (int j = 0; j < BASE_LEN - 1; ++j) {
            oss << 0;
          }
        } else {
          for (int j = static_cast<int>(std::log10(big_number[i]));
               j < BASE_LEN - 1; ++j) {
            oss << 0;
          }
        }
      }
      oss << big_number[i];
    }
    return oss.str();
  }

  BigInteger() = default;

  BigInteger(const int input) : is_positive(input >= 0 ? true : false) {
    if (input == 0) {
      big_number.push_back(0);
      return;
    }
    if (input >= BASE) {
      big_number.push_back(std::abs(input % BASE));
      big_number.push_back(std::abs(input / BASE));
    } else {
      big_number.push_back(std::abs(input));
    }
  }

  explicit BigInteger(const std::string& str) {
    long long cell = 0;
    int cnt = 0;
    for (int i = static_cast<int>(str.size()) - 1; i > 0; --i) {
      cell = (str[i] - '0') * static_cast<long long>(std::pow(10, cnt)) + cell;
      ++cnt;

      if (cnt == BASE_LEN) {
        big_number.push_back(cell);
        cell = 0;
        cnt = 0;
      }
    }
    if (str[0] == '-') {
      is_positive = false;
    } else {
      cell = (str[0] - '0') * static_cast<long long>(std::pow(10, cnt)) + cell;
      ++cnt;
    }

    if (cnt != 0) {
      big_number.push_back(cell);
    }
  }

  static bool Modulo_Compare(const BigInteger&, const BigInteger&);

  BigInteger operator-() const {
    BigInteger res = *this;
    res.is_positive = !res.is_positive;
    return res;
  }

  BigInteger& operator+=(const BigInteger& input);

  BigInteger& operator-=(const BigInteger& input);

  BigInteger& operator++() {
    *this += 1;
    return *this;
  }

  BigInteger operator++(int);

  BigInteger& operator--() {
    *this -= 1;
    return *this;
  }

  BigInteger operator--(int);

  BigInteger& operator*=(const BigInteger&);

  BigInteger& operator/=(const BigInteger&);

  BigInteger& operator%=(const BigInteger&);

  friend BigInteger operator*(const BigInteger&, const BigInteger&);

  friend BigInteger operator/(const BigInteger&, const BigInteger&);

  explicit operator bool() const {
    return !(this->big_number[0] == 0 && this->big_number.size() == 1);
  }
};

bool operator==(const BigInteger& left, const BigInteger& right) {
  if (left.getSign() != right.getSign() ||
      right.getNum().size() != left.getNum().size()) {
    return false;
  }
  for (int i = left.getNum().size() - 1; i >= 0; --i) {
    if (left.getNum()[i] != right.getNum()[i]) {
      return false;
    }
  }
  return true;
}

BigInteger& BigInteger::operator+=(const BigInteger& input) {
  size_t maxsize = std::max(input.big_number.size(), big_number.size());
  if (is_positive != input.is_positive) {
    is_positive = input.is_positive;
    (*this) -= input;
    is_positive = !is_positive;
    if (*this == 0) {
      is_positive = true;
    }
  } else {
    big_number.resize(maxsize + 2);
    for (size_t i = 0; i < big_number.size(); ++i) {
      if (input.big_number.size() > i) {
        big_number[i] += input.big_number[i];
      }
    }
    for (size_t i = 0; i < big_number.size() - 1; ++i) {
      if (big_number[i] >= BASE) {
        big_number[i] -= BASE;
        big_number[i + 1]++;
      }
    }
    remove_leading_zeroes();
  }
  if (*this == 0) {
    is_positive = true;
  }
  return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& input) {
  std::vector<long long> arr_copy = input.big_number;
  if (is_positive != input.is_positive) {
    is_positive = input.is_positive;
    (*this) += input;
    is_positive = !(is_positive);
    if (*this == 0) {
      is_positive = true;
    }
  } else {
    if (*this == input) {
      big_number.resize(1);
      big_number[0] = 0;
      is_positive = true;
      return *this;
    } else if (Modulo_Compare(*this, input)) {
      arr_copy = big_number;
      big_number = input.big_number;
      is_positive = !is_positive;
    }
    size_t ms = std::max(big_number.size(), arr_copy.size());
    big_number.resize(ms + 2);
    for (size_t i = 0; i < big_number.size(); ++i) {
      if (arr_copy.size() > i) {
        big_number[i] -= arr_copy[i];
      }
    }
    for (size_t i = 0; i < big_number.size() - 1; ++i) {
      if (big_number[i] < 0) {
        big_number[i] += BASE;
        big_number[i + 1]--;
      }
    }
    remove_leading_zeroes();
    if (*this == 0) {
      is_positive = true;
    }
  }
  return *this;
}

bool operator<(const BigInteger& left, const BigInteger& right) {
  if (left.getSign() != right.getSign()) {
    return !(left.getSign());
  }

  if (left.getNum().size() != right.getNum().size()) {
    return (left.getNum().size() < right.getNum().size()) ^
           (!left.getSign());
  }

  for (int i = left.getNum().size() - 1; i >= 0; --i) {
    if (left.getNum()[i] > right.getNum()[i]) {
      return false;
    }
    if (left.getNum()[i] < right.getNum()[i]) {
      return true;
    }
  }
  return false;
}

bool operator<=(const BigInteger& left, const BigInteger& right) {
  return (left < right || left == right);
}

bool operator>(const BigInteger& left, const BigInteger& right) {
  return !(left <= right);
}

bool operator>=(const BigInteger& left, const BigInteger& right) {
  return !(left < right);
}

bool operator!=(const BigInteger& left, const BigInteger& right) {
  return !(left == right);
}

std::istream& operator>>(std::istream& in, BigInteger& input) {
  std::string str;
  in >> str;

  input = BigInteger(str);

  return in;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& input) {
  std::string str = input.toString();

  out << str;
  return out;
}

BigInteger operator+(const BigInteger& left, const BigInteger& right) {
  BigInteger res = left;
  res += right;
  return res;
}

BigInteger operator-(const BigInteger& left, const BigInteger& right) {
  BigInteger res = left;
  res -= right;
  return res;
}

BigInteger operator*(const BigInteger& left, const BigInteger& right) {
  BigInteger res;
  if (left == 0 || right == 0) {
    return res;
  }
  res.is_positive = !(left.is_positive ^ right.is_positive);
  res.big_number.resize(left.big_number.size() + right.big_number.size() + 1);
  for (size_t i = 0; i < left.big_number.size(); ++i) {
    for (size_t j = 0; j < right.big_number.size(); ++j) {
      res.big_number[i + j] += (left.big_number[i] * right.big_number[j]);
    }
  }
  for (size_t i = 0; i < res.big_number.size(); ++i) {
    long long next = res.big_number[i] / BigInteger::BASE;
    if (next != 0) {
      res.big_number[i] %= BigInteger::BASE;
      res.big_number[i + 1] += next;
    }
  }
  res.remove_leading_zeroes();
  return res;
}

BigInteger operator/(const BigInteger& left, const BigInteger& right) {
  assert(right != 0 && "Zero division");
  if (right.big_number.size() > left.big_number.size()) {
    BigInteger ret(0);
    return (ret);
  }
  if (right == left) {
    BigInteger ret(1);
    return ret;
  }
  BigInteger result, temp, copy = right;
  copy.is_positive = true;
  result.big_number.resize(left.big_number.size());
  for (int i = static_cast<int>(left.big_number.size()) - 1; i >= 0; --i) {
    temp.Shift();
    temp.big_number[0] = left.big_number[i];
    temp.remove_leading_zeroes();

    int found = 0, left = 0, right = BigInteger::BASE;
    while (left <= right) {
      int middle = (left + right) / 2;
      BigInteger t_bigint = copy * middle;

      if (t_bigint <= temp) {
        found = middle;
        left = middle + 1;
      } else {
        right = middle - 1;
      }
    }
    result.big_number[i] = found;
    temp = temp - copy * found;
  }
  result.is_positive = !(left.is_positive ^ right.is_positive);
  result.remove_leading_zeroes();
  return result;
}

BigInteger operator%(BigInteger& left, const BigInteger& right) {
  return left - right * (left / right);
}

BigInteger BigInteger::operator++(int) {
  *this += 1;
  return (*this - 1);
}

BigInteger BigInteger::operator--(int) {
  *this -= 1;
  return (*this + 1);
}

BigInteger& BigInteger::operator*=(const BigInteger& input) {
  *this = *this * input;
  return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& input) {
  *this = *this / input;
  return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& input) {
  *this = *this % input;
  return *this;
}

BigInteger operator""_bi(const char* ch_arr, unsigned long) {
  BigInteger x(ch_arr);
  return x;
}

bool BigInteger::Modulo_Compare(const BigInteger& a, const BigInteger& b) {
  if (a.big_number.size() != b.big_number.size()) {
    return (a.big_number.size() < b.big_number.size());
  }
  for (int i = a.big_number.size() - 1; i >= 0; --i) {
    if (a.big_number[i] < b.big_number[i]) {
      return true;
    }
    if (a.big_number[i] > b.big_number[i]) {
      return false;
    }
  }
  return false;
}

class Rational {
 public:
  BigInteger num = 0;
  BigInteger denom = 1;

 private:
  static const int double_precision = 40;

  static BigInteger gcd(BigInteger a, BigInteger b) {
    if (a == 0 || b == 0) {
      return 0;
    } else {
      if (a < b) {
        std::swap(a, b);
      }
      while (a % b != 0) {
        a = a % b;
        std::swap(a, b);
      }
      return b;
    }
  }

  void normalize() {
    if (num == 0) {
      denom = 1;
      return;
    }
    if (num == 1) {
      return;
    }
    if (num == denom) {
      num = 1;
      denom = 1;
      return;
    }
    if (denom > num && denom % num == 0) {
      denom /= num;
      num = 1;
      return;
    }
    if (num > denom && num % denom == 0) {
      num /= denom;
      denom = 1;
      return;
    }

    BigInteger temp = gcd(num.absVal(), denom.absVal());
    if (temp > 1) {
      num /= temp;
      denom /= temp;
    }
  }

 public:
  std::string toString() const {
    std::ostringstream oss;
    oss << num;
    if (denom != 1) {
      oss << '/' << denom;
    }
    return oss.str();
  }

  std::string asDecimal(size_t precision = 0) const {
    if (precision == 0) {
      return (num / denom).toString();
    }
    BigInteger n = num;
    std::vector<int> nums;
    BigInteger r = (n / denom);
    n = n.absVal();
    n %= denom;
    size_t i = 0;
    ++precision;
    while (i < precision) {
      if (n == 0) {
        nums.push_back(0);
      } else {
        n *= 10;
        while (n < denom) {
          n *= 10;
          nums.push_back(0);
        }
        if (i < precision) {
          nums.push_back((n / denom).getNum()[0]);
        }
        n %= denom;
      }
      ++i;
    }

    if (nums[precision - 1] >= 5) {
      ++nums[precision - 2];
      for (size_t j = nums.size() - 2; j > 0; --j) {
        if (nums[j] >= 10) {
          nums[j] -= 10;
          ++nums[j + 1];
        } else {
          break;
        }
      }
      if (nums[0] >= 10) {
        nums[0] -= 10;
        ++r;
      }
    }
    std::string str = ((num.getSign() ^ denom.getSign()) ? "-" : "");
    str += r.toString();
    str += '.';
    for (size_t j = 0; j < precision - 1; ++j) {
      str += std::to_string(nums[j]);
    }
    return str;
  }

  Rational() = default;

  Rational(const BigInteger& num1, const BigInteger& denom1)
      : num(num1), denom(denom1) {
    normalize();
  }

  Rational(const BigInteger& input) : num(input) {}

  Rational(const int input) : num(input) {}

  Rational& operator+=(const Rational& input) {
    if (denom == input.denom) {
      num += input.num;
      normalize();
      return *this;
    }

    num *= input.denom;
    num += (input.num * denom);
    denom *= input.denom;
    normalize();
    return *this;
  }

  Rational& operator-=(const Rational& input) {
    if (denom == input.denom) {
      num -= input.num;
      normalize();
      return *this;
    }

    num *= input.denom;
    num -= (input.num * denom);
    denom *= input.denom;
    normalize();
    return *this;
  }

  Rational& operator*=(const Rational& input) {
    num *= input.num;
    denom *= input.denom;
    normalize();
    return *this;
  }

  Rational& operator/=(const Rational& input) {
    assert(input.num != 0 && "Zero Division");

    if (this->num == 0) {
      return *this;
    }

    num *= input.denom;
    denom *= input.num;
    if (!denom.getSign() ^ num.getSign()) {
      num = num.absVal();
    } else {
      num = num.negativeVal();
    }
    denom = denom.absVal();
    normalize();
    return *this;
  }

  Rational operator-() const {
    Rational ret = *this;
    if (ret.num > 0) {
      ret.num = ret.num.negativeVal();
    } else {
      ret.num = ret.num.absVal();
    }
    return ret;
  }

  explicit operator double() const {
    std::istringstream iss(asDecimal(double_precision));

    double res;
    iss >> res;

    return res;
  }

  friend std::ostream& operator<<(std::ostream& out, const Rational input) {
    out << input.toString();

    return out;
  }
};

bool operator==(const Rational& left, const Rational& right) {
  return (left.num * right.denom) == (left.denom * right.num);
}

bool operator<(const Rational& left, const Rational& right) {
  return ((left.num * right.denom) < (left.denom * right.num));
}

bool operator!=(const Rational& left, const Rational& right) {
  return !(left == right);
}

bool operator>(const Rational& left, const Rational& right) {
  return ((left.num * right.denom) > (left.denom * right.num));
}

bool operator<=(const Rational& left, const Rational& right) {
  return !(left > right);
}

bool operator>=(const Rational& left, const Rational& right) {
  return !(left < right);
}

Rational operator+(const Rational& x, const Rational& y) {
  Rational res = x;
  res += y;
  return res;
}

Rational operator-(const Rational& x, const Rational& y) {
  Rational res = x;
  res -= y;
  return res;
}

Rational operator*(const Rational& x, const Rational& y) {
  Rational res = x;
  res *= y;
  return res;
}

Rational operator/(const Rational& x, const Rational& y) {
  Rational res = x;
  res /= y;
  return res;
}