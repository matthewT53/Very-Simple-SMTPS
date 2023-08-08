/*
  Example of how to this SMTP library to send an email with two images as attachments.
*/

#include "attachment/attachment.hpp"
#include "email/email.hpp"

using namespace smtp;

int main(void) {
  EmailParams params{
      "test_username",                                                     // smtp username
      "test_password",                                                     // smtp password
      "smtps://email-smtp.ap-southeast-2.amazonaws.com:465",               // smtp server
      "test_to@gmail.com",                                                 // to
      "test_from@gmail.com",                                               // from
      "",                                                                  // cc
      "Testing sending attachments",                                       // subject
      "Hey listen friend here are some attachments for you to play with!", // body
  };
  Email e{params};

  Attachment a{"mountain-beach.jpg"};
  Attachment a2{"mountain-beach2.jpg"};
  e.addAttachment(a);
  e.addAttachment(a2);

  e.send();

  return 0;
}
