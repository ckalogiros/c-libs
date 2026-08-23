/*
 * Created by ckalo on 8/17/26.
 * https://wikis.khronos.org/opengl/Performance
 */

#include "opengl_perf.h"
#include "glad.h"
#include "logger.h"
// #include "my_types.h"
#include "precision_timer.h"

static GLuint   query_id;       ///
static GLuint64 time;           /// the timer, in nanoseconds
static GLuint64 time_counter;   /// how many time measurements
static GLuint64 avg_time;       /// the accumulated time, in nanoseconds

void gl_perf_create_timer()
{
    glGenQueries(1, &query_id);
}
void gl_perf_timer_start()
{
    glFinish(); /// wait gpu to finsh any previous work
    glBeginQuery(GL_TIME_ELAPSED, query_id);
}
void gl_perf_timer_stop()
{
    glFinish(); /// wait gpu to finsh all operations
    glEndQuery(GL_TIME_ELAPSED);
    glGetQueryObjectui64v(query_id, GL_QUERY_RESULT, &time);

    avg_time += time;
    time_counter++;
}

void gl_perf_timer_print()
{
    // print("%0.6f ms\n", time / NANO_TO_MILI);
    print("%f ms\n", time / (double)NANO_TO_MILI);
}

void gl_perf_average_print()
{
    print("%s gl permormance average time: %0.6f ms %s\n", ry, (avg_time/time_counter) / (double)NANO_TO_MILI, CLEAR);
}
