#include "ruby.h"
#include "SDL2/SDL_vulkan.h"

// from ruby-sdl2
typedef struct Window {
  SDL_Window* window;
  int num_renderers;
  int max_renderers;
  struct Renderer** renderers;
} Window;

VALUE rb_mSDL2 = Qnil,
      rb_cWindow = Qnil,
      rb_mFlags = Qnil,
      rb_mVulkan = Qnil;

/*
 * Loads the Vulkan library. If `path` is given, it will be used; otherwise
 * the library will be searched for dynamically. Raises an error if the
 * library could not be loaded.
 */
static VALUE Vulkan_load_library(int argc, VALUE *argv, VALUE self) {
  const char *path = NULL;
  if (argc > 0 && !NIL_P(argv[0])) path = StringValuePtr(argv[0]);
  int result = SDL_Vulkan_LoadLibrary(path);
  if (result != 0) rb_raise(rb_eRuntimeError, "Failed to load Vulkan library: %s", SDL_GetError());
  return self;
}

/*
 * Unloads the Vulkan library.
 */
static VALUE Vulkan_unload_library(VALUE self) {
  SDL_Vulkan_UnloadLibrary();
  return self;
}

/*
 * Returns an integer value which is the memory address of the
 * vkGetInstanceProcAddr function. The integer value will need to be converted
 * into something more useful, for instance using
 * [Fiddle](https://github.com/ruby/fiddle). Returns `nil` if the address
 * would be NULL.
 */
static VALUE Vulkan_get_vk_get_instance_proc_addr(VALUE self) {
  unsigned long long addr = (unsigned long long) SDL_Vulkan_GetVkGetInstanceProcAddr();
  return LL2NUM(addr);
}

/*
 * Returns an array of Vulkan extension names. Raises an error if they could
 * not be retrieved.
 */
static VALUE Window_get_instance_extension_names(VALUE self) {
  Window *window_data = NULL;
  Data_Get_Struct(self, Window, window_data);
  if (window_data == NULL || window_data->window == NULL)
    rb_raise(rb_eArgError, "Not a window");

  unsigned int count = 0;
  if (!SDL_Vulkan_GetInstanceExtensions(window_data->window, &count, NULL))
    rb_raise(rb_eRuntimeError, "Could not query number of extensions");
  
  const char **extension_names = malloc(count * sizeof(const char *));
  if (!SDL_Vulkan_GetInstanceExtensions(window_data->window, &count, extension_names)) {
    free(extension_names);
    rb_raise(rb_eRuntimeError, "Could not query extension names");
  }

  VALUE ary = rb_ary_new();
  for (unsigned int i = 0; i < count; i++) {
    rb_ary_push(ary, rb_str_new2(extension_names[i]));
  }
  free(extension_names);
  return ary;
}

/*
 * Creates a Vulkan drawing surface for this window. The `instance` argument
 * should be a number which represents the memory address of the `VkInstance`
 * (which is a pointer type).
 *
 * Raises a runtime error if the operation fails.
 *
 * Returns a number representing the memory address of the created surface.
 *
 */
static VALUE Window_create_surface(VALUE self, VALUE instance_addr) {
  Window *window_data = NULL;
  Data_Get_Struct(self, Window, window_data);
  if (window_data == NULL || window_data->window == NULL)
    rb_raise(rb_eArgError, "Not a window");

  VkInstance instance = (VkInstance) NUM2LL(instance_addr);
  VkSurfaceKHR surface;
  if (!SDL_Vulkan_CreateSurface(window_data->window, instance, &surface))
    rb_raise(rb_eRuntimeError, "Failed to create drawing surface");

  return LL2NUM((unsigned long long) surface);
}

/*
 * Returns an array containing `[width, height]` which are the drawable
 * dimensions of window in pixels.
 */
static VALUE Window_get_drawable_size(VALUE self) {
  Window *window_data = NULL;
  Data_Get_Struct(self, Window, window_data);
  if (window_data == NULL || window_data->window == NULL)
    rb_raise(rb_eArgError, "Not a window");

  int width  = 0;
  int height = 0;
  SDL_Vulkan_GetDrawableSize(window_data->window, &width, &height);

  VALUE ary = rb_ary_new();
  rb_ary_push(ary, INT2NUM(width));
  rb_ary_push(ary, INT2NUM(height));
  return ary;
}

void Init_sdl2_vulkan(void) {
  rb_mSDL2   = rb_define_module("SDL2");
  rb_cWindow = rb_const_get(rb_mSDL2, rb_intern("Window"));
  rb_mFlags  = rb_const_get(rb_cWindow, rb_intern("Flags"));
  rb_mVulkan = rb_define_module_under(rb_mSDL2, "Vulkan");

  rb_define_const(rb_mFlags, "VULKAN", INT2NUM(SDL_WINDOW_VULKAN));
  rb_define_singleton_method(rb_mVulkan, "load_library",              Vulkan_load_library,                 -1);
  rb_define_singleton_method(rb_mVulkan, "vk_get_instance_proc_addr", Vulkan_get_vk_get_instance_proc_addr, 0);
  rb_define_singleton_method(rb_mVulkan, "unload_library",            Vulkan_unload_library,                0);
  rb_define_method(rb_cWindow, "vk_instance_extensions", Window_get_instance_extension_names, 0);
  rb_define_method(rb_cWindow, "vk_create_surface",      Window_create_surface,               1);
  rb_define_method(rb_cWindow, "vk_drawable_size",       Window_get_drawable_size,            0);
}
