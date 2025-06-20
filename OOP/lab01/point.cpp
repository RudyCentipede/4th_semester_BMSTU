#include "point.h"

point_t point_set_default()
{
    return point_t{0.0, 0.0, 0.0};
}

void point_move(point_t &point, const move_data_t &coeff)
{
    point.x += coeff.dx;
    point.y += coeff.dy;
    point.z += coeff.dz;
}

static double to_rad(const double &angle)
{
    return (M_PI / 180) * angle;
}

static void rotate_x(point_t &point, const double angle)
{
    double r_angle = to_rad(angle);
    double r_cos = cos(r_angle);
    double r_sin = sin(r_angle);
    double tmp_y = point.y;

    point.y = point.y * r_cos + point.z * r_sin;
    point.z = point.z * r_cos - tmp_y * r_sin;
}

static void rotate_y(point_t &point, const double angle)
{
    double r_angle = to_rad(angle);
    double r_cos = cos(r_angle);
    double r_sin = sin(r_angle);
    double tmp_x = point.x;

    point.x = point.x * r_cos - point.z * r_sin;
    point.z = point.z  * r_cos + tmp_x * r_sin;
}

static void rotate_z(point_t &point, const double angle)
{
    double r_angle = to_rad(angle);
    double r_cos = cos(r_angle);
    double r_sin = sin(r_angle);
    double tmp_x = point.x;

    point.x = point.x * r_cos + point.y * r_sin;
    point.y = point.y * r_cos - tmp_x * r_sin;
}

static void move_to_local_center(point_t &point, const point_t &rotate_center)
{
    point_t point_neg = point_to_negative(rotate_center);
    move_data_t move_data = point_to_move_data(point_neg);
    point_move(point, move_data);
}

static void move_to_global_center(point_t &point, const point_t &rotate_center)
{
    move_data_t move_data = point_to_move_data(rotate_center);
    point_move(point, move_data);
}

static void rotate(point_t &point, const rotate_data_t &coeff)
{
    rotate_x(point, coeff.angle_x);
    rotate_y(point, coeff.angle_y);
    rotate_z(point, coeff.angle_z);
}

static void scale(point_t &point, const scale_data_t &coeff)
{
    point.x *= coeff.kx;
    point.y *= coeff.ky;
    point.z *= coeff.kz;
}

void point_rotate(point_t &point, const point_t &rotate_center, const rotate_data_t &coeff)
{
    move_to_local_center(point, rotate_center);
    rotate(point, coeff);
    move_to_global_center(point, rotate_center);
}

void point_scale(point_t &point, const point_t &scale_center, const scale_data_t &coeff)
{
    move_to_local_center(point, scale_center);
    scale(point, coeff);
    move_to_global_center(point, scale_center);
}

return_codes_t point_fread(point_t &point, FILE *in)
{
    if (in == NULL)
        return ERROR_FILE_OPEN;

    if(fscanf(in, "%lf %lf %lf", &point.x, &point.y, &point.z) != 3)
        return ERROR_FILE_READ;

    return SUCCESS;
}

return_codes_t point_fwrite(const point_t &point, FILE *out)
{
    if (out == NULL)
        return ERROR_FILE_OPEN;

    if (fprintf(out, "%lf %lf %lf\n", point.x, point.y, point.z) < 0)
        return ERROR_FILE_WRITE;

    return SUCCESS;
}

move_data_t point_to_move_data(const point_t &point)
{
    return move_data_t{point.x, point.y, point.z};
}

point_t point_to_negative(const point_t &point)
{
    return point_t{-point.x, -point.y, -point.z};
}
