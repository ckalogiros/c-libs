/*
 * Created by ckalo on 2/2/26.
 */

#include "gl_perf_timer.h"
#include "glad.h"
#include "logger.h"

#define GL_QUERIES 4

static GLuint64 startTime, endTime;
static GLuint queryID[GL_QUERIES];
static GLuint64 accum = 0;
static GLuint64 delta = 0;
static uint32 t_cnt = 0;
static uint32 gl_queries_cur = 0;
static uint32 gl_queries_cnt = 0;
GLint done_start = FALSE;
GLint done = FALSE;
GLint query_start = FALSE;
GLint query_stop = FALSE;

void
gl_perf_init()
{
    // Generate query objects
    glGenQueries(GL_QUERIES, queryID);
}

void
gl_perf_destroy()
{
    // Cleanup
    glDeleteQueries(2, queryID);
}

void
gl_perf_start()
{
    // Get start timestamp
    if(!query_start) {
        glQueryCounter(queryID[0], GL_TIMESTAMP);
        query_start = TRUE;
    }
    else glGetQueryObjectiv(queryID[1], GL_QUERY_RESULT_AVAILABLE, &done_start);
    // if(done_start) {
    //     glGetQueryObjectui64v(queryID[0], GL_QUERY_RESULT, &startTime);
    // }
}

void
gl_perf_stop()
{
    // Get end timestamp
    if(!query_stop) {
        glQueryCounter(queryID[1], GL_TIMESTAMP);
        query_stop = TRUE;
    }
    else glGetQueryObjectiv(queryID[1], GL_QUERY_RESULT_AVAILABLE, &done);

    // print("end done:%u\n", done);

    if(done) {
        // Get end timestamp
        // glQueryCounter(queryID[1], GL_TIMESTAMP);

        // Get the timestamps
        // glGetQueryObjectui64v(queryID[0], GL_QUERY_RESULT, &startTime);
        if(done_start) {
            glGetQueryObjectui64v(queryID[0], GL_QUERY_RESULT, &startTime);
        }

        glGetQueryObjectui64v(queryID[1], GL_QUERY_RESULT, &endTime);
        // if(done_start) {
        //     glGetQueryObjectui64v(queryID[0], GL_QUERY_RESULT, &startTime);
        // }

        // Calculate duration in nanoseconds
        delta = endTime - startTime;
        accum += delta;
        // double gpuTimeMS = gpuTime / 1000000.0;

        if(t_cnt >= 100) {
            // double micro = delta / 1000.0;
            double micro = (accum / t_cnt) / 1000.0;
            double mili = (accum / t_cnt) / 1000000.0;

            // print("GPU time: %.3f nano\n", accum/t_cnt);
            print("gpu_t: %s%llu%s nano %s%.3f%s micro %s%.3f%s mili\n",

                  YELLOW, accum / t_cnt, CLEAR,
                  // YELLOW, delta, CLEAR,
                  YELLOW, micro, CLEAR,
                  YELLOW, mili, CLEAR
            );

            t_cnt = accum = 0;
            done = FALSE;
            done_start = FALSE;
        }

        query_start = FALSE;
        query_stop = FALSE;
    }

    // // Wait for results (or check availability asynchronously)
    // while (!done) {
    //     glGetQueryObjectiv(queryID[1], GL_QUERY_RESULT_AVAILABLE, &done);
    // }
    //
    // // Get the timestamps
    // glGetQueryObjectui64v(queryID[0], GL_QUERY_RESULT, &startTime);
    // glGetQueryObjectui64v(queryID[1], GL_QUERY_RESULT, &endTime);
    //
    // // Calculate duration in nanoseconds
    // delta = endTime - startTime;
    // accum += delta;
    // // double gpuTimeMS = gpuTime / 1000000.0;
    //
    // if(t_cnt >= 100) {
    //
    //     // double micro = delta / 1000.0;
    //     double micro = (accum/t_cnt) / 1000.0;
    //     double mili = (accum/t_cnt) / 1000000.0;
    //
    //     // print("GPU time: %.3f nano\n", accum/t_cnt);
    //     print("gpu_t: %s%llu%s nano %s%.3f%s micro %s%.3f%s mili\n",
    //
    //         YELLOW, accum/t_cnt, CLEAR,
    //         // YELLOW, delta, CLEAR,
    //         YELLOW, micro, CLEAR,
    //         YELLOW, mili, CLEAR
    //         );
    //     t_cnt = accum = 0;
    // }

    t_cnt++;
}







