cpputest_dep = dependency(
    'cpputest',
    required : true
)

test_srcs = [
    'run_tests.cpp',
    'email_tests.cpp',
    'base64_tests.cpp',
    'mime_tests.cpp',
    'secure_strings_tests.cpp'
]

# incdir is inherited from the root meson.build file.
# smtp_lib comes from compiling the ./src directory.
tests_exe = executable(
    'smtp_tests', 
    test_srcs, 
    include_directories : incdir,
    link_with : smtp_lib,
    dependencies : cpputest_dep,
    cpp_args : ['-Wall', '-Werror']
)

test('smtp_lib_tests', tests_exe)