#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

size_t ft_strlen(const char *str) // ne sert qu'à write !
{
	size_t i = 0;

	while (str[i])
		++i;
	return (i);
}

int fatal_error(void)
{
	write(STDERR_FILENO, "error: fatal\n", 13);
	exit (1);
}

int cd(char **cmd) // cd sans pipe ou apres un pipe ou ;
{
	int i = 0;

	while (cmd[i]) // sert à s'assurer que cd est juste une commande + 1 arg
		i++;
	if (i != 2)
		write(STDERR_FILENO, "error: cd: bad arguments\n", 25);
	else if (chdir(cmd[1]) == -1) // si ok -> chdir. Si chdir échoue : message d'erreur
	{
		write(STDERR_FILENO, "error: cd: cannot change directory to ", 38);
		write(STDERR_FILENO, cmd[1], ft_strlen(cmd[1]));
		write(STDERR_FILENO, "\n", 1);
	}
	else
		return (0);
	return (1);
}

void child_execute_command(char **cmd, char **env)	// ici on envoie l'arg (cmd) avec un index de départ i !!
{													// càd le reste du tableau argv à partir d'argv[i] !!
	if (!strcmp(cmd[0], "cd")) // regle le cas ou un cd est apres un pipe 
		exit(cd(cmd));			// on exit en ayant fait cd
	execve(cmd[0], cmd, env); // on execve (cmd[0]=cde initial, cmd=tableau argv a partir d'argv[i], env)
	write(STDERR_FILENO, "error: cannot execute ", 22); // si execve s'execute, ne retoure rien et on revient
	write(STDERR_FILENO, cmd[0], ft_strlen(cmd[0]));	// dans le parent grace a wait, sinon return et on 
	write(STDERR_FILENO, "\n", 1);						// lance ces messages d'erreur avec un exit(1)
	exit(1);
}

void execute_command(char **cmd, char **env) // ici on reçoit argv avec un index de départ
{
	int i, j, last, readFrom, writeTo;
	int dup_in, dup_out, pid;
	int fd[2];

//-----   BLOC CD   -----//
	j = 0;
	while (cmd[j] && strcmp(cmd[j], "|")) // tant qu'on est pas sur un pipe, on avance
		++j;
	if (!cmd[j] && !strcmp(cmd[0], "cd")) // si on a plus d'arg et que le 1er arg est cd -> cd !
	{
		cd(cmd);
		return ;
	}
//-----   BLOC DUP INIT  -----//
	if ((readFrom = dup(STDIN_FILENO)) == -1) // on tf STDIN en readFrom
		fatal_error();
	if ((dup_in = dup(STDIN_FILENO)) == -1) // on tf STDIN en dup_in (numero de fd > a readFrom)
		fatal_error();
	if ((dup_out = dup(STDOUT_FILENO)) == -1) // on tf STDOUT en dup_out
		fatal_error();
//-----   BLOC PIPE / FORK / CHILD   -----//
	last = i = j = 0; // on (ré)initialise tout a 0
	while (!last) // tant que last = 0
	{
	//-----   SOUS BLOC LAST   -----//s
		while (cmd[j] && strcmp(cmd[j], "|")) // tant qu'on est pas sur un pipe, on avance
			++j;
		if (!cmd[j]) // si on a plus d'arg : on met last a 1 (donc on a fini d'exec toutes les commandes)
			last = 1;
		cmd[j] = NULL; 
	//-----   SOUS BLOC PIPE / SORTIE   -----//
		if (pipe(fd) == -1) // on ouvre un tunnel pipe 
			fatal_error();
		if (last) // si on est a notre derniere commande -> on donne a la sortie writeTo la valeur de dup_out
			writeTo = dup_out;
		else
			writeTo = fd[1]; // sinon, on donne a la sortie writeTo la valeur de l'entree du tunnel 
	//-----   SOUS BLOC DUP2   -----//
		if (dup2(readFrom, STDIN_FILENO) == -1) // on force readFrom sur STDIN
			fatal_error();
		close(readFrom); // on close readFrom (propreté)
		if (dup2(writeTo, STDOUT_FILENO) == -1) // on force writeTo sur STDOUT
			fatal_error();
		close(writeTo); // on close writeTo (propreté)
	//-----   SOUS BLOC FORK / CHILD   -----//
		if ((pid = fork()) == -1) // on forke !!
			fatal_error();
		if (pid == 0) // ici l'enfant
			child_execute_command(cmd + i, env); // ATTENTION c'est ici que l'incrementation finale du i fctn !
		else // ici le parent : on donne a readFrom la valeur de sortie du tunnel
			readFrom = fd[0];
		i = ++j;
	}
//-----   BLOC WAIT / CLOSE   -----//
	while (waitpid(-1, NULL, 0) != -1); // ici on attend que tous les childs soient exécutés
	if (dup2(dup_in, STDIN_FILENO) == -1) // on force dup_in sur STDIN
		fatal_error();
	close(dup_in); // on close tout proprement !
	close(fd[1]);
	close(readFrom);
}

int main(int argc, char **argv, char **env)
{
	int i = 1;
	int j = 1;

	while (i < argc)
	{
		while (argv[j] && strcmp(argv[j], ";"))	// tant qu'on n'est pas sur un ";", on browse les arguments 
			j++;
		argv[j] = NULL; // ici , argv[j] est soit the end of argv, soit un ";" !
		if (argv[i]) // on retourne sur argv[1] puis plus tard sur la commande apres le ";"
			execute_command(argv + i, env); // on envoie **argv, et son index i pour execution
		i = ++j; // ATTENTION : on incrémente avant car on ne veut pas retomber sur le ";" ! et a la prochaine
	}				// boucle, on aura un j au prochain arg !
	return (0);
}
