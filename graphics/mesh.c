/*
 * Created by ckalo on 6/29/26.
 */

#include "mesh.h"

#include "my_macros.h"


float triangle_vertices[VERTICES_PER_TRI*VERTEX_POS2] = {
    -1,  1,
    -1, -1,
     1, -1,
};
float rectangle_vertices[VERTICES_PER_QUAD*VERTEX_POS2] = {
    -1,  1,
    -1, -1,
     1, -1,
    -1,  1,
     1, -1,
     1,  1,
};
float pentagon_vertices[VERTICES_PER_PENTAGON*VERTEX_POS2] = {
    0.0,        1.0,
   -0.95105652, 0.30901699,
   -0.58778525,-0.80901699,

    0.0,        1.0,
   -0.58778525,-0.80901699,
    0.58778525,-0.80901699,

    0.0,        1.0,
    0.58778525,-0.80901699,
    0.95105652, 0.30901699,
};
float octagon_vertices[VERTICES_PER_OCTAGON*VERTEX_POS2] = {
    0.0,        1.0,
   -0.70710678, 0.70710678,
   -1.0,        0.0,

    0.0,        1.0,
   -1.0,        0.0,
   -0.70710678,-0.70710678,

    0.0,        1.0,
   -0.70710678,-0.70710678,
    0.0,       -1.0,

    0.0,        1.0,
    0.0,       -1.0,
    0.70710678,-0.70710678,

    0.0,        1.0,
    0.70710678,-0.70710678,
    1.0,        0.0,

    0.0,        1.0,
    1.0,        0.0,
    0.70710678, 0.70710678,


};

float cube_vertices[VERTICES_PER_CUBE*VERTEX_POS3] = {
    -1, -1, 1, /// FRONT
     1, -1, 1,
     1,  1, 1,
    -1, -1, 1,
     1,  1, 1,
    -1,  1, 1,
     1, -1,-1, /// BACK
    -1, -1,-1,
    -1,  1,-1,
     1, -1,-1,
    -1,  1,-1,
     1,  1,-1,
    -1, -1,-1, /// LEFT
    -1, -1, 1,
    -1,  1, 1,
    -1, -1,-1,
    -1,  1, 1,
    -1,  1,-1,
     1, -1, 1, /// RIGHT
     1, -1,-1,
     1,  1,-1,
     1, -1, 1,
     1,  1,-1,
     1,  1, 1,
    -1,  1, 1, /// TOP
     1,  1, 1,
     1,  1,-1,
    -1,  1, 1,
     1,  1,-1,
    -1,  1,-1,
    -1, -1,-1, /// BOTTOM
     1, -1,-1,
     1, -1, 1,
    -1, -1,-1,
     1, -1, 1,
    -1, -1, 1
};
float octahedron_vertices[VERTICES_PER_OCTAHEDRON*VERTEX_POS3] = {
    /// octahedron's vertex to vertex distance is √(1² + 1² + 1²) = √(1 + 1 + 1) = √3
    /// in order to have similar dimention with the cube
    1.73,  0,  0, /// Face 1 (right-top-front)
    0,  0,  1.73,
    0,  1.73,  0,
    1.73,  0,  0, /// Face 2 (right-bottom-front)
    0,  0,  1.73,
    0, -1.73,  0,
   -1.73,  0,  0, /// Face 3 (left-top-front)
    0,  0,  1.73,
    0,  1.73,  0,
   -1.73,  0,  0, /// Face 4 (left-bottom-front)
    0,  0,  1.73,
    0, -1.73,  0,
    1.73,  0,  0, /// Face 5 (right-top-back)
    0,  1.73,  0,
    0,  0, -1.73,
    1.73,  0,  0, /// Face 6 (right-bottom-back)
    0, -1.73,  0,
    0,  0, -1.73,
   -1.73,  0,  0, /// Face 7 (left-top-back)
    0,  1.73,  0,
    0,  0, -1.73,
   -1.73,  0,  0, /// Face 8 (left-bottom-back)
    0, -1.73,  0,
    0,  0, -1.73,
};

