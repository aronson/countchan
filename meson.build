project('countchan', 'cpp', default_options : ['cpp_std=c++17', 'buildtype=debugoptimized', 'b_lto=true', 'warning_level=1'])

add_global_arguments('-fexceptions', language : ['cpp'])
add_global_arguments('-frtti', language : ['cpp'])

mpt = dependency('openmpt', static : true, fallback : ['openmpt', 'openmpt_dep'])

sources = files('countchan.cpp')

executable('countchan',
           dependencies : [mpt],
           win_subsystem : 'console',
           sources : sources
)

