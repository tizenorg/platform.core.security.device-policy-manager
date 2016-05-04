#ifndef __RUNTIME_KEY_MANAGER_H__
#define __RUNTIME_KEY_MANAGER_H__

namespace key {

class Manager {
public:
    Manager() = delete;
    Manager(const Manager&) = delete;
    Manager(Manager&&) = delete;

    Manager& operator=(const Manager&) = delete;
    Manager& operator=(Manager&&) = delete;

    static bool isKeyExist(const std::string& keyName);
    static void addKey(const std::string& keyName, const std::string& data);
    static std::string getKey(const std::string& keyName);
};

} // namespace key

#endif
