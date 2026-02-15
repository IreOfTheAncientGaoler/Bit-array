#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

class BitArray {
private:
    std::vector<unsigned char> data;
    int bit_count;
    int getByteIndex(int bit_index) const {
        return bit_index / 8;
    }
    int getBitInByte(int bit_index) const {
        return bit_index % 8;
    }
    void checkIndex(int index) const {
        if (index < 0 || index >= bit_count) {
            throw std::out_of_range("Индекс бита вне допустимого диапазона");
        }
    }
    void checkSameSize(const BitArray& other) const {
        if (bit_count != other.bit_count) {
            throw std::invalid_argument("Битовые массивы должны быть одинакового размера");
        }
    }
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
    void swap(BitArray& other) {
        std::swap(data, other.data);
        std::swap(bit_count, other.bit_count);
    }
    

    BitArray& operator=(const BitArray& other) = default;

    void resize(int new_size, bool value = false) {
        if (new_size < 0) {
            throw std::invalid_argument("Размер не может быть отрицательным");
        }
        
        int new_byte_count = (new_size + 7) / 8;
        int old_byte_count = data.size();
        

        data.resize(new_byte_count);
        if (new_byte_count > old_byte_count) {
            unsigned char fill_value = value ? 0xFF : 0x00;
            for (int i = old_byte_count; i < new_byte_count; ++i) {
                data[i] = fill_value;
            }
        }
        
        bit_count = new_size;
        if (bit_count > 0) {
            int last_bit = bit_count % 8;
            if (last_bit != 0) {
                unsigned char mask = (1 << last_bit) - 1;
                data.back() &= mask;
            }
        }
    }
    

    void clear() {
        data.clear();
        bit_count = 0;
    }
    void push_back(bool bit) {
        resize(bit_count + 1, false);
        set(bit_count - 1, bit);
    }
    

    BitArray& operator&=(const BitArray& other) {
        checkSameSize(other);
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] &= other.data[i];
        }
        return *this;
    }
    BitArray& operator|=(const BitArray& other) {
        checkSameSize(other);
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] |= other.data[i];
        }
        return *this;
    }
    
    BitArray& operator^=(const BitArray& other) {
        checkSameSize(other);
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] ^= other.data[i];
        }
        return *this;
    }
BitArray& operator<<=(int n) {
        if (n < 0) {
            throw std::invalid_argument("Сдвиг не может быть отрицательным");
        }
        
        if (n >= bit_count) {
            reset();
            return *this;
        }
        for (int i = bit_count - 1; i >= n; --i) {
            set(i, (*this)[i - n]);
        }
        for (int i = 0; i < n; ++i) {
            set(i, false);
        }
        return *this;
    }
    
    BitArray& operator>>=(int n) {
        if (n < 0) {
            throw std::invalid_argument("Сдвиг не может быть отрицательным");
        }
        
        if (n >= bit_count) {
            reset();
            return *this;
        }
        
        for (int i = 0; i < bit_count - n; ++i) {
            set(i, (*this)[i + n]);
        }
        for (int i = bit_count - n; i < bit_count; ++i) {
            set(i, false);
        }
        return *this;
    }
    BitArray operator<<(int n) const {
        BitArray result(*this);
        result <<= n;
        return result;
    }
    BitArray operator>>(int n) const {
        BitArray result(*this);
        result >>= n;
        return result;
    }
    BitArray& set(int index, bool value = true) {
        checkIndex(index);
        int byte_idx = getByteIndex(index);
        int bit_idx = getBitInByte(index);
        
        if (value) {
            data[byte_idx] |= (1 << bit_idx);
        } else {
            data[byte_idx] &= ~(1 << bit_idx);
        }
        return *this;
    }
    

    BitArray& set() {
        std::fill(data.begin(), data.end(), 0xFF);
        

        if (bit_count > 0) {
            int last_bit = bit_count % 8;
            if (last_bit != 0) {
                unsigned char mask = (1 << last_bit) - 1;
                data.back() &= mask;
            }
        }
        return *this;
    }
    BitArray& reset(int index) {
        return set(index, false);
    }
    BitArray& reset() {
        std::fill(data.begin(), data.end(), 0x00);
        return *this;
    }
    bool any() {
        for (unsigned char byte : data) {
            if (byte != 0) return true;
        }
        return false;
    }
    bool none() {
        return !any();
    }
    

    BitArray operator~() {
        BitArray result(*this);
        for (unsigned char& byte : result.data) {
            byte = ~byte;
        }
        if (bit_count > 0) {
            int last_bit = bit_count % 8;
            if (last_bit != 0) {
                unsigned char mask = (1 << last_bit) - 1;
                result.data.back() &= mask;
            }
        }
        return result;
    }
    

    int count() const {
        int result = 0;
        for (unsigned char byte : data) {
            unsigned char b = byte;
            while (b) {
                result += (b & 1);
                b >>= 1;
            }
        }
        return result;
    }
    
    bool operator[](int index) const {
        checkIndex(index);
        int byte_idx = getByteIndex(index);
        int bit_idx = getBitInByte(index);
        return (data[byte_idx] >> bit_idx) & 1;
    }
    
    int size() const {
        return bit_count;
    }
    
    bool empty() const {
        return bit_count == 0;
    }
    
    std::string to_string() const {
        std::string result;
        result.reserve(bit_count);
        for (int i = bit_count - 1; i >= 0; --i) {
            result += (*this)[i] ? '1' : '0';
        }
        return result;
    }
};


bool operator==(const BitArray& a, const BitArray& b) {
    if (a.size() != b.size()) return false;
    for (int i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

bool operator!=(const BitArray& a, const BitArray& b) {
    return !(a == b);
}

BitArray operator&(const BitArray& a, const BitArray& b) {
    BitArray result(a);
    result &= b;
    return result;
}

BitArray operator|(const BitArray& a, const BitArray& b) {
    BitArray result(a);
    result |= b;
    return result;
}
BitArray operator^(const BitArray& a, const BitArray& b) {
    BitArray result(a);
    result ^= b;
    return result;
}
