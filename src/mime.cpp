#include <iostream>

#include "mime.h"

const std::string smtp::Mime::boundary = "-----Boundary-----\r\n";

smtp::Mime::Mime( const std::string &t_user_agent )
{
    m_mime = "User-Agent: " + t_user_agent + "\r\n";
    m_mime += "MIME-Version: 1.0\r\n";
    m_mime += "Content-Type: multipart/mixed;\r\n";
    m_mime += "boundary=" + smtp::Mime::boundary;
    m_mime += "\r\nThis is a multi-part message in MIME format.\r\n";
    m_mime += smtp::Mime::boundary;
}

void smtp::Mime::addAttachment( const std::string &t_attachment )
{
    m_files.push_back(t_attachment);
}

void smtp::Mime::addMessage( const std::string &t_message )
{
    m_messages.push_back(t_message);
}

void smtp::Mime::build()
{
    for (const std::string &message : m_messages)
    {
        m_mime += "Content-Type: text/plain; charset=utf-8; format=flowed\r\n";
        m_mime += "Content-Transfer-Encoding: 7bit\r\n";
        m_mime += message + "\r\n";
        m_mime += smtp::Mime::boundary;
    }

    //TODO:
    // for (const std::string &attachment : m_files)
    // {
        
    // }
}
