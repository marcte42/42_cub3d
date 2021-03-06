/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mterkhoy <mterkhoy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 17:56:13 by mterkhoy          #+#    #+#             */
/*   Updated: 2021/04/12 10:41:28 by marcte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		spread(t_data *data, t_list **list, t_crd p)
{
	if (ft_strchr("20 ", data->cfg.map_tmp[p.y - 1][p.x]))
	{
		data->cfg.map_tmp[p.y - 1][p.x] = '-';
		ft_lstadd_front(list, ft_lstnew(point_create(data, p.x, p.y - 1)));
	}
	if (ft_strchr("20 ", data->cfg.map_tmp[p.y + 1][p.x]))
	{
		data->cfg.map_tmp[p.y + 1][p.x] = '-';
		ft_lstadd_front(list, ft_lstnew(point_create(data, p.x, p.y + 1)));
	}
	if (ft_strchr("20 ", data->cfg.map_tmp[p.y][p.x - 1]))
	{
		data->cfg.map_tmp[p.y][p.x - 1] = '-';
		ft_lstadd_front(list, ft_lstnew(point_create(data, p.x - 1, p.y)));
	}
	if (ft_strchr("20 ", data->cfg.map_tmp[p.y][p.x + 1]))
	{
		data->cfg.map_tmp[p.y][p.x + 1] = '-';
		ft_lstadd_front(list, ft_lstnew(point_create(data, p.x + 1, p.y)));
	}
	return (0);
}

int		is_map_leaking(int x, int y, t_data *data)
{
	t_list	*list;
	t_crd	p;

	if (x == 0 || x == data->cfg.map_size.x - 1
		|| y == 0 || y == data->cfg.map_size.y - 1)
		return (1);
	list = NULL;
	list = ft_lstadd_front(&list, ft_lstnew(point_create(data, x, y)));
	while (list)
	{
		p = ft_lstpop(&list);
		if (data->cfg.map_tmp[p.y][p.x] == '-' && (p.y == 0
			|| p.x == 0 || p.y == data->cfg.map_size.y - 1
			|| p.x == data->cfg.map_size.x - 1)
			|| data->cfg.map_tmp[p.y - 1][p.x] == ' '
			|| data->cfg.map_tmp[p.y + 1][p.x] == ' '
			|| data->cfg.map_tmp[p.y][p.x - 1] == ' '
			|| data->cfg.map_tmp[p.y][p.x + 1] == ' ')
		{
			ft_lstclear(&list, free);
			return (1);
		}
		spread(data, &list, p);
	}
	return (0);
}

void	map_to_mat(t_data *data)
{
	size_t	i;
	t_list	*lst;

	lst = data->cfg.map_lst;
	if (!(data->cfg.map = malloc(data->cfg.map_size.y * sizeof(char *))) ||
		!(data->cfg.map_tmp = malloc(data->cfg.map_size.y * sizeof(char *))))
		exit_failure(data, "Malloc failed");
	i = -1;
	while (++i < data->cfg.map_size.y)
	{
		if (!(data->cfg.map[i] = malloc(data->cfg.map_size.x * sizeof(char))) ||
		!(data->cfg.map_tmp[i] = malloc(data->cfg.map_size.x * sizeof(char))))
			exit_failure(data, "Malloc failed");
		ft_memset(data->cfg.map[i], ' ', data->cfg.map_size.x);
		ft_memset(data->cfg.map_tmp[i], ' ', data->cfg.map_size.x);
		data->cfg.map[i] = ft_memcpy(data->cfg.map[i], lst->content,
			ft_strlen(lst->content));
		data->cfg.map_tmp[i] = ft_memcpy(data->cfg.map_tmp[i], lst->content,
			ft_strlen(lst->content));
		lst = lst->next;
	}
}

void	parse_player(t_data *data)
{
	int		i;
	int		j;

	i = -1;
	while (++i < data->cfg.map_size.y)
	{
		j = -1;
		while (++j < data->cfg.map_size.x)
			if (ft_strchr("NSWE", data->cfg.map[i][j]))
			{
				data->player.pos.x = j;
				data->player.pos.y = i;
				data->player.angle = get_angle(data->cfg.map[i][j]);
				while (i < data->cfg.map_size.y)
				{
					while (++j < data->cfg.map_size.x)
						if (ft_strchr("NSWE", data->cfg.map[i][j]))
							exit_failure(data, "Multiple players on map");
					j = -1;
					i++;
				}
				return ;
			}
	}
	exit_failure(data, "Player not found");
}

void	parse_map(char **params, t_data *data)
{
	size_t	i;
	size_t	len;
	char	*new_line;
	t_list	*portion;

	len = ft_strlen(data->line);
	i = -1;
	while (data->line[++i])
	{
		if (len > data->cfg.map_size.x)
			data->cfg.map_size.x = len;
		if (!ft_strchr(TILES, data->line[i]))
			error_params(data, params, "Forbidden map tiles");
	}
	if (!(portion = ft_lstnew(ft_strdup(data->line))))
		exit_failure(data, "Malloc failed");
	ft_lstadd_back(&data->cfg.map_lst, portion);
	data->cfg.map_size.y++;
}
