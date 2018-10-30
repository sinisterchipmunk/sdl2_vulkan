require "test_helper"

SDL2.init(SDL2::INIT_EVERYTHING)

class Sdl2VulkanTest < Minitest::Test
  def test_flags
    refute_nil SDL2::Window::Flags::VULKAN
  end

  def test_library_loading
    assert SDL2::Vulkan.load_library(ENV['PATH_TO_VULKAN'])
    refute_nil SDL2::Vulkan.vk_get_instance_proc_addr

    window = SDL2::Window.create("vulkan-test", 0, 0, 640, 480, SDL2::Window::Flags::VULKAN)
    assert window.vk_instance_extensions.kind_of?(Array)

    # requires a vulkan instance to function properly, which is really a bit
    # beyond the scope of this gem
    assert window.respond_to?(:vk_create_surface)

    size = window.vk_drawable_size
    assert size.kind_of?(Array)
    assert size[0] > 0
    assert size[1] > 0
  end
end
