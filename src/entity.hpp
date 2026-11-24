#ifndef HAWKSTAR_ENTITY_HPP_
#define HAWKSTAR_ENTITY_HPP_

#include "types.hpp"

/**
 * processing player movement
 *     - what happens if the user presses left and right at the same time?
 *         ideally nothing
 *     - is it better to concern ourselves with forces rather than states?
 *         - jump + run behaves differently to run + jump
 *         - there is no way to avoid some form of state machine, but I guess
 *         forces can concern themselves with 'can this force apply'.
 *         - state needed to handle animation
 *         how do we best handle this? I think it is probably better to be
 *         state first, physics second, to handle the cooked physics of fun.
 *         - do we have any benefit to incorporating direction into state, or
 *         do we track that separately. then, there are two aspects to direction:
 *             - the direction we are travelling
 *             - the direction we are facing
 * 
 * conclusion - keep high-level state types, maintain separate information that is
 * required for rendering. 
 */

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
        PLAYER_STATE_JUMPING,
        PLAYER_STATE_COYOTE,
    };

    enum PlayerAction
    {
        PLAYER_ACTION_LEFT,
        PLAYER_ACTION_RIGHT,
        PLAYER_ACTION_UP,
        PLAYER_ACTION_DOWN,
    };

    class Application;
    class StageMap;

    class Entity
    {
    public:
        Entity(Application *application);

        void move(world_point pos);

        world_point centre() const noexcept
        {
            return bounds_.centre();
        }

        inline world_rectangle const &bounds() const noexcept
        {
            return bounds_;
        }

        void periodic(StageMap &map, double deltat);

        void actionOn(StageMap &map, enum PlayerAction);
        void actionOff(StageMap &map, enum PlayerAction);

    private:
        void enterState(enum PlayerState);

        Application *application_;

        world_rectangle bounds_;
        world_vector velocity_;
        world_vector delta_velocity_;
        double max_velocity_;
        double max_jump_;
        enum PlayerState state_;
    };
}

#endif
