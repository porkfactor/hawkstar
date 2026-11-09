#ifndef HAWKSTAR_RESOURCE_MANAGER_HPP_
#define HAWKSTAR_RESOURCE_MANAGER_HPP_

#include <string>
#include <vector>

namespace hawkstar
{
    class ResourceManager
    {
    public:
        ResourceManager(ResourceManager const &) = delete;
        ResourceManager &operator = (ResourceManager const &) = delete;

        static ResourceManager &instance(void);

        void addSearchPath();

    private:
        std::vector<std::string> search_;
        ResourceManager();
    };
}

#endif
