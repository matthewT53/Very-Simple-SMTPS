#ifndef MIME_H
#define MIME_H

#include <string>

class Mime 
{
    public:
        Mime() {}
        Mime( const std::string &t_version = "1.0", const std::string &t_user_agent = "test-bot");

         // Adds a file specified by t_filename as an attachment to this mime message.
         // Throws an exception 
        bool addAttachment( const std::string &t_filename );

        // Constructs the mime message with the version, user agent as well as any attachments.
        void build();

        // Dumps the mime message to STDOUT.
        // TODO: Replace this with an overloaded << operator.
        void dump() const;

    private:
        std::string m_message;

};

#endif