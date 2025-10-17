#include "Buffer.h"
#include <cstring>
#include <stdexcept>

Buffer::Buffer() : ponteiro(0) {}

void Buffer::clear() {
    data.clear();
    ponteiro = 0;
}

void Buffer::pack(int valor) {
    data.resize(data.size() + sizeof(int));
    std::memcpy(data.data() + data.size() - sizeof(int), &valor, sizeof(int));
}

void Buffer::packFixo(const std::string& str, int tamanho) {
    std::string temp = str;
    temp.resize(tamanho, '\0'); 
    data.insert(data.end(), temp.begin(), temp.end());
}

void Buffer::packDelimitado(const std::string& str, char delim) {
    data.insert(data.end(), str.begin(), str.end());
    data.push_back(delim);
}

void Buffer::packComprimento(const std::string& str) {
    short tamanho = static_cast<short>(str.length());
    data.resize(data.size() + sizeof(short));
    std::memcpy(data.data() + data.size() - sizeof(short), &tamanho, sizeof(short));
    data.insert(data.end(), str.begin(), str.end());
}

int Buffer::unpackInt() {
    if (ponteiro + sizeof(int) > data.size()) {
        throw std::runtime_error("Buffer underflow on unpackInt");
    }
    int valor;
    std::memcpy(&valor, data.data() + ponteiro, sizeof(int));
    ponteiro += sizeof(int);
    return valor;
}

std::string Buffer::unpackFixo(int tamanho) {
    if (ponteiro + tamanho > data.size()) {
        throw std::runtime_error("Buffer underflow on unpackFixo");
    }
    std::string str(data.data() + ponteiro, tamanho);
    ponteiro += tamanho;
    str.erase(str.find_last_not_of('\0') + 1);
    return str;
}

std::string Buffer::unpackDelimitado(char delim) {
    std::string str;
    while (ponteiro < data.size() && data[ponteiro] != delim) {
        str += data[ponteiro];
        ponteiro++;
    }
    if (ponteiro < data.size()) { 
        ponteiro++;
    }
    return str;
}

std::string Buffer::unpackComprimento() {
    if (ponteiro + sizeof(short) > data.size()) {
         throw std::runtime_error("Buffer underflow on unpackComprimento size");
    }
    short tamanho;
    std::memcpy(&tamanho, data.data() + ponteiro, sizeof(short));
    ponteiro += sizeof(short);

    if (ponteiro + tamanho > data.size()) {
         throw std::runtime_error("Buffer underflow on unpackComprimento data");
    }
    std::string str(data.data() + ponteiro, tamanho);
    ponteiro += tamanho;
    return str;
}

bool Buffer::read(std::istream& stream, int tamanho) {
    clear();
    data.resize(tamanho);
    stream.read(data.data(), tamanho);
    return stream.gcount() == tamanho;
}

bool Buffer::readDelimitado(std::istream& stream, char delim) {
    clear();
    char ch;
    while (stream.get(ch) && ch != delim) {
        data.push_back(ch);
    }
    if (stream) { 
        data.push_back(delim);
        return true;
    }
    return !data.empty(); 
}

void Buffer::write(std::ostream& stream) const {
    stream.write(data.data(), data.size());
}

int Buffer::size() const {
    return data.size();
}