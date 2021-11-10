#pragma once

#include "../../builders/query_i.hpp"

namespace flecs 
{

// System builder interface
template<typename Base, typename ... Components>
class system_builder_i : public query_builder_i<Base, Components ...> {
    using BaseClass = query_builder_i<Base, Components ...>;
public:
    system_builder_i()
        : BaseClass(nullptr)
        , m_desc(nullptr)
        , m_add_count(0) { }

    system_builder_i(ecs_system_desc_t *desc) 
        : BaseClass(&desc->query)
        , m_desc(desc)
        , m_add_count(0) { }

    /** Specify string-based signature. */
    Base& signature(const char *signature) {
        m_desc->query.filter.expr = signature;
        return *this;
    }

    /** Specify when the system should be ran.
     *
     * @param kind The kind that specifies when the system should be ran.
     */
    Base& kind(entity_t kind) {
        m_desc->entity.add[0] = kind;
        return *this;
    }

    /** Set system interval.
     * This operation will cause the system to be ran at the specified interval.
     *
     * The timer is synchronous, and is incremented each frame by delta_time.
     *
     * @param interval The interval value.
     */
    Base& interval(FLECS_FLOAT interval) {
        m_desc->interval = interval;
        return *this;
    }

    /** Set system rate.
     * This operation will cause the system to be ran at a multiple of the 
     * provided tick source. The tick source may be any entity, including
     * another system.
     *
     * @param tick_source The tick source.
     * @param rate The multiple at which to run the system.
     */
    Base& rate(const entity_t tick_source, int32_t rate) {
        m_desc->rate = rate;
        m_desc->tick_source = tick_source;
        return *this;
    }

    /** Set system rate.
     * This operation will cause the system to be ran at a multiple of the 
     * frame tick frequency. If a tick source was provided, this just updates
     * the rate of the system.
     *
     * @param rate The multiple at which to run the system.
     */
    Base& rate(int32_t rate) {
        m_desc->rate = rate;
        return *this;
    }
    
    /** Associate system with entity */
    Base& self(flecs::entity self) {
        m_desc->self = self;
        return *this;
    }

    /** Set system context */
    Base& ctx(void *ptr) {
        m_desc->ctx = ptr;
        return *this;
    }

protected:
    virtual flecs::world_t* world_v() = 0;

private:
    operator Base&() {
        return *static_cast<Base*>(this);
    }

    ecs_system_desc_t *m_desc;
    int32_t m_add_count;
};

}