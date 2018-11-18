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

require 'fiddle'
%w(
  SIZEOF_VOIDP SIZEOF_CHAR SIZEOF_SHORT SIZEOF_INT SIZEOF_LONG SIZEOF_LONG_LONG
  SIZEOF_FLOAT SIZEOF_DOUBLE SIZEOF_SIZE_T SIZEOF_SSIZE_T SIZEOF_PTRDIFF_T
  SIZEOF_INTPTR_T SIZEOF_UINTPTR_T
).each do |const_name|
  if Fiddle.constants.include?(const_name.to_sym)
    $CFLAGS << " -DHAVE_#{const_name}=1 -D#{const_name}=#{Fiddle.const_get(const_name.to_sym)}"
  end
end

have_header("SDL_vulkan.h")
create_makefile("sdl2_vulkan/sdl2_vulkan")
