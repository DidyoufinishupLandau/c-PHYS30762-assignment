#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<sstream>

class complex 
{
private:
	double x{ std::nan("")};
	double y{ std::nan("")};
public:
	complex() = default;
	complex(double input_x, double input_y):
		x{ input_x }, y{input_y}
	{}
	~complex() { std::cout << "Destroying complex" << std::endl; }

	double get_real() const { return x; }
	double get_imaginary() const { return y; }
	double modulus() const;
	double arg() const;

	complex complex_conjugate() const;

	void set_x(const double input_x);
	void set_y(const double input_y);

	complex operator+(const complex& complex_number) const;
	complex operator-(const complex complex_number) const;
	complex operator*(const complex real_num) const;
	complex operator/(const complex real_num) const;

	friend std::ostream& operator<<(std::ostream& ostream, const complex& complex_num);
	friend std::istream& operator>>(std::istream& istream, complex& complex_num);
};

std::ostream& operator<<(std::ostream& ostream, const complex& complex_num) 
{
	//consider the positive sign and negative sign of imagianry part
	if (complex_num.y > 0) { ostream << complex_num.x << " + i" << complex_num.y << std::endl; }
	if (complex_num.y == 0) { ostream << complex_num.x << std::endl; }
	if (complex_num.y < 0) { ostream << complex_num.x << " - i" << abs(complex_num.y) << std::endl; }
	return ostream;
}

bool is_numeric(std::string string)
{
	//check if input string numerical
	if (string.empty()) return false;//if string is empty "" return false
	for (char c : string) // for all string 
	{
		// check if the single character is digit or not, . - + is considered
		//as int type so need further in c++
		if (!std::isdigit(c) && c != '.' && c != '-' && c != '+')
			return false;
	}
	//if numerical return true
	return true;
}

std::istream& operator>>(std::istream& istream, complex& complex_num)
{
	double x_val;
	double y_val;
	std::string input_string;
	//read only single line
	if (std::getline(istream, input_string)) 
	{
		std::string s1;
		std::string s2;
		std::string s3;
		std::stringstream ss(input_string);
		//pass the input to s1, s2 s3. 
		//s1 is real part 
		//s2 is + or - sign. 
		//s3 is imaginary part 
		ss >> s1;
		ss >> s2;
		std::getline(ss, s3, 'i');
		ss >> s3;
		//return positive imagianry part
		if (is_numeric(s1) && is_numeric(s3) && (s2 == "+"))
		{
			x_val = std::stod(s1);
			y_val = std::stod(s3);
			complex_num.x = x_val;
			complex_num.y = y_val;
			return istream;
		}
		//return negative imaginary part
		if (is_numeric(s1) && is_numeric(s3) && (s2 == "-"))
		{
			x_val = std::stod(s1);
			y_val = std::stod(s3);
			complex_num.x = x_val;
			complex_num.y = -y_val;
			return istream;
		}
		//input error
		else
		{
			std::cout << "Invalid input format for complex number" << std::endl;
			return istream;
		}
	}
	return istream;
}

complex complex::complex_conjugate() const 
{
	complex new_complex(x,-y);
	return new_complex;
}

double complex::modulus() const
{
	double modulus = pow(pow(x, 2) + pow(y, 2), 0.5);
	return modulus;
}

double complex::arg() const
{
	//return radian
	double argument = std::atan2(y, x);
	if (argument < 0) { argument = 2 * 3.14159265358979323846 + argument; }
	return argument;
}

void complex::set_x(const double input_x) 
{
	x += input_x;
}
void complex::set_y(const double input_y) 
{
	y += input_y;
}
//overload functions
complex complex::operator+(const complex& complex_number) const
{
	complex new_complex(x+complex_number.get_real(), y+complex_number.get_imaginary());
	return new_complex;
}


complex complex::operator-(const complex complex_number) const
{
	complex new_complex(x - complex_number.get_real(), y - complex_number.get_imaginary());
	return new_complex;
}

