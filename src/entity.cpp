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
     * FSM processing. 
     * 
     * input event -> check state / action trigger, update state
     * 
     * processing loop
     *     check state, try perform action, update state
     * 
     * running:
     *     check we can complete action -> interact with map to determine
     *     if we can complete the action, or if the action would be blocked
     * 
     *     if the action is blocked, perform partial movement (if any) and
     *     update state.
     * 
     * jumping:
     *     check we can complete action -> interact with map to determine
     *     if we can complete the action, or if the action would be blocked
     * 
     *     if the action is blocked, perform partial movement (if any) and
     *     update state.
     * 
     * freefall:
     *     check we can complete action -> interact with map to determine
     *     if we can complete the action, or if the action would be blocked
     * 
     *     if the action is blocked, perform partial movement (if any) and
     *     update state.
     * 
     *     ... this is starting to feel a bit repetitive.
     * 
     * Now, for additional things. We want to potential trigger actions for
     * objects the player touches. Examples:
     *     - Floor based triggers. We must signal to all tiles when an entity
     *       traverses
     *     - Floor based traps. following the trigger, our continual ground
     *       detection should recognize a move to freefall. 
     *     - Wall based traps. We must signal to tiles we attempt to run/jump
     *       into (i.e. failed/blocked traversal), and perform collission 
     *       signal
     *     - Ceiling based traps. Notionally the same as wall based.
     *     - Fall damage. When downward motion is blocked, we need to check
     *       our impact velocity and hurt outselves accordingly.
     * 
     *     - Moving platforms. The player must move with the platform. Moving
     *       the platform must send a notification to all objects on the
     *       platform. the OO approach would be to have a pub/sub pattern so that
     *       on entering the platform the player is implicitly subscribed to
     *       movement notifications. This would equally work with horizontally
     *       scrolling platforms. Bonus - the player velocity would need to be
     *       summed with the platform velocity.
     * 
     * Thought: rather than ground based detection, do we just keep gravity as
     * a constant force, and as walking will have blocked collisions? it's a more
     * elegant solution since we're not performing separate ground checks. the FSM
     * would then deal only with the application of external forces.
     * 
     * still might not hurt to have a 'are we on the ground?' check. also, how do 
     * we track the fact that we're no longer touching a tile. A 'bounce' is one
     * thing, a persistent sit is another. the inelegant approach would be to (on
     * each motion) gather a list of touched tiles, and delta against previously
     * touched tiles. it would work, but feels crunchy. 
     * 
     * try-move needs moving into entity. there's nothing to negotiate on this point. 
     * 
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

    template<typename _Type>
    static inline _Type constexpr align_floor(_Type value, _Type align)
    {
        return value & ~(align - 1);
    }

    template<typename _Type>
    static inline _Type constexpr align_ceil(_Type value, _Type align)
    {
        return align_floor(value +  align - 1, align);
    }

    void Entity::actionOn(StageMap const &map SDL_UNUSED, enum PlayerAction a)
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
                velocity_ = world_vector{velocity_.x(), max_velocity_};
                enterState(PLAYER_STATE_JUMPING);
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

    void Entity::actionOff(StageMap const &map SDL_UNUSED, enum PlayerAction a)
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

    void Entity::periodic(StageMap const &map SDL_UNUSED, double deltat SDL_UNUSED)
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

                try_move(map, velocity_);
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
                try_move(map, velocity_);
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
                    try_move(map, velocity_);
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
                    try_move(map, velocity_);
                }
            }
            break;
        case PLAYER_STATE_FREEFALL:
            {
                velocity_ += -gravity;

                if (!try_move(map, velocity_))
                {
                    velocity_ = world_vector{velocity_.x(), world_integral_t{}};
                    enterState(velocity_.x() ? PLAYER_STATE_RUNNING : PLAYER_STATE_IDLE);
                }
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

    bool Entity::try_move(StageMap const &map, world_vector const &v) noexcept
    {
        world_point pos{this->bounds_.position()};
        bool rv{true};

        /* horizontal motion */
        if (v.x())
        {
            auto ty = static_cast<tile_integral_t>(pos.y());
            auto wx = pos.x() + v.x();

            /* rightward motion */
            if (v.x() > world_integral_t{})
            {
                pos += world_vector{v.x(), 0.0};
            }
            /* leftward motion */
            else
            {
                auto start = align_floor(static_cast<tile_integral_t>(std::floor(pos.x())), map.tile_width) / map.tile_width;
                auto end = align_ceil(static_cast<tile_integral_t>(std::floor(wx)), map.tile_width) / map.tile_width;

                for (auto tx = start; tx >= end; --tx)
                {
                    if (map.getId(tx - 1, ty) != 0)
                    {
                        wx = static_cast<world_integral_t>(tx * map.tile_width);
                        rv = false;
                        break;
                    }
                }

                pos = world_point{wx, pos.y()};
            }
        }

        /* vertical motion */
        if (v.y())
        {
            auto tx = static_cast<tile_integral_t>(pos.x());
            auto wy = pos.y() + v.y();

            /* upward motion */
            if (v.y() > world_integral_t{})
            {
                pos += world_vector{0.0, v.y()};
            }
            /* downward motion */
            else
            {
                auto start = align_floor(static_cast<tile_integral_t>(std::floor(pos.y())), map.tile_height) / map.tile_height;
                auto end = align_ceil(static_cast<tile_integral_t>(std::floor(wy)), map.tile_height) / map.tile_height;

                for (auto ty = start; ty >= end; --ty)
                {
                    if (map.getId((tx / map.tile_height), ty - 1) != 0)
                    {
                        wy = static_cast<world_integral_t>(ty * map.tile_height);
                        rv = false;
                        break;
                    }
                }

                pos = world_point(pos.x(), wy);
            }
        }

        this->move(pos);

        return rv;
    }

    void Entity::move(world_point const &pos)
    {
        bounds_.move(pos);
    }
}
