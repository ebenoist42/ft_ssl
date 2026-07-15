/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 14:59:16 by ebenoist          #+#    #+#             */
/*   Updated: 2026/07/15 17:40:59 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

void	to_hex(t_data *data, const uint8_t *buf, size_t len, char *out)
{
	uint8_t	digest[32];
	size_t	i;
	data->cmd->hash(buf, len, digest);          // md5 OU sha256 selon la table
	i = 0;

// transforme brut (bit) octets lisible via conv en hexa
	while (i < data->cmd->digest_size)
	{
		out[i * 2]     = "0123456789abcdef"[digest[i] >> 4];
		out[i * 2 + 1] = "0123456789abcdef"[digest[i] & 0xf];
		i++;
	}
	out[data->cmd->digest_size * 2] = '\0';
}

static void print(t_data *data, char *result,int i)
{
	if(data->q){
			//printf("%s (%s) = ", data->cmd->label ,data->inputs[i].value);
			printf("%s\n",result);
			return;}
	else if (data->r){
		printf("%s ",result);
		if(data->inputs[i].type == 1)
			printf("\"%s\"\n",data->inputs[i].value);
		else
			printf("%s\n",data->inputs[i].value);}
	else{
		if (data->inputs[i].type == 1)
			printf("%s (\"%s\") = %s\n", data->cmd->label, data->inputs[i].value, result);
		else
			printf("%s (%s) = %s\n", data->cmd->label, data->inputs[i].value, result);
	}
	return;
	printf("%s \n",result);

}

void	hash(t_data *data)
{
	char result[65]; //pour couvrir sh256 et md5
//stdin priorite

	if(data->read_stdin)
		hash_stdin(data);

	for(int i = 0; i < data->n_inputs; i++)
	{
		if(data->inputs[i].type == 1)
		{
			to_hex(data,(const uint8_t *)data->inputs[i].value, ft_strlen(data->inputs[i].value), result);
			print(data, result, i);
		}
		else if(data->inputs[i].type == 2)
		{
			size_t size;
			uint8_t *buffer = read_file(data, data->inputs[i].value, &size);
			if(!buffer)
				continue;
			to_hex(data, buffer, size, result);
			print(data, result, i);
			free(buffer);
		}
	}
}

int	main(int ac, char **av)
{
	t_data data;
	ft_memset(&data, 0, sizeof(data));
	init_data(ac, av, &data);
	hash(&data);
	free_exit(0, &data);
}