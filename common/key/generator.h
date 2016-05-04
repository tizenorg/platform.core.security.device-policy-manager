#ifndef __RUNTIME_KEY_GENERATOR_H__
#define __RUNTIME_KEY_GENERATOR_H__

namespace key {

class Generator {
public:
    Generator() = delete;
    Generator(const Generator&) = delete;
    Generator(Generator&&) = delete;

    Generator& operator=(const Generator&) = delete;
    Generator& operator=(Generator&&) = delete;

    static void initialize();
    static std::string wrapKey(const std::string& decrypted, const std::string& salt, int len);
    static std::string generateRandomKey(int len);

private:
    static std::string hexConvert(unsigned char* src, int srcLen);
};

} // namespace key

#endif
