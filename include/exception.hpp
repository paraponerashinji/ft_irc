#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP
class IrcException: public std::exception
{
    public:
        virtual int errorCode() const = 0;
};
#endif