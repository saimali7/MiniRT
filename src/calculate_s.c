#include "../inc/MiniRt.h"
#include "../inc/Minilibx.h"
#include "../Libft/libft.h"

float *clamp(float min, float max, float *value)
{
    int i = 0;
    while (i < 3)
    {
        if (value[i] > max)
            value[i] = max;
        else if (value[i] < min)
            value[i] = min;
        i++;
    }
    return (value);
}

void    ft_putpixel(int x, int y, float *color, t_disp *display)
{
    char	*temp;
	int		position;

    x = WIDHT/2 + x;
    y = HEIGHT/2 - y - 1;
	position = x * 4 + 4 * WIDHT * y;
	temp = display->img.addr;
	temp[position] = color[2];
	temp[position + 1] = color[1];
	temp[position + 2] = color[0];
}


float *Convert_Viewport(int x, int y, t_rt *rt)
{
    float *direction;

    float fov = (float)rt->camera.fov;
    float aspect =  WIDHT / HEIGHT;
	float new_width = (tan(fov / 2 * (M_PI / 180))) * 2;
	float new_hight = new_width / aspect;
	float x_pix = new_width / WIDHT;
	float y_pix = new_hight / HEIGHT;
    direction =(float *) malloc(sizeof(float) * 3); //add check
    direction[0] = x * x_pix;
    direction[1] = y * y_pix;
    direction[2] = 1.0;
    normalize_vect(direction);
    return (direction);
}

void    intersect_sphere(t_rt *rt, float *direction, t_sphere *sphere, float *intersect)
{
    float *oc;
    float origin[3]; //replace with actual camera cord.
    float s_center[3];
    float a;
    float b;
    float c; //saving dot product as int?
    float discriminant;
	(void) rt;

    s_center[0] = sphere->coord[0];
    s_center[1] = sphere->coord[1];
    s_center[2] = sphere->coord[2];


    origin[0] = rt->camera.coord[0];
    origin[1] = rt->camera.coord[1];
    origin[2] = rt->camera.coord[2];

    oc = subtr_vec(origin, s_center);
    a = dot_product_vect(direction, direction);
    b = 2 * dot_product_vect(oc, direction);
    c = dot_product_vect(oc, oc) - sphere->radius * sphere->radius;

    discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        intersect[0] = INF;
        intersect[1] = INF;
        return ;
    }
    intersect[0] = (-b + sqrt(discriminant)) / (2 * a);
    intersect[1] = (-b - sqrt(discriminant)) / (2 * a);
    return ;
}

float lighting(float *point, float *normal, t_rt *rt)
{
    float intesity;
    float *vec_light;
    float normal_dot;
    float length_n;

    length_n = length_vect(normal);
    intesity = 0.0;

    intesity += rt->ambient.ratio;
    vec_light = subtr_vec(rt->light.coord, point);

    normal_dot = dot_product_vect(normal, vec_light);
    if (normal_dot > 0)
        intesity += rt->light.ratio * normal_dot / (length_n * length_vect(vec_light));
    return (intesity);
}

float     *trace_ray(t_rt *rt, float *direction, int min, int max)
{
    float closest_t = INF;
    t_sphere *closest_sphere = NULL;
    t_sphere *sphere;
    float   *intersect;
    float   *point;
    float   *normal;
    float   *color;

    sphere = rt->sphere;
    intersect = malloc(sizeof(float) * 2);
    while (sphere != NULL)
    {
        intersect_sphere(rt, direction, sphere, intersect);
        if (intersect[0] < closest_t && min < intersect[0] && intersect[0] < max)
        {
            closest_t = intersect[0];
            closest_sphere = sphere;
        }
        if (intersect[1] < closest_t && min < intersect[1] && intersect[1] < max)
        {
            closest_t = intersect[1];
            closest_sphere = sphere;
        }
        sphere = sphere->next;
    }
    if (closest_sphere == NULL)
    {
       return ((float *)calloc(sizeof(float), 3));
    }
    point = add_vect(rt->camera.coord, multiply_vect(closest_t, direction));
    normal = subtr_vec(point, closest_sphere->coord);
    normal = multiply_vect(1.0 / length_vect(normal), normal);
    color = multiply_vect(lighting(point, normal, rt), closest_sphere->color);
    
    return (color);
}

void    calculate_s(t_disp *display , t_rt *rt)
{
    int x;
    int y;
    float *color;
    float *direction;

    y = -HEIGHT/2;
    x = 0;
    while (y < HEIGHT/2)
    {
        x = -WIDHT/2;
        while (x < WIDHT/2)
        {
            direction = Convert_Viewport(x, y, rt);
            //direction = direction * rt->camera.orient[0]
            color = trace_ray(rt, direction, 1 , INF);
            ft_putpixel(x, y , clamp(0.0, 255.0, color), display);
            x++;
        }
        y++;
    }
    mlx_put_image_to_window(display->mlx, display->mlx_win, display->img.img, 0, 0);
	mlx_hook(display->mlx_win, 17, 0, exit_hook, 0);
	mlx_key_hook(display->mlx_win, ft_key, rt);
	mlx_loop(display->mlx);
}
