#ifndef HAWKSTAR_ENTITY_HPP_
#define HAWKSTAR_ENTITY_HPP_

#include "point.hpp"
#include "vector.hpp"

namespace hawkstar
{
    enum PlayerState
    {
        PLAYER_STATE_IDLE,
        PLAYER_STATE_ACCELERATING,
        PLAYER_STATE_RUNNING,
        PLAYER_STATE_STOPPING,
        PLAYER_STATE_SKIDDING,
        PLAYER_STATE_FREEFALL,
        PLAYER_STATE_CROUCHING,
    };

    enum PlayerAction
    {
        PLAYER_ACTION_LEFT,
        PLAYER_ACTION_RIGHT,
        PLAYER_ACTION_UP,
        PLAYER_ACTION_DOWN,
    };

    class Entity
    {
    public:
        Entity();

        void move(point<double> pos);

        point<double> const &position() const noexcept
        {
            return position_;
        }

        void periodic(void);

        void actionOn(enum PlayerAction);
        void actionOff(enum PlayerAction);

    private:
        point<double> position_;
        vector<double> velocity_;
        vector<double> delta_velocity_;
        double max_velocity_;
        enum PlayerState state_;
    };
}

#endif
