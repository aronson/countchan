project('countchan', 'cpp', default_options : ['cpp_std=c++17', 'buildtype=minsize', 'default_library=static', 'b_lto=false', 'warning_level=1'])

add_global_arguments('-fexceptions', language : ['cpp'])
add_global_arguments('-frtti', language : ['cpp'])

mpt = dependency('openmpt', static : true, fallback : ['openmpt', 'openmpt_dep'])

sources = files('countchan.cpp')

if meson.get_compiler('cpp').get_id() != 'clang'
    link_args = [
        '-static-libgcc',
        '-static-libstdc++',
    ]
else
    link_args = []
endif

if host_machine.system() == 'windows'
    link_args += '-Wl,-Bstatic,--whole-archive'
    link_args += '-lwinpthread'
    link_args += '-Wl,--no-whole-archive'
endif

executable('countchan',
           dependencies : [dependency('threads'), mpt],
           win_subsystem : 'console',
           link_args : link_args,
           sources : sources
)

