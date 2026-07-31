/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 11:46:26 by ebenoist          #+#    #+#             */
/*   Updated: 2026/07/30 12:45:32 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

static const t_command	g_commands[] = {
	{ "md5",    "MD5",    16, ft_md5 },
	{ "sha256", "SHA256", 32, ft_sha256 },
	{ NULL, NULL, 0, NULL }
};

static int	check_commande(char **av, t_data *data)
{
	int	i;

	i = 0;
	while (g_commands[i].name)
	{
		if (ft_strcmp(g_commands[i].name, av[1]) == 0)
		{
			data->cmd = &g_commands[i];
			return(0);
		}
		i++;
	}
	fprintf(stderr, "Invalid command '%s'; type \"help\" for a list. Try with \"md5\" or \"sha256\" in first place ! \n", av[1]);
	return(1);
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

static int	count_words(const char *s)
{
	int	n;
	int	in;

	n = 0;
	in = 0;
	while (*s)
	{
		if (*s != ' ' && *s != '\t')
		{
			if (!in)
			{
				n++;
				in = 1;
			}
		}
		else
			in = 0;
		s++;
	}
	return (n);
}

static char	**split_line(char *line, int *ac)
{
	char	**av;
	int		k;

	av = malloc(sizeof(char *) * (count_words(line) + 2));
	if (!av)
		return (NULL);
	av[0] = "ft_ssl";
	k = 1;
	while (*line)
	{
		while (*line == ' ' || *line == '\t')
			line++;
		if (*line)
		{
			av[k++] = line;
			while (*line && *line != ' ' && *line != '\t')
				line++;
			if (*line)
				*line++ = '\0';
		}
	}
	av[k] = NULL;
	*ac = k;
	return (av);
}

int	run_command(int ac, char **av, t_data *data)
{
	ft_memset(data, 0, sizeof(*data));
	if (check_commande(av, data))
		return (1);
	check_flag(ac, av, data);
	hash(data);
	if (data->inputs)
		free(data->inputs);
	return (0);
}

void	run_stdin_commands(void)
{
	uint8_t	*input;
	size_t	size;
	char	*s;
	char	*line;
	char	**av;
	int		ac;
	t_data	data;

	input = read_fd(0, &size);
	if (!input)
		return ;
	s = (char *)input;
	while (*s)
	{
		line = s;
		while (*s && *s != '\n')
			s++;
		if (*s == '\n')
			*s++ = '\0';
		av = split_line(line, &ac);
		if (av)
		{
			if (ac > 1)
				run_command(ac, av, &data);
			free(av);
		}
	}
	free(input);
}