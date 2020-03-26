cpputest_dep = dependency(
    'cpputest',
    required : true
)

test_srcs = [
    'runTests.cpp',
    'emailTests.cpp',
    'base64Tests.cpp',
    'mimeTests.cpp'
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