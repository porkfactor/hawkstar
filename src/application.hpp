#ifndef HAWKSTAR_APPLICATION_HPP_
#define HAWKSTAR_APPLICATION_HPP_

namespace hawkstar
{
    class Application
    {
    public:
        Application();

        inline void *downcast() noexcept
        {
            return static_cast<void *>(this);
        }

        inline void const *downcast() const noexcept
        {
            return static_cast<void const *>(this);
        }

        static inline Application *upcast(void *v)
        {
            return static_cast<Application *>(v);
        }

    private:
    };
}

#endif
