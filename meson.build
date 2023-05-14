project(
    'smtp_library',
    'cpp',
    version : '1.0.0',
)

base_cpp_args = [
  '-std=c++20',
  '-Werror',
  '-Wall',
  '-Wextra',
  '-fsanitize=address',
  '--coverage'
]

incdir = include_directories('headers')

subdir('src')
subdir('tests')
subdir('examples')
