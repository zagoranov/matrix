#include <iostream>
#include <map>
#include <cassert>
#include <iterator>


template<typename T, T defValue>
class Matrix {
	using RetTuple = std::tuple<long, long, T>;
	friend class ref;
	
	std::map<std::pair<long, long>, T> MatrixData;
	T dvalue;

public:
	Matrix() { 
		MatrixData.clear(); 
		dvalue = defValue;
	}
	~Matrix() {}
	size_t size() { return MatrixData.size(); }

	//
	class ref
	{
		Matrix& cont_;
		long row_;
		long column_;
	public:
		ref(Matrix& cont, long i) : cont_(cont), row_(i) {}
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
			column_ = i;
			return *this;
		}
		ref& operator=(T value)
		{
			auto pos = std::make_pair(row_, column_);
			auto it = cont_.MatrixData.find(pos);
			if (value == cont_.dvalue) {
				if(it != cont_.MatrixData.end())
					cont_.MatrixData.erase(it);
			}
			else
				cont_.MatrixData[pos] = value;
			return *this;
		}
	};
	ref operator [](long i)
	{
		return ref(*this, i);
	}

	// 
	class iterator {
	public:
		typename std::map<std::pair<long, long>, T>::iterator it;
		iterator(typename std::map<std::pair<long, long>, T>::iterator _it) : it(_it) {}
		bool operator!=(const iterator & other) const {	return it != other.it;	}
		iterator operator++() {	it++; 	return *this; }
		RetTuple operator*() { 
			long l1 = (*it).first.first;
			long l2 = (*it).first.second;
			return std::make_tuple<long, long, T>(std::move(l1), std::move(l2), std::move((*it).second));
		}
	};
	iterator begin() { return MatrixData.begin(); }
	iterator end() { return MatrixData.end(); }
};


int main()
{
	Matrix<int, 0> matrix;
	assert(matrix.size() == 0); 

	for (size_t i = 0; i < 10; ++i)
		matrix[i][i] = i;
	for(size_t i = 0, j = 9; i < 10; ++i, j--)
		matrix[i][j] = i;

	for (size_t i = 0; i < 10; ++i) {
		for (size_t j = 0; j < 10; ++j)
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
	std::system("pause");
}
