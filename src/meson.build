curl_dep = dependency(
    'libcurl',
    required : true
)

smtp_srcs = [
    'email/email.cpp',
    'mime/mime.cpp',
    'utils/base64/base64.cpp',
    'attachment/attachment.cpp',
    'date_time/date_time_now.cpp',
]

# incdir comes from the meson build in the ./ directory
smtp_lib = library(
    'smtp_lib',
    smtp_srcs,
    include_directories : incdir,
    dependencies : curl_dep,
    cpp_args : base_cpp_args,
    link_args: base_linker_args,
    install: true,
)
