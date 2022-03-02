#include "../../Includes/lib.hpp"

static void	parse_cmd(data<user *> &data , user *cursor, std::string buf)
{
	std::string cmd = buf.substr(0, buf.find(' '));

	/*
	* Le but principal du protocole IRC est de fournir une base afin que des clients puissent communiquer entre eux.
	* PRIVMSG et NOTICE sont les seuls messages disponibles qui réalisent
	* effectivement l'acheminement d'un message textuel d'un client à un autre,
	* le reste le rend juste possible et assure que cela se passe de façon fiable et structurée.
	*/
	if (cmd == "NICK")
		cmd_nick(data, cursor, buf);
	else if (cmd == "USER")
		cmd_user(data, cursor, buf);
	else if (cmd == "NOTICE")
		cmd_notice(data, cursor, buf);
	else if (cmd == "PRIVMSG")
		cmd_privmsg(data, cursor, buf);
	else if (cmd == "TOPIC")
		cmd_topic(data, cursor, buf);
	else if (cmd == "KICK")
		cmd_kick(data, cursor, buf);
	else if (cmd == "EXIT")
		data.online = false;
	else if (cmd == "INVITE")
		cmd_invite(data, cursor, buf);
	else if(cmd == "PASS")
	{
		std::string err = ":server " + std::string(ERR_ALREADYREGISTRED) + " " + cmd + " :You are already register\r\n";
		send(cursor->getSd(), err.c_str(), err.length(), 0);
	}
	else if (cmd == "LIST")
	{
		/*
		* Paramètres: [<canal>]
		* Le message LIST est utilisé pour lister les canaux et leur sujet.
		*/
		cmd_list(data, cursor, buf);
	}
	else if (cmd == "JOIN") // a faire
	{
		/*
		* join permet de rejoindre un canal si il existe ou de le cree dans le cas echean.
		* l'utilisateur doit être invité si le canal est en mode "sur invitation seulement" (voir MODE)
		* le pseudo/nom d'utilisateur/nom d'hôte ne doit pas correspondre à un bannissement actif.
		* une clef peut etre demander si le salon en a une.
		* ex: JOIN #foo,#bar fubar,foobar ;
		* accède au canal #foo en utilisant la clé "fubar", et au canal #bar en utilisant la clé "foobar".
		*/
		cmd_join(data, cursor, buf);
	}
	else if (cmd == "MODE") // a faire
	{
		/*
		* Ce referer a l'article 4.2.3 de http://abcdrfc.free.fr/rfc-vf/rfc1459.html#411 .
		* Les mode a faire :
		* o - donne/retire les privilèges d'opérateur de canal
		* pour le flag o - le nombre de paramètres est restreint à trois par commande
		* i - drapeau de canal accessible uniquement sur invitation
		* k - définit la clé du canal (mot de passe)
		*/
		cmd_mode(data, cursor, buf);
	}
	else if (cmd != "PONG")
	{
		std::string str = ":server " + std::string(ERR_UNKNOWNCOMMAND) + " " + cmd + " :Unknown command\r\n";
		send(cursor->getSd(), str.c_str(), str.length(), 0);
	}
}

void	cmd_process(data<user *> &data)
{
	int		sd;
	int		ret_read;
	int		size_adress = sizeof(data.address);
	char	buffer[1024];

	for (std::vector<user *>::iterator it = data.users.begin(); it != data.users.end(); it++)
	{
		user *cursor = *it;
		sd = cursor->getSd();

		if (FD_ISSET(sd, &data.readfds))
		{
			if ((ret_read = read(sd, buffer, 1024)) == 0)
			{
				/*
				* des quelqu'un ce deconnecte, recuperer ses detail et les affiche.
				*/
				getpeername(sd, (struct sockaddr*)&data.address, (socklen_t*)&data.address);
				std::cout << "User " << cursor->getNick() << " disconnected, ip " << inet_ntoa(data.address.sin_addr) << " port " << ntohs(data.address.sin_port) << std::endl;
				/*
				* Close le socket et le set a 0 dans la liste.
				*/
				close(sd);
				sd = 0;
				/*
				* Supprime l'utilisateur de la liste.
				*/
				delete_user(data, cursor);
				return ;
			}
			else
			{
				buffer[ret_read] = '\0';
				if (buffer[ret_read - 1] != '\n')
				{
					cursor->setBuffer(cursor->getBuffer() + buffer);
					return;
				}
				if (cursor->getBuffer().empty())
					cursor->setBuffer(buffer);
				else
					cursor->setBuffer(cursor->getBuffer() + buffer);
				std::cout << "Cmd " << " : " << cursor->getBuffer() << std::endl;
				std::string command = cursor->getBuffer().substr(0, cursor->getBuffer().find(' '));

				/*
				* Si l'utilisateur est deja accepter,
				* Nous allons commencer a traiter la commande.
				*/
				if (cursor->getAccept() == true)
					parse_cmd(data, cursor, buffer);
				else
				{
					try
					{
						/*
						* A la premier connection d'un user la commande PASS <mdp> est envoye du clien
						* Si le mdp est correct alors user.accepte est set a true.
						* Par la suite le client va nous envoye :
						* NICK <nickname>
						* Puis USER <login> <distance server> <* <realname>>
						*/
						cmd_pass(data, cursor, cursor->getBuffer());
					}
					catch(const std::exception& e)
					{
						send(cursor->getSd(), e.what(), std::strlen(e.what()), 0);
						close(sd);
						sd = 0;
						delete_user(data, cursor);
						return ;
					}
				}
				cursor->cleanBuffer();
			}
		}
	}
}