void mesh_build_dodecahedron();




float blender_ico_shere_vertices_indexed[] = {
    0.000000, -1.000000,  0.000000,
    0.723607, -0.447220,  0.525725,
   -0.276388, -0.447220,  0.850649,
   -0.894426, -0.447216,  0.000000,
   -0.276388, -0.447220, -0.850649,
    0.723607, -0.447220, -0.525725,
    0.276388,  0.447220,  0.850649,
   -0.723607,  0.447220,  0.525725,
   -0.723607,  0.447220, -0.525725,
    0.276388,  0.447220, -0.850649,
    0.894426,  0.447216,  0.000000,
    0.000000,  1.000000,  0.000000,
   -0.162456, -0.850654,  0.499995,
    0.425323, -0.850654,  0.309011,
    0.262869, -0.525738,  0.809012,
    0.850648, -0.525736,  0.000000,
    0.425323, -0.850654, -0.309011,
   -0.525730, -0.850652,  0.000000,
   -0.688189, -0.525736,  0.499997,
   -0.162456, -0.850654, -0.499995,
   -0.688189, -0.525736, -0.499997,
    0.262869, -0.525738, -0.809012,
    0.951058,  0.000000,  0.309013,
    0.951058,  0.000000, -0.309013,
    0.000000,  0.000000,  1.000000,
    0.587786,  0.000000,  0.809017,
   -0.951058,  0.000000,  0.309013,
   -0.587786,  0.000000,  0.809017,
   -0.587786,  0.000000, -0.809017,
   -0.951058,  0.000000, -0.309013,
    0.587786,  0.000000, -0.809017,
    0.000000,  0.000000, -1.000000,
    0.688189,  0.525736,  0.499997,
   -0.262869,  0.525738,  0.809012,
   -0.850648,  0.525736,  0.000000,
   -0.262869,  0.525738, -0.809012,
    0.688189,  0.525736, -0.499997,
    0.162456,  0.850654,  0.499995,
    0.525730,  0.850652,  0.000000,
   -0.425323,  0.850654,  0.309011,
   -0.425323,  0.850654, -0.309011,
    0.162456,  0.850654, -0.499995,
};

void create_blender_ico_sphere(float* out)
{
    uint32 vcnt = 0;
    // float* in = dodecahedron_vertices;
}


/**************************************************************************************************/
#define PHI 1.618033988749895f
#define INV_PHI 0.618033988749895f


/// Generate 20 vertices of a regular dodecahedron (circumradius = 1)
void generate_dodecahedron_vertices(float vertices[20]) {
    /// Normalization factor to make circumradius = 1
    float scale = 1.0f / sqrt(1.0f);

    /// Base vertices: permutations of (±1, ±1, ±1) normalized
    float cube_verts[8][3] = {
        { 1,  1,  1}, { 1,  1, -1}, { 1, -1,  1}, { 1, -1, -1},
        {-1,  1,  1}, {-1,  1, -1}, {-1, -1,  1}, {-1, -1, -1}
    };
    uint32 vcnt = 0;
    for (int i = 0; i < 8; i++) {
        float len = sqrt(cube_verts[i][0]*cube_verts[i][0] +
                         cube_verts[i][1]*cube_verts[i][1] +
                         cube_verts[i][2]*cube_verts[i][2]);
        vertices[vcnt++] = cube_verts[i][0] / len;
        vertices[vcnt++] = cube_verts[i][1] / len;
        vertices[vcnt++] = cube_verts[i][2] / len;
    }

    /// Base vertices: permutations of (0, ±1/φ, ±φ) normalized
    float golden_verts[12][3] = {
        { 0,  INV_PHI,  PHI}, { 0,  INV_PHI, -PHI},
        { 0, -INV_PHI,  PHI}, { 0, -INV_PHI, -PHI},
        { INV_PHI,  PHI, 0}, { INV_PHI, -PHI, 0},
        {-INV_PHI,  PHI, 0}, {-INV_PHI, -PHI, 0},
        { PHI, 0,  INV_PHI}, { PHI, 0, -INV_PHI},
        {-PHI, 0,  INV_PHI}, {-PHI, 0, -INV_PHI}
    };

    vcnt = 0;
    for (int i = 0; i < 12; i++) {
        float len = sqrt(golden_verts[i][0]*golden_verts[i][0] +
                         golden_verts[i][1]*golden_verts[i][1] +
                         golden_verts[i][2]*golden_verts[i][2]);
        vertices[8 + vcnt++] = golden_verts[i][0] / len;
        vertices[8 + vcnt++] = golden_verts[i][1] / len;
        vertices[8 + vcnt++] = golden_verts[i][2] / len;
    }
}