// SAVE
//  /*
//  * Created by ckalo on 2/2/26.
//  */
//
// #include "gl_perf_timer.h"
// #include "glad.h"
// #include "logger.h"
//
//
// static GLuint64 startTime, endTime;
// static GLuint queryID[2];
// static GLuint64 accum = 0;
// static GLuint64 delta = 0;
// static uint32 t_cnt = 0;
// GLint done = FALSE;
//
// void
// gl_perf_init()
// {
//     // Generate query objects
//     glGenQueries(2, queryID);
// }
//
// void
// gl_perf_destroy()
// {
//     // Cleanup
//     glDeleteQueries(2, queryID);
// }
//
// void
// gl_perf_start()
// {
//     // Get start timestamp
//     glQueryCounter(queryID[0], GL_TIMESTAMP);
// }
//
// void
// gl_perf_stop()
// {
//     // Get end timestamp
//     glQueryCounter(queryID[1], GL_TIMESTAMP);
//
//     glGetQueryObjectiv(queryID[1], GL_QUERY_RESULT_AVAILABLE, &done);
//
//     if(done) {
//         // Get end timestamp
//         glQueryCounter(queryID[1], GL_TIMESTAMP);
//
//         // Get the timestamps
//         glGetQueryObjectui64v(queryID[0], GL_QUERY_RESULT, &startTime);
//         glGetQueryObjectui64v(queryID[1], GL_QUERY_RESULT, &endTime);
//
//         // Calculate duration in nanoseconds
//         delta = endTime - startTime;
//         accum += delta;
//         // double gpuTimeMS = gpuTime / 1000000.0;
//
//         if(t_cnt >= 1) {
//             // double micro = delta / 1000.0;
//             double micro = (accum / t_cnt) / 1000.0;
//             double mili = (accum / t_cnt) / 1000000.0;
//
//             // print("GPU time: %.3f nano\n", accum/t_cnt);
//             print("gpu_t: %s%llu%s nano %s%.3f%s micro %s%.3f%s mili\n",
//
//                   YELLOW, accum / t_cnt, CLEAR,
//                   // YELLOW, delta, CLEAR,
//                   YELLOW, micro, CLEAR,
//                   YELLOW, mili, CLEAR
//             );
//
//             t_cnt = accum = 0;
//             done = FALSE;
//         }
//     }
//
//     // // Wait for results (or check availability asynchronously)
//     // while (!done) {
//     //     glGetQueryObjectiv(queryID[1], GL_QUERY_RESULT_AVAILABLE, &done);
//     // }
//     //
//     // // Get the timestamps
//     // glGetQueryObjectui64v(queryID[0], GL_QUERY_RESULT, &startTime);
//     // glGetQueryObjectui64v(queryID[1], GL_QUERY_RESULT, &endTime);
//     //
//     // // Calculate duration in nanoseconds
//     // delta = endTime - startTime;
//     // accum += delta;
//     // // double gpuTimeMS = gpuTime / 1000000.0;
//     //
//     // if(t_cnt >= 100) {
//     //
//     //     // double micro = delta / 1000.0;
//     //     double micro = (accum/t_cnt) / 1000.0;
//     //     double mili = (accum/t_cnt) / 1000000.0;
//     //
//     //     // print("GPU time: %.3f nano\n", accum/t_cnt);
//     //     print("gpu_t: %s%llu%s nano %s%.3f%s micro %s%.3f%s mili\n",
//     //
//     //         YELLOW, accum/t_cnt, CLEAR,
//     //         // YELLOW, delta, CLEAR,
//     //         YELLOW, micro, CLEAR,
//     //         YELLOW, mili, CLEAR
//     //         );
//     //     t_cnt = accum = 0;
//     // }
//
//     t_cnt++;
// }
//
