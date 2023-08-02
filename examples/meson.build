example_srcs = [
    'send_attachments.cpp'
]

examples_exe = executable(
    'examples',
    example_srcs,
    include_directories : incdir,
    link_with : smtp_lib,
    link_args : base_linker_args,
    cpp_args : base_cpp_args,
)
