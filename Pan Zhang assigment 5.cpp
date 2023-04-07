#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>
//to make the printed result more readable, 
class matrix
{
private:
	size_t num_rows{ 0 };
	size_t num_columns{ 0 };
	size_t size{ 0 };
	double* data{ nullptr };
public:
	//constructors
	matrix(){}
	matrix(size_t input_num_rows, size_t input_num_columns);
	~matrix() { delete[] data; } 
	matrix(const matrix& input_matrix); 
	matrix(matrix&& input_matrix) noexcept;

	//get private data;
	size_t get_size() const{ return num_rows * num_columns; }
	size_t get_column() const{ return num_columns; }
	size_t get_rows() const{ return num_rows; }
	
	//overload operator
	double& operator[](size_t i) const;
	matrix& operator=(const matrix& right_matrix);
	matrix& operator=(matrix&& right_matrix) noexcept;

	matrix operator+(const matrix& right_matrix)const;
	matrix operator-(const matrix& right_matrix) const;
	matrix operator*(const matrix& right_matrix) const;
	
	//friends
	friend std::ostream& operator<<(std::ostream& ostream, const matrix& input_matrix);
	friend void operator>>(std::string input_string, matrix& input_matrix);
	friend matrix operator*(const double input_double, matrix& input_matrix);
	friend matrix operator*(matrix& input_matrix, const double input_double);
	
	//funtionality
	matrix _delete(size_t del_row, size_t del_column);
	double det();
};

