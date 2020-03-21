//
// Created by Aryan on 10/28/18.
//

#ifndef GAME_UNIFORM_H
#define GAME_UNIFORM_H

#include <iostream>
#include <ostream>

#include "system.h"

template<u32 A, u32 B, typename T = f32>
struct Uniform {
    T data[A * B];

    static const u32 r{A};
    static const u32 c{B};
    static const u32 n{A * B};

    template<typename... Args>
    Uniform(Args const &... a) {
        T unpack[] = {static_cast<T>(a)...};
        auto z = sizeof...(a);
        for (auto i = 0; i < n; i++) {
            data[i] = i < z ? unpack[i] : 0.0f;
        }

    }

    Uniform(const Uniform &a) {
        for (auto i = 0; i < n; i++) {
            data[i] = a[i];
        }
    }

    Uniform(const Uniform &&o) noexcept {
        *this = std::move(o);
    }


    T operator[](const u8 i) const {
        return data[i];
    }

    T &operator[](const u8 i) {
        return data[i];
    }

    T *column(const u8 i) {
        return data + (i * c);
    }


    static const Uniform<B, A, T> Transpose(const Uniform<A, B, T> &a) {
        auto x = Uniform<B, A, T>{};
        for (auto i = 0; i < x.n; i++) {
            auto ix = i % x.c;
            auto jx = i / x.c;
            x[i] = a[jx + x.r * ix];
        }
        return x;
    };

    friend Uniform<B, A, T> operator~(const Uniform<A, B, T> &a) {
        return Transpose(a);
    };



    friend bool operator==(const Uniform &a, const Uniform &b) {
        if(a.n != b.n)
            return false;
        for (auto i = 0; i < a.n; i++) {
            if(a[i] != b[i])
                return false;
        }
        return true;
    };

    Uniform<A, B, T> &operator=(const Uniform<A, B, T> &a) {
        for (auto i = 0; i < this->n; i++) {
            this->data[i] = a[i];
        }
        return *this;
    };
    Uniform<A, B, T> &operator=(const Uniform<A, B, T> &&a) {
        for (auto i = 0; i < this->n; i++) {
            this->data[i] = a[i];
        }
        return *this;
    };

    friend Uniform<A, B, T> operator+(const Uniform<A, B, T> &a, const Uniform<A, B, T> &b) {
        auto x = Uniform<A, B, T>{};
        for (auto i = 0; i < x.n; i++) {
            x[i] = a[i] + b[i];
        }
        return x;
    };


    friend Uniform<A, B, T> operator-(const Uniform<A, B, T> &a) {
        auto x = Uniform<A, B, T>{};
        for (auto i = 0; i < x.n; i++) {
            x[i] = -a[i];
        }
        return x;
    };


    friend Uniform<A, B, T> operator-(const Uniform<A, B, T> &a, const Uniform<A, B, T> &b) {
        auto x = Uniform<A, B, T>{};
        for (auto i = 0; i < x.n; i++) {
            x[i] = a[i] - b[i];
        }
        return x;
    };


    friend Uniform<A, B, T> operator*(const Uniform<A, B, T> &a, const f32 &b) {
        auto x = Uniform<A, B, T>{};
        for (auto i = 0; i < x.n; i++) {
            x[i] = a[i] * b;
        }
        return x;
    };

    friend Uniform<A, B, T> operator*(const f32 &b, const Uniform<A, B, T> &a) {
        auto x = Uniform<A, B, T>{};
        for (auto i = 0; i < x.n; i++) {
            x[i] = a[i] * b;
        }
        return x;
    };

    friend Uniform<A, B, T> operator/(const Uniform<A, B, T> &a, const f32 &b) {
        auto x = Uniform<A, B, T>{};
        for (auto i = 0; i < x.n; i++) {
            x[i] = a[i] / b;
        }
        return x;
    };

    void print(std::ostream &os) const {
        os << "Uniform(" << r << "x" << c << "): [" << std::endl;
        for (auto i = 0; i < r; i++) {
            os << "  ";
            for (auto j = 0; j < c; j++) {
                os << data[j + i * c];
                if (j < c - 1)
                    os << "  ";
            }
            os << "";
            if (i < r - 1)
                os << "   " << std::endl;
        }
        os << std::endl << "]";
    }

};

#endif //GAME_UNIFORM_H
