/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 11:52:40 by ebenoist          #+#    #+#             */
/*   Updated: 2026/07/15 17:31:05 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

void free_exit( int out, t_data *data)
{
	if(data->inputs)
		free(data->inputs);
	exit(out);
}

void	hash_stdin(t_data *data)
{
	char	result[65];
	size_t	size;
// read 0 = read stdin
	uint8_t	*buffer = read_fd(0, &size);
	if (!buffer)
		return ;
	to_hex(data, buffer, size, result);
	if (data->q)
		printf("%s\n", result);
	else
		printf("(stdin)= %s\n", result);
	free(buffer);
}

uint8_t	*read_fd(int fd, size_t *out_size)
{
	uint8_t	tmp[1024];
	uint8_t	*buffer;
	uint8_t	*new;
	ssize_t	n;
	size_t	size;

	buffer = NULL;
	size = 0;
	while ((n = read(fd, tmp, sizeof(tmp))) > 0)
	{
		new = malloc(size + n);
		if (!new)
			return (free(buffer), NULL);
		ft_memcpy(new, buffer, size); 
		ft_memcpy(new + size, tmp, n);
		free(buffer);
		buffer = new;
		size += n;
	}
// taille octet lu, utile pour que le hashage est bien le bn nombre d'oct a lire
// *passage par adresse permet de modifier la valeur, meme sans retour.
	*out_size = size;
	if (!buffer)
		buffer =malloc(1);

	return (buffer);
}

uint8_t	*read_file(t_data *data, const char *path, size_t *out_size)
{
	uint8_t	*buffer;
	int		fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		fprintf(stderr, "ft_ssl: %s: %s: %s\n",
			data->cmd->name, path, strerror(errno));
		return (NULL);
	}
	buffer = read_fd(fd, out_size);
	close(fd);
	return (buffer);
}
