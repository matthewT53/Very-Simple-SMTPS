#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace smtp {

class IMime {
    public:
        IMime() {}
        virtual ~IMime() {}

        // Adds a file specified by t_filename as an attachment to this mime message
        virtual void addAttachment(const std::string &p_filename) = 0;

        // Removes the attachment specified by filename from this Mime document
        virtual void removeAttachment(const std::string &p_filename) = 0;

        // Adds a body to the email address
        virtual void addMessage(const std::string &p_message) = 0;

        // Removes the body message specified by the message contents from this Mime
        // document
        virtual void removeMessage(const std::string &p_message) = 0;

        // Constructs the mime message with the version, user agent as well as any
        // attachments
        virtual std::vector<std::string> build() const = 0;

    protected:
        virtual std::ostream &output(std::ostream &p_out) const { return p_out; }

    friend std::ostream &operator<<(std::ostream &p_out, const IMime &p_mime) 
    {
        return p_mime.output(p_out);
    }
};

} // namespace smtp