#include <iostream>
#include <cstdlib>
#include <fstream>
#include <bitset>

using namespace std;

void printFileBits(const string& filename) {
    ifstream file(filename, ios::binary);
    char byte_raw;
    
    cout << "Файл " << filename << ": ";
    while (file.get(byte_raw)) {
        cout << bitset<8>(static_cast<uint8_t>(byte_raw)) << " ";
    }
    cout << endl;
    file.close();
}

int main(){
    const string name1 = "file1.bin";
    const string name2 = "file2.bin";
    const string outName = "result.bin";

    string content1, content2;
    cout << "Enter file1: ";
    getline(cin, content1);
    ofstream createFile1(name1, ios::binary);
    if (!createFile1.is_open()) {
        cerr << "Error: cannot create file1" << name1 << endl;
        return 1;
    }
    createFile1.write(content1.c_str(), content1.size());
    createFile1.close();

    cout << "Enter file2: ";
    getline(cin, content2);

    ofstream createFile2(name2, ios::binary);
    if (!createFile2.is_open()) {
        cerr << "Error: cannot create file2" << name2 << endl;
        return 1;
    }
    createFile2.write(content2.c_str(), content2.size());
    createFile2.close();

    ifstream file1("file1.bin", ios::binary);
    ifstream file2("file2.bin", ios::binary);
    ofstream outFile("result.bin", ios::binary);

    if (!file1.is_open() || !file2.is_open() || !outFile.is_open()) {
        cerr << "Error: cannot open the file" << endl;
        return 1;
    }
    char byte1_raw, byte2_raw;

    while (file1.get(byte1_raw) && file2.get(byte2_raw)) {
        uint8_t b1 = static_cast<uint8_t>(byte1_raw);
        uint8_t b2 = static_cast<uint8_t>(byte2_raw);

        uint8_t outByte1 = 0;
        uint8_t outByte2 = 0;

        for (int i = 7; i >= 0; --i) {
            uint8_t bitA = (b1 >> i) & 1;
            uint8_t bitC = (b2 >> i) & 1;

            uint8_t firstBit, secondBit;
            if (bitA <= bitC) {
                firstBit = bitA;
                secondBit = bitC;
            } else {
                firstBit = bitC;
                secondBit = bitA;
            }

            if (i >= 4) {
                int shift = (i - 4) * 2;
                outByte1 |= (firstBit << (shift + 1));
                outByte1 |= (secondBit << shift);
            } else {
                int shift = i * 2;
                outByte2 |= (firstBit << (shift + 1));
                outByte2 |= (secondBit << shift);
            }
        }

        outFile.put(static_cast<char>(outByte1));
        outFile.put(static_cast<char>(outByte2));
    }
    cout << "Ready!" << endl;
    file1.close();
    file2.close();
    outFile.close();

    printFileBits("file1.bin");
    printFileBits("file2.bin");
    printFileBits("result.bin");
    return 0;
}