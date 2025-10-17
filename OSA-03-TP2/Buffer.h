#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <string>
#include <iostream>

class Buffer {
private:
    std::vector<char> data;
    int ponteiro;

public:
    Buffer();

    void clear();

    void packFixo(const std::string& str, int tamanho);
    void packDelimitado(const std::string& str, char delim);
    void packComprimento(const std::string& str);
    void pack(int valor);

    std::string unpackFixo(int tamanho);
    std::string unpackDelimitado(char delim);
    std::string unpackComprimento();
    int unpackInt();

    bool read(std::istream& stream, int tamanho);
    bool readDelimitado(std::istream& stream, char delim);
    void write(std::ostream& stream) const;
    
    int size() const;
};

#endif