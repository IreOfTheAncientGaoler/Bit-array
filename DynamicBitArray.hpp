#ifndef DYNAMICBITARRAY_H
#define DYNAMICBITARRAY_H


#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

class BitArray {
private:
    std::vector<unsigned char> data;
    int bit_count;
    int getByteIndex(int bit_index);
    int getBitInByte(int bit_index);
    void checkIndex(int index);
    void checkSameSize(const BitArray& other);
public:

    BitArray() : bit_count(0) {}
    
    explicit BitArray(int num_bits, unsigned long value = 0) : bit_count(num_bits) {
        if (num_bits < 0) {
            throw std::invalid_argument("Размер не может быть отрицательным");
        }
        

        int byte_count = (num_bits + 7) / 8;
        data.resize(byte_count, 0);
        

        if (num_bits > 0) {
            for (int i = 0; i < sizeof(unsigned long) && i < byte_count; ++i) {
                data[i] = (value >> (i * 8)) & 0xFF;
            }
        }
    }
    
    BitArray(const BitArray& other) = default;
    ~BitArray() = default;
    void swap(BitArray& other);
    

    BitArray& operator=(const BitArray& other) = default;

    void resize(int new_size, bool value = false);
    

    void clear();
    void push_back(bool bit);
    

    BitArray& operator&=(const BitArray& other);
    BitArray& operator|=(const BitArray& other);
    
    BitArray& operator^=(const BitArray& other);
    BitArray& operator<<=(int n);
    BitArray& operator>>=(int n);
    BitArray operator<<(int n);
    BitArray operator>>(int n);
    BitArray& set(int index, bool value = true);

    BitArray& set();
    BitArray& reset(int index);
    BitArray& reset();
    bool any();
    bool none();
    

    BitArray operator~();
    

    int count();
    bool operator[](int index);
    int size();
    bool empty();
    std::string to_string();
};


bool operator==(const BitArray& a, const BitArray& b);
bool operator!=(const BitArray& a, const BitArray& b);
BitArray operator&(const BitArray& a, const BitArray& b);
BitArray operator|(const BitArray& a, const BitArray& b);
BitArray operator^(const BitArray& a, const BitArray& b);
#endif