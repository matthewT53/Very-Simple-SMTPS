example_srcs = [
    'example1.cpp'
]

examples_exe = executable(
    'examples',
    example_srcs,
    include_directories : incdir,
    link_with : smtp_lib,
    cpp_args : ['-Wall', '-Werror'],
)