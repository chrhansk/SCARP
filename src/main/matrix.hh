#ifndef MATRIX_HH
#define MATRIX_HH

#include <vector>

#include "util.hh"

template<class T>
class Matrix
{
public:
  typedef typename std::vector<T>::const_reference ConstRef;
  typedef typename std::vector<T>::reference Ref;
private:
  const idx _num_rows;
  const idx _num_cols;
  std::vector<T> values;

  idx index(idx i, idx j) const
  {
    return i*_num_cols + j;
  }

public:
  Matrix(idx num_rows, idx num_cols, const T& value = T())
    : _num_rows(num_rows),
      _num_cols(num_cols),
      values(num_rows*num_cols, value)
  {
  }

  idx num_rows() const
  {
    return _num_rows;
  }

  idx num_cols() const
  {
    return _num_cols;
  }

  ConstRef operator()(const idx i, const idx j) const
  {
    return values.at(index(i, j));
  }

  Ref operator()(const idx i, const idx j)
  {
    return values.at(index(i, j));
  }


};

#endif /* MATRIX_HH */