/// Face indices (12 faces, each with 5 vertex indices)
const int face_indices[12][5] = {
    { 0,  8, 12, 16,  4},  // face 0 (top front)
    { 0,  4,  6, 10,  2},  // face 1 (front)
    { 0,  2,  3,  7,  1},  // face 2 (right front)
    { 0,  1, 17, 12,  8},  // face 3 (top right)
    { 1,  7, 11,  9, 17},  // face 4 (right back)
    { 2, 10, 13, 15,  3},  // face 5 (bottom front)
    { 3, 15, 19, 11,  7},  // face 6 (bottom back)
    { 4, 16, 18, 14,  9},  // face 7 (top left)
    { 4,  9, 11, 19, 18},  // face 8 (left back)
    { 5, 14, 18, 19, 15},  // face 9 (left bottom)
    { 5, 15, 13, 10,  6},  // face 10 (bottom)
    { 5,  6,  4, 16, 18}   // face 11 (back)
};

/// Triangulate a pentagon (fan from first vertex)
void triangulate_pentagon(const float vertices[], const int pentagon[5],
                          float* out, int* offset) {
    /// Triangle 1: (0, 1, 2)
    out[(*offset)++] = vertices[pentagon[0]];
    out[(*offset)++] = vertices[pentagon[1]];
    out[(*offset)++] = vertices[pentagon[2]];

    /// Triangle 2: (0, 2, 3)
    out[(*offset)++] = vertices[pentagon[0]];
    out[(*offset)++] = vertices[pentagon[2]];
    out[(*offset)++] = vertices[pentagon[3]];

    /// Triangle 3: (0, 3, 4)
    out[(*offset)++] = vertices[pentagon[0]];
    out[(*offset)++] = vertices[pentagon[3]];
    out[(*offset)++] = vertices[pentagon[4]];
}

/// Generate complete dodecahedron triangles (108 vertices = 36 triangles)
/// Returns number of vertices (always 108)
int generate_dodecahedron_triangles(float* out_triangles) {
    float vertices[20*3];
    generate_dodecahedron_vertices(vertices);

    int offset = 0;
    for (int f = 0; f < 12; f++) {
        triangulate_pentagon(vertices, face_indices[f], out_triangles, &offset);
    }
    for (int f = 0; f < 108; f++) {
        // if((f%3)==0) print("\n[%u]:", f);
        if((f%3)==0) print("\n");
        print("%f, ", out_triangles[f]);
    }
    return offset;  /// Returns 108
}






