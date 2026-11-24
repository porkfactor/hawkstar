#include <SDL3/SDL.h>
#include "entity.hpp"
#include "stage_map.hpp"

namespace hawkstar
{
    static world_vector const gravity = world_vector(0.0, 1.0);

    /**
     * physics
     * 
     * v => velocity (m / s)
     * s => distance fallen (m)
     * u => initial velocity (m / s)
     * g => gravity (m / s^2)
     * 
     * free fall:
     * velocity : v = gt
     * distance : s = (g * (t * t)) / 2.0
     * velocity : v * v = 2.0 * g * s
     * 
     * general:
     * velocity : v = u + (g * t)
     * distance : s = (u * t) + ((g * (t * t)) / 2.0)
     * 
     * terminal velocity:
     * m = mass (kg)
     * d = drag co-efficient (kg/m^3)
     * A = area (m^3)
     * 
     * mg = (p * v^2 * d * A) / 2.0
     * v = sqrt((2 * m * g) / (p * d * A))
     * 
     */

    Entity::Entity(Application *a) :
        application_(a),
        bounds_{16.0, 16.0, 16.0, 16.0},
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
     * 
     * 
     * ok, refigure this. we have two things:
     *     - input events coming from some point in the processing loop
     *     - movement events coming from the scene processing. 
     * 
     * we currently don't consider that the scene processing triggers events
     * very well (or, at least, the only event triggers currently come from
     * input). 
     * 
     * how do we want to handle coyote time? we need to update the scene (i.e.
     * move the player according to their velocity), check if they are grounded
     * and do the needful. 
     * 
     * so, in effect the scene processing needs to:
     *     1/ check if we're moving
     *     2/ if true, check the player is able to complete the movement
     *     3/ move the player according to the restrictions
     *     4/ check the player location and update state (i.e. does the
     *     completion of this movement trigger anything).
     * 
     * seems a bit off having to perform two location checks, but optimise
     * when its a problem.
     * 
     * addendum: these may be distinct location checks. at the very least we 
     * can assume tacit location information in the input processing since this
     * is based on state that has already updated in relation to the last cycle.
     * 
     */

    void Entity::enterState(enum PlayerState s)
    {
        switch (state_ = s)
        {
        case PLAYER_STATE_IDLE:
            velocity_.zero();
            delta_velocity_.zero();
            break;
        case PLAYER_STATE_RUNNING:
            delta_velocity_.zero();
        default:
            break;
        }
    }

    void Entity::actionOn(StageMap &map SDL_UNUSED, enum PlayerAction a)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "+action [%d] state=%d\n",
            static_cast<int>(a),
            static_cast<int>(state_)
        );

        switch (state_)
        {
        case PLAYER_STATE_IDLE:
            switch (a)
            {
            case PLAYER_ACTION_LEFT:
            case PLAYER_ACTION_RIGHT:
                delta_velocity_ = (a == PLAYER_ACTION_LEFT) ? world_vector(-1.0, 0.0) : world_vector(1.0, 0.0);
                enterState(PLAYER_STATE_ACCELERATING);
                break;
            case PLAYER_ACTION_DOWN:
                break;
            case PLAYER_ACTION_UP:
                velocity_ = world_vector{0.0, max_velocity_};
                enterState(PLAYER_STATE_JUMPING);
                break;
            }
            break;

        
        case PLAYER_STATE_ACCELERATING:
            break;

        case PLAYER_STATE_RUNNING:
            switch (a)
            {
            case PLAYER_ACTION_LEFT:
            case PLAYER_ACTION_RIGHT:
            case PLAYER_ACTION_DOWN:
                break;
            case PLAYER_ACTION_UP:
                
                break;
            }
            break;

        case PLAYER_STATE_STOPPING:
            switch (a)
            {
            case PLAYER_ACTION_LEFT:
            case PLAYER_ACTION_RIGHT:
                delta_velocity_ = (a == PLAYER_ACTION_LEFT) ? world_vector(-1.0, 0.0) : world_vector(1.0, 0.0);
                enterState(PLAYER_STATE_ACCELERATING);
            default:
                break;
            }
            break;

        default:
            break;
        }

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "+action [%d] state=%d v=<%f,%f> dv=<%f,%f>\n",
            static_cast<int>(a),
            static_cast<int>(state_),
            velocity_.x(), velocity_.y(),
            delta_velocity_.x(), delta_velocity_.y()
        );
    }

    void Entity::actionOff(StageMap &map SDL_UNUSED, enum PlayerAction a)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "-action [%d] state=%d\n",
            static_cast<int>(a),
            static_cast<int>(state_)
        );

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
                delta_velocity_ = (a == PLAYER_ACTION_LEFT) ? world_vector(2.0, 0.0) : world_vector(-2.0, 0.0);
                state_ = PLAYER_STATE_STOPPING;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "-action [%d] state=%d v=<%f,%f> dv=<%f,%f>\n",
            static_cast<int>(a),
            static_cast<int>(state_),
            velocity_.x(), velocity_.y(),
            delta_velocity_.x(), delta_velocity_.y()
        );

    }

    void Entity::periodic(StageMap &map SDL_UNUSED, double deltat SDL_UNUSED)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "periodic state=%d\n",
            static_cast<int>(state_)
        );

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
                    enterState(PLAYER_STATE_RUNNING);
                }

                bounds_.move(bounds_.position() + velocity_);
            }
            break;
        case PLAYER_STATE_RUNNING:
            {
            #if 0
                // TODO: 1/ check if we're on ground

                if (!map.raycast(bounds_, world_vector{0.0, -1.0}, 0.0))
                {
                    enterState(PLAYER_STATE_FREEFALL);
                }
                else
                {
                    auto distance = velocity_.magnitude();
                    auto space = map.raycast(bounds_, velocity_, velocity_.magnitude());

                    if (distance > space)
                    {
                        enterState(PLAYER_STATE_IDLE);
                    }

                    bounds_.move(bounds_.position() + velocity_);
                }

                // TOOD: 2/ check and restrict intended movement
                // TODO: 3/ perform movement
                // TODO: 4/ put in stop state if required
            #else
                auto position = map.try_move(bounds_.position(), velocity_);
                bounds_.move(position);
            #endif
            }
            break;
        case PLAYER_STATE_STOPPING:
            {
                if (delta_velocity_.magnitude() >= velocity_.magnitude())
                {
                    enterState(PLAYER_STATE_IDLE);
                }
                else
                {
                    velocity_ += delta_velocity_;
                    auto position = map.try_move(bounds_.position(), velocity_);
                    bounds_.move(position);
                }
            }
            break;
        case PLAYER_STATE_JUMPING:
            {
                if (gravity.magnitude() >= velocity_.magnitude())
                {
                    velocity_.zero();
                    enterState(PLAYER_STATE_FREEFALL);
                }
                else
                {
                    velocity_ += -gravity;
                    bounds_.move(bounds_.position() + velocity_);
                }
            }
            break;
        case PLAYER_STATE_FREEFALL:
            {
                velocity_ += -gravity;

                auto position = map.try_move(bounds_.position(), velocity_);

                // TODO: temporary bodge while we fix the interface - detect if we hit something (badly)
                if (position != (bounds_.position() + velocity_))
                {
                    enterState(PLAYER_STATE_IDLE);
                }

                bounds_.move(position);
            }
            break;
        default:
            break;
        }

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "periodic state=%d v=<%f,%f> dv=<%f,%f>\n",
            static_cast<int>(state_),
            velocity_.x(), velocity_.y(),
            delta_velocity_.x(), delta_velocity_.y()
        );
    }
}
