/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 14:59:35 by ebenoist          #+#    #+#             */
/*   Updated: 2026/07/15 17:31:58 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
#define FT_SSL_H

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

typedef struct s_command
{
	const char	*name; //pour trouver la commande
	const char	*label; //pour l'affichage
	size_t		digest_size; // 16 md5 - 32 sha256
	void		(*hash)(const uint8_t *, size_t, uint8_t *);
}	t_command;

typedef enum e_input_type
{
	INPUT_STRING = 1,
	INPUT_FILE = 2,
	INPUT_STDIN = 3,
}	t_input_type;

typedef struct s_input
{
	t_input_type	type;
	char			*value;   //str (-s) ou nom de fichier, inutile pour stdin
}	t_input;

typedef struct s_data{
	const t_command	*cmd; 
	size_t 		size_algo;
	const char *str;
// flag
	int	p;
	int	q;
	int	r;
	t_input	*inputs;
	int		n_inputs;
	int		read_stdin; 

}t_data;

void	*ft_memset(void *pointer, int value, size_t count);
void	init_data(int ac, char **av, t_data *data);
int	    ft_strcmp(const char *s1, const char *s2);
size_t	ft_strlen(const char *str);
void	ft_md5(const uint8_t *msg, size_t len, uint8_t digest[16]);
void	ft_sha256(const uint8_t *msg, size_t len, uint8_t digest[32]);
void	free_exit(int out, t_data *data);
void	*ft_memcpy(void *dest, const void *src, size_t count);
uint8_t	*read_fd(int fd, size_t *out_size);
uint8_t	*read_file(t_data *data, const char *path, size_t *out_size);
void	to_hex(t_data *data, const uint8_t *buf, size_t len, char *out);
void	hash_stdin(t_data *data);

#endif