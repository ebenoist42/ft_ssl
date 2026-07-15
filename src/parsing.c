/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 11:46:26 by ebenoist          #+#    #+#             */
/*   Updated: 2026/07/15 17:23:49 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

static const t_command	g_commands[] = {
	{ "md5",    "MD5",    16, ft_md5 },
	{ "sha256", "SHA256", 32, ft_sha256 },
	{ NULL, NULL, 0, NULL }
};

static void	check_commande(char **av, t_data *data)
{
	int	i;

	i = 0;
	while (g_commands[i].name)
	{
		if (ft_strcmp(g_commands[i].name, av[1]) == 0)
		{
			data->cmd = &g_commands[i];
			return;
		}
		i++;
	}
	fprintf(stderr, "Invalid command '%s'; type \"help\" for a list. Try with \"md5\" or \"sha256\" in first place ! \n", av[1]);
	exit(1);
}
static void	check_flag(int ac, char **av, t_data *data)
{
	data->inputs = malloc(sizeof(t_input) * ac);
	int end_flag = 0;
	
	for(int i = 2; i < ac; i++){
	if(!end_flag && ft_strcmp(av[i], "-p") == 0)
		data->p = 1;
	else if(!end_flag && ft_strcmp(av[i], "-r") == 0)
		data->r = 1;
	else if(!end_flag && ft_strcmp(av[i], "-q") == 0)
		data->q = 1;
	else if(!end_flag && ft_strcmp(av[i], "-s") == 0){
		if(!av[i + 1]){
			printf("Error : Please provide an argument.\n");
			free_exit(1, data);}
		data->inputs[data->n_inputs].type = INPUT_STRING;
		data->inputs[data->n_inputs].value = av[i + 1];
		data->n_inputs++;
		end_flag++;
		i++;}
	else{
		data->inputs[data->n_inputs].type = INPUT_FILE;
		data->inputs[data->n_inputs].value = av[i];
		data->n_inputs++;
		end_flag++;}	
	}
//si 0 input dans la commande (str // file) && pas de flag -p = lecture du stdin.
	data->read_stdin = data->p || (data->n_inputs == 0);
}

void init_data(int ac, char **av, t_data *data)
{
	if(ac < 2){
		printf("Error : Wrong number of arguemnts\n");
		exit(1);
	}
	check_commande(av, data);
	check_flag(ac, av, data);
}

