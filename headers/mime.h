#ifndef MIME_H
#define MIME_H

#include <iostream>
#include <string>
#include <vector>

namespace smtp
{

class Mime 
{
    public:
        Mime( const std::string &t_user_agent = "test-bot" );

         // Adds a file specified by t_filename as an attachment to this mime message.
        void addAttachment( const std::string &t_filename );
        void addMessage( const std::string &t_message );

        // Constructs the mime message with the version, user agent as well as any attachments.
        void build();

        friend std::ostream & operator<<(std::ostream &t_out, const Mime &t_mime);

    private:
        std::string m_mime;
        std::vector<std::string> m_files;
        std::vector<std::string> m_messages;

        const static std::string boundary; 
};

std::ostream &operator<<( std::ostream &t_out, const smtp::Mime &t_mime )
{
    return t_out;
}

}

#endif