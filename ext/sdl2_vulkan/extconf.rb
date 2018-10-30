require 'mkmf'

# helpers "borrowed" from ruby-sdl2
def add_cflags(str)
  $CFLAGS += " " + str
end

def add_libs(str)
  $LOCAL_LIBS += " " + str
end

def run_config_program(*args)
  IO.popen([*args], "r") do |io|
    io.gets.chomp
  end
end

def sdl2config_with_command
  sdl2_config = with_config('sdl2-config', 'sdl2-config')
  add_cflags(run_config_program(sdl2_config, "--cflags"))
  add_libs(run_config_program(sdl2_config, "--libs"))
end

def sdl2config_on_mingw
  have_library("mingw32")
  have_library("SDL2")
  add_libs("-mwindows")
end

case RbConfig::CONFIG["arch"]
when /mingw/
  sdl2config_on_mingw
else
  sdl2config_with_command
end

have_header("SDL_vulkan.h")
create_makefile("sdl2_vulkan/sdl2_vulkan")
