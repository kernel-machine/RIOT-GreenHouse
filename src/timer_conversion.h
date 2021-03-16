//
// Created by Luca Giovannesi.
//

#ifndef GREENHOUSE_TIMER_CONVERSION_H
#define GREENHOUSE_TIMER_CONVERSION_H


#define HZ2US(hz)   (1000000 / (hz))
#define US2S(us)    ((us) * 1e-6f)
#define US2MS(us)   ((us) * 1e-3f)
#define MS2US(ms)   ((ms) * 1000)
#define MS2S(ms)    ((ms) * 1e-3f)
#define HZ2S(hz)    US2S(HZ2US(hz))
#define S2MS(s)     (s*1000)
#endif //GREENHOUSE_TIMER_CONVERSION_H
