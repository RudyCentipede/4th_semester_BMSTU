#include <iostream>
#include <vector>
#include "Matrix.h"

int main()
{
    try
    {
        std::cout << "--------------CONSTRUCTORS--------------\n";
        Matrix<float> mtrx1(3, 3);
        mtrx1.fill(0);
        std::cout << "Null matrix:\n" << mtrx1;

        Matrix<float> mtrx2(mtrx1);
        std::cout << "Copy constructor:\n" << mtrx2;

        Matrix<float> mtrx3({ { 5.2f, 2.5f, 3.8f }, { 1.2f, 2.3f, 3.2f } });
        std::cout << "Matrix from initializer_list:\n" << mtrx3;

        std::vector<float> vec = {1, 2, 3, 4, 5, 6};
        Matrix<float> mtrx4(vec, 2, 3);
        std::cout << "Matrix from std::vector:\n" << mtrx4;

        std::cout << "--------------BASIC OPERATIONS--------------\n";
        mtrx1.fill(0);
        for (size_t i = 0; i < mtrx1.rowCount(); ++i)
            mtrx1[i][i] = 1;
        std::cout << "Identity matrix:\n" << mtrx1;

        mtrx2.fill(0);
        for (size_t i = 0; i < mtrx2.rowCount(); ++i)
            mtrx2[i][i] = 2;
        std::cout << "Diagonal matrix:\n" << mtrx2;

        std::cout << "mtrx1 + mtrx2:\n" << (mtrx1 + mtrx2);

        std::cout << "mtrx2 - mtrx1:\n" << (mtrx2 - mtrx1);

        std::cout << "mtrx1 * 3:\n" << (mtrx1 * 3);

        std::cout << "mtrx2 / 2:\n" << (mtrx2 / 2);

        std::cout << "--------------ASSIGNMENT OPERATORS--------------\n";
        mtrx1 += mtrx2;
        std::cout << "mtrx1 += mtrx2:\n" << mtrx1;

        mtrx1 -= mtrx2;
        std::cout << "mtrx1 -= mtrx2:\n" << mtrx1;

        mtrx1 *= 2;
        std::cout << "mtrx1 *= 2:\n" << mtrx1;

        mtrx1 /= 2;
        std::cout << "mtrx1 /= 2:\n" << mtrx1;

        std::cout << "--------------ELEMENT ACCESS--------------\n";
        std::cout << "mtrx3(1, 0) = " << mtrx3(1, 0) << std::endl;
        mtrx3(1, 0) = 42.0f;
        std::cout << "After set mtrx3(1, 0) = 42:\n" << mtrx3;

        std::cout << "--------------ITERATORS--------------\n";
        std::cout << "Matrix 3 (iterator):\n";
        auto iter = mtrx3.begin();
        for (size_t i = 0; iter != mtrx3.end(); ++i, ++iter)
        {
            if (i > 0 && i % mtrx3.colCount() == 0)
                std::cout << std::endl;
            std::cout << *iter << ' ';
        }
        std::cout << std::endl;

        std::cout << "Matrix 3 (const_iterator):\n";
        const Matrix<float>& const_mtrx3 = mtrx3;
        auto citer = const_mtrx3.cbegin();
        for (size_t i = 0; citer != const_mtrx3.cend(); ++i, ++citer)
        {
            if (i > 0 && i % const_mtrx3.colCount() == 0)
                std::cout << std::endl;
            std::cout << *citer << ' ';
        }
        std::cout << std::endl;

        std::cout << "--------------RESIZE/RESHAPE--------------\n";
        mtrx3.reshape(3, 2);
        std::cout << "Reshaped mtrx3 (3x2):\n" << mtrx3;

        mtrx3.resize(2, 2, 0.0f);
        std::cout << "Resized mtrx3 (2x2):\n" << mtrx3;

        std::cout << "--------------ZERO & IDENTITY CHECK--------------\n";
        std::cout << "mtrx1.isZero(): " << mtrx1.isZero() << std::endl;
        std::cout << "mtrx1.isIdentity(): " << mtrx1.isIdentity() << std::endl;

        std::cout << "--------------DETERMINANT & INVERSE--------------\n";
        Matrix<float> mtrx5({{1, 2, -3}, {4, -5, -6}, {7, -8, 9}});
        std::cout << "Matrix 5:\n" << mtrx5;
        std::cout << "det(mtrx5) = " << mtrx5.determinant() << std::endl;
        std::cout << "Inverse of mtrx5:\n" << mtrx5.inverse();

        std::cout << "--------------DOT PRODUCT--------------\n";
        Matrix<float> mtrx6({{1, 2}, {3, 4}});
        Matrix<float> mtrx7({{2, 0}, {1, 2}});
        std::cout << "mtrx6:\n" << mtrx6;
        std::cout << "mtrx7:\n" << mtrx7;
        std::cout << "mtrx6.dot(mtrx7):\n" << mtrx6.dot(mtrx7);

        std::cout << "--------------COMPARISON--------------\n";
        std::cout << "mtrx6 == mtrx7: " << (mtrx6 == mtrx7) << std::endl;
        std::cout << "mtrx6 != mtrx7: " << (mtrx6 != mtrx7) << std::endl;
    }
    catch (const MatrixException& ex)
    {
        std::cout << ex.what();
    }
    return 0;
}
