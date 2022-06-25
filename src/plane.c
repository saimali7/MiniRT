#include "../inc/MiniRt.h"
#include "../inc/Minilibx.h"
#include "../Libft/libft.h"


float    intersect_plane(t_vect origin, t_vect direction, t_plane *plane, float *intersect)
{
    float  denom;
    t_vect   pl;
    
    denom = dot_product_vect(plane->orient, direction);
    if (fabs(denom) > 1e-6)
    {
        pl = subtr_vec(plane->coord, origin);
        *intersect = dot_product_vect(pl, plane->orient) / denom;
        return (*intersect >= 0);
    }
    return (0);
}

// int     trace_ray_plane(t_rt *rt, float *direction)
// {
//     t_plane *closest_plane = NULL;
//     t_plane *plane;
//     float	intersect;
// 	float	closest_t;

//     plane = rt->plane;
// 	closest_t = HUGE_VAL;
//     while (plane != NULL)
//     {
//         if (intersect_plane(rt, direction, plane, &intersect))
//         {
//             if (intersect < closest_t) 
//             {
//                 closest_plane = plane;
//                 closest_t = intersect;
//             }
//         }
//         plane = plane->next;
//     }
//     if (closest_plane == NULL)
//         return (0x000000);
//     else
//         return ((65536  * closest_plane->color[0]) + (256 * closest_plane->color[1]) + closest_plane->color[2]);
// }