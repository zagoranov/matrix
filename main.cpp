#include <iostream>
#include <map>
#include <vector>
#include <cassert>
#include <iterator>
#include <string>


template<typename T, T defValue, size_t Dimension>
class Matrix {
	std::map<std::vector<unsigned>, T> MatrixData;
	T dvalue;
	size_t dim;
public:
	Matrix() : dvalue(defValue), dim(Dimension) { }
	~Matrix() {}
	size_t size() const { return MatrixData.size(); }
	size_t dimension() const { return dim; }

	class ref
	{
		Matrix& cont_;
		std::vector<unsigned> indexes;
	public:
		ref(Matrix& cont, long i) : cont_(cont) { indexes.push_back(i); }

		operator const T& () const {
			auto it = cont_.MatrixData.find(indexes);
			if (it == cont_.MatrixData.end())
				return cont_.dvalue;
			else
				return it->second;
		}

		ref& operator [](long i)
		{
			if (indexes.size() + 1 > cont_.dimension()) {
				throw std::overflow_error("Too much index arguments");
			}
			else {
				indexes.push_back(i);
			}
			return *this;
		}

		ref& operator=(T value)
		{
			if (indexes.size() < cont_.dimension())
				throw std::overflow_error("Not enough index arguments");
			auto it = cont_.MatrixData.find(indexes);
			if (it != cont_.MatrixData.end()) {
				if (value == cont_.dvalue)
					cont_.MatrixData.erase(it);
				else
					(*it).second = value;
			}
			else
				if (value != cont_.dvalue) {
					cont_.MatrixData.insert({ indexes, value });
				}
			return *this;
		}
	};
	ref operator [](long i)
	{
		return ref(*this, i);
	}

	class iterator {
	public:
		typename std::map<std::vector<unsigned>, T>::iterator it;
		iterator(typename std::map<std::vector<unsigned>, T>::iterator _it) : it(_it) {}
		bool operator!=(const iterator & other) const { return it != other.it; }
		iterator operator++() { ++it; 	return *this; }
		std::pair<std::vector<unsigned>, T> operator*() { return *it; }
	};
	iterator begin() { return MatrixData.begin(); }
	iterator end() { return MatrixData.end(); }
};


std::string Accumulate(std::vector<unsigned>& v) {
	std::string result;
	for (unsigned const& u : v) { result += "[" + std::to_string(u) + "]"; }
	return result;
}


int main()
{
	try {
		Matrix<int, 0, 2> matrix;
		assert(matrix.size() == 0);

		for (size_t i = 0; i < 10; ++i)
			matrix[i][i] = i;
		for (size_t i = 0, j = 9; i < 10; ++i, j--)
			matrix[i][j] = i;

		for (size_t i = 1; i < 9; ++i) {
			for (size_t j = 1; j < 9; ++j)
				std::cout << matrix[i][j] << " ";
			std::cout << std::endl;
		}

		std::cout << "Size: " << matrix.size() << std::endl;

		for (const auto& c : matrix)
		{
			std::cout << Accumulate(c.first) << " = " << c.second << "; ";
		}

	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	//int i;  std::cin >> i;
}