matrix::matrix(size_t input_num_rows, size_t input_num_columns)
{
	//std::cout << "parameterised constructor called" << std::endl;
	num_rows = input_num_rows;
	num_columns = input_num_columns;
	size = num_rows * num_columns;
	data = new double[size];
	for (size_t i{ 0 }; i < size; i++) { data[i] = 0; }
}
matrix::matrix(const matrix& input_matrix) //copy constructor
{
	std::cout << "copy constructor called" << std::endl;
	num_rows = input_matrix.num_rows;
	num_columns = input_matrix.num_columns;
	size = input_matrix.size;
	data = new double[size];
	for (size_t i = 0; i < size; i++) 
	{
		data[i] = input_matrix.data[i];
	}
}
matrix::matrix(matrix&& input_matrix) noexcept//move constructor
{
	std::cout << "move constructor called" << std::endl;
	num_rows = input_matrix.num_rows;
	num_columns = input_matrix.num_columns;
	size = input_matrix.size;
	data = input_matrix.data;
	input_matrix.size = 0;
	input_matrix.num_columns = 0;
	input_matrix.num_rows = 0;
	input_matrix.data = nullptr;
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

double& matrix::operator[](size_t i)const
{
	if (i < 0 || i >= size)
	{
		std::cout << "error,size less than zero." << std::endl;
	}

	return data[i];
}

matrix& matrix::operator=(const matrix& right_matrix)
{
	if (&right_matrix == this) return *this;	//self-assignment check;

	delete[] data;
	data = nullptr;
	size = 0;

	num_columns = right_matrix.get_column();
	num_rows = right_matrix.get_rows();
	size = num_columns * num_rows;
	data = new double[size];
	if (size>0) 
	{
		for (size_t i{ 0 }; i < size; i++) 
		{
			data[i] = right_matrix.data[i]; 
		}
	}
	return *this;
}
matrix& matrix::operator=(matrix&& right_matrix) noexcept
{
	std::cout << "move assignement" << std::endl;
	std::swap(size, right_matrix.size);
	std::swap(data, right_matrix.data);
	std::swap(num_columns, right_matrix.num_columns);
	std::swap(num_rows, right_matrix.num_rows);
	return *this;
}
matrix matrix::operator+(const matrix& right_matrix)const
{
	size_t column_1 = num_columns;
	size_t row_1 = num_rows;
	size_t column_2 = right_matrix.get_column();
	size_t row_2 = right_matrix.get_rows();

	if (column_1 == column_2 && row_1 == row_2)//check dimension
	{
		matrix new_matrix(row_1, column_1);
		for (int i{ 0 }; i < size; i++)
		{
			new_matrix.data[i] = data[i] + right_matrix.data[i];
		}
		return new_matrix;
	}
	else
	{
		matrix new_matrix;
		std::cout << "Dimension doesn't match!" << std::endl;
		return new_matrix;
	}

}

matrix matrix::operator-(const matrix& right_matrix)const
{
	size_t column_1 = num_columns;
	size_t row_1 = num_rows;
	size_t column_2 = right_matrix.get_column();
	size_t row_2 = right_matrix.get_rows();
	matrix new_matrix(row_1, column_1);
	if (column_1 == column_2 && row_1 == row_2)
	{
		for (int i{ 0 }; i < size; i++)
		{
			new_matrix.data[i] = data[i] - right_matrix.data[i];
		}
		return new_matrix;
	}
	else
	{
		matrix new_matrix;
		std::cout << "Dimension doesn't match!" << std::endl;
		return new_matrix;
	}
}
matrix matrix::operator*(const matrix& right_matrix)const
{
	size_t column_1 = num_columns;
	size_t row_1 = num_rows;
	size_t column_2 = right_matrix.get_column();
	size_t row_2 = right_matrix.get_rows();
	if (column_1 == row_2)
	{
		matrix new_matrix(row_1, column_2);
		for (size_t i{ 0 }; i < row_1; i++)//iterating through the rows of left matrix
		{
			for (size_t k{ 0 }; k < column_2; k++)//iterating through the columns left matrix
			{
				double temp{ 0 };
				for (size_t j{ 0 }; j < column_1; j++)//iterating through the columns right matrix
				{
					temp += (data[j + i * column_1] * right_matrix.data[k + j * column_2]);
					if (j + 1 == column_1) { new_matrix.data[i * column_1 + k] = temp;}
				}
			}
		}
		return new_matrix;
	}
	else
	{
		matrix new_matrix;
		std::cout << "Dimension doesn't match!" << std::endl;
		return new_matrix;
	}
}

matrix operator*(const double input_double, matrix& input_matrix)
{
	matrix new_matrix(input_matrix.num_rows, input_matrix.num_columns);
	for (size_t i{ 0 }; i < input_matrix.size; i++) { new_matrix.data[i] = input_matrix.data[i] * input_double; }
	return new_matrix;
}

matrix operator*(matrix& input_matrix, const double input_double)
{
	matrix new_matrix(input_matrix.num_rows, input_matrix.num_columns);
	for (size_t i{ 0 }; i < input_matrix.size; i++) { new_matrix.data[i] = input_matrix.data[i] * input_double; }
	return new_matrix;
}

matrix matrix::_delete(size_t del_row, size_t del_column) 
{
	size_t new_rows = num_rows;
	size_t new_columns = num_columns;
	if (del_row != 0) { new_rows = num_rows - 1; }
	if (del_column != 0) { new_columns = num_columns - 1; }
	size_t new_size = new_rows * new_columns;

	matrix new_matrix(new_rows, new_columns);
	size_t track_index{ 0 };
	for (size_t i{ 0 }; i < num_rows; i++)
	{
		if (i != del_row - 1)
		{
			for (size_t j{ 0 }; j < num_columns; j++)
			{
				if (j != del_column - 1)
				{
					new_matrix[track_index] = data[j + i * num_rows];
					track_index += 1;
				}
			}
		}
	}
	return new_matrix;
}

std::ostream& operator<<(std::ostream& ostream, matrix& input_matrix)
{
	size_t size = input_matrix.get_size();
	size_t column = input_matrix.get_column();
	size_t rows = input_matrix.get_rows();
	if (size != 0) {
		for (size_t i{ 0 }; i < rows; i++)
		{
			for (size_t j{ 0 }; j < column; j++)
			{
				if (j == 0)
				{
					std::cout << "[";
				}
				if (j + 1 == column)
				{
					std::cout << input_matrix[j + i * column] << "]" << std::endl;//This line potentially read nullptr
					break;
				}
				std::cout << input_matrix[j + i * column] << ", ";
			}
		}
		std::cout << std::endl;
	}
	if (size == 0) 
	{
		std::cout << "[]" << std::endl;
	}
	return ostream;
}


//input once a row each time.
void operator>>(std::string input_string, matrix& input_matrix)
{
	size_t num_rows = input_matrix.get_rows();
	size_t num_columns = input_matrix.get_column();
	std::string s1;
	std::stringstream ss(input_string);
	std::vector<std::vector<double>> input_vector_2D;
	while (std::getline(ss, s1, ';'))//read rows
	{
		std::vector<double> input_vector_1D;
		std::string s2;//matrix elements
		std::stringstream ss(s1);//read column
		while (std::getline(ss, s2, ','))//read matrix elements
		{
			double matrix_element = std::stod(s2);
			input_vector_1D.push_back(matrix_element);
		}
		input_vector_2D.push_back(input_vector_1D);
	}
	size_t input_columns = input_vector_2D[0].size();
	size_t input_rows = input_vector_2D.size();

	if (num_columns == input_columns && num_rows == input_rows)
	{
		for (size_t i{ 0 }; i < input_rows; i++)
		{
			for (size_t j{ 0 }; j < input_columns; j++)
			{
				input_matrix[j + i * input_columns] = input_vector_2D[i][j];
			}
		}
	}
	if (num_columns != input_columns || num_rows != input_rows)
	{
		matrix new_matrix(input_rows, input_columns);
		input_matrix = new_matrix;
		std::cout << "size doesn't match, auto extend matrix size" << std::endl;
		for (size_t i{ 0 }; i < input_rows; i++)
		{
			for (size_t j{ 0 }; j < input_columns; j++)
			{
				input_matrix[j + i * input_columns] = input_vector_2D[i][j];
			}
		}
	}
}
double matrix::det()
{
	std::vector<std::vector<matrix>> temp_matrix_vector_2D;
	std::vector<matrix> temp_matrix_vector_1D;
	std::vector<std::vector<double>> temp_coeffcient_2D;
	std::vector<double> temp_coeffcient_1D;
	if (num_columns == num_rows && (num_columns > 2 || num_columns == 2))//if matrix dimension is greater than 2*2
	{
		matrix target_matrix = *this;//we find the minor of target matrix, initialy set to our input matrix
		double coefficient{0};//coefficient before minor.
		//two assistance temp value
		matrix temp_matrix;
		size_t target_index{ 0 };
		
		//store minor and minor in vector.
		temp_coeffcient_1D.push_back(1);//store coefficient of same dimension minor at same 1D vector 
		temp_coeffcient_2D.push_back(temp_coeffcient_1D);
		temp_matrix_vector_1D.push_back(*this);//store same dimension minor in same 1D vector
		temp_matrix_vector_2D.push_back(temp_matrix_vector_1D);
		while (temp_matrix_vector_2D[target_index][0].get_column() != 2)//if the minor greater than 2*2 dimension
																		//The iteration continues.
		{
			temp_matrix_vector_1D.clear();
			temp_coeffcient_1D.clear();
			//iterate through minors, find smaller minor!
			for (size_t i{ 0 }; i < temp_matrix_vector_2D[target_index].size(); i++)
			{
				target_matrix = temp_matrix_vector_2D[target_index][i];//assign the target matrix
				//
				// vector: [[matrix1],[matrix2],[matrix3]...]
				// now assign 
				// matrix1--->target_matrix 
				// then 
				// matrix2--->target_matrix 
				//	then matrix3--->target_matrix
				// .
				// .
				// .
				//
				for (size_t j{ 0 }; j < target_matrix.get_column(); j++)
					//
					// if our target matrix has the form
					// [ 1, 2, 3, ]
					// [ 4, 5, 6, ]
					// [ 7, 8, 9, ]
					// temp matrix will be_______________
					//			|----> coefficent		|
					//			|						|		
					// [ 1, 2, |3,| ]					|
					//									|
					// [ 4, |5, 6,|<--------------------|
					// [ 7, |8, 9,|<--------------------|
					//				
					// then 
					// [     |2|     ]
					// [ |4,|   | 6,| ]
					// [ |7,|   | 9,|]
					//
				{
					temp_matrix = target_matrix._delete(1, j + 1);//find minor
					coefficient = pow(-1, 1 + j + 1) * target_matrix[j];//find coefficent
					temp_matrix_vector_1D.push_back(temp_matrix);
					temp_coeffcient_1D.push_back(coefficient* temp_coeffcient_2D[target_index][i]);
				}
			}
			target_index += 1;//now the minor's minor is found. If minor's minor dimension is greater than 2*2
							  //same procedure will continue
			temp_matrix_vector_2D.push_back(temp_matrix_vector_1D);
			temp_coeffcient_2D.push_back(temp_coeffcient_1D);
		}
		double determinant(0);
		//the last line is just 2*2 matrix, add their determinant up and assign 2*2 matrix with coefficient;
		for (size_t i{}; i < temp_matrix_vector_2D[target_index].size(); i++)
		{
			target_matrix = temp_matrix_vector_2D[target_index][i];
			coefficient = temp_coeffcient_2D[target_index][i];
			determinant += coefficient*(target_matrix[0] * target_matrix[3] - target_matrix[1] * target_matrix[2]);
		}
		return determinant;
	}
	std::cout << "Not a squared matrix" << std::endl;
	double determinant = 0;
	return determinant;
}
int main()
{
	//initialize
	matrix A;
	matrix B;
	matrix C;
	matrix result;
	"1,2,3;9,8,7;4,2,6;" >> A;
	"5,5,4;1,2,3;6,9,8;" >> B;
	"3,4,1;2,5,6;" >> C;
	std::cout << "A" << std::endl;
	std::cout << A << std::endl;
	std::cout << "B" << std::endl;
	std::cout << B << std::endl;
	std::cout << "C" << std::endl;
	std::cout << C << std::endl;
	
	//calculation
	result = A + B;
	std::cout << "A + B = " << std::endl;
	std::cout << result << std::endl;
	result = A - B;
	std::cout << "A - B = " << std::endl;
	std::cout << result << std::endl;
	result = A * B;
	std::cout << "A * B = " << std::endl;
	std::cout << result << std::endl;
	result = C * B;
	std::cout << "C * B = " << std::endl;
	std::cout << result << std::endl;
	result = B * C;
	std::cout << "B * C = " << std::endl;
	std::cout << result << std::endl;
	result = A + C;
	std::cout << "A + C = " << std::endl;
	std::cout << result << std::endl;
	
	//calculate the minor of A by _delete funtion
	matrix minor_A = A._delete(1,1);
	std::cout << "The minor of A(delete row 1 and column1)" << std::endl;
	std::cout<< minor_A << std::endl;
	//calculate determinate A
	double determinant_A = A.det();
	std::cout << "The determinant of A is: " << determinant_A << std::endl;

	//copy constructor
	std::cout << "Show copy constructor" << std::endl;
	matrix copy = A;
	"1,2,3;2,0,0" >> A;
	std::cout << "This is the copy of A" << std::endl;
	std::cout << copy << std::endl;
	std::cout << "This is the modified A" << std::endl;
	std::cout << A << std::endl;
	//move constructor
	matrix move = std::move(A);
	std::cout << "Move A to new matrix:" << std::endl;
	std::cout << move << std::endl;
	std::cout << "This is the A" << std::endl;
	std::cout << A << std::endl;//A's data is moved to matrix 'move', so it will show a warning.

	return 0;
}