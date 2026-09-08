#include "../include/exception.hpp"

IrcException::IrcException(const std::string &target) : target(target) {}

int ERR_NOSUCHNICK::errorCode() const { return 401; }
int ERR_NOSUCHCHANNEL::errorCode() const { return 403; }
int ERR_TOOMANYTARGETS::errorCode() const { return 407; }
int ERR_NOTEXTTOSEND::errorCode() const { return 412; }
int ERR_UNKNOWNCOMMAND::errorCode() const { return 421; }
int ERR_NONICKNAMEGIVEN::errorCode() const { return 431; }
int ERR_ERRONEUSNICKNAME::errorCode() const { return 432; }
int ERR_NICKNAMEINUSE::errorCode() const { return 433; }
int ERR_NICKCOLLISION::errorCode() const { return 436; }
int ERR_USERNOTINCHANNEL::errorCode() const { return 441; }
int ERR_NOTONCHANNEL::errorCode() const { return 442; }
int ERR_USERONCHANNEL::errorCode() const { return 443; }
int ERR_NOLOGIN::errorCode() const { return 444; }
int ERR_NOTREGISTERED::errorCode() const { return 451; }
int ERR_NEEDMOREPARAMS::errorCode() const { return 461; }
int ERR_ALREADYREGISTRED::errorCode() const { return 462; }
int ERR_PASSWDMISMATCH::errorCode() const { return 464; }
int ERR_KEYSET::errorCode() const { return 467; }
int ERR_CHANNELISFULL::errorCode() const { return 471; }
int ERR_UNKNOWNMODE::errorCode() const { return 472; }
int ERR_INVITEONLYCHAN::errorCode() const { return 473; }
int ERR_BADCHANNELKEY::errorCode() const { return 475; }
int ERR_CHANOPRIVSNEEDED::errorCode() const { return 482; }