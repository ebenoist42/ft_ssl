/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 10:26:44 by ebenoist          #+#    #+#             */
/*   Updated: 2026/07/13 11:01:23 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

/* helper de test uniquement : chaine -> hexa, via strlen */
char	*md5_hex(const char *s)
{
	static char		hex[33];
	uint8_t			digest[16];
	size_t			i;

	ft_md5((const uint8_t *)s, ft_strlen(s), digest);
	i = 0;
	while (i < 16)
	{
		sprintf(hex + i * 2, "%02x", digest[i]);
		i++;
	}
	return (hex);
}


int	main(int argc, char **argv)
{
	int	i = 1;

	while (i < argc){
		printf("MD5(\"%s\") = %s\n", argv[i], md5_hex(argv[i]));
		i++;}
	return (0);
}