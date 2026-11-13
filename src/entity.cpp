#include "entity.hpp"

namespace hawkstar
{
    Entity::Entity() :
        position_{},
        delta_velocity_{},
        max_velocity_{8.0},
        state_{PLAYER_STATE_IDLE}
    {}

    /**
     * so, we need a FSM for each entity. no biggie. the usual FSM dealio
     * 
     *     state + event [if guard] -> action
     * 
     * design is always state oriented, implementation is always event 
     * oriented. make it state oriented is probably safest bet. 
     * 
     * so we have a table of states <s1, s2, ... sn> and on an event
     * we look at the event triggers for that state. we may have multiple
     * possible transitions for any event so might as well 
     */

    void Entity::actionOn(enum PlayerAction a)
    {
        switch (state_)
        {
        case PLAYER_STATE_IDLE:
            switch (a)
            {
            case PLAYER_ACTION_LEFT:
            case PLAYER_ACTION_RIGHT:
                delta_velocity_ = (a == PLAYER_ACTION_LEFT) ? vector<double>(-1.0, 0.0) : vector<double>(1.0, 0.0);
                state_ = PLAYER_STATE_ACCELERATING;
                break;
            case PLAYER_ACTION_DOWN:
            case PLAYER_ACTION_UP:
                break;
            }
            break;

        case PLAYER_STATE_ACCELERATING:
            break;

        case PLAYER_STATE_RUNNING:
            break;

        default:
            break;
        }
    }

    void Entity::actionOff(enum PlayerAction a)
    {
        switch (state_)
        {
        case PLAYER_STATE_IDLE:
            break;
        case PLAYER_STATE_ACCELERATING:
        case PLAYER_STATE_RUNNING:
            switch (a)
            {
            case PLAYER_ACTION_LEFT:
            case PLAYER_ACTION_RIGHT:
                delta_velocity_ = (a == PLAYER_ACTION_LEFT) ? vector<double>(2.0, 0.0) : vector<double>(-2.0, 0.0);
                state_ = PLAYER_STATE_STOPPING;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }

    void Entity::periodic(void)
    {
        switch (state_)
        {
        case PLAYER_STATE_IDLE:
            break;
        case PLAYER_STATE_ACCELERATING:
            {
                velocity_ += delta_velocity_;

                double magnitude = velocity_.magnitude();

                if (magnitude >= max_velocity_)
                {
                    velocity_ *= (max_velocity_ / magnitude);
                    state_ = PLAYER_STATE_RUNNING;
                }
            }
            break;
        case PLAYER_STATE_RUNNING:
            break;
        case PLAYER_STATE_STOPPING:
            {
                velocity_ += delta_velocity_;
    
                if (velocity_.magnitude() <= 0.0)
                {
                    velocity_.clear();
                    state_ = PLAYER_STATE_IDLE;
                }
            }
            break;
        default:
            break;
        }

        position_ += velocity_;
    }
}
