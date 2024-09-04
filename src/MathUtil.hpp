#ifndef _MATHUTIL_HPP_
#define _MATHUTIL_HPP_

#include <cstddef>
#include <cmath>

namespace cgel {

    constexpr float PI = 3.1415926535897932384626433f;
    constexpr float HALF_PI = PI / 2.f;

    float radians(const float degrees) {return degrees * (PI / 180.f);}
    float degrees(const float radians) {return radians * (180.f / PI);}

    template<typename Type, size_t Rows, size_t Cols>
    class Matrix {

        protected:
            Type m_data[Rows][Cols];
        
        public:

            Matrix() {
                for (size_t i = 0; i < Rows; i++)
                    for (size_t j = 0; j < Cols; j++)
                        m_data[i][j] = 0;
            }

            Matrix<Type, Cols, Rows> T() const {
                Matrix<Type, Cols, Rows> trans;
                for (size_t i = 0; i < Rows; i++)
                    for (size_t j = 0; j < Cols; j++)
                        trans[j][i] = m_data[i][j];
                return trans;
            }

            Matrix<Type, Cols, Rows> transposed() const {
                Matrix<Type, Cols, Rows> trans;
                for (size_t i = 0; i < Rows; i++)
                    for (size_t j = 0; j < Cols; j++)
                        trans[j][i] = m_data[i][j];
                return trans;
            }


            Type *operator[](const size_t i) {return m_data[i];}
            const Type *operator[](const size_t i) const {return m_data[i];}


            Matrix &operator= (const Matrix &t) {
                for (size_t i = 0; i < Rows; i++)
                    for (size_t j = 0; j < Cols; j++)
                        m_data[i][j] = t[i][j];
                return *this;
            }


            Matrix operator+ (const Matrix &t) const {
                Matrix add;
                for (size_t i = 0; i < Rows; i++)
                    for (size_t j = 0; j < Cols; j++)
                        add[i][j] = m_data[i][j] + t[i][j];
                return add;
            }

            Matrix operator- (const Matrix &t) const {
                Matrix sub;
                for (size_t i = 0; i < Rows; i++)
                    for (size_t j = 0; j < Cols; j++)
                        sub[i][j] = m_data[i][j] - t[i][j];
                return sub;
            }

            Matrix operator* (const Type s) const {
                Matrix scale;
                for (size_t i = 0; i < Rows; i++)
                    for (size_t j = 0; j < Cols; j++)
                        scale[i][j] = m_data[i][j] * s;
                return scale;
            }

            friend Matrix operator* (const Type s, const Matrix &t) {
                Matrix scale;
                for (size_t i = 0; i < Rows; i++)
                    for (size_t j = 0; j < Cols; j++)
                        scale[i][j] = t[i][j] * s;
                return scale;
            }

            Matrix operator/ (const Type s) const {
                Matrix scale;
                for (size_t i = 0; i < Rows; i++)
                    for (size_t j = 0; j < Cols; j++)
                        scale[i][j] = m_data[i][j] / s;
                return scale;
            }

            Matrix &operator+=(const Matrix &t) {
                return *this = *this + t;
            }

            Matrix &operator-=(const Matrix &t) {
                return *this = *this - t;
            }

            Matrix &operator*=(const Type t) {
                return *this = *this * t;
            }

            Matrix &operator/=(const Type t) {
                return *this = *this / t;
            }

            // * Matrix multiplication method *
            template<size_t R, size_t C, std::enable_if_t<(Cols == R)> * = nullptr>
            Matrix<Type, Rows, C> operator* (const Matrix<Type, R, C> &t) const {
                Matrix<Type, Rows, C> mul;
                for (size_t i = 0; i < Rows; i++) {
                    for (size_t j = 0; j < C; j++) {
                        mul[i][j] = 0;
                        for (size_t k = 0; k < Cols; k++) 
                            mul[i][j] += m_data[i][k] * t[k][j];
                    }
                }
                return mul;
            }



            Matrix &assign(const Matrix &t) {
                for (size_t i = 0; i < Rows; i++)
                    for (size_t j = 0; j < Cols; j++)
                        m_data[i][j] = t[i][j];
                return *this;
            }


