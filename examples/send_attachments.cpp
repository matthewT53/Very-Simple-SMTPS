/*
  Example of how to this SMTP library to send an email with two images as attachments.
*/

#include "attachment/attachment.hpp"
#include "date_time/date_time_now.hpp"
#include "email/email.hpp"

using namespace smtp;

int main(void) {
  EmailParams params{"test_username", "test_password",
                     "smtps://email-smtp.ap-southeast-2.amazonaws.com:465"};
  Email e{params};

  e.setTo("test_to@gmail.com");
  e.setFrom("test_from@gmail.com");
  e.setSubject("Testing sending attachments");
  e.setBody("Hey listen friend here are some attachments for you to play with!");
  e.setDate(DateTimeNow());

  Attachment a{"mountain-beach.jpg"};
  Attachment a2{"mountain-beach2.jpg"};
  e.addAttachment(a);
  e.addAttachment(a2);

  e.send();

  return 0;
}
