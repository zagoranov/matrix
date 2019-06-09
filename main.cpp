#include <iostream>
#include <map>
#include <cassert>
#include <iterator>


template<typename T, T defValue, size_t N>
class Matrix {
	using RetTuple = std::tuple<long, long, T>;
	std::map<std::pair<long, long>, T> MatrixData;
	T dvalue;

public:
	size_t iN;

	Matrix() { 
		dvalue = defValue;
		iN = N;
	}
	~Matrix() {}
	size_t size() const { return MatrixData.size(); }


	class ref
	{
		Matrix& cont_;
		long row_;
		long column_;
	public:
		ref(Matrix& cont, long i) : cont_(cont), row_(i),column_(-1) {}
		operator const T& () const { 
			auto pos = std::make_pair(row_, column_);
			auto it = cont_.MatrixData.find(pos);
			if (it == cont_.MatrixData.end())
				return cont_.dvalue;
			else 
				return it->second;
		}
		ref& operator [](long i)
		{
			if (column_ > -1) {
				throw std::overflow_error("Too much index arguments");
			}
			else {
				column_ = i;
			}
			return *this;
		}
		ref& operator=(T value)
		{
			if(column_ < 0)
				throw std::overflow_error("Not enough index arguments");
			auto pos = std::make_pair(row_, column_);
			auto it = cont_.MatrixData.find(pos);
			if (it != cont_.MatrixData.end()) {
				if (value == cont_.dvalue)
					cont_.MatrixData.erase(it);
				else
					(*it).second = value;
			}
			else
				if (value != cont_.dvalue)
					cont_.MatrixData.insert({ pos, value });
			return *this;
		}
	};
	ref operator [](long i)
	{
		return ref(*this, i);
	}

	class iterator {
	public:
		typename std::map<std::pair<long, long>, T>::iterator it;
		iterator(typename std::map<std::pair<long, long>, T>::iterator _it) : it(_it) {}
		bool operator!=(const iterator & other) const {	return it != other.it;	}
		iterator operator++() {	++it; 	return *this; }
		RetTuple operator*() {
			long l1 = (*it).first.first;
			long l2 = (*it).first.second;
			return std::make_tuple(l1, l2, (*it).second);
		}
	};
	iterator begin() { return MatrixData.begin(); }
	iterator end() { return MatrixData.end(); }
};



int main()
{
	Matrix<int, 0, 2> matrix;
	assert(matrix.size() == 0); 

	for (size_t i = 0; i < 10; ++i)
		matrix[i][i] = i;
	for(size_t i = 0, j = 9; i < 10; ++i, j--)
		matrix[i][j] = i;

	for (size_t i = 1; i < 9; ++i) {
		for (size_t j = 1; j < 9; ++j)
			std::cout << matrix[i][j] << " ";
		std::cout << std::endl;
	}

	std::cout << "Size: " << matrix.size() << std::endl;

	for (auto c : matrix)
	{
		int x;
		int y;
		int v;
		std::tie(x, y, v) = c;
		std::cout << "[" << x <<"]["<< y << "] = " << v << "; ";
	}


	//int i;  std::cin >> i;
}
