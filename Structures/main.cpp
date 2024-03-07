#include <iostream>
#include <ctime>
#include "NumberTheory//NumberTheory.h" // Предполагается, что файл number_theory.h содержит объявление класса EratosthenesSieve

int main() {
    size_t n = 1e9;
    NumberTheory::EratosthenesSieve sieve(n);
    clock_t t1, t2;

    t1 = clock();
    sieve.build();
    t2 = clock();
    std::cout << "Time taken: " << double(t2 - t1) / CLOCKS_PER_SEC << " seconds" << std::endl;

    size_t k;
    std::cout << "Enter a number to check for primality: ";
    std::cin >> k;

    std::cout << "Is " << k << " prime? " << std::boolalpha << sieve.is_prime(k) << std::endl;
    std::cout << "Is " << k << " prime using sqrt method? " << std::boolalpha << NumberTheory::EratosthenesSieve::is_prime_sqrt_method(k) << std::endl;

    return 0;
}
