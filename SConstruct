<<<<<<< HEAD
SConscript(['godot_openxr/SConstruct','libstardustxr/SConstruct'])
=======
#!python
import os, subprocess

opts = Variables([], ARGUMENTS)

# Gets the standard flags CC, CCX, etc.
env = DefaultEnvironment()

# Define our options
opts.Add(EnumVariable('target', "Compilation target", 'debug', ['d', 'debug', 'r', 'release']))
opts.Add(BoolVariable('use_llvm', "Use the LLVM / Clang compiler", 'no'))

# Local dependency paths, adapt them to your setup
godot_headers_path = "godot-cpp/godot_headers/"
cpp_bindings_path = "godot-cpp/"
#cpp_library = "libgodot-cpp.linux.debug.64"

# Updates the environment with the option variables.
opts.Update(env)

# Process some arguments
if env['use_llvm']:
    env['CC'] = 'clang'
    env['CXX'] = 'clang++'

if env['target'] in ('debug', 'd'):
    env.Append(CCFLAGS = ['-fPIC', '-g3','-Og', '-std=c++17'])
else:
    env.Append(CCFLAGS = ['-fPIC', '-g','-O3', '-std=c++17'])

# make sure our binding library is properly includes
env.Append(CPPPATH=['.', godot_headers_path, 'include/', cpp_bindings_path + 'include/', cpp_bindings_path + 'include/core/', cpp_bindings_path + 'include/gen/', '/usr/local/lib/x86_64-linux-gnu/'])
env.Append(LIBPATH=[cpp_bindings_path + 'bin/'])
env.Append(LIBS=[ 'stardustxr-server'])

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=['src/'])
sources = Glob('src/*.cpp')

library = env.SharedLibrary(target='../godot-scene/addons/libstardustxr/bin/linux/libstardustxr-godot.so' , source=sources)

Default(library)

# Generates help for the -h scons option.
Help(opts.GenerateHelpText(env))
>>>>>>> dev
