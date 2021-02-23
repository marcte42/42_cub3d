/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mterkhoy <mterkhoy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 10:13:52 by mterkhoy          #+#    #+#             */
/*   Updated: 2021/02/22 23:48:16 by mterkhoy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

float	normalize_angle(float angle)
{
	if (angle < 0)
		angle += M_PI * 2;
	else if (angle > M_PI * 2)
		angle -= M_PI * 2;
	return (angle);
}

void	print_matrix(t_data *data)
{
	size_t i;
	size_t j;

	i = -1;
	while (++i < data->cfg.map_size.y)
	{
		j = -1;
		while (++j < data->cfg.map_size.x)
		{
			printf("%c", data->cfg.map[i][j]);
		}
		printf("\n");
	}
}

int		params_count(char **params)
{
	size_t i;

	i = 0;
	if (!params)
		return (0);
	while (params[i])
		i++;
	return (i);
}

int		only_digit(const char *s)
{
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (0);
		s++;
	}
	return (1);
}

int		ft_issuffix(char *s1, char *s2)
{
	size_t	s1_len;
	size_t	s2_len;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	return (!ft_strcmp(s1 + (s1_len - s2_len), s2));
}

int		is_wall(t_data *data, float x, float y)
{
	if (ft_strchr("1 ", data->cfg.map[(int)y / TILE_SIZE][(int)x / TILE_SIZE]))
		return (1);
	return (0);
}

float	hit_distance(float p1_x, float p1_y, float p2_x, float p2_y)
{
	float	x;
	float	y;

	x = p1_x - p2_x;
	if (x < 0)
		x *= -1;
	y = p1_y - p2_y;
	if (y < 0)
		y *= -1;
	return (sqrtf(pow(x, 2) + pow(y, 2)));
}
