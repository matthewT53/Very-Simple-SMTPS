cpputest_dep = dependency(
    'cpputest',
    required : true
)

doctest_dep = dependency(
    'doctest',
    required: true
)

test_srcs = [
    'main.cpp',
    'email/email_tests.cpp',
    'mime/mime_tests.cpp',
    'utils/base64_tests.cpp',
    'utils/secure_strings_tests.cpp'
]

# incdir is inherited from the root meson.build file.
# smtp_lib comes from compiling the ./src directory.
tests_exe = executable(
    'smtp_tests',
    test_srcs,
    include_directories : incdir,
    link_with : smtp_lib,
    link_args : base_linker_args,
    dependencies : [cpputest_dep, doctest_dep],
    cpp_args : base_cpp_args
)

test('smtp_lib_tests', tests_exe)
