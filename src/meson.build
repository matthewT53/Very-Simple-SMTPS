curl_dep = dependency(
    'libcurl',
)

smtp_srcs = [
    'email.cpp',
    'mime.cpp',
    'base64.cpp'
]

linker_args = [
  '-lstdc++fs',
]

# incdir comes from the meson build in the ./ directory
smtp_lib = shared_library(
    'smtp_lib',
    smtp_srcs,
    include_directories : incdir,
    dependencies : curl_dep,
    cpp_args : base_cpp_args,
    link_args: linker_args,
)
