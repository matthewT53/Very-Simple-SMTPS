curl_dep = dependency(
    'libcurl',
    required : true
)

smtp_srcs = [
    'email.cpp',
    'mime.cpp',
    'base64.cpp'
]

# incdir comes from the meson build in the ./ directory
smtp_lib = shared_library(
    'smtp_lib',
    smtp_srcs,
    include_directories : incdir,
    dependencies : curl_dep,
    cpp_args : ['-Wall', '-Werror']
)