/* THREE.js */
float dodecahedron_vertices[VERTICES_PER_DODECAHEDRON * VERTEX_POS3];
bool is_dodecahedron_build = FALSE;
void mesh_build_dodecahedron()
{
    /*
        const t = ( 1 + Math.sqrt( 5 ) ) / 2;
        const r = 1 / t;

        const vertices = [

            -1, -1, -1,
            -1, -1,  1,
            -1,  1, -1,
            -1,  1,  1,
             1, -1, -1,
             1, -1,  1,
             1,  1, -1,
             1,  1,  1,

             0, -r, -t,
             0, -r,  t,
             0,  r, -t,
             0,  r,  t,

            -r, -t, 0,
            -r,  t, 0,
             r, -t, 0,
             r,  t, 0,

            -t, 0, -r,
             t, 0, -r,
            -t, 0,  r,
             t, 0,  r
        ];
     */
    uint32 i = 0;
    float _0 = 0.0f;
    float vv = 1.f;
    float tt = (1.0 + sqrt(5)) / 2;
    float rr = 1.0/tt;
    float in[20][VERTEX_POS3] = {
        {-vv,  -vv,  -vv },  // [0] TRIANGLE 0 VPOS3
        {-vv,  -vv,   vv },  // [1] TRIANGLE 1 VPOS3 1
        {-vv,   vv,  -vv },  // [2]
        {-vv,   vv,   vv },  // [3]
        { vv,  -vv,  -vv },  // [4]
        { vv,  -vv,   vv },  // [5]
        { vv,   vv,  -vv },  // [6]
        { vv,   vv,   vv },  // [7]
        { _0,  -rr,  -tt },  // [8]
        { _0,  -rr,   tt },  // [9]
        { _0,   rr,  -tt },  // [10]
        { _0,   rr,   tt },  // [11]
        {-rr,  -tt,   _0,},  // [12]
        {-rr,   tt,   _0,},  // [13]
        { rr,  -tt,   _0,},  // [14]
        { rr,   tt,   _0,},  // [15]
        {-tt,   _0,  -rr },  // [16]
        { tt,   _0,  -rr },  // [17]
        {-tt,   _0,   rr },  // [18]
        { tt,   _0,   rr }   // [19]
    };
    const uint32 total_vertex_count = FACES_PER_DODECAHEDRON * VERTICES_PER_PENTAGON * VERTEX_POS3;
    uint32 tri_ind[FACES_PER_DODECAHEDRON * VERTICES_PER_PENTAGON] = {
        3  ,  11 ,  7   , // [0] 3 VERTICES = 1 TRIANGLE
        3  ,  7  ,  15  , // [1]
        3  ,  15 ,  13  , // [2]
        7  ,  19 ,  17  , // [3]
        7  ,  17 ,  6   , // [4]
        7  ,  6  ,  15  , // [5]
        17 ,  4  ,  8   , // [6]
        17 ,  8  ,  10  , // [7]
        17 ,  10 ,  6   , // [8]
        8  ,  0  ,  16  , // [9]
        8  ,  16 ,  2   , // [10]
        8  ,  2  ,  10  , // [11]
        0  ,  12 ,  1   , // [12]
        0  ,  1  ,  18  , // [13]
        0  ,  18 ,  16  , // [14]
        6  ,  10 ,  2   , // [15]
        6  ,  2  ,  13  , // [16]
        6  ,  13 ,  15  , // [17]
        2  ,  16 ,  18  , // [18]
        2  ,  18 ,  3   , // [19]
        2  ,  3  ,  13  , // [20]
        18 ,  1  ,  9   , // [21]
        18 ,  9  ,  11  , // [22]
        18 ,  11 ,  3   , // [23]
        4  ,  14 ,  12  , // [24]
        4  ,  12 ,  0   , // [25]
        4  ,  0  ,  8   , // [26]
        11 ,  9  ,  5   , // [27]
        11 ,  5  ,  19  , // [28]
        11 ,  19 ,  7   , // [29]
        19 ,  5  ,  14  , // [30]
        19 ,  14 ,  4   , // [31]
        19 ,  4  ,  17  , // [32]
        1  ,  12 ,  14  , // [33]
        1  ,  14 ,  5   , // [34]
        1  ,  5  ,  9   , // [35]
    };

    for(uint32 k = 0; k < total_vertex_count/3; k++) {

        print("k = %3u ", k);
        dodecahedron_vertices[i++] = in[tri_ind[k]][0]; print("[%3u]: %4.1f, ", k, dodecahedron_vertices[i-1]);
        dodecahedron_vertices[i++] = in[tri_ind[k]][1]; print("%4.1f, ", dodecahedron_vertices[i-1]);
        dodecahedron_vertices[i++] = in[tri_ind[k]][2]; print("%4.1f, ", dodecahedron_vertices[i-1]);
        print("\n");
        if(k > total_vertex_count) assert(0>0);
        // dodecahedron_vertices[i++] = in[tri_ind[k]][0] * .6;
        // dodecahedron_vertices[i++] = in[tri_ind[k]][1] * .6;
        // dodecahedron_vertices[i++] = in[tri_ind[k]][2] * .6;
    }

    is_dodecahedron_build = TRUE;
}