complex complex::operator*(const complex complex_number) const
{
	double x_input = complex_number.get_real();
	double y_input = complex_number.get_imaginary();
	complex new_complex(x*x_input-y*y_input, x*y_input + y*x_input);
	return new_complex;
}

complex complex::operator/(const complex complex_number) const
{
	double x_input = complex_number.get_real();
	double y_input = complex_number.get_imaginary();
	double modulus_sqaure_input = pow(complex_number.modulus(), 2);

	complex new_complex((x * x_input + y * y_input)/ modulus_sqaure_input, (y*x_input-x*y_input)/ modulus_sqaure_input);
	return new_complex;
}

int main()
{
	//initialize complex 1 and complex 2 and print out.
	complex complex_num_1(3, 4);
	complex complex_num_2(1, -2);
	std::cout << "complex 1:" << complex_num_1 << std::endl;
	std::cout << "complex 2:" << complex_num_2 << std::endl;
	//get real part
	std::cout << "complex 1 real part:" << complex_num_1.get_real() << std::endl;
	std::cout << "complex 2 real part:" << complex_num_2.get_real() << std::endl;
	//get imaginary part
	std::cout << "complex 1 imaginary part:" << complex_num_1.get_imaginary() << std::endl;
	std::cout << "complex 2 imaginary part:" << complex_num_2.get_imaginary() << std::endl;
	//now initialize complex 3 to test the functionalities.
	complex complex_num_3;
	complex_num_3 = complex_num_1 + complex_num_2;
	std::cout << "Sum of complex 1 and complex 2:" << complex_num_3 << std::endl;
	complex_num_3 = complex_num_1 - complex_num_2;
	std::cout << "complex 1 - complex 2: " << complex_num_3 << std::endl;
	complex_num_3 = complex_num_2 - complex_num_1;
	std::cout << "complex 2 - complex 1: " << complex_num_3 << std::endl;
	complex_num_3 = complex_num_1 * complex_num_2;
	std::cout << "complex 2 * complex 1: " << complex_num_3 << std::endl;

	complex_num_3 = complex_num_1 / complex_num_2;
	std::cout << "complex 1 / complex 2: " << complex_num_3 << std::endl;
	complex_num_3 = complex_num_2 / complex_num_1;
	std::cout << "complex 2 / complex 1: " << complex_num_3 << std::endl;

	double argument_1 = complex_num_1.arg();
	double argument_2 = complex_num_2.arg();
	std::cout << "The argument of 3 + i4 is:" << argument_1 << " rad" << std::endl;
	std::cout << "The argument of 1 - i2 is:" << argument_2 << " rad" << std::endl;
	
	double modulus_1 = complex_num_1.modulus();
	double modulus_2 = complex_num_2.modulus();
	std::cout << "The modulus of 3 + i4 is:" << modulus_1 << std::endl;
	std::cout << "The modulus of 1 - i2 is:" << modulus_2 << std::endl;

	complex complex_conjugate_1 = complex_num_1.complex_conjugate();
	complex complex_conjugate_2 = complex_num_2.complex_conjugate();
	std::cout << "The complex conjugate of 3 + i4 is:" << complex_conjugate_1 << std::endl;
	std::cout << "The complex conjugate of 1 - i2 is:" << complex_conjugate_2 << std::endl;
	//now test if the complex times its complex conjugate gives its modulus square

	std::cout << "The modulus square of 3 + i4 is:" << complex_conjugate_1* complex_num_1 << std::endl;
	std::cout << "The modulus square of 1 - i2 is:" << complex_conjugate_2* complex_num_2 << std::endl;


	complex input_complex;
	std::cout << "Input a complex number, format a + ib" << std::endl;
	std::cin >> input_complex;
	std::cout << "The input value is:" << input_complex << std::endl;

	return 0;
}