            Matrix add(const Matrix &t) const {
                Matrix add;
                for (size_t i = 0; i < Rows; i++)
                    for (size_t j = 0; j < Cols; j++)
                        add[i][j] = m_data[i][j] + t[i][j];
                return add;
            }

            Matrix subtract(const Matrix &t) const {
                Matrix sub;
                for (size_t i = 0; i < Rows; i++)
                    for (size_t j = 0; j < Cols; j++)
                        sub[i][j] = m_data[i][j] - t[i][j];
                return sub;
            }

            Matrix multiply(const Type s) const {
                Matrix scale;
                for (size_t i = 0; i < Rows; i++)
                    for (size_t j = 0; j < Cols; j++)
                        scale[i][j] = m_data[i][j] * s;
                return scale;
            }

            friend Matrix multiply(const Type s, const Matrix &t) {
                Matrix scale;
                for (size_t i = 0; i < Rows; i++)
                    for (size_t j = 0; j < Cols; j++)
                        scale[i][j] = t[i][j] * s;
                return scale;
            }

            Matrix divide(const Type s) const {
                Matrix scale;
                for (size_t i = 0; i < Rows; i++)
                    for (size_t j = 0; j < Cols; j++)
                        scale[i][j] = m_data[i][j] / s;
                return scale;
            }

            Matrix &add_assign(const Matrix &t) {
                return *this = *this + t;
            }

            Matrix &subtract_assign(const Matrix &t) {
                return *this = *this - t;
            }

            Matrix &multiply_assign(const Type t) {
                return *this = *this * t;
            }

            Matrix &divide_assign(const Type t) {
                return *this = *this / t;
            }

            // * Matrix multiplication method *
            template<size_t R, size_t C, std::enable_if_t<(Cols == R)> * = nullptr>
            Matrix<Type, Rows, C> multiply(const Matrix<Type, R, C> &t) const {
                Matrix<Type, Rows, C> mul;
                for (size_t i = 0; i < Rows; i++) {
                    for (size_t j = 0; j < C; j++) {
                        mul[i][j] = 0;
                        for (size_t k = 0; k < Cols; k++) 
                            mul[i][j] += m_data[i][k] * t[k][j];
                    }
                }
                return mul;
            }



            
    };




    template<typename Type>
    class Matrix<Type, 1, 2> {

        private:
            Type m_data[1][2];
        
        public:

            Matrix() = default;

            Matrix(const Type x, const Type y) {
                this->m_data[0][0] = x;
                this->m_data[0][1] = y;
            }

            Matrix(const Matrix<Type, 1, 2> &t) {
                this->m_data[0][0] = t[0][0];
                this->m_data[0][1] = t[0][1];
            }

            Type *operator[](const size_t i) {return m_data[i];}
            const Type *operator[](const size_t i) const {return m_data[i];}

            Type &X() {return this->m_data[0][0];}
            Type X() const {return this->m_data[0][0];}
            Type &Y() {return this->m_data[0][1];}
            Type Y() const {return this->m_data[0][1];}



            Matrix &operator= (const Matrix &t) {
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 2; j++)
                        m_data[i][j] = t[i][j];
                return *this;
            }

