#ifndef __RUNTIME_KEY_GENERATOR_H__
#define __RUNTIME_KEY_GENERATOR_H__

namespace runtime {

class keyGenerator {
public:
    keyGenerator() = delete;
    keyGenerator(const keyGenerator&) = delete;
    keyGenerator(keyGenerator&&) = delete;

    keyGenerator& operator=(const keyGenerator&) = delete;
    keyGenerator& operator=(keyGenerator&&) = delete;

    static void initialize();
    static std::string wrapKey(const std::string& decrypted, const std::string& salt, int len);
    static std::string generateRandomKey(int len);

private:
    static std::string hexConvert(unsigned char* src, int srcLen);
};

}

#endif
