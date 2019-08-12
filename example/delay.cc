#include <cstdint>

void delay_loop_2(uint16_t);

const uint32_t F_CPU = 3686400;

struct DelayTicks
{
    constexpr DelayTicks(uint16_t l, uint16_t t)
      : loops(l)
      , ticks(t)
    {}
    constexpr DelayTicks(double ms)
      : loops(0)
      , ticks(0)
    {
        constexpr double ticks_1ms = (F_CPU) / 4e3;
        constexpr double ticks_100us = ((F_CPU) / 4e3) / 10;

        double fullTicks{ticks_1ms * ms};
        if (fullTicks > 65535)
        {
            loops = ms*10;
            ticks = ticks_100us;
        } else {
            loops = 1;
            if (fullTicks < 1)
            {
                ticks = 1;
            } else {
                ticks = fullTicks;
            }
        }
    }

    uint16_t loops;
    uint16_t ticks;
};

inline void
delayTicks(DelayTicks dt)
{
    while (dt.loops)
    {
        delay_loop_2(dt.ticks);
        --dt.loops;
    }
}

inline void
delayMs(double ms)
{
    delayTicks(DelayTicks(ms));
}

int main()
{
    delayMs(200);

    return 0;
}