            Matrix operator+ (const Matrix &t) const {
                Matrix add;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 2; j++)
                        add[i][j] = m_data[i][j] + t[i][j];
                return add;
            }

            Matrix operator- (const Matrix &t) const {
                Matrix sub;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 2; j++)
                        sub[i][j] = m_data[i][j] - t[i][j];
                return sub;
            }

            Matrix operator* (const Type s) const {
                Matrix scale;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 2; j++)
                        scale[i][j] = m_data[i][j] * s;
                return scale;
            }

            friend Matrix operator* (const Type s, const Matrix &t) {
                Matrix scale;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 2; j++)
                        scale[i][j] = t[i][j] * s;
                return scale;
            }

            Matrix operator/ (const Type s) const {
                Matrix scale;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 2; j++)
                        scale[i][j] = m_data[i][j] / s;
                return scale;
            }

            Matrix &operator+=(const Matrix &t) {
                return *this = *this + t;
            }

            Matrix &operator-=(const Matrix &t) {
                return *this = *this - t;
            }

            Matrix &operator*=(const Type t) {
                return *this = *this * t;
            }

            Matrix &operator/=(const Type t) {
                return *this = *this / t;
            }

            // * Matrix multiplication method *
            template<size_t R, size_t C, std::enable_if_t<(2 == R)> * = nullptr>
            Matrix<Type, 1, C> operator* (const Matrix<Type, R, C> &t) const {
                Matrix<Type, 1, C> mul;
                for (size_t i = 0; i < 1; i++) {
                    for (size_t j = 0; j < C; j++) {
                        mul[i][j] = 0;
                        for (size_t k = 0; k < 2; k++) 
                            mul[i][j] += m_data[i][k] * t[k][j];
                    }
                }
                return mul;
            }

            Matrix operator*=(const Matrix<Type, 2, 2> &t) {
                return *this = *this * t;
            }

            Matrix &assign (const Matrix &t) {
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 2; j++)
                        m_data[i][j] = t[i][j];
                return *this;
            }

            Matrix add (const Matrix &t) const {
                Matrix add;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 2; j++)
                        add[i][j] = m_data[i][j] + t[i][j];
                return add;
            }

            Matrix subtract (const Matrix &t) const {
                Matrix sub;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 2; j++)
                        sub[i][j] = m_data[i][j] - t[i][j];
                return sub;
            }

            Matrix multiply (const Type s) const {
                Matrix scale;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 2; j++)
                        scale[i][j] = m_data[i][j] * s;
                return scale;
            }

            Matrix divide (const Type s) const {
                Matrix scale;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 2; j++)
                        scale[i][j] = m_data[i][j] / s;
                return scale;
            }

            Matrix &add_assign(const Matrix &t) {
                return *this = *this + t;
            }

            Matrix &subtract_assign(const Matrix &t) {
                return *this = *this - t;
            }

            Matrix &multiply_assign(const Type t) {
                return *this = *this * t;
            }

            Matrix &divide_assign(const Type t) {
                return *this = *this / t;
            }

            // * Matrix multiplication method *
            template<size_t R, size_t C, std::enable_if_t<(2 == R)> * = nullptr>
            Matrix<Type, 1, C> multiply (const Matrix<Type, R, C> &t) const {
                Matrix<Type, 1, C> mul;
                for (size_t i = 0; i < 1; i++) {
                    for (size_t j = 0; j < C; j++) {
                        mul[i][j] = 0;
                        for (size_t k = 0; k < 2; k++) 
                            mul[i][j] += m_data[i][k] * t[k][j];
                    }
                }
                return mul;
            }

            Matrix multiply_assign(const Matrix<Type, 2, 2> &t) {
                return *this = *this * t;
            }



    };






    template<typename Type>
    class Matrix<Type, 1, 3> {

        private:
            Type m_data[1][3];
        
        public:

            Matrix() = default;

            Matrix(const Type x, const Type y, const Type z) {
                this->m_data[0][0] = x;
                this->m_data[0][1] = y;
                this->m_data[0][2] = z;
            }

            Matrix(const Matrix<Type, 1, 3> &t) {
                this->m_data[0][0] = t[0][0];
                this->m_data[0][1] = t[0][1];
                this->m_data[0][2] = t[0][2];
            }

            Type *operator[](const size_t i) {return m_data[i];}
            const Type *operator[](const size_t i) const {return m_data[i];}

            Type &X() {return this->m_data[0][0];}
            Type X() const {return this->m_data[0][0];}
            Type &Y() {return this->m_data[0][1];}
            Type Y() const {return this->m_data[0][1];}
            Type &Z() {return this->m_data[0][2];}
            Type Z() const {return this->m_data[0][2];}



            Matrix &operator= (const Matrix &t) {
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 3; j++)
                        m_data[i][j] = t[i][j];
                return *this;
            }

            Matrix operator+ (const Matrix &t) const {
                Matrix add;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 3; j++)
                        add[i][j] = m_data[i][j] + t[i][j];
                return add;
            }

            Matrix operator- (const Matrix &t) const {
                Matrix sub;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 3; j++)
                        sub[i][j] = m_data[i][j] - t[i][j];
                return sub;
            }

            Matrix operator* (const Type s) const {
                Matrix scale;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 3; j++)
                        scale[i][j] = m_data[i][j] * s;
                return scale;
            }

            friend Matrix operator* (const Type s, const Matrix &t) {
                Matrix scale;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 3; j++)
                        scale[i][j] = t[i][j] * s;
                return scale;
            }

            Matrix operator/ (const Type s) const {
                Matrix scale;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 3; j++)
                        scale[i][j] = m_data[i][j] / s;
                return scale;
            }

            Matrix &operator+=(const Matrix &t) {
                return *this = *this + t;
            }

            Matrix &operator-=(const Matrix &t) {
                return *this = *this - t;
            }

            Matrix &operator*=(const Type t) {
                return *this = *this * t;
            }

            Matrix &operator/=(const Type t) {
                return *this = *this / t;
            }

            // * Matrix multiplication method *
            template<size_t R, size_t C, std::enable_if_t<(3 == R)> * = nullptr>
            Matrix<Type, 1, C> operator* (const Matrix<Type, R, C> &t) const {
                Matrix<Type, 1, C> mul;
                for (size_t i = 0; i < 1; i++) {
                    for (size_t j = 0; j < C; j++) {
                        mul[i][j] = 0;
                        for (size_t k = 0; k < 3; k++) 
                            mul[i][j] += m_data[i][k] * t[k][j];
                    }
                }
                return mul;
            }

            Matrix operator*=(const Matrix<Type, 3, 3> &t) {
                return *this = *this * t;
            }

            Matrix &assign (const Matrix &t) {
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 3; j++)
                        m_data[i][j] = t[i][j];
                return *this;
            }

            Matrix add (const Matrix &t) const {
                Matrix add;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 3; j++)
                        add[i][j] = m_data[i][j] + t[i][j];
                return add;
            }

            Matrix subtract (const Matrix &t) const {
                Matrix sub;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 3; j++)
                        sub[i][j] = m_data[i][j] - t[i][j];
                return sub;
            }

            Matrix multiply (const Type s) const {
                Matrix scale;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 3; j++)
                        scale[i][j] = m_data[i][j] * s;
                return scale;
            }

            Matrix divide (const Type s) const {
                Matrix scale;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 3; j++)
                        scale[i][j] = m_data[i][j] / s;
                return scale;
            }

            Matrix &add_assign(const Matrix &t) {
                return *this = *this + t;
            }

            Matrix &subtract_assign(const Matrix &t) {
                return *this = *this - t;
            }

            Matrix &multiply_assign(const Type t) {
                return *this = *this * t;
            }

            Matrix &divide_assign(const Type t) {
                return *this = *this / t;
            }

            // * Matrix multiplication method *
            template<size_t R, size_t C, std::enable_if_t<(3 == R)> * = nullptr>
            Matrix<Type, 1, C> multiply (const Matrix<Type, R, C> &t) const {
                Matrix<Type, 1, C> mul;
                for (size_t i = 0; i < 1; i++) {
                    for (size_t j = 0; j < C; j++) {
                        mul[i][j] = 0;
                        for (size_t k = 0; k < 3; k++) 
                            mul[i][j] += m_data[i][k] * t[k][j];
                    }
                }
                return mul;
            }

            Matrix multiply_assign(const Matrix<Type, 3, 3> &t) {
                return *this = *this * t;
            }



            Matrix<Type, 3, 1> T() const {
                Matrix<Type, 3, 1> trans;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 3; j++)
                        trans[j][i] = m_data[i][j];
                return trans;
            }

            Matrix<Type, 3, 1> transposed() const {
                Matrix<Type, 3, 1> trans;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 3; j++)
                        trans[j][i] = m_data[i][j];
                return trans;
            }


            Type dot(const Matrix &v) const {
                return X() * v.X() + Y() * v.Y() + Z() * v.Z();
            }

            Matrix cross(const Matrix &v) const {
                Matrix prod;
                prod.X() = Y() * v.Z() - Z() * v.Y();
                prod.Y() = Z() * v.X() - X() * v.Z();
                prod.Z() = X() * v.Y() - Y() * v.X();
                return prod;
            }

            Type norm() const {
                return sqrtf(X() * X() + Y() * Y() + Z() * Z());
            }

            Type norm(const Matrix &v) const {
                return sqrtf((X() - v.X()) * (X() - v.X()) + (Y() - v.Y()) * (Y() - v.Y()) + (Z() - v.Z()) * (Z() - v.Z()));
            }

            Matrix &normalize() {
                const Type scalar = norm();
                X() /= scalar;
                Y() /= scalar;
                Z() /= scalar;
                return *this;
            }

            Matrix unit() const {
                Matrix v;
                const Type scalar = norm();
                v.X() = X() / scalar;
                v.Y() = Y() / scalar;
                v.Z() = Z() / scalar;
                return v;
            }



    };






    // 4D Vec (1x4 Matrix/Tensor)
    // * H methods treat the Vec4D as homogenous *
    template<typename Type>
    class Matrix<Type, 1, 4> {

        private:
            Type m_data[1][4];

        public:

            Matrix() = default;

            Matrix(const Type x, const Type y, const Type z) {
                this->m_data[0][0] = x;
                this->m_data[0][1] = y;
                this->m_data[0][2] = z;
                this->m_data[0][3] = 1;
            }

            Matrix(const Type x, const Type y, const Type z, const Type w) {
                this->m_data[0][0] = x;
                this->m_data[0][1] = y;
                this->m_data[0][2] = z;
                this->m_data[0][3] = w;
            }

            Matrix(const Matrix<Type, 1, 4> &t) {
                this->m_data[0][0] = t[0][0];
                this->m_data[0][1] = t[0][1];
                this->m_data[0][2] = t[0][2];
                this->m_data[0][3] = t[0][3];
            }

            Type *operator[](const size_t i) {return m_data[i];}
            const Type *operator[](const size_t i) const {return m_data[i];}

            Type &X() {return this->m_data[0][0];}
            Type X() const {return this->m_data[0][0];}
            Type &Y() {return this->m_data[0][1];}
            Type Y() const {return this->m_data[0][1];}
            Type &Z() {return this->m_data[0][2];}
            Type Z() const {return this->m_data[0][2];}
            Type &W() {return this->m_data[0][3];}
            Type W() const {return this->m_data[0][3];}

            Matrix &operator= (const Matrix &t) {
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 4; j++)
                        m_data[i][j] = t[i][j];
                return *this;
            }

            Matrix operator+ (const Matrix &t) const {
                Matrix add;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 4; j++)
                        add[i][j] = m_data[i][j] + t[i][j];
                return add;
            }

            Matrix operator- (const Matrix &t) const {
                Matrix sub;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 4; j++)
                        sub[i][j] = m_data[i][j] - t[i][j];
                return sub;
            }

            Matrix operator* (const Type s) const {
                Matrix scale;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 4; j++)
                        scale[i][j] = m_data[i][j] * s;
                return scale;
            }

            friend Matrix operator* (const Type s, const Matrix &t) {
                Matrix scale;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 4; j++)
                        scale[i][j] = t[i][j] * s;
                return scale;
            }

            Matrix operator/ (const Type s) const {
                Matrix scale;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 4; j++)
                        scale[i][j] = m_data[i][j] / s;
                return scale;
            }

            Matrix &operator+=(const Matrix &t) {
                return *this = *this + t;
            }

            Matrix &operator-=(const Matrix &t) {
                return *this = *this - t;
            }

            Matrix &operator*=(const Type t) {
                return *this = *this * t;
            }

            Matrix &operator/=(const Type t) {
                return *this = *this / t;
            }

            // * Matrix multiplication method *
            template<size_t R, size_t C, std::enable_if_t<(4 == R)> * = nullptr>
            Matrix<Type, 1, C> operator* (const Matrix<Type, R, C> &t) const {
                Matrix<Type, 1, C> mul;
                for (size_t i = 0; i < 1; i++) {
                    for (size_t j = 0; j < C; j++) {
                        mul[i][j] = 0;
                        for (size_t k = 0; k < 4; k++) 
                            mul[i][j] += m_data[i][k] * t[k][j];
                    }
                }
                return mul;
            }

            Matrix operator*=(const Matrix<Type, 4, 4> &t) {
                return *this = *this * t;
            }

            Matrix &assign (const Matrix &t) {
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 4; j++)
                        m_data[i][j] = t[i][j];
                return *this;
            }

            Matrix add (const Matrix &t) const {
                Matrix add;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 4; j++)
                        add[i][j] = m_data[i][j] + t[i][j];
                return add;
            }

            Matrix subtract (const Matrix &t) const {
                Matrix sub;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 4; j++)
                        sub[i][j] = m_data[i][j] - t[i][j];
                return sub;
            }

            Matrix multiply (const Type s) const {
                Matrix scale;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 4; j++)
                        scale[i][j] = m_data[i][j] * s;
                return scale;
            }

            Matrix divide (const Type s) const {
                Matrix scale;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 4; j++)
                        scale[i][j] = m_data[i][j] / s;
                return scale;
            }

            Matrix &add_assign(const Matrix &t) {
                return *this = *this + t;
            }

            Matrix &subtract_assign(const Matrix &t) {
                return *this = *this - t;
            }

            Matrix &multiply_assign(const Type t) {
                return *this = *this * t;
            }

            Matrix &divide_assign(const Type t) {
                return *this = *this / t;
            }

            // * Matrix multiplication method *
            template<size_t R, size_t C, std::enable_if_t<(4 == R)> * = nullptr>
            Matrix<Type, 1, C> multiply (const Matrix<Type, R, C> &t) const {
                Matrix<Type, 1, C> mul;
                for (size_t i = 0; i < 1; i++) {
                    for (size_t j = 0; j < C; j++) {
                        mul[i][j] = 0;
                        for (size_t k = 0; k < 4; k++) 
                            mul[i][j] += m_data[i][k] * t[k][j];
                    }
                }
                return mul;
            }

            Matrix multiply_assign(const Matrix<Type, 4, 4> &t) {
                return *this = *this * t;
            }

            Matrix addH(const Matrix &t) const {
                return {X() + t.X(), Y() + t.Y(), Z() + t.Z(), W()};
            }

            Matrix subtractH(const Matrix &t) const {
                return {X() - t.X(), Y() - t.Y(), Z() - t.Z(), W()};
            }

            Matrix multiplyH(const Type s) const {
                return {X() * s, Y() * s, Z() * s, W()};
            }

            friend Matrix multiplyH(const Type s, const Matrix &t) {
                return {t.X() * s, t.Y() * s, t.Z() * s, t.W()};
            }

            Matrix divideH(const Type s) const {
                return {X() / s, Y() / s, Z() / s, W()};
            }

            Matrix &addH_assign(const Matrix &t) {
                X() += t.X();
                Y() += t.Y();
                Z() += t.Z();
                return *this;
            }

            Matrix &subtractH_assign(const Matrix &t) {
                X() -= t.X();
                Y() -= t.Y();
                Z() -= t.Z();
                return *this;
            }

            Matrix &multiplyH_assign(const Type s) {
                X() *= s;
                Y() *= s;
                Z() *= s;
                return *this;
            }

            Matrix &divideH_assign(const Type s) {
                X() /= s;
                Y() /= s;
                Z() /= s;
                return *this;
            }



            Matrix<Type, 4, 1> T() const {
                Matrix<Type, 4, 1> trans;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 4; j++)
                        trans[j][i] = m_data[i][j];
                return trans;
            }

            Matrix<Type, 4, 1> transposed() const {
                Matrix<Type, 4, 1> trans;
                for (size_t i = 0; i < 1; i++)
                    for (size_t j = 0; j < 4; j++)
                        trans[j][i] = m_data[i][j];
                return trans;
            }

            Matrix &mapW() {
                const Type w = this->m_data[0][3];
                this->m_data[0][0] /= w;
                this->m_data[0][1] /= w;
                this->m_data[0][2] /= w;
                this->m_data[0][3] = 1;
                return *this;
            }

            Matrix mappedW() const {
                Matrix map;
                const Type w = this->m_data[0][3];
                map[0][0] = this->m_data[0][0] / w;
                map[0][1] = this->m_data[0][1] / w;
                map[0][2] = this->m_data[0][2] / w;
                map[0][3] = 1;
                return map;
            }

            Type dotH(const Matrix &v) const {
                return X() * v.X() + Y() * v.Y() + Z() * v.Z();
            }

            Matrix crossH(const Matrix &v) const {
                Matrix prod;
                prod.X() = Y() * v.Z() - Z() * v.Y();
                prod.Y() = Z() * v.X() - X() * v.Z();
                prod.Z() = X() * v.Y() - Y() * v.X();
                return prod;
            }

            Type normH() const {
                return sqrtf(X() * X() + Y() * Y() + Z() * Z());
            }

            Type normH(const Matrix &v) const {
                return sqrtf((X() - v.X()) * (X() - v.X()) + (Y() - v.Y()) * (Y() - v.Y()) + (Z() - v.Z()) * (Z() - v.Z()));
            }

            Matrix &normalizeH() {
                const Type scalar = normH();
                X() /= scalar;
                Y() /= scalar;
                Z() /= scalar;
                return *this;
            }

            Matrix unitH() const {
                Matrix v;
                const Type scalar = normH();
                v.X() = X() / scalar;
                v.Y() = Y() / scalar;
                v.Z() = Z() / scalar;
                v.W() = 1;
                return v;
            }

            
    };

    // Type definitions
    template<typename Type> using Vec2 = Matrix<Type, 1, 2>;
    template<typename Type> using Vec3 = Matrix<Type, 1, 3>;
    template<typename Type> using Vec4 = Matrix<Type, 1, 4>;

    using Vec2u = Vec2<unsigned>;
    using Vec2i = Vec2<int>;
    using Vec2f = Vec2<float>;

    using Vec3u = Vec3<unsigned>;
    using Vec3i = Vec3<int>;
    using Vec3f = Vec3<float>;

    using Vec4u = Vec4<unsigned>;
    using Vec4i = Vec4<int>;
    using Vec4f = Vec4<float>;

    // Convenient methods for building matrices.
    template<typename Type, size_t N>
    Matrix<Type, N, N> make_identity() {
        Matrix<Type, N, N> m;
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                if (i == j) m[i][j] = 1;
                else        m[i][j] = 0;
            }
        } 
        return m;  
    }

    template<typename Type, size_t N>
    Matrix<Type, N, N> make_identity_like(const Matrix<Type, N, N> &) {
        Matrix<Type, N, N> m;
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                if (i == j) m[i][j] = 1;
                else        m[i][j] = 0;
            }
        } 
        return m;  
    }

    template<typename Type>
    Matrix<Type, 4, 4> make_rotationX_4x4(const Type theta) {
        Matrix<Type, 4, 4> m;
        const Type sin_theta = sinf(theta);
        const Type cos_theta = cosf(theta);
        m[0][0] = 1;
        m[1][1] = cos_theta;
        m[2][2] = cos_theta;
        m[3][3] = 1;
        m[1][2] = -sin_theta;
        m[2][1] = sin_theta;
        return m;
    }

    template<typename Type>
    Matrix<Type, 4, 4> make_rotationX_4x4(const Type sin_theta, const Type cos_theta) {
        Matrix<Type, 4, 4> m;
        m[0][0] = 1;
        m[1][1] = cos_theta;
        m[2][2] = cos_theta;
        m[3][3] = 1;
        m[1][2] = -sin_theta;
        m[2][1] = sin_theta;
        return m;
    }

    template<typename Type>
    Matrix<Type, 4, 4> make_rotationY_4x4(const Type theta) {
        Matrix<Type, 4, 4> m;
        const Type sin_theta = sinf(theta);
        const Type cos_theta = cosf(theta);
        m[0][0] = cos_theta;
        m[1][1] = 1;
        m[2][2] = cos_theta;
        m[3][3] = 1;
        m[0][2] = -sin_theta;
        m[2][0] = sin_theta;
        return m;
    }

    template<typename Type>
    Matrix<Type, 4, 4> make_rotationY_4x4(const Type sin_theta, const Type cos_theta) {
        Matrix<Type, 4, 4> m;
        m[0][0] = cos_theta;
        m[1][1] = 1;
        m[2][2] = cos_theta;
        m[3][3] = 1;
        m[0][2] = -sin_theta;
        m[2][0] = sin_theta;
        return m;
    }

    template<typename Type>
    Matrix<Type, 4, 4> make_rotationZ_4x4(const Type theta) {
        Matrix<Type, 4, 4> m;
        const Type sin_theta = sinf(theta);
        const Type cos_theta = cosf(theta);
        m[0][0] = cos_theta;
        m[1][1] = cos_theta;
        m[2][2] = 1;
        m[3][3] = 1;
        m[0][1] = -sin_theta;
        m[1][0] = sin_theta;
        return m;
    }

    template<typename Type>
    Matrix<Type, 4, 4> make_rotationZ_4x4(const Type sin_theta, const Type cos_theta) {
        Matrix<Type, 4, 4> m;
        m[0][0] = cos_theta;
        m[1][1] = cos_theta;
        m[2][2] = 1;
        m[3][3] = 1;
        m[0][1] = -sin_theta;
        m[1][0] = sin_theta;
        return m;
    }

    template<typename Type>
    Matrix<Type, 4, 4> make_translation_4x4(const Matrix<Type, 1, 4> &t) {
        Matrix<Type, 4, 4> m = make_identity<Type, 4>();
        m[0][3] = t[0][0];
        m[1][3] = t[0][1];
        m[2][3] = t[0][2];
        return m;
    }

    template<typename Type>
    Matrix<Type, 4, 4> make_scaling_4x4(const Matrix<Type, 1, 4> &t) {
        Matrix<Type, 4, 4> m;
        m[0][0] = t[0][0];
        m[1][1] = t[0][1];
        m[2][2] = t[0][2];
        m[3][3] = 1;
        return m;
    }

    template<typename Type>
    Matrix<Type, 4, 4> make_projection_4x4(const Type W, const Type H, const Type FOV = 3.14159f / 2.f, const Type z_near = 0.01, const Type z_far = 100) {
        Matrix<Type, 4, 4> m;
        const Type aspect_ratio = W / H;
        const Type focal_length = 1.f / tanf(FOV / 2.f);

        m[0][0] = focal_length / aspect_ratio;
        m[1][1] = focal_length;
        m[2][2] = z_far / (z_far - z_near);
        m[3][2] = (-z_far * z_near) / (z_far - z_near);
        m[2][3] = 1;
        return m;
    };

    template<typename Type>
    Matrix<Type, 4, 4> make_pointat_4x4(const Vec4<Type> &pos, const Vec4<Type> &target, const Vec4<Type> &up) {
        const Vec4<Type> new_forward = target.subtractH(pos).unitH();
        const Vec4<Type> up_projection = new_forward.multiplyH(up.dotH(new_forward));
        const Vec4<Type> new_up = up.subtractH(up_projection).unitH();
        const Vec4<Type> new_right = new_up.crossH(new_forward);

        Matrix<Type, 4, 4> m;
        m[0][0] = new_right.X();    m[0][1] = new_right.Y();    m[0][2] = new_right.Z();    m[0][3] = 0;
        m[1][0] = new_up.X();       m[1][1] = new_up.Y();       m[1][2] = new_up.Z();       m[1][3] = 0;
        m[2][0] = new_forward.X();  m[2][1] = new_forward.Y();  m[2][2] = new_forward.Z();  m[2][3] = 0;
        m[3][0] = pos.X();          m[3][1] = pos.Y();          m[3][2] = pos.Z();          m[3][3] = 1;
        return m;
    }

    template<typename Type>
    Matrix<Type, 4, 4> make_quick_inverse_4x4(const Matrix<Type, 4, 4> &t) {
        Matrix<Type, 4, 4> m;
        m[0][0] = t[0][0]; m[0][1] = t[1][0]; m[0][2] = t[2][0]; m[0][3] = 0;
    		m[1][0] = t[0][1]; m[1][1] = t[1][1]; m[1][2] = t[2][1]; m[1][3] = 0;
    		m[2][0] = t[0][2]; m[2][1] = t[1][2]; m[2][2] = t[2][2]; m[2][3] = 0;
    		m[3][0] = -(t[3][0] * m[0][0] + t[3][1] * m[1][0] + t[3][2] * m[2][0]);
    		m[3][1] = -(t[3][0] * m[0][1] + t[3][1] * m[1][1] + t[3][2] * m[2][1]);
    		m[3][2] = -(t[3][0] * m[0][2] + t[3][1] * m[1][2] + t[3][2] * m[2][2]);
    		m[3][3] = 1;
        return m;
    }


}



#endif
