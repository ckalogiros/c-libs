*
struct command_render_queue_params rq_params = {.rqidx = 0, .vertex_count = num_objects*VERTICES_PER_QUAD};
needs to add the ammount of vertices for that specific call to render_queue_add_draw_count()
                                                                   ^
                                                                   |
cmd_add_command((void*) render_queue_add_draw_count, &rq_params, sizeof(struct command_render_queue_params));

* also we cannot insert triangles and quads to the same vertex buffer because of the 
* stride