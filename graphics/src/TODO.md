 ! IMPORTANT !
 the buffer_array is a sparse dynamic array, 
 we always have to check if an element is a NILL object(deleted/removed)

----

* the update of vertex an index buffers
  should happen as a command in a command buffer
* add fdebug for index buffers
* move from graphics project to engine project
  or create an objects project?
* # fdebug the attribute descriptor as well as the shader descripptor

----

# --- !IMIDIATELY! ---
* unify attributes.c with shader_descriptor.c
    There is no case in which a group of objects with the same attributes to
    belong to different shader build
* RENAME ALL "enum SID_ID" variables to "input_idx"

----

# RESTRICTIONS: IMPORTANT!
in the shader_id.h, all functions  
```{shaderinfo_flag_to_flagname, shaderinfo_id_to_flagname, shaderinfo_id_to_flag, shaderinfo_flag_to_id, etc}```
should not being used, instead the ```struct shader_entry shader_inputs_table[SHADER_ATTRIBUTES_ARRAY_COUNT]```
should be used. The reason is that in case of user-defines shader inputs, the above functions will fail because the
flag is a compile time #define.

----

# REMOVE UNUSED
* Is "shader_inputs_table[]" used??? each shader build has its own shader entries.

----

# MILESTONE: set and update uniforms

----

# MILESTONE: instanced geometry

    multiple shapes per VB is prohibited, check agaist is_instanced_geometry in
    order to create new gfx for different shapes:
    if(is_instanced) create_new_attribs_group();
 
``struct shader_descriptor:``
is no longer valid to call 

``shader_attributes_enable``
this is the job of the attributes.c file, via the instancing functionality



### ISSUES
The ``gfx_vertex_buffer_set_data`` adds in the attributes descriptor order
but the shader has changed the attribute layout for instanced rendering

``vpos,wpo,scale,rot,col ---> vpos,col,wpos,scale,rot``

----

# MILESTONE: render text

----

# MILESTONE: implement custom Anti Aliasing

----
