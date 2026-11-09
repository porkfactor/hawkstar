#include <ResourceManager.hpp>

namespace hawkstar
{
    ResourceManager::ResourceManager()
    {

    }

    ResourceManager &ResourceManager::instance(void)
    {
        static ResourceManager instance_;

        return instance_;
    }

    
}
