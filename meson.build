project(
    'smtp_library',
    'cpp',
    version : '1.5.0',
)

base_cpp_args = [
  '-std=c++17',
  '-Werror',
  '-Wall',
  '-Wextra',
]

base_linker_args = [
  '-fsanitize=address',
  '--coverage'
]

incdir = include_directories('src')

subdir('src')
subdir('tests')
subdir('examples')