/**************************************************************************************************
 * VERTEX INDICES
 */
vind_t triangle_indices[] = {
    0, 1, 2
};
vind_t rectangle_indices[] = {
    0, 1, 2,
    0, 2, 3
};



/**************************************************************************************************
 * GETTERS
 */

float* mesh_get_mesh_from_type(enum MESH_TYPE type)
{
    /// forget to call mesh_build_dodecahedron()
    if(!is_dodecahedron_build && (type == MESH_DODECAHEDRON)) assert(0 > 0);

    switch(type) {
        case MESH_TRIANGLE     : return triangle_vertices;
        case MESH_RECTANGLE    : return rectangle_vertices;
        case MESH_PENTAGON     : return pentagon_vertices;
        case MESH_OCTAGON      : return octagon_vertices;
        case MESH_CUBE         : return cube_vertices;
        case MESH_OCTAHEDRON   : return octahedron_vertices;
        case MESH_DODECAHEDRON : return dodecahedron_vertices;
        default: assert(0 < 0);
    }
}
vind_t* mesh_get_mesh_indices_from_type(enum MESH_TYPE type)
{
    switch(type) {
        case MESH_TRIANGLE     : return triangle_indices;
        case MESH_RECTANGLE    : return rectangle_indices;
        // case MESH_PENTAGON     : return pentagon_vertices;
        // case MESH_OCTAGON      : return octagon_vertices;
        // case MESH_CUBE         : return cube_vertices;
        // case MESH_OCTAHEDRON   : return octahedron_vertices;
        // case MESH_DODECAHEDRON : return dodecahedron_vertices;
        default: assert(0 < 0);
    }
}
vind_t mesh_get_mesh_indices_count_from_type(enum MESH_TYPE type)
{
    switch(type) {
        case MESH_TRIANGLE     : return ARRAY_LENGTH(triangle_indices);
        case MESH_RECTANGLE    : return ARRAY_LENGTH(rectangle_indices);
        // case MESH_PENTAGON     : return pentagon_vertices;
        // case MESH_OCTAGON      : return octagon_vertices;
        // case MESH_CUBE         : return cube_vertices;
        // case MESH_OCTAHEDRON   : return octahedron_vertices;
        // case MESH_DODECAHEDRON : return dodecahedron_vertices;
        default: assert(0 < 0);
    }
}
uint8 mesh_get_vpos_num_elems(enum MESH_TYPE type)
{
    switch(type) {
        case MESH_TRIANGLE     : return 2;
        case MESH_RECTANGLE    : return 2;
        case MESH_PENTAGON     : return 2;
        case MESH_OCTAGON      : return 2;
        case MESH_CUBE         : return 3;
        case MESH_OCTAHEDRON   : return 3;
        case MESH_DODECAHEDRON : return 3;
        default: assert(0 < 0);
    }
}
char* mesh_get_name_from_type(enum MESH_TYPE type)
{
    switch(type) {
        case MESH_TRIANGLE     : return "TRIANGLE    ";
        case MESH_RECTANGLE    : return "RECTANGLE   ";
        case MESH_PENTAGON     : return "PENTAGON    ";
        case MESH_OCTAGON      : return "OCTAGON     ";
        case MESH_CUBE         : return "CUBE        ";
        case MESH_OCTAHEDRON   : return "OCTAHEDRON  ";
        case MESH_DODECAHEDRON : return "DODECAHEDRON";
        default: assert(0 < 0);
    }
}